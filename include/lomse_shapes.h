//---------------------------------------------------------------------------------------
// This file is part of the Lomse library.
// Copyright (c) 2010-present, Lomse Developers
//
// Licensed under the MIT license.
//
// See LICENSE and NOTICE.md files in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#ifndef __LOMSE_SHAPES_H__        //to avoid nested includes
#define __LOMSE_SHAPES_H__

#include "lomse_shape_base.h"
#include "lomse_injectors.h"
#include "lomse_image.h"

//GmoShapeDebug
#include "lomse_vertex_source.h"
#include "agg_trans_affine.h"

namespace lomse
{

//forward declarations
class FontStorage;
class ImoImage;
class ImoStyle;

//---------------------------------------------------------------------------------------
// a shape drawn by using a single glyph
class GmoShapeGlyph : public GmoSimpleShape
{
protected:
    unsigned int m_glyph;
    USize m_shiftToDraw;
    FontStorage* m_pFontStorage;
    LibraryScope& m_libraryScope;
    double m_fontHeight;

public:
    virtual ~GmoShapeGlyph() {}

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;

    LUnits get_relative_baseline_y() const { return m_shiftToDraw.height; }
    LUnits get_baseline_y() const override { return m_origin.y + get_relative_baseline_y(); }

protected:
    GmoShapeGlyph(ImoObj* pCreatorImo, int type, ShapeId idx, unsigned int nGlyph,
                  UPoint pos, Color color, LibraryScope& libraryScope,
                  double fontHeight);

