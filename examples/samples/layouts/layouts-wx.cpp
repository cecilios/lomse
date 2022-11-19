//---------------------------------------------------------------------------------------
// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http:unlicense.org>
//---------------------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/app.h>
    #include <wx/frame.h>
    #include <wx/menu.h>
    #include <wx/msgdlg.h>
    #include <wx/filedlg.h>
    #include <wx/image.h>
    #include <wx/dc.h>
    #include <wx/dcmemory.h>
    #include <wx/dcclient.h>
    #include <wx/event.h>
    #include <wx/sizer.h>
    #include <wx/button.h>
    #include <wx/checkbox.h>
    #include <wx/statbox.h>
    #include <wx/radiobox.h>
    #include <wx/combobox.h>
#endif

#include <iostream>

//lomse headers
#include <lomse_doorway.h>
#include <lomse_document.h>
#include <lomse_graphic_view.h>
#include <lomse_interactor.h>
#include <lomse_presenter.h>
#include <lomse_events.h>
#include <lomse_tasks.h>
#include <lomse_command.h>
#include <lomse_interval.h>
#include <lomse_score_utilities.h>

using namespace lomse;


//---------------------------------------------------------------------------------------
// Define the application
class MyApp: public wxApp
{
public:
    bool OnInit();

};

//forward declarations
class MyCanvas;

//---------------------------------------------------------------------------------------
// Define the main frame
class MyFrame: public wxFrame
{
public:
    MyFrame();
    virtual ~MyFrame();

    //commands
    void open_test_document();

    //event handlers (these functions should _not_ be virtual)
    void on_quit(wxCommandEvent& event);
    void on_about(wxCommandEvent& event);

protected:
    //accessors
    MyCanvas* get_active_canvas() const { return m_canvas; }

    //event handlers
    void on_open_file(wxCommandEvent& WXUNUSED(event));
    void on_zoom_in(wxCommandEvent& WXUNUSED(event));
    void on_zoom_out(wxCommandEvent& WXUNUSED(event));
    void on_full_score_layout(wxCommandEvent& WXUNUSED(event));
    void on_first_instr_layout(wxCommandEvent& WXUNUSED(event));
    void on_last_instr_layout(wxCommandEvent& WXUNUSED(event));
    void on_two_last_instr_layout(wxCommandEvent& WXUNUSED(event));

    //lomse related
    void initialize_lomse();

    void create_menu();

    LomseDoorway m_lomse;        //the Lomse library doorway
    MyCanvas* m_canvas;

    DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------------------------------------
// MyCanvas is a window on which we show the scores
class MyCanvas : public wxWindow
{
public:
    MyCanvas(wxFrame *frame, LomseDoorway& lomse);
    ~MyCanvas();

    void update_view_content();

    //callback wrappers
    static void wrapper_update_window(void* pThis, SpEventInfo pEvent);

    //commands
    void open_file(const wxString& fullname);
    void zoom_in();
    void zoom_out();
    void full_score_layout();
    void first_instr_layout();
    void last_instr_layout();
    void two_last_instr_layout();

protected:
    //event handlers
    void on_paint(wxPaintEvent& WXUNUSED(event));
    void on_size(wxSizeEvent& event);
    void on_key_down(wxKeyEvent& event);
    void on_mouse_event(wxMouseEvent& event);

    void delete_rendering_buffer();
    void create_rendering_buffer(int width, int height);
    void copy_buffer_on_dc(wxDC& dc);
    void update_rendering_buffer_if_needed();
    void force_redraw();
    void update_window();
    void on_key(int x, int y, unsigned key, unsigned flags);
    unsigned get_keyboard_flags(wxKeyEvent& event);
    unsigned get_mouse_flags(wxMouseEvent& event);

    //edition related
    void transpose_notes_chromatically(FIntval interval, bool fUp);
    void transpose_notes_diatonically(int steps, bool fUp);


    // In this first example we are just going to display an score on the window.
    // Let's define the necessary variables:
    LomseDoorway&   m_lomse;        //the Lomse library doorway
    Presenter*      m_pPresenter;

    //the Lomse View renders its content on a bitmap.
    //Here we define the wxImage to be used as rendering buffer
    wxImage*            m_buffer;		//the image to serve as buffer

    //some additinal variables
    bool    m_view_needs_redraw;      //to control when the View must be re-drawn


    DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------------------------------------
//A dialog for displaying transposition options for transposing notes
class DlgTransposeNotes : public wxDialog
{
protected:
    FIntval* m_interval;
    int* m_steps;
    bool* m_fUp;
    bool* m_fDiatonically;

    wxRadioBox* m_radMethod;
    wxRadioBox* m_radUpDown;
    wxComboBox* m_cboInterval;
    wxButton* m_btnOk;
    wxButton* m_btnCancel;


public:
    DlgTransposeNotes(wxWindow* parent, FIntval* interval, int* steps, bool* fUp,
                      bool *fDiatonically);
    virtual ~DlgTransposeNotes();

protected:
    void create_controls();
    void load_options();
    void load_combo_values();
    void load_intervals();
    void load_steps();

