//---------------------------------------------------------------------------------------
// This file is part of the Lomse library.
// Copyright (c) 2010-present, Lomse Developers
//
// Licensed under the MIT license.
//
// See LICENSE and NOTICE.md files in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#ifndef __LOMSE_LAYOUTER_H__        //to avoid nested includes
#define __LOMSE_LAYOUTER_H__

#include <unordered_map>

#include "lomse_basic.h"
#include "lomse_injectors.h"

namespace lomse
{

//forward declarations
class DocModel;
class ImoContentObj;
class GraphicModel;
class GmoBox;
class ImoScoreLayout;
class ImoStyles;
class ScoreLayoutOptions;


#define LOMSE_INFINITE_LENGTH   11000000.0f //1.1x10^7 LU = 110 m = 523 DIN A4 vertical pages

//---------------------------------------------------------------------------------------
/** %ScoreLayoutOptions: encloses the layout and rendering options for a music score
*/
class ScoreLayoutOptions
{
protected:
    ImoId m_idScoreLayout = k_no_imoid;
    DocModel* m_pModel = nullptr;

public:
    ScoreLayoutOptions() {}
    ScoreLayoutOptions(ImoScore* pScore);

    ImoScoreLayout* get_score_layout();
    inline void select_score_layout(ImoId id) { m_idScoreLayout = id; }

    inline void update_doc_model(DocModel* pModel) { m_pModel = pModel; }
};

//---------------------------------------------------------------------------------------
/** %ViewOptions: encloses the layout and rendering options for a View
*/
class ViewOptions
{
protected:
    int m_constrains = 0;
    LUnits m_viewWidth = 0.0f;
    std::unordered_map<ImoId, ScoreLayoutOptions*> m_scoreOptions;    //options for each score

public:
    ViewOptions() {}
    ViewOptions(Document* pDoc) { initialize(pDoc); }

    //set default options
    void initialize(Document* pDoc);

    //accessors
    ScoreLayoutOptions* get_score_options(ImoId scoreId);

    //update ptr to DocModel to use
    void update_doc_model(DocModel* pModel);

    //layout options
    bool select_layout(ImoScore* pScore, const std::string& layoutName);
    bool select_layout(ImoScore* pScore, ImoId idLayout);

};

//---------------------------------------------------------------------------------------
/** %Layouter: Abstract class to implement the layout algorithm for any document
    content item.
*/
class Layouter
{
protected:
    int m_result;               //value from ELayoutResult
    GraphicModel* m_pGModel;
    Layouter* m_pParentLayouter;
    LibraryScope& m_libraryScope;
    ViewOptions* m_pOptions;
    ImoStyles* m_pStyles;
    GmoBox* m_pItemMainBox;     //this layouter main box
    Layouter* m_pCurLayouter;
    ImoContentObj* m_pItem;
    bool m_fAddShapesToModel;
    int m_constrains;

    //position (relative to page origin) and available space in current box
    LUnits m_availableWidth;
    LUnits m_availableHeight;
    UPoint m_pageCursor;

    //constructor
    Layouter(ImoContentObj* pItem, Layouter* pParent, GraphicModel* pGModel,
             LibraryScope& libraryScope, ViewOptions* pOptions, ImoStyles* pStyles,
             bool fAddShapesToModel);
    //constructor for DocumentLayouter
    Layouter(LibraryScope& libraryScope);

public:
    virtual ~Layouter() {}

    enum ELayoutResult
    {
        k_layout_not_finished = 0,
        k_layout_success,
        k_layout_failed_auto_scale,        //auto-scaling applied. Need to re-layout
    };

    virtual void layout_in_box() = 0;
    virtual void prepare_to_start_layout() { m_result = k_layout_not_finished; }
    virtual bool is_item_layouted() { return m_result != k_layout_not_finished; }
    virtual void set_layout_result(int value) { m_result = value; }
    virtual int get_layout_result() { return m_result; }
    virtual void create_main_box(GmoBox* pParentBox, UPoint pos, LUnits width,
                                 LUnits height) = 0;
    virtual void save_score_layouter(Layouter* pLayouter) {
        m_pParentLayouter->save_score_layouter(pLayouter);
    }
    inline void set_constrains(int constrains) { m_constrains = constrains; }

    inline GraphicModel* get_graphic_model() { return m_pGModel; }
    inline LibraryScope& get_library_scope() { return m_libraryScope; }
    inline ImoStyles* get_styles() { return m_pStyles; }
    inline ViewOptions* get_options() { return m_pOptions; }

    void add_end_margins();
    LUnits set_box_height();
    inline GmoBox* get_item_main_box() { return m_pItemMainBox; }

    inline bool must_add_shapes_to_model() { return m_fAddShapesToModel; }

protected:
    virtual GmoBox* start_new_page();

    Layouter* create_layouter(ImoContentObj* pItem, ViewOptions* pOptions, int constrains=0);
    int layout_item(ImoContentObj* pItem, GmoBox* pParentBox, int constrains,
                    ViewOptions* pOptions);

    void set_cursor_and_available_space();

    inline UPoint get_cursor() { return m_pageCursor; }
    inline LUnits get_available_width() { return m_availableWidth; }
    inline LUnits get_available_height() { return m_availableHeight; }
};


//----------------------------------------------------------------------------------
class LayouterFactory
{
public:
    LayouterFactory() {}
    ~LayouterFactory() {}

    static Layouter* create_layouter(ImoContentObj* pImo, Layouter* pParent,
                                     ViewOptions* pOptions);

private:
    static bool compute_value_for_add_shapes_flag(ImoContentObj* pItem,
                                                  bool fInheritedValue);
};


//----------------------------------------------------------------------------------
class NullLayouter : public Layouter
{
protected:

public:
    NullLayouter(ImoContentObj* pItem, Layouter* pParent, GraphicModel* pGModel,
                 LibraryScope& libraryScope)
        : Layouter(pItem, pParent, pGModel, libraryScope, nullptr, nullptr, false)
    {
    }
    ~NullLayouter() {}

    void layout_in_box() override {}
    bool is_item_layouted() override { return true; }
    void create_main_box(GmoBox* UNUSED(pParentBox), UPoint UNUSED(pos),
                         LUnits UNUSED(width), LUnits UNUSED(height)) override {}
};


}   //namespace lomse

#endif    // __LOMSE_LAYOUTER_H__

