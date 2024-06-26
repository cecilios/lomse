//---------------------------------------------------------------------------------------
// This file is part of the Lomse library.
// Copyright (c) 2010-present, Lomse Developers
//
// Licensed under the MIT license.
//
// See LICENSE and NOTICE.md files in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#ifndef _LOMSE_BUTTON_CTRL_H__
#define _LOMSE_BUTTON_CTRL_H__

#include "lomse_control.h"

namespace lomse
{

//---------------------------------------------------------------------------------------
// A simple, clickable button
class ButtonCtrl : public Control
{
protected:
    string m_label;
    UPoint m_pos;
    LUnits m_width;
    LUnits m_height;
    Color m_normalColor;
    Color m_overColor;
    bool m_fMouseIn;

public:
    ButtonCtrl(LibraryScope& libScope, Control* pParent, Document* pDoc,
               const string& label, LUnits width=-1.0f, LUnits height=-1.0f,
               ImoStyle* pStyle=nullptr);

    //Control mandatory overrides
    USize measure() override;
    GmoBoxControl* layout(LibraryScope& libraryScope, UPoint pos) override;
    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
    void handle_event(SpEventInfo pEvent) override;
    LUnits width() override { return m_width; }
    LUnits height() override { return m_height; }
    LUnits top() override { return m_pos.y; }
    LUnits bottom() override { return m_pos.y + m_height; }
    LUnits left() override { return m_pos.x; }
    LUnits right() override { return m_pos.x + m_width; }

    //specific methods
    void set_label(const string& text);
    void set_tooltip(const string& text);

    //colors
    void set_bg_color(Color color);
    void set_mouse_over_color(Color color);
    void set_mouse_in(bool fValue);

protected:
    GmoBoxControl* m_pMainBox;
    LUnits  m_xLabel;
    LUnits  m_yLabel;

    ImoStyle* create_default_style();
    void center_text();

};


} //namespace lomse

#endif    //_LOMSE_BUTTON_CTRL_H__