    //event handlers
    virtual void on_method_changed(wxCommandEvent& event);
    virtual void on_ok(wxMouseEvent& event);
    virtual void on_cancel(wxMouseEvent& event);
};

//---------------------------------------------------------------------------------------
//A dialog for displaying transposition options for transposing keys
class DlgTransposeKey : public wxDialog
{
protected:
    EKeySignature* m_newKey;
    int* m_mode;
    bool m_fMayor;

    wxRadioBox* m_radDirection;
    wxComboBox* m_cboKeySignature;
    wxButton* m_btnOk;
    wxButton* m_btnCancel;

public:
    DlgTransposeKey(wxWindow* parent, EKeySignature oldKey, EKeySignature* newKey,
                    int* mode);
    virtual ~DlgTransposeKey();

protected:
    void create_controls();
    void load_options();
    void load_combo_values();

    //event handlers
    virtual void on_ok( wxMouseEvent& event );
    virtual void on_cancel( wxMouseEvent& event );
};


//=======================================================================================
// MyApp implementation
//=======================================================================================

IMPLEMENT_APP(MyApp)

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame;
    frame->Show(true);
    SetTopWindow(frame);

    frame->open_test_document();

    return true;
}


//=======================================================================================
// MyFrame implementation
//=======================================================================================

//---------------------------------------------------------------------------------------
// constants for menu IDs
enum
{
    //new IDs
    k_menu_file_open = wxID_HIGHEST + 1,
    k_menu_layouts_full_score,
    k_menu_layouts__first_instr,
    k_menu_layouts_last_instr,
    k_menu_layouts_two_last_instrs,

    //using standard IDs
    //it is important for the id corresponding to the "About" command to have
    //this standard value as otherwise it won't be handled properly under Mac
    //(where it is special and put into the "Apple" menu)
    k_menu_file_quit = wxID_EXIT,
    k_menu_help_about = wxID_ABOUT,
    k_menu_zoom_in = wxID_ZOOM_IN,
    k_menu_zoom_out = wxID_ZOOM_OUT,
};

//---------------------------------------------------------------------------------------
// events table
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(k_menu_file_quit, MyFrame::on_quit)
    EVT_MENU(k_menu_help_about, MyFrame::on_about)
    EVT_MENU(k_menu_file_open, MyFrame::on_open_file)
    EVT_MENU(k_menu_zoom_in, MyFrame::on_zoom_in)
    EVT_MENU(k_menu_zoom_out, MyFrame::on_zoom_out)
    EVT_MENU(k_menu_layouts_full_score, MyFrame::on_full_score_layout)
    EVT_MENU(k_menu_layouts__first_instr, MyFrame::on_first_instr_layout)
    EVT_MENU(k_menu_layouts_last_instr, MyFrame::on_last_instr_layout)
    EVT_MENU(k_menu_layouts_two_last_instrs, MyFrame::on_two_last_instr_layout)

END_EVENT_TABLE()

//---------------------------------------------------------------------------------------
MyFrame::MyFrame()
	: wxFrame(nullptr, wxID_ANY, _T("Lomse sample for wxWidgets"),
			  wxDefaultPosition, wxSize(850, 600))
{
    create_menu();
    initialize_lomse();

    // create our one and only child -- it will take our entire client area
    m_canvas = new MyCanvas(this, m_lomse);
    wxSizer *sz = new wxBoxSizer(wxVERTICAL);
    sz->Add(m_canvas, 3, wxGROW);
    SetSizer(sz);
}

//---------------------------------------------------------------------------------------
MyFrame::~MyFrame()
{
}

//---------------------------------------------------------------------------------------
void MyFrame::create_menu()
{
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(k_menu_file_open, _T("&Open..."));
    fileMenu->AppendSeparator();
    fileMenu->Append(k_menu_file_quit, _T("E&xit"));

    wxMenu* zoomMenu = new wxMenu;
    zoomMenu->Append(k_menu_zoom_in);
    zoomMenu->Append(k_menu_zoom_out);

    wxMenu* layoutsMenu = new wxMenu;
    layoutsMenu->Append(k_menu_layouts_full_score, _T("&Full score"));
    layoutsMenu->Append(k_menu_layouts__first_instr, _T("First part"));
    layoutsMenu->Append(k_menu_layouts_last_instr, _T("&Last part"));
    layoutsMenu->Append(k_menu_layouts_two_last_instrs, _T("&Parts 3 and 4"));

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(k_menu_help_about, _T("&About"));

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, _T("&File"));
    menuBar->Append(zoomMenu, _T("&Zoom"));
    menuBar->Append(layoutsMenu, _T("&Layouts"));
    menuBar->Append(helpMenu, _T("&Help"));

    SetMenuBar(menuBar);
}

//---------------------------------------------------------------------------------------
void MyFrame::on_quit(wxCommandEvent& WXUNUSED(event))
{
    Close(true /*force to close*/);
}

//---------------------------------------------------------------------------------------
void MyFrame::on_about(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(_T("Lomse: wxWidgets sample to test layouts"),
                 _T("About wxWidgets Lomse sample"),
                 wxOK | wxICON_INFORMATION, this);
}

