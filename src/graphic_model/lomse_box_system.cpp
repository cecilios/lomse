//---------------------------------------------------------------------------------------
// This file is part of the Lomse library.
// Copyright (c) 2010-present, Lomse Developers
//
// Licensed under the MIT license.
//
// See LICENSE and NOTICE.md files in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#include "lomse_box_system.h"

#include "lomse_box_slice.h"
#include "lomse_internal_model.h"
#include "lomse_shape_staff.h"
#include "lomse_timegrid_table.h"
#include "lomse_drawer.h"
#include "lomse_gm_measures_table.h"

namespace lomse
{

//---------------------------------------------------------------------------------------
GmoBoxSystem::GmoBoxSystem(ImoScore* pScore)
    : GmoBox(GmoObj::k_box_system, pScore)
    , m_pGridTable(nullptr)
    , m_iPage(0)
    , m_dxFirstMeasure(0.0)
{
    //for unit tests it assumes a maximum of 10 instruments
    //TODO: Fix test by providing a ptr to ImoScore.
    int numInstruments = (pScore ? pScore->get_num_instruments() : 10);

	m_iFirstMeasure.assign(numInstruments, -1);
    m_nMeasures.assign(numInstruments, 0);
}

//---------------------------------------------------------------------------------------
GmoBoxSystem::~GmoBoxSystem()
{
    delete m_pGridTable;
}

//---------------------------------------------------------------------------------------
GmoShapeStaff* GmoBoxSystem::add_staff_shape(GmoShapeStaff* pShape)
{
	m_staffShapes.push_back(pShape);
    add_shape(pShape, GmoShape::k_layer_staff);
    return pShape;
}

//---------------------------------------------------------------------------------------
GmoShapeStaff* GmoBoxSystem::get_staff_shape(int absStaff)
{
	//returns the shape for staff absStaff (0..n-1). absStaff is the staff number
    //referred to total staves in system

    return m_staffShapes[absStaff];
}

//---------------------------------------------------------------------------------------
GmoShapeStaff* GmoBoxSystem::get_staff_shape(int iInstr, int iStaff)
{
    if (iInstr == -1)   //last instrument
        iInstr = get_num_instruments() - 1;

    if (iStaff == -1)   //last staff for instrument
    {
        if (iInstr == int(m_firstStaff.size() - 1) )
            return m_staffShapes.back();
        else
            return m_staffShapes[ m_firstStaff[iInstr] - 1 ];
    }


    if (iInstr == 0)
        return m_staffShapes[iStaff];
    else
        return m_staffShapes[ m_firstStaff[iInstr-1] + iStaff ];
}

//---------------------------------------------------------------------------------------
void GmoBoxSystem::reposition_slices_and_shapes(const vector<LUnits>& yOrgShifts,
                                                const vector<LUnits>& heights,
                                                const vector<LUnits>& barlinesHeight,
                                                const vector<vector<LUnits>>& relStaffTopPositions,
                                                LUnits bottomMarginIncr,
                                                SystemLayouter* pSysLayouter)

{
    vector<GmoBox*>::iterator it;
    for (it=m_childBoxes.begin(); it != m_childBoxes.end(); ++it)
    {
        GmoBoxSlice* pSlice = static_cast<GmoBoxSlice*>(*it);
        pSlice->reposition_slices_and_shapes(yOrgShifts, heights, barlinesHeight,
                                             relStaffTopPositions, bottomMarginIncr,
                                             pSysLayouter);
    }

    //increase height
    m_size.height += (yOrgShifts.back() + bottomMarginIncr);
}

//---------------------------------------------------------------------------------------
void GmoBoxSystem::reposition_slices(USize shift)
{
    vector<GmoBox*>::iterator it;
    for (it=m_childBoxes.begin(); it != m_childBoxes.end(); ++it)
    {
        GmoBoxSlice* pSlice = static_cast<GmoBoxSlice*>(*it);
        pSlice->shift_origin(shift);
    }

    //shift origin
    m_origin.y += shift.height;
}

//---------------------------------------------------------------------------------------
void GmoBoxSystem::remove_free_space_at_bottom_and_adjust_slices()
{
    //remove free space at bottom
    LUnits space = m_uFreeAtBottom;
    set_height( get_height() - m_uFreeAtBottom );
    m_uFreeAtBottom = 0.0f;

    //reduce height of slices
    vector<GmoBox*>::iterator it;
    for (it=m_childBoxes.begin(); it != m_childBoxes.end(); ++it)
    {
        GmoBoxSlice* pSlice = static_cast<GmoBoxSlice*>(*it);
        pSlice->reduce_last_instrument_height(space);
    }
}

//---------------------------------------------------------------------------------------
GmoBoxSliceInstr* GmoBoxSystem::get_first_instr_slice(int iInstr)
{
    GmoBoxSlice* pSlice = static_cast<GmoBoxSlice*>(m_childBoxes[0]);
    return pSlice->get_instr_slice(iInstr);
}

//---------------------------------------------------------------------------------------
GmoBoxSliceInstr* GmoBoxSystem::find_instr_slice_at(LUnits x, LUnits y)
{
    vector<GmoBox*>::iterator it;
    for (it=m_childBoxes.begin(); it != m_childBoxes.end(); ++it)
    {
        GmoBoxSlice* pSlice = static_cast<GmoBoxSlice*>(*it);
        URect bbox = pSlice->get_bounds();
        if (bbox.contains(x, y))
            return pSlice->find_instr_slice_at(x, y);
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------
GmoBoxSliceStaff* GmoBoxSystem::get_first_slice_staff_for(int iInstr, int iStaff)
{
    GmoBoxSlice* pSlice = static_cast<GmoBoxSlice*>(m_childBoxes[0]);
    return pSlice->get_slice_staff_for(iInstr, iStaff);
}

//---------------------------------------------------------------------------------------
void GmoBoxSystem::add_num_staves_for_instrument(int staves)
{
    if (m_firstStaff.size() == 0)
        m_firstStaff.push_back(staves);
    else
        m_firstStaff.push_back( m_firstStaff.back() + staves);
}

//---------------------------------------------------------------------------------------
int GmoBoxSystem::instr_number_for_staff(int absStaff)
{
    int maxInstr = int( m_firstStaff.size() );
    for (int i=0; i < maxInstr; ++i)
    {
        if (absStaff < m_firstStaff[i])
            return i;
    }
    return maxInstr;
}

//---------------------------------------------------------------------------------------
int GmoBoxSystem::staff_number_for(int absStaff, int UNUSED(iInstr))
{
    if (absStaff < m_firstStaff[0])
        return absStaff;

    int staff = absStaff;
    int maxInstr = int( m_firstStaff.size() );
    for (int i=1; i < maxInstr; ++i)
    {
        if (staff < m_firstStaff[i])
            return staff - m_firstStaff[i-1];
    }
    return staff - m_firstStaff.back();
}

//---------------------------------------------------------------------------------------
int GmoBoxSystem::staff_at(LUnits y)
{
    //The y coordinate should be within system box limits.

    int maxStaff = int( m_staffShapes.size() );
    int iStaff = 0;
    GmoShapeStaff* pStaff = m_staffShapes[iStaff++];
    LUnits bottomPrev = pStaff->get_bottom();
    while (iStaff < maxStaff)
    {
        GmoShapeStaff* pStaff = m_staffShapes[iStaff++];
        LUnits limit = bottomPrev + (pStaff->get_top() - bottomPrev) / 2.0f;
        if (y < limit)
            return iStaff-2;
        bottomPrev = pStaff->get_bottom();
    }
    return maxStaff-1;
}

//---------------------------------------------------------------------------------------
TimeUnits GmoBoxSystem::start_time()
{
    return  m_pGridTable->start_time();
}

//---------------------------------------------------------------------------------------
TimeUnits GmoBoxSystem::end_time()
{
    return  m_pGridTable->end_time();
}

//---------------------------------------------------------------------------------------
LUnits GmoBoxSystem::get_x_for_note_rest_at_time(TimeUnits timepos)
{
    return m_pGridTable->get_x_for_note_rest_at_time(timepos);
}

//---------------------------------------------------------------------------------------
LUnits GmoBoxSystem::get_x_for_barline_at_time(TimeUnits timepos)
{
    return m_pGridTable->get_x_for_barline_at_time(timepos);
}

//---------------------------------------------------------------------------------------
string GmoBoxSystem::dump_timegrid_table()
{
    return m_pGridTable->dump();
}

//---------------------------------------------------------------------------------------
int GmoBoxSystem::get_num_instruments()
{
    ImoScore* pScore = static_cast<ImoScore*>( get_creator_imo() );
    return pScore->get_num_instruments();
}

//---------------------------------------------------------------------------------------
LUnits GmoBoxSystem::tenths_to_logical(Tenths value, int iInstr, int iStaff)
{
    GmoShapeStaff* pStaff = get_staff_shape(iInstr, iStaff);
    return (value * pStaff->get_staff_line_spacing()) / 10.0f;
}

//---------------------------------------------------------------------------------------
void GmoBoxSystem::add_barline_info(int iMeasure, int iInstr)
{
	if (m_iFirstMeasure[iInstr] == -1)
	    m_iFirstMeasure[iInstr] = iMeasure;

    m_nMeasures[iInstr]++;
}

//---------------------------------------------------------------------------------------
int GmoBoxSystem::get_first_measure(int iInstr)
{
    return m_iFirstMeasure[iInstr];
}

//---------------------------------------------------------------------------------------
int GmoBoxSystem::get_num_measures(int iInstr)
{
    return m_nMeasures[iInstr];
}

//---------------------------------------------------------------------------------------
string GmoBoxSystem::dump_measures_info()
{
    stringstream s;
    s << endl << "BoxSystem (first, total):";
    vector<int>::iterator it;
    int iInstr = 0;
    for (it = m_nMeasures.begin(); it != m_nMeasures.end(); ++it, ++iInstr)
    {
        s << " (" << fixed << setprecision(2) << m_iFirstMeasure[iInstr] << ", "
          << *it << ")";
    }
    s << endl;
    return s.str();
}

//---------------------------------------------------------------------------------------
void GmoBoxSystem::draw_box_bounds(Drawer* pDrawer, double xorg, double yorg, Color& color)
{
    GmoBox::draw_box_bounds(pDrawer, xorg, yorg, color);

    //draw free space limits
    if (m_uFreeAtTop != 0.0f || m_uFreeAtBottom != 0.0f)
    {
        pDrawer->begin_path();
        pDrawer->fill( Color(255, 255, 255, 0) );     //background white transparent
        pDrawer->stroke( color );
        pDrawer->stroke_width(20.0);    //0.2 mm
        if (m_uFreeAtTop != 0.0f)
        {
            pDrawer->move_to(xorg, yorg + m_uFreeAtTop);
            pDrawer->hline_to(xorg + get_width());
        }
        if (m_uFreeAtBottom != 0.0f)
        {
            pDrawer->move_to(xorg, yorg + get_height() - m_uFreeAtBottom);
            pDrawer->hline_to(xorg + get_width());
        }
        pDrawer->end_path();
    }
}


}  //namespace lomse
