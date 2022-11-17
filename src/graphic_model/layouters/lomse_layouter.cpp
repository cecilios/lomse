//---------------------------------------------------------------------------------------
// This file is part of the Lomse library.
// Copyright (c) 2010-present, Lomse Developers
//
// Licensed under the MIT license.
//
// See LICENSE and NOTICE.md files in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#include "lomse_layouter.h"

#include "lomse_gm_basic.h"
#include "lomse_internal_model.h"
#include "lomse_blocks_container_layouter.h"
#include "lomse_score_layouter.h"
#include "lomse_inlines_container_layouter.h"
#include "lomse_table_layouter.h"
#include "lomse_logger.h"
#include "private/lomse_document_p.h"

namespace lomse
{

//=======================================================================================
// helper Visitor for initializing ViewOptions for scores with default values
//=======================================================================================
class ViewOptionsVisitor : public Visitor<ImoScore>
{
protected:
    std::unordered_map<ImoId, ScoreLayoutOptions*>& m_map;

public:
    ViewOptionsVisitor(std::unordered_map<ImoId, ScoreLayoutOptions*>& optsMap)
        : Visitor<ImoScore>(), m_map(optsMap) {}

    void start_visit(ImoScore* pImo) override
    {
        ImoId id = pImo->get_id();
        ScoreLayoutOptions* pOpts = LOMSE_NEW ScoreLayoutOptions(pImo);
        m_map[id] = pOpts;
    }
};


//=======================================================================================
// ViewOptions implementation
//=======================================================================================
void ViewOptions::initialize(Document* pDoc)
{
    ImoDocument* pImoDoc = pDoc->get_im_root();
    if (pImoDoc)
    {
        ViewOptionsVisitor v(m_scoreOptions);
        pImoDoc->accept_visitor(v);
    }
}

//---------------------------------------------------------------------------------------
ScoreLayoutOptions* ViewOptions::get_score_options(ImoId scoreId)
{
	unordered_map<ImoId, ScoreLayoutOptions*>::iterator it = m_scoreOptions.find(scoreId);
	if (it != m_scoreOptions.end())
		return it->second;
    else
        return nullptr;
}

//---------------------------------------------------------------------------------------
void ViewOptions::update_doc_model(DocModel* pModel)
{
    for (auto a : m_scoreOptions)
        a.second->update_doc_model(pModel);
}

//---------------------------------------------------------------------------------------
bool ViewOptions::select_layout(ImoScore* pScore, const std::string& layoutName)
{
    ScoreLayoutOptions* opts = get_score_options(pScore->get_id());
    if (!opts)
        return false;

    ImoScoreLayout* pLayout = pScore->get_score_layout(layoutName);
    if (!pLayout)
        return false;

    opts->select_score_layout(pLayout->get_id());
    return true;    //success
}

//---------------------------------------------------------------------------------------
bool ViewOptions::select_layout(ImoScore* pScore, ImoId idLayout)
{
    ScoreLayoutOptions* opts = get_score_options(pScore->get_id());
    if (!opts)
        return false;

    opts->select_score_layout(idLayout);
    return true;    //success
}


//=======================================================================================
// ScoreLayoutOptions implementation
//=======================================================================================
ScoreLayoutOptions::ScoreLayoutOptions(ImoScore* pScore)
{
    m_idScoreLayout = pScore->get_score_layout()->get_id();
    m_pModel = pScore->get_doc_model();
}

//---------------------------------------------------------------------------------------
ImoScoreLayout* ScoreLayoutOptions::get_score_layout()
{
    return static_cast<ImoScoreLayout*>( m_pModel->get_pointer_to_imo(m_idScoreLayout) );
}

//=======================================================================================
// Layouter implementation
//=======================================================================================
Layouter::Layouter(ImoContentObj* pItem, Layouter* pParent, GraphicModel* pGModel,
                   LibraryScope& libraryScope, ViewOptions* pOptions, ImoStyles* pStyles,
                   bool fAddShapesToModel)
    : m_result(k_layout_not_finished)
    , m_pGModel(pGModel)
    , m_pParentLayouter(pParent)
    , m_libraryScope(libraryScope)
    , m_pOptions(pOptions)
    , m_pStyles(pStyles)
    , m_pItemMainBox(nullptr)
    , m_pCurLayouter(nullptr)
    , m_pItem(pItem)
    , m_fAddShapesToModel(fAddShapesToModel)
    , m_constrains(0)
    , m_availableWidth(0.0f)
    , m_availableHeight(0.0f)
{
}

//---------------------------------------------------------------------------------------
// constructor for DocumentLayouter
Layouter::Layouter(LibraryScope& libraryScope)
    : m_result(k_layout_not_finished)
    , m_pGModel(nullptr)
    , m_pParentLayouter(nullptr)
    , m_libraryScope(libraryScope)
    , m_pStyles(nullptr)
    , m_pItemMainBox(nullptr)
    , m_pCurLayouter(nullptr)
    , m_pItem(nullptr)
    , m_fAddShapesToModel(false)
    , m_constrains(0)
    , m_availableWidth(0.0f)
    , m_availableHeight(0.0f)
{
}

//---------------------------------------------------------------------------------------
GmoBox* Layouter::start_new_page()
{
    LOMSE_LOG_DEBUG(Logger::k_layout, string(""));

    GmoBox* pParentBox = m_pParentLayouter->start_new_page();

    m_pageCursor = m_pParentLayouter->get_cursor();
    m_availableWidth = m_pParentLayouter->get_available_width();
    m_availableHeight = m_pParentLayouter->get_available_height();

    create_main_box(pParentBox, m_pageCursor, m_availableWidth, m_availableHeight);

    return m_pItemMainBox;
}

//---------------------------------------------------------------------------------------
int Layouter::layout_item(ImoContentObj* pItem, GmoBox* pParentBox, int constrains,
                          ViewOptions* pOptions)
{
    LOMSE_LOG_DEBUG(Logger::k_layout,
        "Laying out id %d %s", pItem->get_id(), pItem->get_name().c_str());

    m_pCurLayouter = create_layouter(pItem, pOptions);
    m_pCurLayouter->set_constrains(constrains);

    m_pCurLayouter->prepare_to_start_layout();
    while (!m_pCurLayouter->is_item_layouted())
    {
        m_pCurLayouter->create_main_box(pParentBox, m_pageCursor,
                                        m_availableWidth, m_availableHeight);
        m_pCurLayouter->layout_in_box();
        m_pCurLayouter->set_box_height();

        if (!m_pCurLayouter->is_item_layouted())
        {
            pParentBox = start_new_page();
        }
    }

    int result = m_pCurLayouter->get_layout_result();
    if (result != k_layout_failed_auto_scale)
    {
        m_pCurLayouter->add_end_margins();

        //update cursor and available space
        GmoBox* pChildBox = m_pCurLayouter->get_item_main_box();
        if (pChildBox)  //AWARE: NullLayouter does not create a box
        {
            m_pageCursor.y = pChildBox->get_bottom();
            m_availableHeight -= pChildBox->get_height();
        }

        if (!pItem->is_score())
            delete m_pCurLayouter;
    }
    return result;
}

//---------------------------------------------------------------------------------------
void Layouter::set_cursor_and_available_space()
{
    m_pageCursor.x = m_pItemMainBox->get_content_left();
    m_pageCursor.y = m_pItemMainBox->get_content_top();

    m_availableWidth = m_pItemMainBox->get_content_width();
    m_availableHeight = m_pItemMainBox->get_content_height();

    LOMSE_LOG_DEBUG(Logger::k_layout,
        "cursor at(%.2f, %.2f), available space(%.2f, %.2f)",
        m_pageCursor.x, m_pageCursor.y, m_availableWidth, m_availableHeight);
}

//---------------------------------------------------------------------------------------
LUnits Layouter::set_box_height()
{
    LUnits start = m_pItemMainBox->get_origin().y;
    LUnits height = m_pageCursor.y - start;
    ImoStyle* pStyle = m_pItem->get_style();
    if (pStyle)
        height = max(pStyle->min_height(), height);
    m_pItemMainBox->set_height(height);
    return height;
}

//---------------------------------------------------------------------------------------
void Layouter::add_end_margins()
{
    ImoStyle* pStyle = m_pItem->get_style();
    if (pStyle && m_pItemMainBox)   //NullLayouter doesn't have main box
    {
        LUnits space = pStyle->margin_bottom()
                       + pStyle->border_width_bottom()
                       + pStyle->padding_bottom();
        m_pItemMainBox->set_height( m_pItemMainBox->get_height() + space );
        m_pageCursor.y += space;
    }
}

//---------------------------------------------------------------------------------------
Layouter* Layouter::create_layouter(ImoContentObj* pItem, ViewOptions* pOptions,
                                    int constrains)
{
    Layouter* pLayouter = LayouterFactory::create_layouter(pItem, this, pOptions);
    if (pItem->is_score())
    {
        save_score_layouter(pLayouter);
        pLayouter->set_constrains(constrains);
    }
    return pLayouter;
}




//=======================================================================================
// LayouterFactory implementation
//=======================================================================================
Layouter* LayouterFactory::create_layouter(ImoContentObj* pItem, Layouter* pParent,
                                           ViewOptions* pOptions)
{
    GraphicModel* pGModel = pParent->get_graphic_model();
    LibraryScope& libraryScope = pParent->get_library_scope();
    ImoStyles* pStyles = pParent->get_styles();
    bool fAddShapesToModel
        = compute_value_for_add_shapes_flag(pItem, pParent->must_add_shapes_to_model());

    switch (pItem->get_obj_type())
    {
        //blocks container objects
        case k_imo_dynamic:
        case k_imo_content:
            return LOMSE_NEW ContentLayouter(pItem, pParent, pGModel, libraryScope,
                                             pOptions, pStyles, fAddShapesToModel);

        case k_imo_multicolumn:
            return LOMSE_NEW MultiColumnLayouter(pItem, pParent, pGModel, libraryScope,
                                                 pOptions, pStyles, fAddShapesToModel);

        case k_imo_list:
            return LOMSE_NEW ListLayouter(pItem, pParent, pGModel, libraryScope,
                                          pOptions, pStyles, fAddShapesToModel);

        case k_imo_listitem:
            return LOMSE_NEW ListItemLayouter(pItem, pParent, pGModel, libraryScope,
                                              pOptions, pStyles, fAddShapesToModel);

        case k_imo_table_cell:
            return LOMSE_NEW TableCellLayouter(pItem, pParent, pGModel, libraryScope,
                                               pOptions, pStyles);    //never adds shapes, until row ready

        case k_imo_score:
            return LOMSE_NEW ScoreLayouter(pItem, pParent, pGModel, libraryScope,
                                           pOptions);

        case k_imo_table:
            return LOMSE_NEW TableLayouter(pItem, pParent, pGModel, libraryScope,
                                           pOptions, pStyles, fAddShapesToModel);

        // inlines container objects
        case k_imo_anonymous_block:
        case k_imo_para:
        case k_imo_heading:
            return LOMSE_NEW InlinesContainerLayouter(pItem, pParent, pGModel, libraryScope,
                                                      pOptions, pStyles, fAddShapesToModel);

        default:
            return LOMSE_NEW NullLayouter(pItem, pParent, pGModel, libraryScope);
    }
}

//---------------------------------------------------------------------------------------
bool LayouterFactory::compute_value_for_add_shapes_flag(ImoContentObj* pItem,
                                                        bool fInheritedValue)
{
    //children of main content block always add shapes
    ImoObj* pParent = pItem->get_parent_imo();
    if (pParent->is_content() && pParent->get_parent_imo()->is_document())
        return true;

    //objects inside a table cell never add shapes. Table cell shapes wiil
    //be added when the table row is fully laid out.
    if (pParent->is_table_cell())
        return false;

    //otherwise inherit from parent
    return fInheritedValue;
}


}  //namespace lomse
