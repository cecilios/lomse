//---------------------------------------------------------------------------------------
// This file is part of the Lomse library.
// Copyright (c) 2010-present, Lomse Developers
//
// Licensed under the MIT license.
//
// See LICENSE and NOTICE.md files in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#ifndef __LOMSE_PEDAL_ENGRAVER_H__
#define __LOMSE_PEDAL_ENGRAVER_H__

#include "lomse_basic.h"
#include "lomse_engraver.h"

namespace lomse
{

//forward declarations
class ImoPedalMark;
class ImoPedalLine;
class ImoDirection;
class GmoShapeInvisible;
class GmoShapePedalLine;
class GmoShapePedalGlyph;
class ScoreMeter;
class InstrumentEngraver;
class VerticalProfile;

//---------------------------------------------------------------------------------------
class PedalMarkEngraver : public AuxObjEngraver
{
public:
    PedalMarkEngraver(const EngraverContext& ctx);

    GmoShapePedalGlyph* create_shape(ImoPedalMark* pPedalMark, UPoint pos,
                                     const Color& color,
                                     GmoShape* pParentShape);

protected:
    double determine_font_size() override;
    static int find_glyph(const ImoPedalMark* pPedalMark);
    LUnits determine_y_pos(LUnits xLeft, LUnits xRight, LUnits yCurrent, LUnits yBaselineOffset, bool fAbove) const;
};

//---------------------------------------------------------------------------------------
class PedalLineEngraver : public RelObjEngraver
{
protected:
    int m_numShapes = 0;
    ImoPedalLine* m_pPedal = nullptr;
    bool m_fPedalAbove = false;

    ImoDirection* m_pStartDirection = nullptr;
    GmoShapeInvisible* m_pStartDirectionShape = nullptr;
    GmoShapeInvisible* m_pEndDirectionShape = nullptr;
    std::vector<GmoShapeInvisible*> m_pedalChangeDirectionShapes;

    const GmoShapePedalGlyph* m_pPedalStartShape = nullptr;
    const GmoShapePedalGlyph* m_pPedalEndShape = nullptr;

    LUnits m_xStart = 0;
    LUnits m_xEnd = 0;
    LUnits m_lineY = 0;

public:
    PedalLineEngraver(LibraryScope& libraryScope, ScoreMeter* pScoreMeter);

    void set_start_staffobj(ImoRelObj* pRO, const AuxObjContext& aoc) override;
    void set_middle_staffobj(ImoRelObj* pRO, const AuxObjContext& aoc) override;
    void set_end_staffobj(ImoRelObj* pRO, const AuxObjContext& aoc) override;

    //RelObjEngraver mandatory overrides
    GmoShape* create_first_or_intermediate_shape(const RelObjEngravingContext& ctx) override;
    GmoShape* create_last_shape(const RelObjEngravingContext& ctx) override;

protected:
    double determine_font_size() override;

    void decide_placement();

    GmoShape* create_shape();
    void compute_shape_position(bool first);
    LUnits determine_shape_position_left(bool first) const;
    LUnits determine_shape_position_right() const;
    LUnits determine_default_base_line_y() const;
    void adjust_vertical_position(LUnits xLeft, LUnits xRight, LUnits height, GmoShapePedalLine* pMainShape = nullptr);

    void add_pedal_changes(GmoShapePedalLine* pMainShape);
    void add_pedal_continuation_text(GmoShapePedalLine* pMainShape);
    void add_pedal_continuation_part_shape(GmoShapePedalLine* pMainShape, GmoShape* pAddedShape);

    void add_line_info(GmoShapePedalLine* pMainShape, const GmoShape* pPedalStartShape, const GmoShape* pPedalEndShape, bool start);
};


}   //namespace lomse

#endif    // __LOMSE_PEDAL_ENGRAVER_H__