//---------------------------------------------------------------------------------------
void MyFrame::initialize_lomse()
{
    // Lomse knows nothing about windows. It renders everything on bitmaps and the
    // user application uses them. For instance, to display it on a wxWindows.
    // Lomse supports a lot of bitmap formats and pixel formats. Therefore, before
    // using the Lomse library you MUST specify which bitmap formap to use.
    //
    // For wxWidgets, I would suggets using a platform independent format. So
    // I will use a wxImage as the rendering  buffer. wxImage is platform independent
    // and its buffer is an array of characters in RGBRGBRGB... format,  in the
    // top-to-bottom, left-to-right order. That is, the first RGB triplet corresponds
    // to the first pixel of the first row; the second RGB triplet, to the second
    // pixel of the first row, and so on until the end of the first row,
    // with second row following after it and so on.
    // Therefore, the pixel format is RGB 24 bits.
    //
    // Let's define the requiered information:

        //the pixel format
        int pixel_format = k_pix_format_rgb24;  //RGB 24bits

        //the desired resolution: 96 pixels per inch
        int resolution = 96;

    //initialize the library with these values
    m_lomse.init_library(pixel_format, resolution);
}

//---------------------------------------------------------------------------------------
void MyFrame::open_test_document()
{
    wxString filename = wxT("./layouts-sample.xml");
    get_active_canvas()->open_file(filename);
}

//---------------------------------------------------------------------------------------
void MyFrame::on_open_file(wxCommandEvent& WXUNUSED(event))
{
    wxString defaultPath = wxT("./");

    wxString filename = wxFileSelector(_("Open score"), defaultPath,
        wxEmptyString, wxEmptyString, wxT("MusicXML files|*.xml;*.mxl;*.musicxml"));

    if (filename.empty())
        return;

    get_active_canvas()->open_file(filename);
}

//---------------------------------------------------------------------------------------
void MyFrame::on_zoom_in(wxCommandEvent& WXUNUSED(event))
{
    get_active_canvas()->zoom_in();
}

//---------------------------------------------------------------------------------------
void MyFrame::on_zoom_out(wxCommandEvent& WXUNUSED(event))
{
    get_active_canvas()->zoom_out();
}

//---------------------------------------------------------------------------------------
void MyFrame::on_full_score_layout(wxCommandEvent& WXUNUSED(event))
{
    get_active_canvas()->full_score_layout();
}

//---------------------------------------------------------------------------------------
void MyFrame::on_first_instr_layout(wxCommandEvent& WXUNUSED(event))
{
    get_active_canvas()->first_instr_layout();
}

//---------------------------------------------------------------------------------------
void MyFrame::on_last_instr_layout(wxCommandEvent& WXUNUSED(event))
{
    get_active_canvas()->last_instr_layout();
}

//---------------------------------------------------------------------------------------
void MyFrame::on_two_last_instr_layout(wxCommandEvent& WXUNUSED(event))
{
    get_active_canvas()->two_last_instr_layout();
}


//=======================================================================================
// MyCanvas implementation
//=======================================================================================

BEGIN_EVENT_TABLE(MyCanvas, wxWindow)
	EVT_KEY_DOWN(MyCanvas::on_key_down)
    EVT_MOUSE_EVENTS(MyCanvas::on_mouse_event)
    EVT_SIZE(MyCanvas::on_size)
    EVT_PAINT(MyCanvas::on_paint)
END_EVENT_TABLE()


//---------------------------------------------------------------------------------------
MyCanvas::MyCanvas(wxFrame *frame, LomseDoorway& lomse)
    : wxWindow(frame, wxID_ANY)
    , m_lomse(lomse)
	, m_pPresenter(nullptr)
	, m_buffer(nullptr)
	, m_view_needs_redraw(true)
{
}

//---------------------------------------------------------------------------------------
MyCanvas::~MyCanvas()
{
	delete_rendering_buffer();

    //delete the Presenter. This will also delete the Document, the Interactor,
    //the View and other related objects
    delete m_pPresenter;
}

//---------------------------------------------------------------------------------------
void MyCanvas::open_file(const wxString& fullname)
{
    //create a new View
    std::string filename( fullname.mb_str(wxConvUTF8) );
    delete m_pPresenter;
    m_pPresenter = m_lomse.open_document(k_view_vertical_book,
                                         filename);

    //get the pointer to the interactor, register to receive desired events and
    //enable edition
    if (SpInteractor spIntor = m_pPresenter->get_interactor(0).lock())
    {
        //ask to receive desired events
        spIntor->add_event_handler(k_update_window_event, this, wrapper_update_window);
    }

    //render the new score
    m_view_needs_redraw = true;
    wxSize size = this->GetClientSize();
    create_rendering_buffer(size.GetWidth(), size.GetHeight());
    Refresh(false /* don't erase background */);
}

//---------------------------------------------------------------------------------------
void MyCanvas::on_size(wxSizeEvent& WXUNUSED(event))
{
    wxSize size = this->GetClientSize();
    create_rendering_buffer(size.GetWidth(), size.GetHeight());

    Refresh(false /* don't erase background */);
}

//---------------------------------------------------------------------------------------
void MyCanvas::on_paint(wxPaintEvent& event)
{
    if (!m_pPresenter)
        event.Skip(false);
    else
    {
        update_rendering_buffer_if_needed();
        wxPaintDC dc(this);
        copy_buffer_on_dc(dc);
    }
}