    void compute_size_origin(double fontHeight, UPoint pos);

};

//---------------------------------------------------------------------------------------
class GmoShapeAccidentals : public GmoCompositeShape, public VoiceRelatedShape
{
public:
    GmoShapeAccidentals(ImoObj* pCreatorImo, ShapeId idx, UPoint UNUSED(pos),
                        Color color)
        : GmoCompositeShape(pCreatorImo, GmoObj::k_shape_accidentals, idx, color)
        , VoiceRelatedShape()
    {
    }
};

//---------------------------------------------------------------------------------------
class GmoShapeAccidental : public GmoShapeGlyph, public VoiceRelatedShape
{
public:
    GmoShapeAccidental(ImoObj* pCreatorImo, ShapeId idx, unsigned int iGlyph, UPoint pos,
                       Color color, LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_accidental_sign, idx, iGlyph,
                        pos, color, libraryScope, fontSize)
        , VoiceRelatedShape()
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeArticulation : public GmoShapeGlyph, public VoiceRelatedShape
{
public:
    GmoShapeArticulation(ImoObj* pCreatorImo, ShapeId idx, int nGlyph, UPoint pos,
                         Color color, LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_articulation, idx, nGlyph, pos, color,
                        libraryScope, fontSize )
        , VoiceRelatedShape()
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeClef : public GmoShapeGlyph
{
//protected:
public:
    GmoShapeClef(ImoObj* pCreatorImo, ShapeId idx, int nGlyph, UPoint pos, Color color,
                 LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_clef, idx, nGlyph, pos, color,
                        libraryScope, fontSize )
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
//A shape for debug purposes, to draw lines, points, etc. used during design and debug
class GmoShapeDebug : public GmoSimpleShape, public VertexSource
{
protected:
    std::list<Vertex> m_vertices;       //list of vertices
    std::list<Vertex>::iterator m_it;   //points to current vertex

    int m_nContour;                 //current countour
    agg::trans_affine   m_trans;    //affine transformation to apply


public:
    GmoShapeDebug(Color color=Color(0,0,0), UPoint uPos=UPoint(0.0f, 0.0f),
                  USize uSize=USize(1000.0f, 1000.0f));

    virtual void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
    virtual void add_vertex(Vertex& vertex);
    virtual void add_vertex(char cmd, LUnits x, LUnits y);
    virtual void close_vertex_list();

    //VertexSource
    unsigned vertex(double* px, double* py) override;
    void rewind(unsigned UNUSED(pathId) = 0) override;

};

//---------------------------------------------------------------------------------------
class GmoShapeDynamicsMark : public GmoShapeGlyph, public VoiceRelatedShape
{
public:
    GmoShapeDynamicsMark(ImoObj* pCreatorImo, ShapeId idx, int nGlyph, UPoint pos,
                         Color color, LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_dynamics_mark, idx, nGlyph,
                        pos, color, libraryScope, fontSize )
        , VoiceRelatedShape()
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeFermata : public GmoShapeGlyph, public VoiceRelatedShape
{
public:
    GmoShapeFermata(ImoObj* pCreatorImo, ShapeId idx, int nGlyph, UPoint pos,
                    Color color, LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_fermata, idx, nGlyph, pos, color,
                        libraryScope, fontSize )
        , VoiceRelatedShape()
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeFingeringContainer : public GmoCompositeShape, public VoiceRelatedShape
{
public:
    GmoShapeFingeringContainer(ImoObj* pCreatorImo, ShapeId idx)
        : GmoCompositeShape(pCreatorImo, GmoObj::k_shape_fingering_box, idx, Color(0,0,0))
        , VoiceRelatedShape()
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeFingering : public GmoShapeGlyph
{
public:
    GmoShapeFingering(ImoObj* pCreatorImo, ShapeId idx, int nGlyph, UPoint pos,
                      LibraryScope& libraryScope, Color color, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_fingering, idx, nGlyph, pos, color,
                        libraryScope, fontSize )
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeImage : public GmoSimpleShape
{
protected:
    SpImage m_image;


    //TO_FIX: Classes derived from Control can not access the constructor!
    //friend class ImageEngrouter;
    //friend class Control;
public:
    GmoShapeImage(ImoObj* pCreatorImo, SpImage image, UPoint pos, USize size);
    void set_image(SpImage image);

public:
    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeInvisible : public GmoSimpleShape
{
public:
    GmoShapeInvisible(ImoObj* pCreatorImo, ShapeId idx, UPoint uPos, USize uSize);

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeKeySignature : public GmoCompositeShape
{
protected:
    LibraryScope& m_libraryScope;

public:
    GmoShapeKeySignature(ImoObj* pCreatorImo, ShapeId idx, UPoint UNUSED(pos),
                         Color color, LibraryScope& libraryScope)
        : GmoCompositeShape(pCreatorImo, GmoObj::k_shape_key_signature, idx, color)
        , m_libraryScope(libraryScope)
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeLyrics : public GmoCompositeShape
{
protected:
    LibraryScope& m_libraryScope;

public:
    GmoShapeLyrics(ImoObj* pCreatorImo, ShapeId idx, Color color,
                   LibraryScope& libraryScope)
        : GmoCompositeShape(pCreatorImo, GmoObj::k_shape_lyrics, idx, color)
        , m_libraryScope(libraryScope)
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeMetronomeMark : public GmoCompositeShape
{
protected:
    LibraryScope& m_libraryScope;

public:
    GmoShapeMetronomeMark(ImoObj* pCreatorImo, ShapeId idx, UPoint UNUSED(pos),
                          Color color, LibraryScope& libraryScope)
        : GmoCompositeShape(pCreatorImo, GmoObj::k_shape_metronome_mark, idx, color)
        , m_libraryScope(libraryScope)
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeMetronomeGlyph : public GmoShapeGlyph
{
public:
    GmoShapeMetronomeGlyph(ImoObj* pCreatorImo, ShapeId idx, unsigned int iGlyph, UPoint pos,
                           Color color, LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_metronome_glyph, idx, iGlyph,
                        pos, color, libraryScope, fontSize)
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeOrnament : public GmoShapeGlyph, public VoiceRelatedShape
{
public:
    GmoShapeOrnament(ImoObj* pCreatorImo, ShapeId idx, int nGlyph, UPoint pos,
                     Color color, LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_ornament, idx, nGlyph, pos, color,
                        libraryScope, fontSize )
        , VoiceRelatedShape()
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeRectangle : public GmoSimpleShape
{
protected:
    LUnits m_radius;

public:
    GmoShapeRectangle(ImoObj* pCreatorImo
                      , int type = GmoObj::k_shape_rectangle
                      , ShapeId idx = 0
                      , const UPoint& pos = UPoint(0.0f, 0.0f)    //top-left corner
                      , const USize& size = USize(0.0f, 0.0f)     //rectangle size
                      , LUnits radius = 0.0f                      //for rounded corners
                      , ImoStyle* pStyle = nullptr       //for line style & background color
                     );
    virtual ~GmoShapeRectangle() {}


    //implementation of virtual methods from base class
    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;

    //settings
    inline void set_radius(LUnits radius) { m_radius = radius; }

};

//---------------------------------------------------------------------------------------
class GmoShapeSimpleLine : public GmoSimpleShape
{
protected:
    LUnits		m_uWidth;
	LUnits		m_uBoundsExtraWidth;
	ELineEdge	m_nEdge;

public:
    virtual ~GmoShapeSimpleLine() {}

    //implementation of virtual methods from base class
    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;

protected:
    GmoShapeSimpleLine(ImoObj* pCreatorImo, int type, LUnits xStart, LUnits yStart,
                       LUnits xEnd, LUnits yEnd, LUnits uWidth, LUnits uBoundsExtraWidth,
                       Color color, ELineEdge nEdge = k_edge_normal);
    void set_new_values(LUnits xStart, LUnits yStart, LUnits xEnd, LUnits yEnd,
                        LUnits uWidth, LUnits uBoundsExtraWidth,
                        Color color, ELineEdge nEdge);

};

//---------------------------------------------------------------------------------------
class GmoShapeStem : public GmoShapeSimpleLine, public VoiceRelatedShape
{
private:
	bool m_fStemDown;

public:
    GmoShapeStem(ImoObj* pCreatorImo, LUnits xPos, LUnits yStart, LUnits yEnd,
                 bool fStemDown, LUnits uWidth, Color color);

    void change_length(LUnits length);
	inline bool is_stem_down() const { return m_fStemDown; }
    void set_stem_up(LUnits xRight, LUnits yNote);
    void set_stem_down(LUnits xLeft, LUnits yNote);
	void adjust(LUnits xLeft, LUnits yTop, LUnits height, bool fStemDown);
    LUnits get_y_note();
    LUnits get_y_flag();
};

//---------------------------------------------------------------------------------------
class GmoShapeTechnical : public GmoShapeGlyph, public VoiceRelatedShape
{
public:
    GmoShapeTechnical(ImoObj* pCreatorImo, ShapeId idx, int nGlyph, UPoint pos,
                      Color color, LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_technical, idx, nGlyph, pos, color,
                        libraryScope, fontSize )
        , VoiceRelatedShape()
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeCodaSegno : public GmoShapeGlyph
{
public:
    GmoShapeCodaSegno(ImoObj* pCreatorImo, ShapeId idx, int nGlyph, UPoint pos,
                      Color color, LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_coda_segno, idx, nGlyph, pos, color,
                        libraryScope, fontSize )
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

////---------------------------------------------------------------------------------------
//class GmoShapeFiguredBass : public lmCompositeShape
//{
//public:
//    GmoShapeFiguredBass(GmoBox* owner, int nShapeIdx, Color nColor)
//        : lmCompositeShape(pOwner, nShapeIdx, nColor, _T("Figured bass"), true)  //true= lmDRAGGABLE
//        { m_nType = GmoObj::k_shape_FiguredBass; }
//    ~GmoShapeFiguredBass() {}
//
//	//info about related shapes
//    inline void OnFBLineAttached(int nLine, GmoShapeLine* pShapeFBLine)
//                    { m_pFBLineShape[nLine] = pShapeFBLine; }
//
//    //overrides
//    void Shift(LUnits uxIncr, LUnits uyIncr);
//
//private:
//    GmoShapeLine*  m_pFBLineShape[2];     //The two lines of a FB line. This is the end FB of the line
//
//};
//
////---------------------------------------------------------------------------------------
//class GmoShapeWindow : public GmoShapeRectangle
//{
//public:
//    GmoShapeWindow(GmoBox* owner, int nShapeIdx,
//                  //position and size
//                  LUnits uxLeft, LUnits uyTop, LUnits uxRight, LUnits uyBottom,
//                  //border
//                  LUnits uBorderWidth, Color nBorderColor,
//                  //content
//                  Color nBgColor = *wxWHITE,
//                  //other
//                  wxString sName = _T("Window"),
//				  bool fDraggable = true, bool fSelectable = true,
//                  bool fVisible = true);
//    virtual ~GmoShapeWindow() {}
//
//    //renderization
//    void on_draw(Drawer* pDrawer, RenderOptions& opt);
//
//	//specific methods
//
//protected:
//
//    wxWindow*       m_pControl;      //the window to embbed
//};
//
////global functions defined in this module
//extern wxBitmap* GetBitmapForGlyph(double rScale, int nGlyph, double rPointSize,
//                                   Color colorF, Color colorB);
//

//---------------------------------------------------------------------------------------
class GmoShapeTimeGlyph : public GmoShapeGlyph
{
public:
    GmoShapeTimeGlyph(ImoObj* pCreatorImo, ShapeId idx, unsigned int iGlyph, UPoint pos,
                  Color color, LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_time_signature_glyph, idx, iGlyph,
                        pos, color, libraryScope, fontSize)
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeTimeSignature : public GmoCompositeShape
{
protected:
    LibraryScope& m_libraryScope;

public:
    GmoShapeTimeSignature(ImoObj* pCreatorImo, ShapeId idx, UPoint UNUSED(pos),
                          Color color, LibraryScope& libraryScope)
        : GmoCompositeShape(pCreatorImo, GmoObj::k_shape_time_signature, idx, color)
        , m_libraryScope(libraryScope)
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeOctaveGlyph : public GmoShapeGlyph
{
public:
    GmoShapeOctaveGlyph(ImoObj* pCreatorImo, ShapeId idx, unsigned int iGlyph, UPoint pos,
                        Color color, LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_octave_glyph, idx, iGlyph,
                        pos, color, libraryScope, fontSize)
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapePedalGlyph : public GmoShapeGlyph
{
public:
    GmoShapePedalGlyph(ImoObj* pCreatorImo, ShapeId idx, unsigned int iGlyph, UPoint pos,
                       Color color, LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_pedal_glyph, idx, iGlyph,
                        pos, color, libraryScope, fontSize)
    {
    }

    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;
};

//---------------------------------------------------------------------------------------
class GmoShapeArpeggio : public GmoSimpleShape
{
protected:
    LibraryScope& m_libraryScope;
    double m_fontHeight;

    LUnits m_unusedSpaceTop;
    LUnits m_unusedSpaceBottom;

    unsigned int m_segmentGlyph;
    unsigned int m_segmentCount;
    unsigned int m_arrowGlyph;
    LUnits m_xInitialAdvance;
    LUnits m_yInitialAdvance;
    LUnits m_segmentAdvance;
    bool m_fUp;

public:
    GmoShapeArpeggio(ImoObj* pCreatorImo, ShapeId idx,
                     LUnits xRight, LUnits yTop, LUnits yBottom,
                     bool fUp, bool fHasArrow,
                     Color color, LibraryScope& libraryScope,
                     double fontHeight);

    //implementation of virtual methods in base class
    void on_draw(Drawer* pDrawer, RenderOptions& opt) override;

    void increase_length_up(LUnits increment);

protected:
    void compute_shape_geometry(LUnits xPosRight, LUnits yTop, LUnits yBottom);
};


}   //namespace lomse

#endif    // __LOMSE_SHAPES_H__

