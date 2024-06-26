//---------------------------------------------------------------------------------------
// This file is part of the Lomse library.
// Copyright (c) 2010-present, Lomse Developers
//
// Licensed under the MIT license.
//
// See LICENSE and NOTICE.md files in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#ifndef __LOMSE_ENGRAVING_OPTIONS_H__        //to avoid nested includes
#define __LOMSE_ENGRAVING_OPTIONS_H__

namespace lomse
{


//Engraving options (all measures in Tenths, unless otherwise stated)
//---------------------------------------------------------------------------------------

//Staff
//---------------------------------------------------------------------------------------
//Default values for instantiating scores when defaults are used.
//**DO NOT** use these constants for other purposes. For engravers, take values from ImoScore.
#define LOMSE_STAFF_LINE_THICKNESS      15.0f   // line thickness. LUnits: 0.15 millimeters
#define LOMSE_STAFF_LINE_SPACING       180.0f   // LUnits: 1.8 mm (staff height = 7.2 mm)
#define LOMSE_STAFF_TOP_MARGIN        1000.0f   // LUnits: 10 millimeters
//** END of restricted values, only for instantiating scores
//---------------------------------------------------------------------------------------


//Barlines
#define LOMSE_THIN_LINE_WIDTH            1.5f   // thin line width
#define LOMSE_THICK_LINE_WIDTH           6.0f   // thick line width
#define LOMSE_LINES_SPACING              4.0f   // space between lines: 4 tenths
#define LOMSE_BARLINE_RADIUS             2.0f   // dots radius: 2 tenths

//Beams
// Rule B1 (GOULD, p.17)(STONE, p.9) Beam thickness is 1/2 space
#define LOMSE_BEAM_THICKNESS             5.0f   //"Beam/Thickness of beam line"
#define LOMSE_BEAM_HOOK_LENGTH          11.0f
#define LOMSE_GRACES_BEAM_THICKNESS      3.0f   //"Beam/Thickness of beam line for grace notes"

//Instruments
#define LOMSE_INSTR_SPACE_AFTER_NAME    10.0f   //"Instr/Space after name/"

//Instruments / Groups
#define LOMSE_GRP_SPACE_AFTER_NAME      10.0f   //"InstrGroup/Space after name"
#define LOMSE_GRP_BRACKET_WIDTH          5.0f   //"InstrGroup/Width of bracket"
#define LOMSE_GRP_BRACKET_GAP            5.0f   //"InstrGroup/Space after bracket"
#define LOMSE_GRP_BRACKET_HOOK          10.0f   //"InstrGroup/Length of hook for bracket"
#define LOMSE_GRP_BRACE_WIDTH           12.5f   //"InstrGroup/Width of brace"
#define LOMSE_GRP_BRACE_GAP              5.0f   //"InstrGroup/Space after brace"
#define LOMSE_GRP_SQBRACKET_WIDTH        8.0f   //"InstrGroup/Width of squared bracket"
#define LOMSE_GRP_SQBRACKET_LINE         2.0f   //"InstrGroup/line thickness for squared bracket"

//Notes / accidentals
#define LOMSE_STEM_THICKNESS             1.2f
#define LOMSE_SPACE_BEFORE_DOT           5.0f
#define LOMSE_SPACE_AFTER_ACCIDENTALS    1.5f
#define LOMSE_SPACE_BETWEEN_ACCIDENTALS  1.5f
#define LOMSE_LEGER_LINE_OUTGOING        5.0f
#define LOMSE_GRACE_NOTES_SCALE          0.60f  //Scaling factor for grace notes size
#define LOMSE_CUE_NOTES_SCALE            0.75f  //Scaling factor for cue notes size
#define LOMSE_SHIFT_WHEN_NOTEHEADS_OVERLAP  3.0f   //Offset for notehead when collision with other voice notehead

//Key signatures
#define LOMSE_SPACE_BETWEEN_KEY_ACCIDENTALS     1.0f

//System layouter
    //spacing function parameters
#define LOMSE_MIN_SPACE                 18.0f   //Smin: space for Dmin (tenths) (including the notehead)
    //space
#define LOMSE_EXCEPTIONAL_MIN_SPACE      2.5f
#define LOMSE_SPACE_AFTER_BARLINE       14.0f
#define LOMSE_SPACE_AFTER_SMALL_CLEF    10.0f
#define LOMSE_MIN_SPACE_BEFORE_BARLINE  10.0f
    //prolog (opening measures) [Stone80, p.44]
#define LOMSE_SPACE_BEFORE_PROLOG        7.5f
#define LOMSE_PROLOG_GAP_BEFORE_KEY     10.0f
#define LOMSE_PROLOG_GAP_BEFORE_TIME    10.0f
#define LOMSE_SPACE_AFTER_PROLOG        15.0f
    //space fot key/time signatures after barlines
#define LOMSE_SPACE_BEFORE_KTS_AFTER_BARLINE    10.0f    //after barline, before key or time signature
#define LOMSE_SPACE_BETWEEN_KTS_AFTER_BARLINE   10.0f    //between key and time signatures after barline
#define LOMSE_SPACE_AFTER_KTS_AFTER_BARLINE      7.5f    //space after key+time signatures after barline
    //staves/systems distances
#define LOMSE_MIN_SPACING_STAVES        15.0f   //Min. vertical space between staves
#define LOMSE_MIN_SPACING_SYSTEMS       30.0f   //Min. vertical space between systems

//tuplets
#define LOMSE_TUPLET_BORDER_LENGHT      10.0f
#define LOMSE_TUPLET_BRACKET_DISTANCE    3.0f
#define LOMSE_TUPLET_BRACKET_THICKNESS   1.0f
#define LOMSE_TUPLET_SPACE_TO_NUMBER     3.0f
#define LOMSE_TUPLET_NESTED_DISTANCE    20.0f

//ties
#define LOMSE_TIE_VERTICAL_SPACE         2.0f   //vertical distance (tenths) from notehead
#define LOMSE_TIE_MAX_THICKNESS          4.0f   //tie thickness at center

//volta brackets
#define LOMSE_VOLTA_JOG_LENGHT         20.0f    //volta jog lenght
#define LOMSE_VOLTA_BRACKET_DISTANCE   35.0f    //top staff line to volta line distance
#define LOMSE_VOLTA_BRACKET_THICKNESS   1.0f    //volta line thickness
#define LOMSE_VOLTA_LEFT_SPACE_TO_TEXT  4.0f    //distance between jog line and text
#define LOMSE_VOLTA_TOP_SPACE_TO_TEXT   3.0f    //distance between volta line and text

//wedges
#define LOMSE_WEDGE_LINE_THICKNESS      1.5f    //line thickness for wedges/hairpins
#define LOMSE_WEDGE_NIENTE_RADIUS       4.0f    //radius for niente circles in wedges
#define LOMSE_WEDGE_HORIZONTAL_ALIGN_DISTANCE 6.0f //space between a wedge and the nearest wedge or dynamic mark
#define LOMSE_WEDGE_ALIGN_MAX_EDGE_SHIFT 50.0f  //maximum amount of space a wedge's end can be horizontally shifted if collision with another wedge or dynamic mark is detected
#define LOMSE_WEDGE_BASELINE_SHIFT_Y    5.0f    //wedge baseline shift relative to its center to properly align with dynamic marks

//octave-shift lines
#define LOMSE_OCTAVE_SHIFT_LINE_THICKNESS  1.0f //thickness for octave-shift lines
#define LOMSE_OCTAVE_SHIFT_SPACE_TO_LINE   2.0f //space from numeral glyph to line start
#define LOMSE_OCTAVE_SHIFT_LINE_SHIFT      3.0f //vertical line shift to compensate glyph baseline

//pedals
#define LOMSE_PEDAL_LINE_THICKNESS         1.0f //thickness for pedal lines
#define LOMSE_PEDAL_SPACE_TO_LINE          2.0f //space from pedal sign to line start
#define LOMSE_PEDAL_STAFF_DISTANCE        40.0f //default distance between pedals and staff lines
#define LOMSE_PEDAL_CONTENT_DISTANCE       5.0f //minimal distance between pedals and the vertical profile
#define LOMSE_PEDAL_CHANGE_GLYPH_OVERLAP   0.5f //overlap between a pedal line and a pedal change symbol, to avoid visual gaps between them

//articulations
#define LOMSE_SPACING_STACKED_ARTICULATIONS  2.0f //space between two stacked articulation marks
#define LOMSE_SPACING_NOTEHEAD_ARTICULATION  5.0f //space between notehead and articulation marks

//lyrics
#define LOMSE_LYRICS_SPACE_TO_MUSIC     12.0f //space between first lyric line and other music notation
#define LOMSE_LYRICS_LINES_EXTRA_SPACE   3.0f //additional space between two lyric lines

//arpeggios
#define LOMSE_ARPEGGIO_SPACE_TO_CHORD    6.0f   //Spacing between an arpeggio and a chord
#define LOMSE_ARPEGGIO_MAX_OUTGOING      3.0f   //Amount of space arpeggio is allowed to go beyond a chord's top or bottom note

//playback
#define LOMSE_STEAL_TIME_SHORT          20.0f //"Playback/Percentage of time to steal for acciaccatura grace notes"
#define LOMSE_STEAL_TIME_LONG           50.0f //"Playback/Percentage of time to steal for appoggiatura grace notes"

//fingering
#define LOMSE_FINGERING_DISTANCE                10.0f   //space above/below staff or other objects and fingering symbols
#define LOMSE_FINGERING_AFTER_SPACE              2.0f   //space after fingering symbol and next fingering symbol
#define LOMSE_FINGERING_VERTICAL_SPACING         2.0f   //vertical spacing when stacking fingering symbols (chords)
#define LOMSE_FINGERING_SPACING_STACKED_ARTICULATIONS  6.0f     //separation to other articulation/technical

}   //namespace lomse

#endif    // __LOMSE_ENGRAVING_OPTIONS_H__