//---------------------------------------------------------------------------------------
void MyCanvas::update_rendering_buffer_if_needed()
{
    if (m_view_needs_redraw)
        update_view_content();

    m_view_needs_redraw = false;
}

//---------------------------------------------------------------------------------------
void MyCanvas::delete_rendering_buffer()
{
    delete m_buffer;
}

//---------------------------------------------------------------------------------------
void MyCanvas::create_rendering_buffer(int width, int height)
{
    //creates a bitmap of specified size and defines it s the rendering
    //buffer for the view.

    delete_rendering_buffer();

    // allocate a new rendering buffer
    m_buffer = new wxImage(width, height);

    //get pointer to wxImage internal bitmap
    unsigned char* pdata = m_buffer->GetData();

    //use this bitmap as Lomse rendering buffer
    if (SpInteractor spInteractor = m_pPresenter->get_interactor(0).lock())
    {
        spInteractor->set_rendering_buffer(pdata, width, height);
    }

    m_view_needs_redraw = true;
}

//---------------------------------------------------------------------------------------
void MyCanvas::force_redraw()
{
    update_view_content();
    update_window();
}

//---------------------------------------------------------------------------------------
void MyCanvas::wrapper_update_window(void* pThis, SpEventInfo UNUSED(pEvent))
{
    static_cast<MyCanvas*>(pThis)->update_window();
}

//---------------------------------------------------------------------------------------
void MyCanvas::update_window()
{
    // Invoking update_window() results in just putting immediately the content
    // of the currently rendered buffer to the window without neither calling
    // any lomse methods nor generating any events  (i.e. Refresh() window)

    wxClientDC dc(this);
    copy_buffer_on_dc(dc);
}

//---------------------------------------------------------------------------------------
void MyCanvas::copy_buffer_on_dc(wxDC& dc)
{
    if (!m_buffer || !m_buffer->IsOk())
        return;

    wxBitmap bitmap(*m_buffer);
    dc.DrawBitmap(bitmap, 0, 0, false /* don't use mask */);
}

//-------------------------------------------------------------------------
void MyCanvas::update_view_content()
{
    //request the view to re-draw the bitmap

    if (!m_pPresenter) return;

    if (SpInteractor spIntor = m_pPresenter->get_interactor(0).lock())
        spIntor->redraw_bitmap();
}

//---------------------------------------------------------------------------------------
void MyCanvas::on_key_down(wxKeyEvent& event)
{
    if (!m_pPresenter) return;

    int nKeyCode = event.GetKeyCode();
    unsigned flags = get_keyboard_flags(event);

    //fix ctrol+key codes
    if (nKeyCode > 0 && nKeyCode < 27)
    {
        nKeyCode += int('A') - 1;
        flags |= k_kbd_ctrl;
    }

    //process key
    switch (nKeyCode)
    {
        case WXK_SHIFT:
        case WXK_ALT:
        case WXK_CONTROL:
            return;      //do nothing

		default:
			on_key(event.GetX(), event.GetY(), nKeyCode, flags);;
	}
}

//-------------------------------------------------------------------------
void MyCanvas::on_key(int x, int y, unsigned key, unsigned UNUSED(flags))
{
    if (SpInteractor spIntor = m_pPresenter->get_interactor(0).lock())
    {
        switch (key)
        {
            case 'D':
                spIntor->switch_task(TaskFactory::k_task_drag_view);
                break;
            case 'S':
                spIntor->switch_task(TaskFactory::k_task_selection);
                break;
            case '+':
                spIntor->zoom_in(x, y);
                force_redraw();
                break;
            case '-':
                spIntor->zoom_out(x, y);
                force_redraw();
                break;
            default:
               return;
        }
    }
}

//-------------------------------------------------------------------------
void MyCanvas::zoom_in()
{
    if (!m_pPresenter) return;

    //do zoom in centered on window center
    if (SpInteractor spIntor = m_pPresenter->get_interactor(0).lock())
    {
        wxSize size = this->GetClientSize();
        spIntor->zoom_in(size.GetWidth()/2, size.GetHeight()/2);
        force_redraw();
    }
}

//-------------------------------------------------------------------------
void MyCanvas::zoom_out()
{
    if (!m_pPresenter) return;

    //do zoom out centered on window center
    if (SpInteractor spIntor = m_pPresenter->get_interactor(0).lock())
    {
        wxSize size = this->GetClientSize();
        spIntor->zoom_out(size.GetWidth()/2, size.GetHeight()/2);
        force_redraw();
    }
}

//-------------------------------------------------------------------------
void MyCanvas::full_score_layout()
{
    if (!m_pPresenter) return;

    if (SpInteractor spIntor = m_pPresenter->get_interactor(0).lock())
    {
        //get the document
        ADocument doc = m_pPresenter->get_document();
        Document* pDoc = doc.internal_object();

        //get the first score (for MusicXML files it is the only score)
        //AScore score = doc.first_score();
        ImoScore* pScore = pDoc->get_first_score();

        //get default layout and select it
        ImoScoreLayout* pScoreLayout = pScore->get_score_layout(k_default_score_layout);
        spIntor->select_layout(pScore, pScoreLayout->get_id());
        force_redraw();
    }
}

