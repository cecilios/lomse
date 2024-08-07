//---------------------------------------------------------------------------------------
// This file is part of the Lomse library.
// Copyright (c) 2010-present, Lomse Developers
//
// Licensed under the MIT license.
//
// See LICENSE and NOTICE.md files in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#include "lomse_staffobjs_cursor.h"

#include "lomse_internal_model.h"


namespace lomse
{


//---------------------------------------------------------------------------------------
StaffObjsCursor::StaffObjsCursor(ImoScore* pScore)
    : m_pColStaffObjs( pScore->get_staffobjs_table() )
    , m_it(m_pColStaffObjs->begin())
    , m_savedPos(m_it)
    , m_numInstruments( pScore->get_num_instruments() )
    , m_numLines( m_pColStaffObjs->num_lines() )
    , m_fScoreIsEmpty( is_end() )
    , m_pLastBarline(nullptr)
{
    initialize_clefs_keys_times(pScore);
}

//---------------------------------------------------------------------------------------
void StaffObjsCursor::initialize_clefs_keys_times(ImoScore* pScore)
{
    m_staffIndex.resize(m_numInstruments);
    m_numStaves = 0;
    for (int i=0; i < m_numInstruments; ++i)
    {
        m_staffIndex[i] = m_numStaves;
        m_numStaves += pScore->get_instrument(i)->get_num_staves();
    }

    m_clefs.assign(m_numStaves, (ColStaffObjsEntry*)nullptr);     //GCC complains if nullptr not casted
    m_keys.assign(m_numStaves, (ColStaffObjsEntry*)nullptr);
    m_times.assign(m_numInstruments, (ColStaffObjsEntry*)nullptr);
    m_octave_shifts.assign(m_numStaves, 0);
}

//---------------------------------------------------------------------------------------
void StaffObjsCursor::move_next()
{
    ImoObj* pSO = imo_object();
    if (pSO->is_clef())
        save_clef();
    else if (pSO->is_key_signature())
        save_key_signature();
    else if (pSO->is_time_signature())
        save_time_signature();
    else if (pSO->is_barline())
        save_barline();
    else if (pSO->is_note_rest())
        save_octave_shift_at_end( static_cast<ImoStaffObj*>(pSO) );

    ++m_it;

    if (!is_end())
    {
        pSO = imo_object();
        if (pSO && pSO->is_note_rest())
            save_octave_shift_at_start( static_cast<ImoStaffObj*>(pSO) );
    }
}

//---------------------------------------------------------------------------------------
void StaffObjsCursor::save_clef()
{
    int iInstr = num_instrument();
    int idx = m_staffIndex[iInstr] + staff();
    m_clefs[idx] = *m_it;
}

//---------------------------------------------------------------------------------------
void StaffObjsCursor::save_octave_shift_at_end(ImoStaffObj* pSO)
{
    if (pSO->get_num_relations() > 0)
    {
        ImoRelations* pRels = pSO->get_relations();
        list<ImoRelObj*>& relobjs = pRels->get_relobjs();
        if (relobjs.size() > 0)
        {
            list<ImoRelObj*>::iterator it;
            for (it = relobjs.begin(); it != relobjs.end(); ++it)
            {
                ImoRelObj* pRO = static_cast<ImoRelObj*>(*it);
                if (pRO->is_octave_shift())
                {
                    if (pSO == pRO->get_end_object())
                    {
                        int idx = m_staffIndex[num_instrument()] + staff();
                        m_octave_shifts[idx] = 0;
                        break;
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------
void StaffObjsCursor::save_octave_shift_at_start(ImoStaffObj* pSO)
{
    int steps = 0;
    bool fOctaveShift = false;
    if (pSO->get_num_relations() > 0)
    {
        ImoRelations* pRels = pSO->get_relations();
        list<ImoRelObj*>& relobjs = pRels->get_relobjs();
        if (relobjs.size() > 0)
        {
            list<ImoRelObj*>::iterator it;
            for (it = relobjs.begin(); it != relobjs.end(); ++it)
            {
                ImoRelObj* pRO = static_cast<ImoRelObj*>(*it);
                if (pRO->is_octave_shift())
                {
                    if (pSO == pRO->get_start_object())
                    {
                        fOctaveShift = true;
                        steps = static_cast<ImoOctaveShift*>(pRO)->get_shift_steps();
                    }
                }
            }
        }
    }

    if (fOctaveShift)
    {
        int idx = m_staffIndex[num_instrument()] + staff();
        m_octave_shifts[idx] = steps;
    }
}

//---------------------------------------------------------------------------------------
void StaffObjsCursor::save_key_signature()
{
    int iInstr = num_instrument();
    int idx = m_staffIndex[iInstr] + staff();
    m_keys[idx] = *m_it;
}

//---------------------------------------------------------------------------------------
void StaffObjsCursor::save_barline()
{
    m_pLastBarline = dynamic_cast<ImoBarline*>( imo_object() );
}

//---------------------------------------------------------------------------------------
void StaffObjsCursor::save_time_signature()
{
    int iInstr = num_instrument();
    m_times[iInstr] = *m_it;
}

//---------------------------------------------------------------------------------------
ImoStaffObj* StaffObjsCursor::get_staffobj()
{
    return dynamic_cast<ImoStaffObj*>( imo_object() );
}

//---------------------------------------------------------------------------------------
ColStaffObjsEntry* StaffObjsCursor::get_clef_entry_for_instr_staff(int iInstr, int iStaff)
{
    int idx = m_staffIndex[iInstr] + iStaff;
    return m_clefs[idx];
}

//---------------------------------------------------------------------------------------
ImoClef* StaffObjsCursor::get_clef_for_instr_staff(int iInstr, int iStaff)
{
    ColStaffObjsEntry* pEntry = get_clef_entry_for_instr_staff(iInstr, iStaff);
    if (pEntry)
        return dynamic_cast<ImoClef*>( pEntry->imo_object() );
    else
        return nullptr;
}

//---------------------------------------------------------------------------------------
int StaffObjsCursor::get_clef_type_for_instr_staff(int iInstr, int iStaff)
{
    ImoClef* pClef = get_clef_for_instr_staff(iInstr, iStaff);
    if (pClef)
        return pClef->get_clef_type();
    else
        return k_clef_undefined;
}

//---------------------------------------------------------------------------------------
ImoClef* StaffObjsCursor::get_applicable_clef()
{
    if (m_fScoreIsEmpty)
        return nullptr;
    else
        return get_clef_for_instr_staff( num_instrument(), staff() );
}

//---------------------------------------------------------------------------------------
int StaffObjsCursor::get_applicable_clef_type()
{
    if (m_fScoreIsEmpty)
        return k_clef_undefined;
    else
        return get_clef_type_for_instr_staff( num_instrument(), staff() );
}

//---------------------------------------------------------------------------------------
int StaffObjsCursor::get_applicable_octave_shift()
{
    int idx = m_staffIndex[num_instrument()] + staff();
    return m_octave_shifts[idx];
}

//---------------------------------------------------------------------------------------
ColStaffObjsEntry* StaffObjsCursor::get_key_entry_for_instr_staff(int iInstr, int iStaff)
{
    int idx = m_staffIndex[iInstr] + iStaff;
    return m_keys[idx];
}

//---------------------------------------------------------------------------------------
ImoKeySignature* StaffObjsCursor::get_key_for_instr_staff(int iInstr, int iStaff)
{
    ColStaffObjsEntry* pEntry = get_key_entry_for_instr_staff(iInstr, iStaff);
    if (pEntry)
        return dynamic_cast<ImoKeySignature*>( pEntry->imo_object() );
    else
        return nullptr;
}

//---------------------------------------------------------------------------------------
ColStaffObjsEntry* StaffObjsCursor::get_prolog_time_entry_for_instrument(int iInstr)
{
    //return entry for time signature only when the time signature has just been defined,
    //that is, when cursor is pointing just after time entry for this instrument

    ColStaffObjsEntry* pEntry = get_time_entry_for_instrument(iInstr);
    ColStaffObjsEntry* pPrev = prev_entry();
    if (pPrev == pEntry)
        return pEntry;

    //there could be other time signatures for other instruments, so move backwards
    ColStaffObjsIterator it(pPrev);
    while (*it && *it != pEntry &&
           ((*it)->imo_object()->is_time_signature()
            || (*it)->imo_object()->is_key_signature()) )
    {
        --it;
    }

    return (*it == pEntry ? pEntry : nullptr);
}

//---------------------------------------------------------------------------------------
ImoKeySignature* StaffObjsCursor::get_applicable_key()
{
    if (m_fScoreIsEmpty)
        return nullptr;
    else
        return get_key_for_instr_staff( num_instrument(), staff() );
}

//---------------------------------------------------------------------------------------
int StaffObjsCursor::get_applicable_key_type()
{
    if (m_fScoreIsEmpty)
        return k_key_undefined;
    else
        return get_key_type_for_instr_staff( num_instrument(), staff() );
}

//---------------------------------------------------------------------------------------
int StaffObjsCursor::get_key_type_for_instr_staff(int iInstr, int iStaff)
{
    ImoKeySignature* pKey = get_key_for_instr_staff(iInstr, iStaff);
    if (pKey)
        return pKey->get_key_type();
    else
        return k_key_undefined;
}

//---------------------------------------------------------------------------------------
ImoTimeSignature* StaffObjsCursor::get_applicable_time_signature()
{
    if (m_fScoreIsEmpty)
        return nullptr;
    else
        return get_time_signature_for_instrument( num_instrument() );
}

//---------------------------------------------------------------------------------------
ImoTimeSignature* StaffObjsCursor::get_time_signature_for_instrument(int iInstr)
{
    ColStaffObjsEntry* pEntry = get_time_entry_for_instrument(iInstr);
    if (pEntry)
        return dynamic_cast<ImoTimeSignature*>( pEntry->imo_object() );
    else
        return nullptr;
}

//---------------------------------------------------------------------------------------
ColStaffObjsEntry* StaffObjsCursor::get_time_entry_for_instrument(int iInstr)
{
    return m_times[iInstr];
}

//---------------------------------------------------------------------------------------
void StaffObjsCursor::save_position()
{
    m_savedPos = m_it;
}

//---------------------------------------------------------------------------------------
void StaffObjsCursor::go_back_to_saved_position()
{
    m_it = m_savedPos;
}

//---------------------------------------------------------------------------------------
TimeUnits StaffObjsCursor::next_staffobj_timepos()
{
    ColStaffObjsEntry* pNextEntry = next_entry();
    if (pNextEntry)
        return pNextEntry->time();
    else
    {
        ImoStaffObj* pSO = get_staffobj();
        return time() + pSO->get_duration();
    }
}

//---------------------------------------------------------------------------------------
int StaffObjsCursor::num_measures()
{
    if (m_fScoreIsEmpty)
        return 0;

    ColStaffObjsEntry* pLastEntry = m_pColStaffObjs->back();
    return pLastEntry->measure() + 1;
}

//---------------------------------------------------------------------------------------
TimeUnits StaffObjsCursor::score_total_duration()
{
    if (m_fScoreIsEmpty)
        return 0.0f;

    ColStaffObjsEntry* pLastEntry = m_pColStaffObjs->back();
    TimeUnits time = pLastEntry->time();
    ImoStaffObj* pSO = pLastEntry->imo_object();
    time += pSO->get_duration();
    return time;
}

//---------------------------------------------------------------------------------------
void StaffObjsCursor::staff_index_to_instr_staff(int idx, int* iInstr, int* iStaff)
{
    *iInstr = 0;
    *iStaff = idx;
    for (int i=0; i < m_numInstruments; ++i)
    {
        if (idx == m_staffIndex[i])
        {
            *iInstr = i;
            *iStaff = 0;
            return;
        }
        else if (idx < m_staffIndex[i])
        {
            *iInstr = i - 1;
            *iStaff = idx - m_staffIndex[i-1];
            return;
        }
    }
}

//---------------------------------------------------------------------------------------
int StaffObjsCursor::num_staves_for_instrument(int iInstr)
{
    int startIdx = m_staffIndex[iInstr];
    if (iInstr == (m_numInstruments-1))
        return m_numStaves - startIdx;
    else
        return m_staffIndex[iInstr+1] - startIdx;
}

//---------------------------------------------------------------------------------------
vector<int> StaffObjsCursor::get_applicable_clefs_for_instrument(int iInstr)
{
    int numStaves = num_staves_for_instrument(iInstr);
    vector<int> clefs(numStaves);
    int idx = staff_index_for(iInstr, 0);
    for (int i=0; i < numStaves; ++i, ++idx)
    {
        ImoClef* clef = static_cast<ImoClef*>( m_clefs[idx]->imo_object() );
        clefs[i] = clef->get_clef_type();
    }
    return clefs;
}


}  //namespace lomse

