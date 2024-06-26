//---------------------------------------------------------------------------------------
// This file is part of the Lomse library.
// Copyright (c) 2010-present, Lomse Developers
//
// Licensed under the MIT license.
//
// See LICENSE and NOTICE.md files in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#ifndef __LOMSE_TUPLET_ENGRAVER_H__        //to avoid nested includes
#define __LOMSE_TUPLET_ENGRAVER_H__

#include "lomse_basic.h"
#include "lomse_injectors.h"
#include "lomse_engraver.h"
#include <list>
using namespace std;

namespace lomse
{

//forward declarations
class ImoObj;
class GmoShapeTuplet;
class GmoShapeBeam;
class ScoreMeter;
class ImoTuplet;
class ImoNote;
class GmoShapeNote;
class ImoNoteRest;
class GmoShape;
class ImoStyle;


//---------------------------------------------------------------------------------------
class TupletEngraver : public RelObjEngraver
{
protected:
    int m_numShapes;
    GmoShapeTuplet* m_pTupletShape;
    string m_label;
    ImoStyle* m_pStyle;
    ImoTuplet* m_pTuplet;
    std::list< pair<ImoNoteRest*, GmoShape*> > m_noteRests;
    bool m_fDrawBracket;
    bool m_fDrawNumber;
    bool m_fAbove;

public:
    TupletEngraver(LibraryScope& libraryScope, ScoreMeter* pScoreMeter);
    ~TupletEngraver();

    //implementation of virtual methods from RelObjEngraver
    void set_start_staffobj(ImoRelObj* pRO, const AuxObjContext& aoc) override;
    void set_middle_staffobj(ImoRelObj* pRO, const AuxObjContext& aoc) override;
    void set_end_staffobj(ImoRelObj* pRO, const AuxObjContext& aoc) override;

    //RelObjEngraver mandatory overrides
    GmoShape* create_first_or_intermediate_shape(const RelObjEngravingContext& ctx) override;
    GmoShape* create_last_shape(const RelObjEngravingContext& ctx) override;

protected:
    void add_text_shape();
    bool check_if_single_beamed_group();

    ImoNoteRest* get_start_noterest() { return m_noteRests.front().first; }
    ImoNoteRest* get_end_noterest() { return m_noteRests.back().first; }

    void compute_y_coordinates(GmoShapeNote* pStart, GmoShapeNote* pEnd);
    GmoShapeNote* get_first_note();
    GmoShapeNote* get_last_note();

    GmoShape* get_start_noterest_shape() { return m_noteRests.front().second; }
    GmoShape* get_end_noterest_shape() { return m_noteRests.back().second; }

    LUnits m_yStart, m_yEnd;

    void decide_if_show_bracket();
    void decide_tuplet_placement();
    void determine_tuplet_text();
    void create_shape();
    void set_shape_details();
    int count_nested_tuplets();

};


}   //namespace lomse

#endif    // __LOMSE_TUPLET_ENGRAVER_H__