//-------------------------------------------------------------------------
void MyCanvas::first_instr_layout()
{
    if (!m_pPresenter) return;

    if (SpInteractor spIntor = m_pPresenter->get_interactor(0).lock())
    {
        //get the document
        ADocument doc = m_pPresenter->get_document();
        Document* pDoc = doc.internal_object();

        //get the first score (for MusicXML files it is the only score)
        //AScore score = doc.first_score();
        ImoScore* pScore = pDoc->get_first_score();

        //create a layout with only tenor and bass voices in reversed order
        ImoSystemLayout* pSysLayout = pScore->new_system_layout("BT");
        pSysLayout->add_instrument(0);     //S is the 1st instrument in the score
//        pSysLayout->add_instrument(3);     //B is the 3rd instrument in the score
//        pSysLayout->add_instrument(2);     //T is the 2nd instrument in the score

        //create a layout using the previous system layout
        ImoScoreLayout* pScoreLayout = pScore->new_score_layout("BT", pSysLayout->get_id());

        //select the created layout
        spIntor->select_layout(pScore, pScoreLayout->get_id());
        force_redraw();
    }
}

//-------------------------------------------------------------------------
void MyCanvas::last_instr_layout()
{
    if (!m_pPresenter) return;

    if (SpInteractor spIntor = m_pPresenter->get_interactor(0).lock())
    {
        //get the document
        ADocument doc = m_pPresenter->get_document();
        Document* pDoc = doc.internal_object();

        //get the first score (for MusicXML files it is the only score)
        //AScore score = doc.first_score();
        ImoScore* pScore = pDoc->get_first_score();

        //create a layout with only tenor and bass voices in reversed order
        ImoSystemLayout* pSysLayout = pScore->new_system_layout("BT");
        pSysLayout->add_instrument(3);     //B is the 4rd instrument in the score

        //create a layout using the previous system layout
        ImoScoreLayout* pScoreLayout = pScore->new_score_layout("BT", pSysLayout->get_id());

        //select the created layout
        spIntor->select_layout(pScore, pScoreLayout->get_id());
        force_redraw();
    }
}

//-------------------------------------------------------------------------
void MyCanvas::two_last_instr_layout()
{
    if (!m_pPresenter) return;

    if (SpInteractor spIntor = m_pPresenter->get_interactor(0).lock())
    {
        //get the document
        ADocument doc = m_pPresenter->get_document();
        Document* pDoc = doc.internal_object();

        //get the first score (for MusicXML files it is the only score)
        //AScore score = doc.first_score();
        ImoScore* pScore = pDoc->get_first_score();

        //create a layout with only tenor and bass voices in reversed order
        ImoSystemLayout* pSysLayout = pScore->new_system_layout("BT");
        pSysLayout->add_instrument(3);     //B is the 4rd instrument in the score
        pSysLayout->add_instrument(2);     //T is the 3nd instrument in the score

        //create a layout using the previous system layout
        ImoScoreLayout* pScoreLayout = pScore->new_score_layout("BT", pSysLayout->get_id());

        //select the created layout
        spIntor->select_layout(pScore, pScoreLayout->get_id());
        force_redraw();
    }
}

//-------------------------------------------------------------------------
void MyCanvas::transpose_notes_chromatically(FIntval interval, bool fUp)
{
    if (SpInteractor spIntor = m_pPresenter->get_interactor(0).lock())
    {
        ::wxBeginBusyCursor();
        string name = gettext("Chromatic transposition");
        if (fUp)
            interval.make_ascending();
        else
            interval.make_descending();

        spIntor->exec_command(
            new CmdTransposeChromatically(interval) );
        ::wxEndBusyCursor();
    }
}

//-------------------------------------------------------------------------
void MyCanvas::transpose_notes_diatonically(int steps, bool fUp)
{
    if (SpInteractor spIntor = m_pPresenter->get_interactor(0).lock())
    {
        ::wxBeginBusyCursor();
        string name = gettext("Diatonic transposition");
        spIntor->exec_command(
            new CmdTransposeDiatonically(steps, fUp) );
        ::wxEndBusyCursor();
    }
}

//---------------------------------------------------------------------------------------
unsigned MyCanvas::get_mouse_flags(wxMouseEvent& event)
{
    unsigned flags = 0;
    if (event.LeftIsDown())     flags |= k_mouse_left;
    if (event.RightIsDown())    flags |= k_mouse_right;
    if (event.MiddleDown())     flags |= k_mouse_middle;
    if (event.ShiftDown())      flags |= k_kbd_shift;
    if (event.AltDown())        flags |= k_kbd_alt;
    if (event.ControlDown())    flags |= k_kbd_ctrl;
    return flags;
}

//---------------------------------------------------------------------------------------
unsigned MyCanvas::get_keyboard_flags(wxKeyEvent& event)
{
    unsigned flags = 0;
    if (event.ShiftDown())   flags |= k_kbd_shift;
    if (event.AltDown()) flags |= k_kbd_alt;
    if (event.ControlDown()) flags |= k_kbd_ctrl;
    return flags;
}

//-------------------------------------------------------------------------
void MyCanvas::on_mouse_event(wxMouseEvent& event)
{
    if (!m_pPresenter) return;

    if (SpInteractor spIntor = m_pPresenter->get_interactor(0).lock())
    {
        wxEventType nEventType = event.GetEventType();
        wxPoint pos = event.GetPosition();
        unsigned flags = get_mouse_flags(event);

        if (nEventType==wxEVT_LEFT_DOWN)
        {
            flags |= k_mouse_left;
            spIntor->on_mouse_button_down(pos.x, pos.y, flags);
        }
        else if (nEventType==wxEVT_LEFT_UP)
        {
            flags |= k_mouse_left;
            spIntor->on_mouse_button_up(pos.x, pos.y, flags);
        }
        else if (nEventType==wxEVT_RIGHT_DOWN)
        {
            flags |= k_mouse_right;
            spIntor->on_mouse_button_down(pos.x, pos.y, flags);
        }
        else if (nEventType==wxEVT_RIGHT_UP)
        {
            flags |= k_mouse_right;
            spIntor->on_mouse_button_up(pos.x, pos.y, flags);
        }
        else if (nEventType==wxEVT_MOTION)
            spIntor->on_mouse_move(pos.x, pos.y, flags);
    }
}


//=======================================================================================
// DlgTransposeNotes implementation
//=======================================================================================
DlgTransposeNotes::DlgTransposeNotes(wxWindow* parent, FIntval* interval, int* steps,
                                     bool* fUp, bool* fDiatonically)
    : wxDialog(parent, wxID_ANY, _("Transpose notes"), wxDefaultPosition, wxDefaultSize,
               wxDEFAULT_DIALOG_STYLE)
    , m_interval(interval)
    , m_steps(steps)
    , m_fUp(fUp)
    , m_fDiatonically(fDiatonically)
{
    create_controls();
    load_options();
}

//---------------------------------------------------------------------------------------
DlgTransposeNotes::~DlgTransposeNotes()
{
	// Disconnect Events
	m_radMethod->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( DlgTransposeNotes::on_method_changed ), nullptr, this );
	m_btnOk->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( DlgTransposeNotes::on_ok ), nullptr, this );
	m_btnCancel->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( DlgTransposeNotes::on_cancel ), nullptr, this );
}

//---------------------------------------------------------------------------------------
void DlgTransposeNotes::create_controls()
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* szrMain;
	szrMain = new wxBoxSizer( wxVERTICAL );

	wxString m_radMethodChoices[] = { _("Chromatically"), _("Diatonically") };
	int m_radMethodNChoices = sizeof( m_radMethodChoices ) / sizeof( wxString );
	m_radMethod = new wxRadioBox( this, wxID_ANY, _("Tranposition method "), wxDefaultPosition, wxDefaultSize, m_radMethodNChoices, m_radMethodChoices, 1, wxRA_SPECIFY_COLS );
	m_radMethod->SetSelection( 1 );
	szrMain->Add( m_radMethod, 1, wxALL|wxEXPAND, 5 );

	wxString m_radUpDownChoices[] = { _("Up"), _("Down") };
	int m_radUpDownNChoices = sizeof( m_radUpDownChoices ) / sizeof( wxString );
	m_radUpDown = new wxRadioBox( this, wxID_ANY, _("Direction "), wxDefaultPosition, wxDefaultSize, m_radUpDownNChoices, m_radUpDownChoices, 1, wxRA_SPECIFY_COLS );
	m_radUpDown->SetSelection( 0 );
	szrMain->Add( m_radUpDown, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	wxStaticBoxSizer* szrInterval;
	szrInterval = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Interval ") ), wxVERTICAL );

	m_cboInterval = new wxComboBox( szrInterval->GetStaticBox(), wxID_ANY, _("Unison"), wxDefaultPosition, wxDefaultSize, 0, nullptr, 0 );
	szrInterval->Add( m_cboInterval, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );


	szrMain->Add( szrInterval, 1, wxEXPAND|wxALL, 5 );

	wxBoxSizer* szrButtons;
	szrButtons = new wxBoxSizer( wxHORIZONTAL );


	szrButtons->Add( 0, 0, 1, wxEXPAND, 5 );

	m_btnOk = new wxButton( this, wxID_ANY, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	szrButtons->Add( m_btnOk, 0, wxALL, 5 );

	m_btnCancel = new wxButton( this, wxID_ANY, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	szrButtons->Add( m_btnCancel, 0, wxALL, 5 );


	szrButtons->Add( 0, 0, 1, wxEXPAND, 5 );


	szrMain->Add( szrButtons, 0, wxTOP|wxBOTTOM|wxEXPAND, 5 );


	this->SetSizer( szrMain );
	this->Layout();
	szrMain->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_radMethod->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( DlgTransposeNotes::on_method_changed ), nullptr, this );
	m_btnOk->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( DlgTransposeNotes::on_ok ), nullptr, this );
	m_btnCancel->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( DlgTransposeNotes::on_cancel ), nullptr, this );
}

//---------------------------------------------------------------------------------------
void DlgTransposeNotes::load_options()
{
    m_radMethod->Select(*m_fDiatonically ? 1 : 0);
    m_radUpDown->Select(*m_fUp ? 0 : 1);
    load_combo_values();
}

//---------------------------------------------------------------------------------------
void DlgTransposeNotes::on_method_changed(wxCommandEvent& event)
{
    load_combo_values();
    event.Skip();
}

//---------------------------------------------------------------------------------------
void DlgTransposeNotes::load_combo_values()
{
    if (m_radMethod->GetSelection() == 1)
        load_steps();
    else
        load_intervals();
}

//---------------------------------------------------------------------------------------
void DlgTransposeNotes::load_intervals()
{
    //Combo box for intervals
    m_cboInterval->Clear();
    m_cboInterval->Append(_("Perfect unison"));
    m_cboInterval->Append(_("Augmented unison"));
    m_cboInterval->Append(_("Diminished second"));
    m_cboInterval->Append(_("Minor second"));
    m_cboInterval->Append(_("Major second"));
    m_cboInterval->Append(_("Augmented second"));
    m_cboInterval->Append(_("Diminished third"));
    m_cboInterval->Append(_("Minor third"));
    m_cboInterval->Append(_("Major third"));
    m_cboInterval->Append(_("Augmented third"));
    m_cboInterval->Append(_("Diminished fourth"));
    m_cboInterval->Append(_("Perfect fourth"));
    m_cboInterval->Append(_("Augmented fourth"));
    m_cboInterval->Append(_("Diminished fifth"));
    m_cboInterval->Append(_("Perfect fifth"));
    m_cboInterval->Append(_("Augmented fifth"));
    m_cboInterval->Append(_("Diminished sixth"));
    m_cboInterval->Append(_("Minor sixth"));
    m_cboInterval->Append(_("Major sixth"));
    m_cboInterval->Append(_("Augmented sixth"));
    m_cboInterval->Append(_("Diminished seventh"));
    m_cboInterval->Append(_("Minor seventh"));
    m_cboInterval->Append(_("Major seventh"));
    m_cboInterval->Append(_("Augmented seventh"));
    m_cboInterval->Append(_("Diminished octave"));
    m_cboInterval->Append(_("Perfect octave"));

    m_cboInterval->SetSelection(0);
}

//---------------------------------------------------------------------------------------
void DlgTransposeNotes::load_steps()
{
    //Combo box for intervals
    m_cboInterval->Clear();
    m_cboInterval->Append(_("Second (1 step)"));
    m_cboInterval->Append(_("Third (2 steps)"));
    m_cboInterval->Append(_("Fourth (3 steps)"));
    m_cboInterval->Append(_("Fifth (4 steps)"));
    m_cboInterval->Append(_("Sixth (5 steps)"));
    m_cboInterval->Append(_("Seventh (6 steps)"));
    m_cboInterval->Append(_("Octave (7 steps)"));

    m_cboInterval->SetSelection(0);
}

//---------------------------------------------------------------------------------------
void DlgTransposeNotes::on_ok(wxMouseEvent& event)
{
    static string intervals[] = {
        "p1",  "a1",  "d2", "m2",  "M2", "a2", "d3",  "m3",  "M3", "a3",
        "d4",  "p4",  "a4", "d5",  "p5", "a5",  "d6",  "m6", "M6",  "a6",
        "d7",  "m7",  "M7",  "a7", "d8", "p8"
    };

    *m_fUp = m_radUpDown->GetSelection() == 0;
    *m_fDiatonically = m_radMethod->GetSelection() == 1;
    if (*m_fDiatonically)
        *m_steps = m_cboInterval->GetSelection() + 1;
    else
        *m_interval = FIntval( intervals[m_cboInterval->GetSelection()] );

    event.Skip();
    EndModal(wxID_OK);
}

//---------------------------------------------------------------------------------------
void DlgTransposeNotes::on_cancel(wxMouseEvent& event)
{
    event.Skip();
    EndModal(wxID_CANCEL);
}


//=======================================================================================
// DlgTransposeNotes implementation
//=======================================================================================
DlgTransposeKey::DlgTransposeKey(wxWindow* parent, EKeySignature oldKey,
                                 EKeySignature* newKey, int* mode)
    : wxDialog(parent, wxID_ANY, _("Transpose by key"), wxDefaultPosition, wxDefaultSize,
               wxDEFAULT_DIALOG_STYLE)
    , m_newKey(newKey)
    , m_mode(mode)
    , m_fMayor(KeyUtilities::is_major_key(oldKey))
{
    create_controls();
    load_options();
}

//---------------------------------------------------------------------------------------
DlgTransposeKey::~DlgTransposeKey()
{
	// Disconnect Events
	m_btnOk->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( DlgTransposeKey::on_ok ), nullptr, this );
	m_btnCancel->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( DlgTransposeKey::on_cancel ), nullptr, this );

}

//---------------------------------------------------------------------------------------
void DlgTransposeKey::create_controls()
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* szrMain;
	szrMain = new wxBoxSizer( wxVERTICAL );

	wxString m_radDirectionChoices[] = { _("Up"), _("Down"), _("Closest") };
	int m_radDirectionNChoices = sizeof( m_radDirectionChoices ) / sizeof( wxString );
	m_radDirection = new wxRadioBox( this, wxID_ANY, _("Direction"), wxDefaultPosition, wxDefaultSize, m_radDirectionNChoices, m_radDirectionChoices, 1, wxRA_SPECIFY_COLS );
	m_radDirection->SetSelection( 0 );
	szrMain->Add( m_radDirection, 1, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* szrNewKey;
	szrNewKey = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("New key signature") ), wxVERTICAL );

	m_cboKeySignature = new wxComboBox( szrNewKey->GetStaticBox(), wxID_ANY, "", wxDefaultPosition, wxSize( 200,-1 ), 0, nullptr, 0 );
	szrNewKey->Add( m_cboKeySignature, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );


	szrMain->Add( szrNewKey, 1, wxEXPAND|wxALL, 5 );

	wxBoxSizer* szrButtons;
	szrButtons = new wxBoxSizer( wxHORIZONTAL );


	szrButtons->Add( 0, 0, 1, wxEXPAND, 5 );

	m_btnOk = new wxButton( this, wxID_ANY, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	szrButtons->Add( m_btnOk, 0, wxALL, 5 );

	m_btnCancel = new wxButton( this, wxID_ANY, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	szrButtons->Add( m_btnCancel, 0, wxALL, 5 );


	szrButtons->Add( 0, 0, 1, wxEXPAND, 5 );


	szrMain->Add( szrButtons, 0, wxTOP|wxBOTTOM|wxEXPAND, 5 );


	this->SetSizer( szrMain );
	this->Layout();
	szrMain->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_btnOk->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( DlgTransposeKey::on_ok ), nullptr, this );
	m_btnCancel->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( DlgTransposeKey::on_cancel ), nullptr, this );
}

//---------------------------------------------------------------------------------------
void DlgTransposeKey::load_options()
{
    load_combo_values();
}

//---------------------------------------------------------------------------------------
void DlgTransposeKey::on_ok(wxMouseEvent& event)
{
    static EKeySignature major[] = {
        k_key_C, k_key_G, k_key_D,  k_key_A,  k_key_E,  k_key_B,  k_key_Fs, k_key_Cs,
                 k_key_F, k_key_Bf, k_key_Ef, k_key_Af, k_key_Df, k_key_Gf, k_key_Cf
    };

    static EKeySignature minor[] = {
        k_key_a, k_key_e, k_key_b, k_key_fs, k_key_cs, k_key_gs, k_key_ds, k_key_as,
                 k_key_d, k_key_g, k_key_c,  k_key_f,  k_key_bf, k_key_ef, k_key_af
    };

    *m_mode = m_radDirection->GetSelection();

    if (m_fMayor)
        *m_newKey = major[m_cboKeySignature->GetSelection()];
    else
        *m_newKey = minor[m_cboKeySignature->GetSelection()];

    event.Skip();
    EndModal(wxID_OK);
}

//---------------------------------------------------------------------------------------
void DlgTransposeKey::on_cancel(wxMouseEvent& event)
{
    event.Skip();
    EndModal(wxID_CANCEL);
}

//---------------------------------------------------------------------------------------
void DlgTransposeKey::load_combo_values()
{
    if (m_fMayor)
    {
        m_cboKeySignature->Clear();
        m_cboKeySignature->Append(_("C major (no accidentals)"));
        m_cboKeySignature->Append(_("G major (1 sharp)"));
        m_cboKeySignature->Append(_("D major (2 sharps)"));
        m_cboKeySignature->Append(_("A major (3 sharps)"));
        m_cboKeySignature->Append(_("E major (4 sharps)"));
        m_cboKeySignature->Append(_("B major (5 sharps)"));
        m_cboKeySignature->Append(_("F sharp major (6 sharps)"));
        m_cboKeySignature->Append(_("C sharp major (7 sharps)"));
        m_cboKeySignature->Append(_("F major (1 flat)"));
        m_cboKeySignature->Append(_("B flat major (2 flats)"));
        m_cboKeySignature->Append(_("E flat major (3 flats)"));
        m_cboKeySignature->Append(_("A flat major (4 flats)"));
        m_cboKeySignature->Append(_("D flat major (5 flats)"));
        m_cboKeySignature->Append(_("G flat major (6 flats)"));
        m_cboKeySignature->Append(_("C flat major (7 flats)"));
    }
    else
    {
        m_cboKeySignature->Clear();
        m_cboKeySignature->Append(_("A minor (no accidentals)"));
        m_cboKeySignature->Append(_("E minor (1 sharp)"));
        m_cboKeySignature->Append(_("B minor (2 sharps)"));
        m_cboKeySignature->Append(_("F sharp minor (3 sharps)"));
        m_cboKeySignature->Append(_("C sharp minor (4 sharps)"));
        m_cboKeySignature->Append(_("G sharp minor (5 sharps)"));
        m_cboKeySignature->Append(_("D sharp minor (6 sharps)"));
        m_cboKeySignature->Append(_("A sharp minor (7 sharps)"));
        m_cboKeySignature->Append(_("D minor (1 flat)"));
        m_cboKeySignature->Append(_("G minor (2 flats)"));
        m_cboKeySignature->Append(_("C minor (3 flats)"));
        m_cboKeySignature->Append(_("F minor (4 flats)"));
        m_cboKeySignature->Append(_("B flat minor (5 flats)"));
        m_cboKeySignature->Append(_("E flat minor (6 flats)"));
        m_cboKeySignature->Append(_("A flat minor (7 flats)"));
    }
    m_cboKeySignature->SetSelection(0);
}
