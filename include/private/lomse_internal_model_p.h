//---------------------------------------------------------------------------------------
// This file is part of the Lomse library.
// Copyright (c) 2010-present, Lomse Developers
//
// Licensed under the MIT license.
//
// See LICENSE and NOTICE.md files in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#ifndef __LOMSE_INTERNAL_MODEL_P__
#define __LOMSE_INTERNAL_MODEL_P__

    //***********************************************************************
    //*                                                                     *
    //*                         -------------                               *
    //*                         W A R N I N G                               *
    //*                         -------------                               *
    //*                                                                     *
    //*    This file is not part of lomse public API. It is here as a       *
    //*    convenience for lomse internal use, while the public API is      *
    //*    being defined and implemented. This header file may change       *
    //*    from version to version, or even be removed without notice.      *
    //*                                                                     *
    //*                        You've been warned!                          *
    //*                                                                     *
    //***********************************************************************

#include "lomse_visitor.h"
#include "lomse_tree.h"
#include "lomse_basic.h"
#include "lomse_score_enums.h"
#include "lomse_shape_base.h"
#include "lomse_events.h"
#include "lomse_pixel_formats.h"
#include "lomse_injectors.h"
#include "lomse_image.h"
#include "lomse_logger.h"
#include "lomse_engraving_options.h"
typedef int TIntAttribute;

#include <string>
#include <list>
#include <vector>
#include <map>
#include <sstream>
using namespace std;

///@cond INTERNALS
namespace lomse
{
///@endcond


//forward declaration of the implementation classes for all API classes
class ImoDocument;

//---------------------------------------------------------------------------------------
//forward declarations
class ColStaffObjs;
class ColStaffObjsEntry;
class LdpElement;
class SoundEventsTable;
class Document;
class DocModel;
class DiatonicPitch;
class EventHandler;
class Control;
class ScorePlayerCtrl;
class ButtonCtrl;
class AttribsContainer;
class ImMeasuresTable;
class ImMeasuresTableEntry;

class ImoAttachments;
class ImoAuxObj;
class ImoBarline;
class ImoBeamData;
class ImoBeamDto;
class ImoBeam;
class ImoBlocksContainer;
class ImoInlinesContainer;
class ImoBoxInline;
class ImoBlockLevelObj;
class ImoButton;
class ImoControl;
class ImoChord;
class ImoContent;
class ImoContentObj;
class ImoDirection;
class ImoDynamic;
class ImoFontStyleDto;
class ImoHeading;
class ImoImage;
class ImoInlineLevelObj;
class ImoInlineWrapper;
class ImoInstrument;
class ImoKeySignature;
class ImoLineStyleDto;
class ImoLink;
class ImoList;
class ImoListItem;
class ImoLyric;
class ImoLyricsTextInfo;
class ImoMultiColumn;
class ImoMusicData;
class ImoNote;
class ImoNoteRest;
class ImoObj;
class ImoOptionInfo;
class ImoParagraph;
class ImoParameters;
class ImoParamInfo;
class ImoPedalLine;
class ImoPedalLineDto;
class ImoRelations;
class ImoRelDataObj;
class ImoRelObj;
class ImoScore;
class ImoScoreLine;
class ImoScorePlayer;
class ImoScoreText;
class ImoSimpleObj;
class ImoSlurDto;
class ImoSoundInfo;
class ImoSounds;
class ImoStaffInfo;
class ImoStaffObj;
class ImoStyle;
class ImoStyles;
class ImoTable;
class ImoTableCell;
class ImoTableBody;
class ImoTableHead;
class ImoTableRow;
class ImoTextItem;
class ImoTextStyle;
class ImoTieData;
class ImoTieDto;
class ImoTimeModificationDto;
class ImoTimeSignature;
class ImoTupletDto;
class ImoTuplet;
class ImoWedge;
class ImoWedgeDto;
class ImoWrapperBox;


//---------------------------------------------------------------------------------------
// some helper defines
#define k_no_visible    false

//---------------------------------------------------------------------------------------
//limits
#define k_max_voices    64      //number of voices per instrument

//---------------------------------------------------------------------------------------
// enums for common values

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes values for placement attribute.
    The placement attribute indicates whether something is above or below another
    element, such as a note or a notation.

    @#include <lomse_internal_model.h>
*/
enum EPlacement
{
    k_placement_default = 0,    ///< Use default placement
    k_placement_above,          ///< Place it above
    k_placement_below,          ///< Place it below
};

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes values for orientation attribute.
    The orientation attribute indicates whether slurs and ties are overhand (tips
    down) or underhand (tips up). This is distinct from the placement attribute:
    placement is relative, one object with respect to another object. But
    orientation is referred to the object itself: turned up or down.

    @#include <lomse_internal_model.h>
*/
enum EOrientation
{
    k_orientation_default = 0,      ///< Default orientation
    k_orientation_over,             ///< Over (tips down)
    k_orientation_under,            ///< Under (tips up)
};

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes values for line type.

    @#include <lomse_internal_model.h>
*/
enum ELineType
{
    k_line_type_solid = 0,  ///< Solid line
    k_line_type_dashed,     ///< Dashed line
    k_line_type_dotted,     ///< Dotted line
    k_line_type_wavy,       ///< Wavy line
};

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes values for tye line-shape attribute. This attribute is used
    to distinguish between straight and curved lines.

    @#include <lomse_internal_model.h>
*/
enum ELineShape
{
    k_line_shape_straight = 0,  ///< Strait line
    k_line_shape_curved,        ///< Curved line
};


//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid values for clefs.

    @#include <lomse_internal_model.h>
*/
enum EClef
{
    k_clef_undefined=-1,    ///< Undefined clef
    k_clef_G2 = 0,          ///< G clef on second line (treble clef)
    k_clef_F4,              ///< F clef on fourth line (bass clef)
    k_clef_F3,              ///< F clef on third line (baritone clef)
    k_clef_C1,              ///< C clef on first line (soprano clef)
    k_clef_C2,              ///< C clef on second line (mezzo-soprano clef)
    k_clef_C3,              ///< C clef on third line (alto clef)
    k_clef_C4,              ///< C clef on fourth line (tenor clef)
    k_clef_percussion,      ///< Percussion clef
    // other clefs not available for exercises
    k_clef_C5,              ///< C clef on fifth line (baritone clef)
    k_clef_F5,              ///< F clef on fifth line (subbass clef)
    k_clef_G1,              ///< G clef on first line (French violin)
    k_clef_8_G2,            ///< G clef on second line, 8ve. above
    k_clef_G2_8,            ///< G clef on second line, 8ve. below
    k_clef_8_F4,            ///< F clef on fourth line, 8ve. above
    k_clef_F4_8,            ///< F clef on fourth line, 8ve below
    k_clef_15_G2,           ///< G clef on second line, 15 above
    k_clef_G2_15,           ///< G clef on second line, 15 below
    k_clef_15_F4,           ///< F clef on fourth line, 15 above
    k_clef_F4_15,           ///< F clef on fourth line, 15 below
    //clefs for supporting MusicXML
    k_clef_TAB,             ///< Tablature scores
    k_clef_none,            ///< none displayed, but behave as G2
    k_max_clef,             ///< Last element, for loops and checks

    //clef sign, for clefs specified as sign and line
    k_clef_sign_G,              ///< G clef
    k_clef_sign_F,              ///< F clef
    k_clef_sign_C,              ///< C clef
    k_clef_sign_percussion,     ///< Percussion clef
    k_clef_sign_TAB,            ///< For tablature
    k_clef_sign_none,           ///< none
};

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid values for standard key signatures.

    @#include <lomse_internal_model.h>
*/
enum EKeySignature
{
    k_key_undefined=-1,         ///< Undefined key signature

    k_key_C=0,                  ///< C major key signature
    k_min_key = k_key_C,        ///< Minimum valid value = C major key signature
    k_min_major_key = k_key_C,  ///< Minimum value for major keys = C major key signature
    k_key_G,                    ///< G major key signature
    k_key_D,                    ///< D major key signature
    k_key_A,                    ///< A major key signature
    k_key_E,                    ///< E major key signature
    k_key_B,                    ///< B major key signature
    k_key_Fs,                   ///< F-sharp major key signature
    k_key_Cs,                   ///< C-sharp major key signature
    k_key_Cf,                   ///< C-flat major key signature
    k_key_Gf,                   ///< G-flat major key signature
    k_key_Df,                   ///< D-flat major key signature
    k_key_Af,                   ///< A-flat major key signature
    k_key_Ef,                   ///< E-flat major key signature
    k_key_Bf,                   ///< B-flat major key signature
    k_key_F,                    ///< F major key signature
    k_max_major_key = k_key_F,  ///< Maximum value for valid major key signatures = F major

    k_key_a,                    ///< A minor key signature
    k_min_minor_key = k_key_a,  ///< Minimum value for minor keys = A minor key signature
    k_key_e,                    ///< E minor key signature
    k_key_b,                    ///< B minor key signature
    k_key_fs,                   ///< F-sharp minor key signature
    k_key_cs,                   ///< C-sharp minor key signature
    k_key_gs,                   ///< G-sharp minor key signature
    k_key_ds,                   ///< D-sharp minor key signature
    k_key_as,                   ///< A-sharp minor key signature
    k_key_af,                   ///< A-flat minor key signature
    k_key_ef,                   ///< E-flat minor key signature
    k_key_bf,                   ///< B-flat minor key signature
    k_key_f,                    ///< F minor key signature
    k_key_c,                    ///< C minor key signature
    k_key_g,                    ///< G minor key signature
    k_key_d,                    ///< D minor key signature
    k_max_minor_key = k_key_d,  ///< Maximum value for valid minor key signatures = D minor
    k_max_key = k_key_d,        ///< Maximum value for valid key signatures = D minor

    k_key_non_standard,         ///< For non-standard keys
};
#define k_num_keys k_max_key - k_min_key + 1

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid modes for standard key signatures.

    @#include <lomse_internal_model.h>
*/
enum EKeyMode
{
    k_key_mode_none = 0,
    k_key_mode_major,
    k_key_mode_minor,
    k_key_mode_dorian,
    k_key_mode_phrygian,
    k_key_mode_lydian,
    k_key_mode_mixolydian,
    k_key_mode_aeolian,
    k_key_mode_ionian,
    k_key_mode_locrian,
};


//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid values for the number of fifths in standard key signatures.

    @#include <lomse_internal_model.h>
*/
enum EKeyFifths
{
    k_fifths_Cf = -7,              ///< C-flat
    k_fifths_Gf,                   ///< G-flat
    k_fifths_Df,                   ///< D-flat
    k_fifths_Af,                   ///< A-flat
    k_fifths_Ef,                   ///< E-flat
    k_fifths_Bf,                   ///< B-flat
    k_fifths_F,                    ///< F
    k_fifths_C,                    ///< C
    k_fifths_G,                    ///< G
    k_fifths_D,                    ///< D
    k_fifths_A,                    ///< A
    k_fifths_E,                    ///< E
    k_fifths_B,                    ///< B
    k_fifths_Fs,                   ///< F-sharp
    k_fifths_Cs,                   ///< C-sharp
};


//---------------------------------------------------------------------------------------
// Note steps: 'step' refers to the diatonic note name in the octave
/** @ingroup enumerations

    This enum describes valid note steps. 'step' refers to the diatonic note name in the octave

    @#include <lomse_pitch.h>
*/
enum ESteps
{
    k_no_pitch = -1,    ///< No pitch assigned
    k_step_undefined = -1,  ///< No step assigned
    k_step_C = 0,       ///< C note (Do)
    k_step_D,           ///< D note (Re)
    k_step_E,           ///< E note (Mi)
    k_step_F,           ///< F note (Fa)
    k_step_G,           ///< G note (Sol)
    k_step_A,           ///< A note (La)
    k_step_B,           ///< B note (Si)
};


//---------------------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid octave numbers.
    The octave is represented by a number in the range 0..9 (scientific notation).
    This is the same meaning as in MIDI (note A in octave 4 = 440Hz).
    The lowest MIDI octave (-1) is not defined.

    @#include <lomse_pitch.h>
*/
enum EOctave
{
    k_octave_undefined = -9,    ///< Octave is not defined
    k_octave_0 = 0,     ///< Octave 0. C0 = 16.352 Hz
    k_octave_1,         ///< Octave 1 (first octave, contra octave). C1 = 32.703 Hz
    k_octave_2,         ///< Octave 2
    k_octave_3,         ///< Octave 3
    k_octave_4,         ///< Octave 4. C4 = middle C. A4 = 440 Hz
    k_octave_5,         ///< Octave 5
    k_octave_6,         ///< Octave 6
    k_octave_7,         ///< Octave 7
    k_octave_8,         ///< Octave 8
    k_octave_9,         ///< Octave 9. B9 = 15,804.3 Hz
};


//---------------------------------------------------------------------------------------
// Accidentals
/** @ingroup enumerations

    This enum describes valid accidentals.

    @#include <lomse_pitch.h>
*/
enum EAccidentals
{
    k_invalid_accidentals = -1,     ///< Invalid value for accidentals
    k_no_accidentals = 0,           ///< No accidental sign

    //standard accidentals
    k_natural,                      ///< Natural accidental sign
    k_flat,                         ///< Flat accidental sign (b)
    k_sharp,                        ///< Sharp accidental sign (#)
    k_flat_flat,                    ///< Two consecutive flat signs (bb)
    k_double_sharp,                 ///< The double sharp symbol (x)
    k_sharp_sharp,                  ///< Two consecutive sharp signs (##)
    k_natural_flat,                 ///< Natural sign followed by flat sign
    k_natural_sharp,                ///< Natural sign followed by sharp sign
	k_acc_triple_sharp,             ///< Triple sharp accidental
	k_acc_triple_flat,              ///< Triple flat accidental

    //microtonal accidentals: Tartini-style quarter-tone accidentals
    k_acc_quarter_flat,             ///< microtonal: quarter flat accidental
    k_acc_quarter_sharp,            ///< microtonal: quarter sharp accidental
	k_acc_three_quarters_flat,      ///< microtonal: three quarters flat accidental
	k_acc_three_quarters_sharp,     ///< microtonal: three quarters sharp accidental

    //microtonal accidentals: quarter-tone accidentals that include arrows pointing down or up
	k_acc_sharp_down,               ///< microtonal: sharp down accidental
	k_acc_sharp_up,                 ///< microtonal: sharp up accidental
	k_acc_natural_down,             ///< microtonal: natural down accidental
	k_acc_natural_up,               ///< microtonal: natural up accidental
	k_acc_flat_down,                ///< microtonal: flat down accidental
	k_acc_flat_up,                  ///< microtonal: flat up accidental
	k_acc_double_sharp_down,        ///< microtonal: double sharp down accidental
	k_acc_double_sharp_up,          ///< microtonal: double sharp up accidental
	k_acc_flat_flat_down,           ///< microtonal: flat flat down accidental
	k_acc_flat_flat_up,             ///< microtonal: flat flat up accidental
	k_acc_arrow_down,               ///< microtonal: arrow down accidental
	k_acc_arrow_up,                 ///< microtonal: arrow up accidental

	//accidentals used in Turkish classical music
	k_acc_slash_quarter_sharp,      ///< Turkish classical music Küçük mücenneb (sharp)
	k_acc_slash_sharp,              ///< Turkish classical music: Büyük mücenneb (sharp)
	k_acc_slash_flat,               ///< Turkish classical music: Bakiye (flat)
	k_acc_double_slash_flat,        ///< Turkish classical music: Büyük mücenneb (flat)

	//superscripted versions of standard accidental signs, used in Turkish folk music
	k_acc_sharp_1,                  ///< Turkish folk music: sharp 1 accidental
	k_acc_sharp_2,                  ///< Turkish folk music: sharp 2 accidental
	k_acc_sharp_3,                  ///< Turkish folk music: sharp 3 accidental
	k_acc_sharp_5,                  ///< Turkish folk music: sharp 5 accidental
	k_acc_flat_1,                   ///< Turkish folk music: flat 1 accidental
	k_acc_flat_2,                   ///< Turkish folk music: flat 2 accidental
	k_acc_flat_3,                   ///< Turkish folk music: flat 3 accidental
	k_acc_flat_4,                   ///< Turkish folk music: flat 4 accidental

    //microtonal flat and sharp accidentals used in Iranian and Persian music
	k_acc_sori,                     ///< Persian sori accidental
	k_acc_koron,                    ///< Persian koron accidental

	//the ‘other’ accidental covers accidentals other than those listed here.
    //it is usually used in combination with the SMuFl glyph to use
	k_acc_other,                    ///< other. Should specify SMuFl glyph to use
};


//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid note/rest types.

    @#include <lomse_internal_model.h>
*/
enum ENoteType
{
    k_unknown_notetype = -1,    ///< Unknown note type
    k_longa = 0,                ///< Longa note
    k_breve = 1,                ///< Double note (breve)
    k_whole = 2,                ///< Whole note (semibreve)
    k_half = 3,                 ///< Half note (minim)
    k_quarter = 4,              ///< Quarter note (crotchet)
    k_eighth = 5,               ///< Eighth note (quaver)
    k_16th = 6,                 ///< Sexteenth note (semiquaver)
    k_32nd = 7,                 ///< Thirty-second note (demisemiquaver)
    k_64th = 8,                 ///< Sixty-fourth note (hemidemisemiquaver)
    k_128th = 9,                ///< Hundred twenty-eighth note (semihemidemisemiquaver)
    k_256th = 10,               ///< Two hundred fifty-six note

    k_max_note_type,            ///< Last element, for loops and checks
};


//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes note/rest durations (in Lomse Time Units). It is
    useful for comparisons.

    @#include <lomse_internal_model.h>
*/
enum ENoteDuration
{
    k_duration_longa_dotted = 1536,     ///< Duration of a dotted longa note
    k_duration_longa = 1024,            ///< Duration of a longa note
    k_duration_breve_dotted = 768,      ///< Duration of a dotted breve note
    k_duration_breve = 512,             ///< Duration of a breve note
    k_duration_whole_dotted = 384,      ///< Duration of a dotted whole note
    k_duration_whole = 256,             ///< Duration of a whole note
    k_duration_half_dotted = 192,       ///< Duration of a dotted half note
    k_duration_half = 128,              ///< Duration of a half note
    k_duration_quarter_dotted = 96,     ///< Duration of a dotted quarter note
    k_duration_quarter = 64,            ///< Duration of a quarter note
    k_duration_eighth_dotted = 48,      ///< Duration of a dotted eighth note
    k_duration_eighth = 32,             ///< Duration of a eighth note
    k_duration_16th_dotted = 24,        ///< Duration of a dotted 16th note
    k_duration_16th = 16,               ///< Duration of a 16th note
    k_duration_32nd_dotted = 12,        ///< Duration of a dotted 32nd note
    k_duration_32nd = 8,                ///< Duration of a 32nd note
    k_duration_64th_dotted = 6,         ///< Duration of a dotted 64th note
    k_duration_64th = 4,                ///< Duration of a 64th note
    k_duration_128th_dotted = 3,        ///< Duration of a dotted 128th note
    k_duration_128th = 2,               ///< Duration of a 128th note
    k_duration_256th = 1,               ///< Duration of a 256th note
};

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid values for barlines.

    @#include <lomse_internal_model.h>
*/
enum EBarline
{
    k_barline_unknown = -1,         ///< Unknown barline type
    k_barline_none,                 ///< No barline
    k_barline_simple,               ///< Simple barline
    k_barline_double,               ///< Double barline
    k_barline_start,                ///< Start barline
    k_barline_end,                  ///< End barline
    k_barline_start_repetition,     ///< Start of repetition barline
    k_barline_end_repetition,       ///< End of repetition barline
    k_barline_double_repetition,    ///< Double repetition barline
    k_barline_double_repetition_alt,  ///< heavy-heavy double repetition. See E.Gould, p.234
    k_barline_heavy_heavy,          ///< heavy-heavy when no repetition
    k_barline_dashed,               ///< dashed barline
    k_barline_dotted,               ///< dotted barline
    k_barline_heavy,                ///< heavy barline
    k_barline_short,                ///< short barline
    k_barline_tick,                 ///< tick barline

    k_max_barline,                  ///< Last element, for loops and checks
};

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid values for the winged attribute. It indicates
	whether a repeat barline has winged extensions that appear above and below
	the barline. The straight and curved values represent single wings,
	while the double-straight and double-curved values represent double wings.
	The none value indicates no wings.

    @#include <lomse_internal_model.h>
*/
enum EBarlineWings
{
    k_wings_none = 0,				///< No wings. Default value
    k_wings_straight,				///< Strait wings
    k_wings_curved,					///< Curved wings
    k_wings_double_straight,		///< Double strait wings
    k_wings_double_curved,			///< Doube curved wings
};

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid values for articulations.

    @#include <lomse_internal_model.h>
*/
enum EArticulations
{
    k_articulation_unknown = -1,        ///< Unknown articulation

    //accents
    k_articulation_accent,					///< Accent
    k_articulation_strong_accent,       	///< Strong accent
    k_articulation_detached_legato,     	///< Detached legato
    k_articulation_legato_duro,				///< Legato duro
    k_articulation_marccato,				///< Marccato
    k_articulation_marccato_legato,			///< Marccato-legato
    k_articulation_marccato_staccato,		///< Marccato-staccato
    k_articulation_marccato_staccatissimo,	///< Marccato-staccatissimo
    k_articulation_mezzo_staccato,			///< Mezzo-staccato
    k_articulation_mezzo_staccatissimo,		///< Mezzo-staccatissimo
    k_articulation_staccato,				///< Staccato
    k_articulation_staccato_duro,			///< Staccato-duro
    k_articulation_staccatissimo_duro,		///< Staccatissimo-duro
    k_articulation_staccatissimo,			///< Staccatissimo
    k_articulation_tenuto,					///< Tenuto

    //jazz pitch articulations
    k_articulation_scoop,					///< Jazz pitch: Scoop
    k_articulation_plop,					///< Jazz pitch: Plop
    k_articulation_doit,					///< Jazz pitch: Doit
    k_articulation_falloff,					///< Jazz pitch: Fall off

    //stress articulations
    k_articulation_stress,              ///< Stress
    k_articulation_unstress,             ///< Unstress

    //other in MusicXML
    k_articulation_spiccato,            ///< Spicato

    //breath marks
    k_articulation_breath_mark,         ///< Breath mark
    k_articulation_caesura,             ///< Caesura

    k_max_articulation,						///< Last element, for loops and checks
};

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid values for argpeggio types.

    @#include <lomse_internal_model.h>
*/
enum EArpeggio
{
    k_arpeggio_standard,    ///< Standard arpeggio
    k_arpeggio_arrow_up,    ///< Arpeggio with an up arrow
    k_arpeggio_arrow_down,  ///< Arpeggio with a down arrow
};

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid values for ornaments.

    @#include <lomse_internal_model.h>
*/
enum EOrnaments
{
    k_ornament_unknown = -1,            ///< Unknown ornament
    k_ornament_trill_mark,              ///< Trill mark
    k_ornament_vertical_turn,           ///< Vertical turn
    k_ornament_shake,                   ///< Shake
    k_ornament_turn,                    ///< Turn
    k_ornament_delayed_turn,            ///< Delayed turn
    k_ornament_inverted_turn,           ///< Inverted turn
    k_ornament_delayed_inverted_turn,   ///< Delayed inverted turn
    k_ornament_mordent,                 ///< Mordent
    k_ornament_inverted_mordent,        ///< Inverted mordent
    k_ornament_wavy_line,               ///< Wavy line
    k_ornament_schleifer,               ///< Schleifer
    k_ornament_tremolo,                 ///< Tremolo
    k_ornament_other,                   ///< Other ornaments (for MusicXML)

    k_max_ornament,                         ///< Last element, for loops and checks
};

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid values for pedal marks.

    @#include <lomse_internal_model.h>
*/
enum EPedalMark
{
    k_pedal_mark_unknown = -1,          ///< Unknown pedal mark
    k_pedal_mark_start,                 ///< Start of a damper pedal
    k_pedal_mark_sostenuto_start,       ///< Start of a sostenuto pedal
    k_pedal_mark_stop,                  ///< Pedal stop
};

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid values for technical marks.

    @#include <lomse_internal_model.h>
*/
enum ETechnical
{
    k_technical_unknown = -1,       ///< Unknown technical mark
    k_technical_up_bow,             ///< Bow up
    k_technical_down_bow,           ///< Bow down
    k_technical_harmonic,           ///< Harmonic
    k_technical_fingering,          ///< Fingering
    k_technical_double_tongue,      ///< Double tongue
    k_technical_triple_tongue,      ///< Triple tongue
    k_technical_hole,               ///< Hole
    k_technical_handbell,           ///< Handbell
    k_technical_fret_string,      ///< Finger & string, for tablature and chord diagrams

    k_max_technical,				///< Last element, for loops and checks
};

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid values for repetition marks and sound directions.

    @#include <lomse_internal_model.h>
*/
enum ERepeatMark
{
    k_repeat_none = 0,				///< No mark
    k_repeat_segno,					///< Segno
    k_repeat_coda,					///< Coda
    k_repeat_fine,					///< Fine
    k_repeat_da_capo,				///< D.C.
    k_repeat_da_capo_al_fine,		///< D.C. al Fine
    k_repeat_da_capo_al_coda,		///< D.C. al Coda
    k_repeat_dal_segno,				///< Dal Segno
    k_repeat_dal_segno_al_fine,		///< Dal Segno al Fine
    k_repeat_dal_segno_al_coda,		///< Dal Segno al Coda
    k_repeat_to_coda,				///< To Coda
};

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    When engravers decide the direction for an stem, the computed value is represented
    by a value from this enum.

    @#include <lomse_internal_model.h>
*/
enum EComputedStem
{
    k_computed_stem_undecided,       ///< No yet computed
    k_computed_stem_up,              ///< Stem will be up. Not forced by the user
    k_computed_stem_down,            ///< Stem will be down. Not forced by the user
    k_computed_stem_forced_up,       ///< Stem is forced by the user to be up
    k_computed_stem_forced_down,     ///< Stem is forced by the user to be down
    k_computed_stem_none,            ///< No stem
};

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    When engravers decide the position for a beam, the computed value is represented
    by a value from this enum.

    @#include <lomse_internal_model.h>
*/
enum EComputedBeam
{
    k_beam_undecided = 0,   ///< No yet computed
    k_beam_above,           ///< Beam will be placed above. Stems up
    k_beam_below,           ///< Beam will be placed below. Stems down
    k_beam_double_stemmed,  ///< Beam with stems at both sides
};

//-----------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid types for internal model objects.

    @#include <lomse_internal_model.h>

    @todo Document other ImoObj type values
*/
enum EImoObjType
{
    // ImoObj (A)
    k_imo_obj=0,

    //ImoDto (A)
    k_imo_dto,
    k_imo_arpeggio_dto,
    k_imo_beam_dto,
    k_imo_border_dto,
    k_imo_color_dto,
    k_imo_font_style_dto,
    k_imo_line_style_dto,
    k_imo_pedal_line_dto,
    k_imo_point_dto,
    k_imo_octave_shift_dto,
    k_imo_size_dto,
    k_imo_slur_dto,
    k_imo_tie_dto,
    k_imo_time_modification_dto,
    k_imo_tuplet_dto,
    k_imo_volta_bracket_dto,
    k_imo_wedge_dto,
    k_imo_dto_last,

    // ImoSimpleObj (A)
    k_imo_simpleobj,

    // value objects, never nodes in tree
    k_imo_bezier_info,
    k_imo_cursor_info,
    k_imo_figured_bass_info,
    k_imo_instr_group,
    k_imo_lyrics_text_info,
    k_imo_midi_info,
    k_imo_page_info,
//    k_imo_play_info,
    k_imo_sound_info,
    k_imo_staff_info,
    k_imo_system_info,
    k_imo_textblock_info,
    k_imo_text_style,

    // nodes in tree
    k_imo_option,
    k_imo_param_info,
    k_imo_style,

    // ImoRelDataObj (A)
    k_imo_reldataobj,
    k_imo_beam_data,
    k_imo_slur_data,
    k_imo_tie_data,
    k_imo_reldataobj_last,

    //ImoCollection(A)
    k_imo_collection,
    k_imo_attachments,
    k_imo_instruments,
    k_imo_instrument_groups,
    k_imo_music_data,
    k_imo_options,
    k_imo_score_titles,
    k_imo_sounds,
    k_imo_parameters,
    k_imo_table_head,
    k_imo_table_body,
    k_imo_collection_last,

    // Special collections
    k_imo_styles,
    k_imo_relations,

    // ImoContainerObj (A)
    k_imo_containerobj,
    k_imo_instrument,
    k_imo_containerobj_last,

    k_imo_simpleobj_last,


    // ImoContentObj (A)
    k_imo_contentobj,

    // ImoScoreObj (A) content only for scores
    k_imo_scoreobj,             ///< <b>Score objects: content only valid for music scores. Any of the following:</b>

    // ImoStaffObj (A)
    k_imo_staffobj,             ///< &nbsp;&nbsp;&nbsp;&nbsp; <b>Staff object. Any of the following:</b>
    k_imo_barline,          ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Barline
    k_imo_clef,             ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Clef
    k_imo_direction,        ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Direction: a musical indication that is not attached to a specific note.
    k_imo_figured_bass,     ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Figured bass mark
    k_imo_go_back_fwd,      ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; GoBackFwd
    k_imo_key_signature,    ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Key signature
    k_imo_note_regular,     ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Note (regular)
    k_imo_note_grace,       ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Note (grace)
    k_imo_note_cue,         ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Note (cue)
    k_imo_rest,             ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Rest
    k_imo_sound_change,     ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Playback parameters
    k_imo_system_break,     ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; System break
    k_imo_time_signature,   ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Time signature
    k_imo_transpose,        ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Transposition information
    k_imo_staffobj_last,

    // ImoAuxObj (A)
    k_imo_auxobj,               ///< &nbsp;&nbsp;&nbsp;&nbsp; <b>Auxiliary object. Any of the following:</b>
    k_imo_dynamics_mark,    ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Dynamics mark
    k_imo_fermata,          ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Fermata
    k_imo_fret_string,      ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Fret and string, for tablature and chord diagrams
    k_imo_metronome_mark,   ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Metronome mark
    k_imo_ornament,         ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Ornament
    k_imo_pedal_mark,       ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Pedal mark
    k_imo_symbol_repetition_mark,   ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Symbol for repetition mark
    k_imo_technical,        ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Technical mark
    k_imo_fingering,        ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Technical mark: Fingering
    k_imo_text_repetition_mark, ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Text for repetition mark

    // ImoArticulation (A)
    k_imo_articulation,
    k_imo_articulation_symbol,  ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Articulation symbol
    k_imo_articulation_line,    ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Articulation line
    k_imo_articulation_last,

    k_imo_score_text,
    k_imo_score_title,
    k_imo_line,
    k_imo_score_line,
    k_imo_text_box,

    // ImoAuxReloObj (A)
    k_imo_auxrelobj,
    k_imo_lyric,        ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Lyrics

    k_imo_auxobj_last,

    // ImoRelObj (A)
    k_imo_relobj,           ///< &nbsp;&nbsp;&nbsp;&nbsp; <b>Relation objects. Any of the following:</b>
    k_imo_arpeggio,         ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Arpeggio
    k_imo_beam,             ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Beam
    k_imo_chord,            ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Chord
    k_imo_grace_relobj,     ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Grace notes relationship
    k_imo_octave_shift,     ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Octave-shift line
    k_imo_pedal_line,       ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Pedal line
    k_imo_slur,             ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Slur
    k_imo_tie,              ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Tie
    k_imo_tuplet,           ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Tuplet
    k_imo_volta_bracket,    ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Volta bracket
    k_imo_wedge,            ///< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Wedge
    k_imo_relobj_last,

    k_imo_scoreobj_last,

    // ImoBlockLevelObj (A)
    k_imo_block_level_obj,	///< <b>Block level objects: Any of the following:</b>
    k_imo_score,		    ///< &nbsp;&nbsp;&nbsp;&nbsp; Music score

    // ImoBlocksContainer (A)
    k_imo_blocks_container,
    k_imo_content,
    k_imo_dynamic,
    k_imo_document,
    k_imo_list,         ///< &nbsp;&nbsp;&nbsp;&nbsp; Text list
    k_imo_listitem,
    k_imo_multicolumn,
    k_imo_table,        ///< &nbsp;&nbsp;&nbsp;&nbsp; Table
    k_imo_table_cell,
    k_imo_table_row,
    k_imo_blocks_container_last,

    // ImoInlinesContainer (A)
    k_imo_inlines_container,
    k_imo_anonymous_block,
    k_imo_heading,      ///< &nbsp;&nbsp;&nbsp;&nbsp; Text header
    k_imo_para,         ///< &nbsp;&nbsp;&nbsp;&nbsp; Paragraph
    k_imo_inlines_container_last,

    k_imo_block_level_obj_last,

    // ImoInlineLevelObj
    k_imo_inline_level_obj,  ///< <b>Inline level objects: Any of the following:</b>
    k_imo_image,                ///< &nbsp;&nbsp;&nbsp;&nbsp; Image
    k_imo_text_item,            ///< &nbsp;&nbsp;&nbsp;&nbsp; Text item
    k_imo_control,
    k_imo_score_player,     ///< &nbsp;&nbsp;&nbsp;&nbsp; Score player control
    k_imo_button,           ///< &nbsp;&nbsp;&nbsp;&nbsp; Button control
    k_imo_control_end,

    // ImoBoxInline (A)
    k_imo_box_inline,
    k_imo_inline_wrapper,
    k_imo_link,             ///< &nbsp;&nbsp;&nbsp;&nbsp; Link
    k_imo_box_inline_last,

    k_imo_inline_level_obj_last,

    k_imo_contentobj_last,
    k_imo_last,

};

//---------------------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid values for score option "Score.JustifyLastSystem". This
    option controls the justification of the last system.

    @#include <lomse_internal_model.h>
*/
enum EJustifyLastSystemOpts
{
	k_justify_never = 0,			///< Never justify last system
    k_justify_barline_final = 1,	///< Justify it only if ends in barline of type final
    k_justify_barline_any = 2,		///< Justify it only if ends in barline of any type
    k_justify_always = 3,			///< Justify it in any case
};

//---------------------------------------------------------------------------------------
/** @ingroup enumerations

    When a system is not justified, the staff lines should always run until right margin
	of the score, but this behavior can be controlled by the score option
	"StaffLines.Truncate". This option only works when a system is not justified and
	the valid values are described by this enum.

    Option 'k_truncate_barline_final' is the default behavior and it can be useful
	for score editors: staff lines will run always to right margin until a barline of
	type final is entered.

    Option 'k_truncate_always' truncates staff lines after last staff object. It can
	be useful for creating score samples (e.g., for ebooks).

    @#include <lomse_internal_model.h>
*/
enum ETruncateStaffLinesOpts
{

    k_truncate_never = 0,			///< Never truncate. Staff lines will always run to right margin.
    k_truncate_barline_final = 1,	///< Truncate only if last object is a barline of type final
    k_truncate_barline_any = 2,		///< Truncate only if last object is a barline of any type
    k_truncate_always = 3,			///< Truncate always, in any case, after last object
};

//---------------------------------------------------------------------------------------
/** @ingroup enumerations

    This enum describes valid flags for score option "Render.SpacingOptions". This
    flags control the algorithms for laying out scores.

    @#include <lomse_internal_model.h>
*/
enum ERenderSpacingOpts
{
    // Lines breaker algorithm
    k_render_opt_breaker_simple     = 0x0001,   ///< use LinesBreakerSimple.
    k_render_opt_breaker_optimal    = 0x0002,   ///< use LinesBreakerOptimal
    k_render_opt_breaker_no_shrink  = 0x0004,   ///< do not shrink lines
};



///@cond INTERNALS
//---------------------------------------------------------------------------------------
// a struct to contain note/rest figure and dots
struct NoteTypeAndDots
{
    NoteTypeAndDots(int nt, int d) : noteType(nt), dots(d) {}

    int noteType;   //ImoNoteRest enum
    int dots;       //0..n
};


//---------------------------------------------------------------------------------------
// utility function to convert typographical points to LUnits
extern LUnits pt_to_LUnits(float pt);


//---------------------------------------------------------------------------------------
// Auxiliary class: A varian value for attributes

// AttribValue can hold one of <int, float, double, bool, string, Color>.
// In C++17 it could be declared as simple as:
//     typedef std::variant<int, float, double, bool, string, Color> AttribValue;
// But we implement it manually to support older compilers.
class AttribValue
{
private:
    union
    {
        int intValue;
        std::string stringValue;
        bool boolValue;
        float floatValue;
        double doubleValue;
        Color colorValue;
    };

    enum AttribType { vt_empty, vt_int, vt_string, vt_bool, vt_float, vt_double, vt_color };
    AttribType m_type = vt_empty;

public:
    AttribValue() {}

    //the five special
    ~AttribValue() { cleanup(); }
    AttribValue(const AttribValue& a) { clone(a); }
    AttribValue& operator= (const AttribValue& a) { clone(a); return *this; }
    AttribValue(AttribValue&&) = delete;
    AttribValue& operator= (AttribValue&&) = delete;

    explicit operator int() const;
    explicit operator float() const;
    explicit operator double() const;
    explicit operator bool() const;
    explicit operator string() const;
    explicit operator Color() const;
    void operator=(int value);
    void operator=(float value);
    void operator=(double value);
    void operator=(bool value);
    void operator=(const std::string& value);
    void operator=(const Color& value);

private:
    AttribValue& clone(const AttribValue& a);
    void cleanup();
    void check_type(AttribType type) const;

};


//=======================================================================================
/** Some attributes are rarely present in an element and defining member variables in
    the element for these attributes is a memory overhead, specially for very common
    elements.

    Thus, class %ImoObj implements a standard default solution for adding attributes
    to any element without using member variables. It is based on maintaining a list
    of %AttrObj objects. And each %AttrObj object in the list represents an additional
    existing attribute with its value.

    As the IM is not a generic DOM and all possible attributes names are known in
    advance, an enummerated type is used for attributes name instead of a string,
    to save memory and speed up comparisons.

    Also the values for attributes are not strings but can be different types:
    string, int, float, bool, color, etc.

    IMPORTANT:
    When modifying Lomse to create a new attribute, appart from adding a new element
    to the enum EImoAttribute, it is necessary to add the new attribute information
    to method AttributesTable::get_data_for(), to describe it.

*/
class AttrObj
{
protected:
    int m_attrbIdx = 0;         //attribute name, from enum EImoAttribute
    AttrObj* m_prev = nullptr;
    AttrObj* m_next = nullptr;

    AttrObj(int idx) : m_attrbIdx(idx) {}

public:
    //the five special
    virtual ~AttrObj() { delete m_next; }
    AttrObj(const AttrObj& a) = default;    //{ clone(a); }
    AttrObj& operator= (const AttrObj& a) = default;    //{ clone(a); return *this; }
    AttrObj(AttrObj&&) = delete;
    AttrObj& operator= (AttrObj&&) = delete;

    //virtual constructor
    virtual AttrObj* clone() = 0;

    const std::string get_name() const;
    static const std::string get_name(int idx);

    inline int get_attrib_idx() const { return m_attrbIdx; }

    inline AttrObj* get_next_attrib() const { return m_next; }
    inline AttrObj* get_prev_attrib() const { return m_prev; }

    int get_int_value() const;
    double get_double_value() const;
    std::string get_string_value() const;
    bool get_bool_value() const;
    float get_float_value() const;
    Color get_color_value() const;

protected:

    friend class AttrList;
    inline void set_next_attrib(AttrObj* pNext) { m_next = pNext; }
    inline void set_prev_attrib(AttrObj* pPrev) { m_prev = pPrev; }

    friend class ImoObj;
    AttrObj* replace_by(AttrObj* newAttr);


};

//=======================================================================================
template <class T> class Attr : public AttrObj
{
protected:
    T m_value;

public:
    Attr(int idx, const T& value) : AttrObj(idx), m_value(value) {}

    //the five special
    ~Attr() override {}
    Attr(const Attr&) = default;
    Attr& operator= (const Attr&) = default;
    Attr(Attr&&) = delete;
    Attr& operator= (Attr&&) = delete;

    //virtual constructor
    Attr<T>* clone() override
    {
        Attr<T>* pAttr = LOMSE_NEW Attr<T>(m_attrbIdx, m_value);
        pAttr->m_prev = m_prev;

        if (m_next)
            pAttr->m_next = m_next->clone();
        else
            pAttr->m_next = nullptr;

        return pAttr;
    }

    inline const T& get_value() { return m_value; }
    inline void set_value(const T& value) { m_value = value; }

};


//---------------------------------------------------------------------------------------
typedef Attr<int> AttrInt;
typedef Attr<double> AttrDouble;
typedef Attr<float> AttrFloat;
typedef Attr<std::string> AttrString;
typedef Attr<bool> AttrBool;
typedef Attr< Color > AttrColor;


//=======================================================================================
class AttrVariant : public AttrObj
{
protected:
    AttribValue m_value;

public:
    AttrVariant(int idx) : AttrObj(idx) {}
    AttrVariant(int idx, const std::string& value);
    AttrVariant(int idx, int value);
    AttrVariant(int idx, float value);
    AttrVariant(int idx, double value);
    AttrVariant(int idx, bool value);
    AttrVariant(int idx, Color value);

    //the five special
    ~AttrVariant() override {}
    AttrVariant(const AttrVariant&) = default;
    AttrVariant& operator= (const AttrVariant&) = default;
    AttrVariant(AttrVariant&&) = delete;
    AttrVariant& operator= (AttrVariant&&) = delete;

    //virtual constructor
    AttrVariant* clone() override
    {
        AttrVariant* pAttr = LOMSE_NEW AttrVariant(m_attrbIdx);
        pAttr->m_value = m_value;
        pAttr->m_prev = m_prev;

        if (m_next)
            pAttr->m_next = m_next->clone();
        else
            pAttr->m_next = nullptr;

        return pAttr;
    }

    inline int get_int_value() { return static_cast<int>(m_value); }
    inline double get_double_value() { return static_cast<double>(m_value); }
    inline std::string get_string_value() { return static_cast<string>(m_value); }
    inline bool get_bool_value() { return static_cast<bool>(m_value); }
    inline float get_float_value() { return static_cast<float>(m_value); }
    inline Color get_color_value() { return static_cast<Color>(m_value); }

    inline void set_string_value(const std::string& value) { m_value = value; }
    inline void set_int_value(int value) { m_value = value; }
    inline void set_double_value(double value) { m_value = value; }
    inline void set_float_value(float value) { m_value = value; }
    inline void set_bool_value(bool value) { m_value = value; }
    inline void set_color_value(Color value) { m_value = value; }

};

//=======================================================================================
/** A simple double linked list to store AttrObj objects with minimum memory footprint.
*/
class AttrList
{
protected:
    AttrObj* m_first = nullptr;

public:
    AttrList() {}

    //the five special
    ~AttrList() { delete m_first; }
    AttrList(const AttrList& a) { clone(a); }
    AttrList& operator= (const AttrList& a) { clone(a); return *this; }
    AttrList(AttrList&&) = delete;
    AttrList& operator= (AttrList&&) = delete;

    //capacity
    size_t size();                                      //returns the number of elements
    inline bool empty() { return m_first == nullptr; }  //checks whether the list is empty

    //modifiers
    void clear();                               //clears the contents
    AttrObj* push_back(AttrObj* newAttr);       //adds an element to the end

    //element access
    inline AttrObj* front() { return m_first; }     //access the first element
    AttrObj* back();                                //access the last element

    //operations
    void remove(TIntAttribute idx);         //removes and deletes the element
    AttrObj* find(TIntAttribute idx);       //access element by name identifier

protected:
    AttrList& clone(const AttrList& a);

};


//=======================================================================================
// Auxiliary data containers. They are composite data types used to encapsulate data
// for the IM, but they are not nodes in the IM tree.
//
// They are just passive data structures (= Plain old data (POD), = Pasive Data Structure
// (PDS), = record), that is a collections of field values, without using object-oriented
// features. The C++ compiler guarantees that there will be no "magic" going on in the
// structure: for example hidden pointers to vtables, offsets that get applied to
// the address when it is cast to other types (at least if the target's Type too),
// constructors, or destructors. Roughly speaking, a type is a Type when the only things
// in it are built-in types and combinations of them. The result is something that
// "acts like" a C type.
//
// This 'Type' classes are auxiliary and they are not nodes in the IM tree.
//=======================================================================================

//---------------------------------------------------------------------------------------
/** %TypeMeasureInfo contains the data associated to a measure, such as its
    displayed number.

    %TypeMeasureInfo objects are not part of the IM tree. Each %TypeMeasureInfo object
    is directly stored in the ImoBarline object that closes the measure. And
    in ImoInstrument if last measure is not closed or if the score has no metric.
*/
struct TypeMeasureInfo
{
    int index;      ///< An optional integer index for the measure, as defined in NMX.
                    ///< The first measure has an index of 1.
    int count;      ///< sequential integer index for the measure. The first measure
                    ///< is counted as 1, even if anacrusis start.
    std::string number; ///< An optional textual number to be displayed for the measure.
    bool fHideNumber;   ///< Override measures number policy for preventing to
                        ///< display the number in this measure.


    TypeMeasureInfo()
        : index(0)
        , count(0)
        , number("")
        , fHideNumber(false)
    {
    }

};


//---------------------------------------------------------------------------------------
/** %TypeTextInfo is a container for a text and its language
*/
struct TypeTextInfo
{
    std::string text;
    std::string language;

    TypeTextInfo() {}
    TypeTextInfo(const std::string& txt) : text(txt) {}
};


//---------------------------------------------------------------------------------------
/** %TypeLineStyle is a container for line style parameters
*/
struct TypeLineStyle
{
    ELineStyle  lineStyle;
    ELineEdge   startEdge;
    ELineEdge   endEdge;
    ELineCap    startStyle;
    ELineCap    endStyle;
    Color       color;
    Tenths      width;
    TPoint      startPoint;
    TPoint      endPoint;

    TypeLineStyle()
        : lineStyle(k_line_solid)
        , startEdge(k_edge_normal)
        , endEdge(k_edge_normal)
        , startStyle(k_cap_none)
        , endStyle(k_cap_none)
        , color(Color(0,0,0,255))
        , width(1.0f)
        , startPoint(0.0f, 0.0f)
        , endPoint(0.0f, 0.0f)
    {
    }

    //getters
    inline ELineStyle get_line_style() const { return lineStyle; }
    inline ELineEdge get_start_edge() const { return startEdge; }
    inline ELineEdge get_end_edge() const { return endEdge; }
    inline ELineCap get_start_cap() const { return startStyle; }
    inline ELineCap get_end_cap() const { return endStyle; }
    inline Color get_color() const { return color; }
    inline Tenths get_width() const { return width; }
    inline TPoint get_start_point() const { return startPoint; }
    inline TPoint get_end_point() const { return endPoint; }

    //setters
    inline void set_line_style(ELineStyle value) { lineStyle = value; }
    inline void set_start_edge(ELineEdge value) { startEdge = value; }
    inline void set_end_edge(ELineEdge value) { endEdge = value; }
    inline void set_start_cap(ELineCap value) { startStyle = value; }
    inline void set_end_cap(ELineCap value) { endStyle = value; }
    inline void set_color(Color value) { color = value; }
    inline void set_width(Tenths value) { width = value; }
    inline void set_start_point(TPoint point) { startPoint = point; }
    inline void set_end_point(TPoint point) { endPoint = point; }
};


//=======================================================================================
// standard interface for ImoObj objects containing an ImoSounds child
//=======================================================================================
#define LOMSE_DECLARE_IMOSOUNDS_INTERFACE                               \
    ImoSounds* get_sounds();                                            \
    void add_sound_info(ImoSoundInfo* pInfo);                           \
    int get_num_sounds();                                               \
    ImoSoundInfo* get_sound_info(const std::string& soundId);           \
    ImoSoundInfo* get_sound_info(int iSound);    //iSound = 0..n-1


//=======================================================================================
// standard interface for ImoObj objects that have inline-level content
//=======================================================================================
#define LOMSE_DECLARE_INLINE_LEVEL_INTERFACE                                            \
    ImoTextItem* add_text_item(const std::string& text, ImoStyle* pStyle=nullptr);      \
    ButtonCtrl* add_button(LibraryScope& libScope, const std::string& label,            \
                           const USize& size, ImoStyle* pStyle=nullptr);                \
    ImoInlineWrapper* add_inline_box(LUnits width=0.0f, ImoStyle* pStyle=nullptr);      \
    ImoLink* add_link(const std::string& url, ImoStyle* pStyle=nullptr);                \
    ImoImage* add_image(unsigned char* imgbuf, VSize bmpSize, EPixelFormat format,      \
                        USize imgSize, ImoStyle* pStyle=nullptr);                       \
    ImoControl* add_control(Control* pCtrol);



//************************************************************
// Objects that form the content of the internal classes
//************************************************************

//===================================================
// Abstract objects hierachy
//===================================================


//---------------------------------------------------------------------------------------
// the root. Any object must derive from it
class ImoObj : public Visitable, public TreeNode<ImoObj>
{
protected:
    DocModel* m_pDocModel = nullptr;
    ImoId m_id = k_no_imoid;
    int m_objtype = -1;
    unsigned int m_flags = 0;
    AttrList m_attribs;

protected:
    ImoObj(int objtype, ImoId id=k_no_imoid);

    friend class ImFactory;
    void set_owner_model(DocModel* pDocModel);
    virtual void initialize_object() {}

public:
    //the five special
    ~ImoObj() override;
    ImoObj(const ImoObj& a) : Visitable(), TreeNode<ImoObj>(a) { clone(a); }
    ImoObj& operator= (const ImoObj& a) = default;  //  { clone(a); return *this; }
    ImoObj(ImoObj&&) = delete;
    ImoObj& operator= (ImoObj&&) = delete;

    //flag values
    enum
    {
        k_dirty             = 0x0001,   //dirty: modified since last "clear_dirty()" ==> need to rebuild GModel
        k_children_dirty    = 0x0002,   //this is not dirty but some children are dirty
        k_edit_terminal     = 0x0004,   //terminal node for edition
        k_editable          = 0x0008,   //in edition, this node can be edited
        k_deletable         = 0x0010,   //if editable, this node can be also deleted
        k_expandable        = 0x0020,   //if editable, more children can be added/inserted
    };

    //dirty
    inline bool is_dirty() { return (m_flags & k_dirty) != 0; }
    void set_dirty(bool value);
    inline bool are_children_dirty() { return (m_flags & k_children_dirty) != 0; }
    void set_children_dirty(bool value);

    //edition flags
    inline bool is_edit_terminal() { return (m_flags & k_edit_terminal) != 0; }
    inline void set_edit_terminal(bool value) { value ? m_flags |= k_edit_terminal
                                                      : m_flags &= ~k_edit_terminal; }
    inline bool is_editable() { return (m_flags & k_editable) != 0; }
    inline void set_editable(bool value) { value ? m_flags |= k_editable
                                                 : m_flags &= ~k_editable; }
    inline bool is_deletable() { return (m_flags & k_deletable) != 0; }
    inline void set_deletable(bool value) { value ? m_flags |= k_deletable
                                                  : m_flags &= ~k_deletable; }
    inline bool is_expandable() { return (m_flags & k_expandable) != 0; }
    inline void set_expandable(bool value) { value ? m_flags |= k_expandable
                                                   : m_flags &= ~k_expandable; }

    //id
    inline ImoId get_id() { return m_id; }
    inline void set_id(ImoId id) { m_id = id; }
    std::string get_xml_id();
    void set_xml_id(const std::string& value);

    //required by Visitable parent class
    void accept_visitor(BaseVisitor& v) override;
    virtual bool has_visitable_children() { return has_children(); }

    //parent / children
    ImoObj* get_child_of_type(int objtype);
    ImoObj* get_ancestor_of_type(int objtype);
    ImoObj* get_parent_imo() { return static_cast<ImoObj*>(get_parent()); }
    void append_child_imo(ImoObj* pImo);
    void remove_child_imo(ImoObj* pImo);
    Document* get_the_document();
    ImoDocument* get_document();
    DocModel* get_doc_model();
    Observable* get_observable_parent();
    ImoContentObj* get_block_level_parent();
    ImoBlockLevelObj* find_block_level_parent();

    //Get the name and source code
    static const std::string& get_name(int type);
    const std::string& get_name() const;
    std::string to_string(bool fWithIds=false);
    inline std::string to_string_with_ids() { return to_string(true); }

    //properties
    virtual bool can_generate_secondary_shapes() { return false; }

    //object classification
    inline int get_obj_type() { return m_objtype; }
    inline bool has_children() { return !is_terminal(); }

    //groups
    inline bool is_dto() { return m_objtype > k_imo_dto
                                  && m_objtype < k_imo_dto_last; }
    inline bool is_simpleobj() { return m_objtype > k_imo_simpleobj
                                        && m_objtype < k_imo_simpleobj_last; }
    inline bool is_collection() { return m_objtype > k_imo_collection
                                         && m_objtype < k_imo_collection_last; }
    inline bool is_reldataobj() { return m_objtype > k_imo_reldataobj
                                         && m_objtype < k_imo_reldataobj_last; }
    inline bool is_containerobj() { return m_objtype > k_imo_containerobj
                                           && m_objtype < k_imo_containerobj_last; }
    inline bool is_contentobj() { return m_objtype > k_imo_contentobj
                                         && m_objtype < k_imo_contentobj_last; }
    inline bool is_scoreobj() { return m_objtype > k_imo_scoreobj
                                       && m_objtype < k_imo_scoreobj_last; }
    inline bool is_staffobj() { return m_objtype > k_imo_staffobj
                                       && m_objtype < k_imo_staffobj_last; }
    inline bool is_auxobj() { return m_objtype > k_imo_auxobj
                                     && m_objtype < k_imo_auxobj_last; }
    inline bool is_relobj() { return m_objtype > k_imo_relobj
                                     && m_objtype < k_imo_relobj_last; }
    inline bool is_auxrelobj() { return m_objtype > k_imo_auxrelobj
                                        && m_objtype < k_imo_auxobj_last; }
    inline bool is_block_level_obj() { return m_objtype > k_imo_block_level_obj
                                              && m_objtype < k_imo_block_level_obj_last; }
    inline bool is_blocks_container() { return m_objtype > k_imo_blocks_container
                                               && m_objtype < k_imo_blocks_container_last; }
    inline bool is_inlines_container() { return m_objtype > k_imo_inlines_container
                                                && m_objtype < k_imo_inlines_container_last; }
    inline bool is_box_inline() { return m_objtype > k_imo_box_inline
                                         && m_objtype < k_imo_box_inline_last; }
    inline bool is_inline_level_obj() { return m_objtype > k_imo_inline_level_obj
                                               && m_objtype < k_imo_inline_level_obj_last; }
    inline bool is_articulation() { return m_objtype > k_imo_articulation
                                           && m_objtype < k_imo_articulation_last; }

    //items
    inline bool is_anonymous_block() { return m_objtype == k_imo_anonymous_block; }
    inline bool is_arpeggio() { return m_objtype == k_imo_arpeggio; }
    inline bool is_articulation_symbol() { return m_objtype == k_imo_articulation_symbol; }
    inline bool is_articulation_line() { return m_objtype == k_imo_articulation_line; }
    inline bool is_attachments() { return m_objtype == k_imo_attachments; }
    inline bool is_barline() { return m_objtype == k_imo_barline; }
    inline bool is_beam() { return m_objtype == k_imo_beam; }
    inline bool is_beam_data() { return m_objtype == k_imo_beam_data; }
    inline bool is_beam_dto() { return m_objtype == k_imo_beam_dto; }
    inline bool is_bezier_info() { return m_objtype == k_imo_bezier_info; }
    inline bool is_border_dto() { return m_objtype == k_imo_border_dto; }
    inline bool is_button() { return m_objtype == k_imo_button; }
    inline bool is_chord() { return m_objtype == k_imo_chord; }
    inline bool is_clef() { return m_objtype == k_imo_clef; }
    inline bool is_color_dto() { return m_objtype == k_imo_color_dto; }
    inline bool is_content() { return m_objtype == k_imo_content; }
    inline bool is_control() { return m_objtype >= k_imo_control
                                      && m_objtype < k_imo_control_end; }
    inline bool is_cursor_info() { return m_objtype == k_imo_cursor_info; }
    inline bool is_direction() { return m_objtype == k_imo_direction; }
    inline bool is_document() { return m_objtype == k_imo_document; }
    inline bool is_dynamic() { return m_objtype == k_imo_dynamic; }
    inline bool is_dynamics_mark() { return m_objtype == k_imo_dynamics_mark; }
    inline bool is_fermata() { return m_objtype == k_imo_fermata; }
    inline bool is_figured_bass() { return m_objtype == k_imo_figured_bass; }
    inline bool is_figured_bass_info() { return m_objtype == k_imo_figured_bass_info; }
    inline bool is_fingering() { return m_objtype == k_imo_fingering; }
    inline bool is_font_style_dto() { return m_objtype == k_imo_font_style_dto; }
    inline bool is_fret_string() { return m_objtype == k_imo_fret_string; }
    inline bool is_go_back_fwd() { return m_objtype == k_imo_go_back_fwd; }
    bool is_gap();      ///a rest representing a goFwd element
    inline bool is_grace_relobj() { return m_objtype == k_imo_grace_relobj; }
    inline bool is_heading() { return m_objtype == k_imo_heading; }
    inline bool is_image() { return m_objtype == k_imo_image; }
    inline bool is_inline_wrapper() { return m_objtype == k_imo_inline_wrapper; }
    inline bool is_instrument() { return m_objtype == k_imo_instrument; }
    inline bool is_instr_group() { return m_objtype == k_imo_instr_group; }
    inline bool is_key_signature() { return m_objtype == k_imo_key_signature; }
    inline bool is_line() { return m_objtype == k_imo_line; }
    inline bool is_line_style() { return m_objtype == k_imo_line_style_dto; }
    inline bool is_link() { return m_objtype == k_imo_link; }
    inline bool is_list() { return m_objtype == k_imo_list; }
    inline bool is_listitem() { return m_objtype == k_imo_listitem; }
    inline bool is_lyric() { return m_objtype == k_imo_lyric; }
    inline bool is_lyrics_text_info() { return m_objtype == k_imo_lyrics_text_info; }
    inline bool is_metronome_mark() { return m_objtype == k_imo_metronome_mark; }
    inline bool is_midi_info() { return m_objtype == k_imo_midi_info; }
    inline bool is_multicolumn() { return m_objtype == k_imo_multicolumn; }
    inline bool is_music_data() { return m_objtype == k_imo_music_data; }
    inline bool is_note() { return is_regular_note() || is_grace_note() || is_cue_note(); }
    inline bool is_regular_note() { return m_objtype == k_imo_note_regular; }
    inline bool is_grace_note() { return m_objtype == k_imo_note_grace; }
    inline bool is_cue_note() { return m_objtype == k_imo_note_cue; }
    inline bool is_note_rest() { return is_note() || m_objtype == k_imo_rest; }
    inline bool is_octave_shift() { return m_objtype == k_imo_octave_shift; }
    inline bool is_octave_shift_dto() { return m_objtype == k_imo_octave_shift_dto; }
    inline bool is_option() { return m_objtype == k_imo_option; }
    inline bool is_ornament() { return m_objtype == k_imo_ornament; }
    inline bool is_page_info() { return m_objtype == k_imo_page_info; }
    inline bool is_paragraph() { return m_objtype == k_imo_para; }
    inline bool is_param_info() { return m_objtype == k_imo_param_info; }
    inline bool is_pedal_mark() const { return m_objtype == k_imo_pedal_mark; }
    inline bool is_pedal_line() const { return m_objtype == k_imo_pedal_line; }
    inline bool is_pedal_dto() const { return m_objtype == k_imo_pedal_line_dto; }
//    inline bool is_play_info() const { return m_objtype == k_imo_play_info; }
    inline bool is_point_dto() { return m_objtype == k_imo_point_dto; }
    inline bool is_rest() { return m_objtype == k_imo_rest; }
    inline bool is_relations() { return m_objtype == k_imo_relations; }
    inline bool is_repetition_mark() { return m_objtype == k_imo_text_repetition_mark
               || m_objtype == k_imo_symbol_repetition_mark; }
    inline bool is_score() { return m_objtype == k_imo_score; }
    inline bool is_score_line() { return m_objtype == k_imo_score_line; }
    inline bool is_score_player() { return m_objtype == k_imo_score_player; }
    inline bool is_score_text() { return m_objtype == k_imo_score_text; }
    inline bool is_score_title() { return m_objtype == k_imo_score_title; }
    inline bool is_size_info() { return m_objtype == k_imo_size_dto; }
    inline bool is_slur() { return m_objtype == k_imo_slur; }
    inline bool is_slur_data() { return m_objtype == k_imo_slur_data; }
    inline bool is_slur_dto() { return m_objtype == k_imo_slur_dto; }
    inline bool is_sound_change() { return m_objtype == k_imo_sound_change; }
    inline bool is_sound_info() { return m_objtype == k_imo_sound_info; }
    inline bool is_spacer() { return m_objtype == k_imo_direction; }
    inline bool is_staff_info() { return m_objtype == k_imo_staff_info; }
    inline bool is_style() { return m_objtype == k_imo_style; }
    inline bool is_styles() { return m_objtype == k_imo_styles; }
    inline bool is_symbol_repetition_mark() { return m_objtype == k_imo_symbol_repetition_mark; }
    inline bool is_system_break() { return m_objtype == k_imo_system_break; }
    inline bool is_system_info() { return m_objtype == k_imo_system_info; }
    inline bool is_table() { return m_objtype == k_imo_table; }
    inline bool is_table_cell() { return m_objtype == k_imo_table_cell; }
    inline bool is_table_body() { return m_objtype == k_imo_table_body; }
    inline bool is_table_head() { return m_objtype == k_imo_table_head; }
    inline bool is_table_row() { return m_objtype == k_imo_table_row; }
    inline bool is_technical() { return m_objtype == k_imo_technical
                                        || m_objtype == k_imo_fingering
                                        || m_objtype == k_imo_fret_string; }
    inline bool is_text_item() { return m_objtype == k_imo_text_item; }
    inline bool is_text_style() { return m_objtype == k_imo_text_style; }
    inline bool is_textblock_info() { return m_objtype == k_imo_textblock_info; }
    inline bool is_text_box() { return m_objtype == k_imo_text_box; }
    inline bool is_text_repetition_mark() { return m_objtype == k_imo_text_repetition_mark; }
    inline bool is_tie() { return m_objtype == k_imo_tie; }
    inline bool is_tie_data() { return m_objtype == k_imo_tie_data; }
    inline bool is_tie_dto() { return m_objtype == k_imo_tie_dto; }
    inline bool is_time_signature() { return m_objtype == k_imo_time_signature; }
    inline bool is_time_modification_dto() { return m_objtype == k_imo_time_modification_dto; }
    inline bool is_transpose() { return m_objtype == k_imo_transpose; }
    inline bool is_tuplet() { return m_objtype == k_imo_tuplet; }
    inline bool is_tuplet_dto() { return m_objtype == k_imo_tuplet_dto; }
    inline bool is_volta_bracket() { return m_objtype == k_imo_volta_bracket; }
    inline bool is_volta_bracket_dto() { return m_objtype == k_imo_volta_bracket_dto; }
    inline bool is_wedge() { return m_objtype == k_imo_wedge; }
    inline bool is_wedge_dto() { return m_objtype == k_imo_wedge_dto; }

    //special checkers
    inline bool is_mouse_over_generator() { return   m_objtype == k_imo_link
                  || m_objtype == k_imo_button
                  ; }


    //IM attributes interface ------------------------------------
    bool has_attributte(TIntAttribute idx);

        //set attribute value
    template<typename T> void set_attribute(TIntAttribute idx, const T& value)
    {
        AttrObj* pAttr = get_attribute(idx);
        if (pAttr)
        {
            Attr<T>* pT = dynamic_cast< Attr<T>* >(pAttr);
            if (pT)
            {
                pT->set_value(value);
                return;
            }

            else
            {
                pAttr->replace_by( LOMSE_NEW Attr<T>(idx, value) );
                return;
            }
        }

        add_attribute( LOMSE_NEW Attr<T>(idx, value) );
        set_dirty(true);
    }

    virtual void set_int_attribute(TIntAttribute idx, int value);
    virtual void set_color_attribute(TIntAttribute idx, Color value);
    virtual void set_bool_attribute(TIntAttribute idx, bool value);
    virtual void set_float_attribute(TIntAttribute idx, float value);
    virtual void set_double_attribute(TIntAttribute idx, double value);
    virtual void set_string_attribute(TIntAttribute idx, const std::string& value);

        //get attribute value
    template<typename T> T get_attribute_value(TIntAttribute idx)
    {
        AttrObj* pAttr = get_attribute(idx);
        if (pAttr)
        {
            Attr<T>* pT = dynamic_cast< Attr<T>* >(pAttr);
            if (pT)
                return pT->get_value();
        }
        T value = T();
        return value;
    }

    virtual int get_int_attribute(TIntAttribute idx);
    virtual float get_float_attribute(TIntAttribute idx);
    virtual double get_double_attribute(TIntAttribute idx);
    virtual Color get_color_attribute(TIntAttribute idx);
    virtual bool get_bool_attribute(TIntAttribute idx);
    virtual string get_string_attribute(TIntAttribute idx);

        //attribute nodes
    AttrObj* get_attribute(TIntAttribute idx) { return m_attribs.find(idx); };
    void remove_attribute(TIntAttribute idx) { m_attribs.remove(idx); };

        //miscellaneous
    virtual size_t get_num_attributes() { return m_attribs.size(); }
    inline AttrObj* get_first_attribute() { return m_attribs.front(); }
    virtual list<TIntAttribute> get_supported_attributes();

protected:
    ImoObj& clone(const ImoObj& a);

    friend class FixModelVisitor;
    inline void anchor_to_model(DocModel* pDocModel) { m_pDocModel = pDocModel; }


    AttrObj* add_attribute(AttrObj* newAttr) { m_attribs.push_back(newAttr); return newAttr; }

    void visit_children(BaseVisitor& v);
    void propagate_dirty();
    void remove_id();

};

//---------------------------------------------------------------------------------------
// base class for DTOs
class ImoDto : public ImoObj
{
protected:
    ImoDto(int objtype) : ImoObj(objtype) {}

public:
    //the five special
    ~ImoDto() override {}
    ImoDto(const ImoDto& a) = default;
    ImoDto& operator= (const ImoDto& a) = default;
    ImoDto(ImoDto&&) = delete;
    ImoDto& operator= (ImoDto&&) = delete;
};

//---------------------------------------------------------------------------------------
class ImoFontStyleDto : public ImoDto
{
public:
    std::string name;
    float size;       // in points
    int style;        // k_font_style_normal, k_font_style_italic
    int weight;       // k_font_weight_normal, k_font_weight_bold

    ImoFontStyleDto()
        : ImoDto(k_imo_font_style_dto)
        , name("Liberation serif")
        , size(12)
        , style(k_font_style_normal)
        , weight(k_font_weight_normal)
    {
    }

    enum { k_font_weight_normal=0, k_font_weight_bold,
           k_font_style_normal, k_font_style_italic
         };
};

//---------------------------------------------------------------------------------------
class ImoColorDto : public ImoDto
{
protected:
    Color m_color;
    bool m_ok;

public:
    ImoColorDto() : ImoDto(k_imo_color_dto), m_color(0, 0, 0, 255), m_ok(true) {}
    ImoColorDto(Int8u r, Int8u g, Int8u b, Int8u a = 255);

    Color& set_from_rgb_string(const std::string& rgb);
    Color& set_from_rgba_string(const std::string& rgba);
    Color& set_from_argb_string(const std::string& argb);
    Color& set_from_string(const std::string& hex);
    inline bool is_ok() { return m_ok; }

    inline Int8u red() { return m_color.r; }
    inline Int8u blue() { return m_color.b; }
    inline Int8u green() { return m_color.g; }
    inline Int8u alpha() { return m_color.a; }
    inline Color& get_color() { return m_color; }

protected:
    Int8u convert_from_hex(const std::string& hex);

};



// just auxiliary objects or data transfer objects
//---------------------------------------------------------------------------------------
class ImoSimpleObj : public ImoObj
{
protected:
    ImoSimpleObj(int objtype, ImoId id) : ImoObj(objtype, id) {}
    ImoSimpleObj(int objtype) : ImoObj(objtype) {}

public:
    //the five special
    ~ImoSimpleObj() override {}
    ImoSimpleObj(const ImoSimpleObj& a) = default;
    ImoSimpleObj& operator= (const ImoSimpleObj& a) = default;
    ImoSimpleObj(ImoSimpleObj&&) = delete;
    ImoSimpleObj& operator= (ImoSimpleObj&&) = delete;
};

//---------------------------------------------------------------------------------------
class ImoTextStyle : public ImoSimpleObj
{
public:
    int word_spacing;
    int text_decoration;
    int vertical_align;
    int text_align;
    LUnits text_indent_length;
    LUnits word_spacing_length;     //for case k_length

    ImoTextStyle()
        : ImoSimpleObj(k_imo_text_style)
        , word_spacing(k_normal)
        , text_decoration(k_decoration_none)
        , vertical_align(k_valign_baseline)
        , text_align(k_align_left)
        , text_indent_length(0.0f)
        , word_spacing_length(0.0f) //not applicable
    {
    }

    //the five special
    ~ImoTextStyle() override {}
    ImoTextStyle(const ImoTextStyle& a) = default;
    ImoTextStyle& operator= (const ImoTextStyle& a) = default;
    ImoTextStyle(ImoTextStyle&&) = delete;
    ImoTextStyle& operator= (ImoTextStyle&&) = delete;

    enum { k_normal=0, k_length, };
    enum { k_decoration_none=0, k_decoration_underline, k_decoration_overline,
           k_decoration_line_through,
         };
    enum { k_valign_baseline, k_valign_sub, k_valign_super, k_valign_top,
           k_valign_text_top, k_valign_middle, k_valign_bottom,
           k_valign_text_bottom
         };
    enum { k_align_left, k_align_right, k_align_center, k_align_justify };
};

//---------------------------------------------------------------------------------------
class ImoStyle : public ImoSimpleObj
{
protected:
    std::string m_name;
    ImoId m_idParent;
    std::map<int, LUnits> m_lunitsProps;
    std::map<int, float> m_floatProps;
    std::map<int, string> m_stringProps;
    std::map<int, int> m_intProps;
    std::map<int, Color> m_colorProps;

    //to control if values are defaults or has been imported/set
    long m_modified = 0L;
    enum {
        k_modified_font_name =      0x00000001,
        k_modified_font_size =      0x00000002,
        k_modified_font_style =     0x00000004,
        k_modified_font_weight =    0x00000008,
    };

    friend class ImFactory;
    ImoStyle() : ImoSimpleObj(k_imo_style), m_name(), m_idParent(k_no_imoid) {}

public:

    //the five special
    ~ImoStyle() override {}
    ImoStyle(const ImoStyle& a) = default;
    ImoStyle& operator= (const ImoStyle& a) = default;
    ImoStyle(ImoStyle&&) = delete;
    ImoStyle& operator= (ImoStyle&&) = delete;

    //text style
    enum { k_spacing_normal=0, k_length, };
    enum { k_decoration_none=0, k_decoration_underline, k_decoration_overline,
           k_decoration_line_through,
         };
    enum { k_valign_baseline, k_valign_sub, k_valign_super, k_valign_top,
           k_valign_text_top, k_valign_middle, k_valign_bottom,
           k_valign_text_bottom
         };
    enum { k_align_left, k_align_right, k_align_center, k_align_justify };

    //font style/weight
    enum { k_font_style_undefined=0, k_font_style_normal, k_font_style_italic };
    enum { k_font_weight_undefined=0, k_font_weight_normal, k_font_weight_bold, };

    //style properties
    enum
    {
        //font
        k_font_file,
        k_font_name,
        k_font_size,
        k_font_style,
        k_font_weight,
        //text
        k_word_spacing,
        k_text_decoration,
        k_vertical_align,
        k_text_align,
        k_text_indent_length,
        k_word_spacing_length,
        k_line_height,
        //color and background
        k_color,
        k_background_color,
        //margin
        k_margin_top,
        k_margin_bottom,
        k_margin_left,
        k_margin_right,
        //padding
        k_padding_top,
        k_padding_bottom,
        k_padding_left,
        k_padding_right,

//    ////border
//    k_border_top,
//    k_border_bottom,
//    k_border_left,
//    k_border_right,

        //border width
        k_border_width_top,
        k_border_width_bottom,
        k_border_width_left,
        k_border_width_right,

        //size
        k_width,
        k_height,
        k_min_width,
        k_min_height,
        k_max_width,
        k_max_height,

        //table
        k_table_col_width,
    };

    //general
    inline const std::string& get_name() { return m_name; }
    inline void set_name(const std::string& value) { m_name = value; }
    void set_parent_style(ImoStyle* pStyle);
    ImoStyle* get_parent_style();

    //utility
    LUnits em_to_LUnits(float em) { return pt_to_LUnits( get_float_property(ImoStyle::k_font_size) * em ); }
    inline bool is_bold() { return get_int_property(ImoStyle::k_font_weight) == k_font_weight_bold; }
    inline bool is_italic() { return get_int_property(ImoStyle::k_font_style) == k_font_style_italic; }

    //check if contains default values or a new value has been set
    bool is_default_style_with_default_values();
    inline bool font_data_modified()  { return m_modified != 0L; }
    inline bool font_name_modified() { return (m_modified & k_modified_font_name) != 0; }
    inline bool font_size_modified() { return (m_modified & k_modified_font_size) != 0; }
    inline bool font_style_modified() { return (m_modified & k_modified_font_style) != 0; }
    inline bool font_weight_modified() { return (m_modified & k_modified_font_weight) != 0; }

    //utility getters/setters to avoid stupid mistakes and to simplify source code
    //font
    inline const std::string& font_file() { return get_string_property(ImoStyle::k_font_file); }
    inline ImoStyle* font_file(const std::string& filename) { set_string_property(ImoStyle::k_font_file, filename); return this; }
    inline const std::string& font_name() { return get_string_property(ImoStyle::k_font_name); }
    inline ImoStyle* font_name(const std::string& name) { set_string_property(ImoStyle::k_font_name, name); return this; }
    inline float font_size() { return get_float_property(ImoStyle::k_font_size); }
    inline ImoStyle* font_size(float value) { set_float_property(ImoStyle::k_font_size, value); return this; }
    inline int font_style() { return get_int_property(ImoStyle::k_font_style); }
    inline ImoStyle* font_style(int value) { set_int_property(ImoStyle::k_font_style, value); return this; }
    inline int font_weight() { return get_int_property(ImoStyle::k_font_weight); }
    inline ImoStyle* font_weight(int value) { set_int_property(ImoStyle::k_font_weight, value); return this; }
    //text
    inline int word_spacing() { return get_int_property(ImoStyle::k_word_spacing); }
    inline ImoStyle* word_spacing(int value) { set_int_property(ImoStyle::k_word_spacing, value); return this; }
    inline int text_decoration() { return get_int_property(ImoStyle::k_text_decoration); }
    inline ImoStyle* text_decoration(int value) { set_int_property(ImoStyle::k_text_decoration, value); return this; }
    inline int vertical_align() { return get_int_property(ImoStyle::k_vertical_align); }
    inline ImoStyle* vertical_align(int value) { set_int_property(ImoStyle::k_vertical_align, value); return this; }
    inline int text_align() { return get_int_property(ImoStyle::k_text_align); }
    inline ImoStyle* text_align(int value) { set_int_property(ImoStyle::k_text_align, value); return this; }
    inline LUnits text_indent_length() { return get_lunits_property(ImoStyle::k_text_indent_length); }
    inline ImoStyle* text_indent_length(LUnits value) { set_lunits_property(ImoStyle::k_text_indent_length, value); return this; }
    inline LUnits word_spacing_length() { return get_lunits_property(ImoStyle::k_word_spacing_length); }
    inline ImoStyle* word_spacing_length(LUnits value) { set_lunits_property(ImoStyle::k_word_spacing_length, value); return this; }
    inline float line_height() { return get_float_property(ImoStyle::k_line_height); }
    inline ImoStyle* line_height(float value) { set_float_property(ImoStyle::k_line_height, value); return this; }
    //color and background
    inline Color color() { return get_color_property(ImoStyle::k_color); }
    inline ImoStyle* color(Color color) { set_color_property(ImoStyle::k_color, color); return this; }
    inline Color background_color() { return get_color_property(ImoStyle::k_background_color); }
    inline ImoStyle* background_color(Color color) { set_color_property(ImoStyle::k_background_color, color); return this; }
    //border-width
    inline ImoStyle* border_width(LUnits value) { set_border_width_property(value); return this; }
    inline LUnits border_width_top() { return get_lunits_property(ImoStyle::k_border_width_top); }
    inline ImoStyle* border_width_top(LUnits value) { set_lunits_property(ImoStyle::k_border_width_top, value); return this; }
    inline LUnits border_width_bottom() { return get_lunits_property(ImoStyle::k_border_width_bottom); }
    inline ImoStyle* border_width_bottom(LUnits value) { set_lunits_property(ImoStyle::k_border_width_bottom, value); return this; }
    inline LUnits border_width_left() { return get_lunits_property(ImoStyle::k_border_width_left); }
    inline ImoStyle* border_width_left(LUnits value) { set_lunits_property(ImoStyle::k_border_width_left, value); return this; }
    inline LUnits border_width_right() { return get_lunits_property(ImoStyle::k_border_width_right); }
    inline ImoStyle* border_width_right(LUnits value) { set_lunits_property(ImoStyle::k_border_width_right, value); return this; }
    //padding
    inline ImoStyle* padding(LUnits value) { set_padding_property(value); return this; }
    inline LUnits padding_top() { return get_lunits_property(ImoStyle::k_padding_top); }
    inline ImoStyle* padding_top(LUnits value) { set_lunits_property(ImoStyle::k_padding_top, value); return this; }
    inline LUnits padding_bottom() { return get_lunits_property(ImoStyle::k_padding_bottom); }
    inline ImoStyle* padding_bottom(LUnits value) { set_lunits_property(ImoStyle::k_padding_bottom, value); return this; }
    inline LUnits padding_left() { return get_lunits_property(ImoStyle::k_padding_left); }
    inline ImoStyle* padding_left(LUnits value) { set_lunits_property(ImoStyle::k_padding_left, value); return this; }
    inline LUnits padding_right() { return get_lunits_property(ImoStyle::k_padding_right); }
    inline ImoStyle* padding_right(LUnits value) { set_lunits_property(ImoStyle::k_padding_right, value); return this; }
    //margin
    inline ImoStyle* margin(LUnits value) { set_margin_property(value); return this; }
    inline LUnits margin_top() { return get_lunits_property(ImoStyle::k_margin_top); }
    inline ImoStyle* margin_top(LUnits value) { set_lunits_property(ImoStyle::k_margin_top, value); return this; }
    inline LUnits margin_bottom() { return get_lunits_property(ImoStyle::k_margin_bottom); }
    inline ImoStyle* margin_bottom(LUnits value) { set_lunits_property(ImoStyle::k_margin_bottom, value); return this; }
    inline LUnits margin_left() { return get_lunits_property(ImoStyle::k_margin_left); }
    inline ImoStyle* margin_left(LUnits value) { set_lunits_property(ImoStyle::k_margin_left, value); return this; }
    inline LUnits margin_right() { return get_lunits_property(ImoStyle::k_margin_right); }
    inline ImoStyle* margin_right(LUnits value) { set_lunits_property(ImoStyle::k_margin_right, value); return this; }
    //size
    inline LUnits width() { return get_lunits_property(ImoStyle::k_width); }
    inline ImoStyle* width(LUnits value) { set_lunits_property(ImoStyle::k_width, value); return this; }
    inline LUnits height() { return get_lunits_property(ImoStyle::k_height); }
    inline ImoStyle* height(LUnits value) { set_lunits_property(ImoStyle::k_height, value); return this; }
    inline LUnits min_width() { return get_lunits_property(ImoStyle::k_min_width); }
    inline ImoStyle* min_width(LUnits value) { set_lunits_property(ImoStyle::k_min_width, value); return this; }
    inline LUnits min_height() { return get_lunits_property(ImoStyle::k_min_height); }
    inline ImoStyle* min_height(LUnits value) { set_lunits_property(ImoStyle::k_min_height, value); return this; }
    inline LUnits max_width() { return get_lunits_property(ImoStyle::k_max_width); }
    inline ImoStyle* max_width(LUnits value) { set_lunits_property(ImoStyle::k_max_width, value); return this; }
    inline LUnits max_height() { return get_lunits_property(ImoStyle::k_max_height); }
    inline ImoStyle* max_height(LUnits value) { set_lunits_property(ImoStyle::k_max_height, value); return this; }

    //table
    inline LUnits table_col_width() { return get_lunits_property(ImoStyle::k_table_col_width); }
    inline ImoStyle* table_col_width(LUnits value) { set_lunits_property(ImoStyle::k_table_col_width, value); return this; }

protected:

    friend class ImoScore;
    friend class ImoStyles;
    void reset_style_modified() { m_modified = 0L; }

    //setters
    void set_string_property(int prop, const std::string& value);
    void set_float_property(int prop, float value);
    void set_int_property(int prop, int value);
    void set_lunits_property(int prop, LUnits value);
    void set_color_property(int prop, Color value);

    //special setters
    void set_margin_property(LUnits value);
    void set_padding_property(LUnits value);
    void set_border_width_property(LUnits value);

    friend class DefineStyleLmdGenerator;
    friend class DefineStyleLdpGenerator;
    friend class DefineStyleMnxGenerator;
    friend class DefineStyleMxlGenerator;

    //getters non-inheriting from parent. Returns true if property exists
    bool get_float_property(int prop, float* value);
    bool get_lunits_property(int prop, LUnits* value);
    bool get_string_property(int prop, string* value);
    bool get_int_property(int prop, int* value);
    bool get_color_property(int prop, Color* value);

    //getters. If value not stored, inherites from parent
    float get_float_property(int prop);
    LUnits get_lunits_property(int prop);
    const std::string& get_string_property(int prop);
    int get_int_property(int prop);
    Color get_color_property(int prop);

};

//---------------------------------------------------------------------------------------
/** %ImoContentObj is the base class from which any object for the renderizable content
    of a document must derive.

    %ImoContentObj objects have style, location, visibility and can
    have attachments and attributes.
*/
class ImoContentObj : public ImoObj
    , public Observable
{
protected:
    ImoId m_styleId;
    Tenths m_txUserLocation;
    Tenths m_tyUserLocation;
    Tenths m_txUserRefPoint;
    Tenths m_tyUserRefPoint;
    bool m_fVisible;

    ImoContentObj(int objtype);
    ImoContentObj(ImoId id, int objtype);

public:
    //the five special
    ~ImoContentObj() override {}
    ImoContentObj(const ImoContentObj&) = default;
    ImoContentObj& operator= (const ImoContentObj&) = default;
    ImoContentObj(ImoContentObj&&) = delete;
    ImoContentObj& operator= (ImoContentObj&&) = delete;

    //getters
    inline Tenths get_user_location_x() { return m_txUserLocation; }
    inline Tenths get_user_location_y() { return m_tyUserLocation; }
    inline Tenths get_user_ref_point_x() { return m_txUserRefPoint; }
    inline Tenths get_user_ref_point_y() { return m_tyUserRefPoint; }
    inline bool is_visible() { return m_fVisible; }

    //setters
    inline void set_user_location_x(Tenths tx) { m_txUserLocation = tx; }
    inline void set_user_location_y(Tenths ty) { m_tyUserLocation = ty; }
    inline void set_user_ref_point_x(Tenths tx) { m_txUserRefPoint = tx; }
    inline void set_user_ref_point_y(Tenths ty) { m_tyUserRefPoint = ty; }
    inline void set_visible(bool visible) { m_fVisible = visible; }

    //attachments (first child)
    ImoAttachments* get_attachments();
    bool has_attachments();
    int get_num_attachments();
    ImoAuxObj* get_attachment(int i);
    void add_attachment(ImoAuxObj* pAO);
    void remove_attachment(ImoAuxObj* pAO);
    void remove_but_not_delete_attachment(ImoAuxObj* pAO);
    ImoAuxObj* find_attachment(int type);

    //overrides for Observable children
    EventNotifier* get_event_notifier() override;
    void add_event_handler(int eventType, EventHandler* pHandler) override;
    void add_event_handler(int eventType, void* pThis,
                           void (*pt2Func)(void* pObj, SpEventInfo event) ) override;
    void add_event_handler(int eventType,
                           void (*pt2Func)(SpEventInfo event) ) override;

    //style
    virtual ImoStyle* get_style(bool fInherit=true);
    ImoStyle* get_inherited_style();
    ImoStyle* copy_style_as(const std::string& name);
    void set_style(ImoStyle* pStyle);

    //font
    inline const std::string font_name()
    {
        ImoStyle* pStyle = get_style();
        return pStyle ? pStyle->font_name() : "";
    }

    //margin
    inline LUnits margin_top()
    {
        ImoStyle* pStyle = get_style();
        return pStyle ? pStyle->margin_top() : 0.0f;
    }
    inline LUnits margin_bottom()
    {
        ImoStyle* pStyle = get_style();
        return pStyle ? pStyle->margin_bottom() : 0.0f;
    }


    //attributes interface
    list<TIntAttribute> get_supported_attributes() override;
    void set_bool_attribute(TIntAttribute attrib, bool value) override;
    bool get_bool_attribute(TIntAttribute attrib) override;
    void set_double_attribute(TIntAttribute attrib, double value) override;
    double get_double_attribute(TIntAttribute attrib) override;
    void set_string_attribute(TIntAttribute attrib, const string& value) override;
    string get_string_attribute(TIntAttribute attrib) override;

protected:
    ImoStyle* get_style_imo();

};

//---------------------------------------------------------------------------------------
class ImoCollection : public ImoSimpleObj
{
protected:
    ImoCollection(int objtype) : ImoSimpleObj(objtype) {}

public:
    //the five special
    ~ImoCollection() override {}
    ImoCollection(const ImoCollection&) = default;
    ImoCollection& operator= (const ImoCollection&) = default;
    ImoCollection(ImoCollection&&) = delete;
    ImoCollection& operator= (ImoCollection&&) = delete;

    //contents.    iItem = 0..n-1
    inline ImoObj* get_item(int iItem) { return get_child(iItem); }
    inline int get_num_items() { return get_num_children(); }
    inline void remove_item(ImoContentObj* pItem) { remove_child_imo(pItem); }

};

//---------------------------------------------------------------------------------------
class ImoRelations : public ImoSimpleObj
{
protected:
    std::list<ImoRelObj*> m_relations;

    friend class ImFactory;
    friend class ImoContentObj;
    ImoRelations() : ImoSimpleObj(k_imo_relations) {}

public:
    ~ImoRelations() override;
    ImoRelations(const ImoRelations& a) : ImoSimpleObj(a) { clone(a); }
    ImoRelations& operator= (const ImoRelations& a) { clone(a); return *this; }
    ImoRelations(ImoRelations&&) = delete;
    ImoRelations& operator= (ImoRelations&&) = delete;

    //overrides, to traverse this special node
    void accept_visitor(BaseVisitor& v) override;
    bool has_visitable_children() override { return get_num_items() > 0; }

    //contents
    ImoRelObj* get_item(int iItem);   //iItem = 0..n-1
    inline int get_num_items() { return int(m_relations.size()); }
    inline std::list<ImoRelObj*>& get_relobjs() { return m_relations; }
    void remove_relation(ImoRelObj* pRO);
    void add_relation(ImoRelObj* pRO);
    ImoRelObj* find_item_of_type(int type);
    void remove_from_all_relations(ImoStaffObj* pSO);

protected:
    ImoRelations& clone(const ImoRelations& a);

};

//---------------------------------------------------------------------------------------
// ContainerObj: A collection of containers and contained objs.
class ImoContainerObj : public ImoObj
{
protected:
    ImoContainerObj(int objtype) : ImoObj(objtype) {}

public:
    //the five special
    ~ImoContainerObj() override {}
    ImoContainerObj(const ImoContainerObj&) = default;
    ImoContainerObj& operator= (const ImoContainerObj&) = default;
    ImoContainerObj(ImoContainerObj&&) = delete;
    ImoContainerObj& operator= (ImoContainerObj&&) = delete;

};

//---------------------------------------------------------------------------------------
// ImoBlockLevelObj: abstract class for all block-level objects
class ImoBlockLevelObj : public ImoContentObj
{
protected:
    ImoBlockLevelObj(ImoId id, int objtype) : ImoContentObj(id, objtype) {}
    ImoBlockLevelObj(int objtype) : ImoContentObj(objtype) {}

public:
    //the five special
    ~ImoBlockLevelObj() override {}
    ImoBlockLevelObj(const ImoBlockLevelObj&) = default;
    ImoBlockLevelObj& operator= (const ImoBlockLevelObj&) = default;
    ImoBlockLevelObj(ImoBlockLevelObj&&) = delete;
    ImoBlockLevelObj& operator= (ImoBlockLevelObj&&) = delete;

};

//---------------------------------------------------------------------------------------
// ImoBlocksContainer: A block-level container for block-level objects
class ImoBlocksContainer : public ImoBlockLevelObj
{
protected:
    ImoBlocksContainer(ImoId id, int objtype) : ImoBlockLevelObj(id, objtype) {}
    ImoBlocksContainer(int objtype) : ImoBlockLevelObj(objtype) {}

public:
     //the five special
    ~ImoBlocksContainer() override {}
    ImoBlocksContainer(const ImoBlocksContainer&) = default;
    ImoBlocksContainer& operator= (const ImoBlocksContainer&) = default;
    ImoBlocksContainer(ImoBlocksContainer&&) = delete;
    ImoBlocksContainer& operator= (ImoBlocksContainer&&) = delete;

    //support for storing content inside an ImoContent node.
    //Container ImoContent node will be automatically created when storing the
    //first content item (first invocation of append_content_item() )
    int get_num_content_items();
    ImoContent* get_content();
    ImoContentObj* get_content_item(int iItem);
    ImoContentObj* get_first_content_item();
    ImoContentObj* get_last_content_item();
    void append_content_item(ImoContentObj* pItem);

    //methods for adding block-level content
    ImoContent* add_content_wrapper(ImoStyle* pStyle=nullptr);
    ImoList* add_list(int type, ImoStyle* pStyle=nullptr);
    ImoMultiColumn* add_multicolumn_wrapper(int numCols, ImoStyle* pStyle=nullptr);
    ImoParagraph* add_paragraph(ImoStyle* pStyle=nullptr);
    ImoScore* add_score(ImoStyle* pStyle=nullptr);

protected:
    void add_to_model(ImoBlockLevelObj* pImo, ImoStyle* pStyle);
    void create_content_container();

};

//---------------------------------------------------------------------------------------
// ImoInlinesContainer: A block-level container for ImoInlineLevelObj objs.
class ImoInlinesContainer : public ImoBlockLevelObj
{
protected:
    ImoInlinesContainer(int objtype) : ImoBlockLevelObj(objtype) {}

public:
    //the five special
    ~ImoInlinesContainer() override {}
    ImoInlinesContainer(const ImoInlinesContainer&) = default;
    ImoInlinesContainer& operator= (const ImoInlinesContainer&) = default;
    ImoInlinesContainer(ImoInlinesContainer&&) = delete;
    ImoInlinesContainer& operator= (ImoInlinesContainer&&) = delete;

    //contents
    inline int get_num_items() { return get_num_children(); }
    inline void remove_item(ImoContentObj* pItem) { remove_child_imo(pItem); }
    inline void add_item(ImoContentObj* pItem) { append_child_imo(pItem); }
    inline ImoContentObj* get_first_item() {
        return dynamic_cast<ImoContentObj*>( get_first_child() );
    }
    inline ImoContentObj* get_last_item() {
        return dynamic_cast<ImoContentObj*>( get_last_child() );
    }

    //methods for adding inline-level content
    LOMSE_DECLARE_INLINE_LEVEL_INTERFACE

};

//---------------------------------------------------------------------------------------
// ImoInlineLevelObj: Abstract class from which any ImoInlinesContainer content object must derive
class ImoInlineLevelObj : public ImoContentObj
{
protected:
    ImoInlineLevelObj(int objtype) : ImoContentObj(objtype) {}

public:
    //the five special
    ~ImoInlineLevelObj() override {}
    ImoInlineLevelObj(const ImoInlineLevelObj&) = default;
    ImoInlineLevelObj& operator= (const ImoInlineLevelObj&) = default;
    ImoInlineLevelObj(ImoInlineLevelObj&&) = delete;
    ImoInlineLevelObj& operator= (ImoInlineLevelObj&&) = delete;

};

//---------------------------------------------------------------------------------------
class ImoBoxInline : public ImoInlineLevelObj
{
protected:
    USize m_size;

    ImoBoxInline(int objtype) : ImoInlineLevelObj(objtype), m_size(0.0f, 0.0f) {}
    ImoBoxInline(int objtype, const USize& size) : ImoInlineLevelObj(objtype)
                                                 , m_size(size) {}
    ImoBoxInline(int objtype, LUnits width, LUnits height) : ImoInlineLevelObj(objtype)
                                                           , m_size(width, height) {}


public:
    //the five special
    ~ImoBoxInline() override {}
    ImoBoxInline(const ImoBoxInline&) = default;
    ImoBoxInline& operator= (const ImoBoxInline&) = default;
    ImoBoxInline(ImoBoxInline&&) = delete;
    ImoBoxInline& operator= (ImoBoxInline&&) = delete;

    //add/remove content
    inline void add_item(ImoInlineLevelObj* pItem) { append_child_imo(pItem); }
    inline void remove_item(ImoContentObj* pItem) { remove_child_imo(pItem); }

    //content traversal
    inline int get_num_items() { return get_num_children(); }
    inline ImoInlineLevelObj* get_first_item() {
        return dynamic_cast<ImoInlineLevelObj*>( get_first_child() );
    }
    inline ImoInlineLevelObj* get_last_item() {
        return dynamic_cast<ImoInlineLevelObj*>( get_last_child() );
    }
    inline ImoInlineLevelObj* get_item(int iItem) {
        return dynamic_cast<ImoInlineLevelObj*>( get_child(iItem) );
    }

    //size
    inline USize& get_size() { return m_size; }
    inline LUnits get_width() { return m_size.width; }
    inline LUnits get_height() { return m_size.height; }
    inline void set_size(const USize& size) { m_size = size; }
    inline void set_width(LUnits value) { m_size.width = value; }
    inline void set_height(LUnits value) { m_size.height = value; }

    //methods for adding inline-level content
    LOMSE_DECLARE_INLINE_LEVEL_INTERFACE

};

//---------------------------------------------------------------------------------------
// An inline-block container for inline objs
class ImoInlineWrapper : public ImoBoxInline
{
protected:
    friend class ImFactory;
    ImoInlineWrapper() : ImoBoxInline(k_imo_inline_wrapper) {}

public:
    //the five special
    ~ImoInlineWrapper() override {}
    ImoInlineWrapper(const ImoInlineWrapper&) = default;
    ImoInlineWrapper& operator= (const ImoInlineWrapper&) = default;
    ImoInlineWrapper(ImoInlineWrapper&&) = delete;
    ImoInlineWrapper& operator= (ImoInlineWrapper&&) = delete;
};

//---------------------------------------------------------------------------------------
// An inline-block wrapper to add link properties to wrapped objs
class ImoLink : public ImoBoxInline
{
private:
    std::string m_url;
    std::string m_language;

    friend class ImFactory;
    ImoLink() : ImoBoxInline(k_imo_link) {}

public:
    //the five special
    ~ImoLink() override {}
    ImoLink(const ImoLink&) = default;
    ImoLink& operator= (const ImoLink&) = default;
    ImoLink(ImoLink&&) = delete;
    ImoLink& operator= (ImoLink&&) = delete;

    //url
    inline std::string& get_url() { return m_url; }
    inline void set_url(const std::string& url) { m_url = url; }
    std::string get_language();
    void set_language(const std::string& value) { m_language = value; }

};

//---------------------------------------------------------------------------------------
// Any atomic displayable object for a Score
class ImoScoreObj : public ImoContentObj
{
protected:
    Color m_color;

    ImoScoreObj(ImoId id, int objtype) : ImoContentObj(id, objtype), m_color(0,0,0) {}
    ImoScoreObj(int objtype) : ImoContentObj(objtype), m_color(0,0,0) {}

public:
    //the five special
    ~ImoScoreObj() override {}
    ImoScoreObj(const ImoScoreObj&) = default;
    ImoScoreObj& operator= (const ImoScoreObj&) = default;
    ImoScoreObj(ImoScoreObj&&) = delete;
    ImoScoreObj& operator= (ImoScoreObj&&) = delete;

    //getters
    inline Color& get_color() { return m_color; }

    //setters
    inline void set_color(Color color) { m_color = color; }

    //IM attributes interface
    void set_int_attribute(TIntAttribute attrib, int value) override;
    int get_int_attribute(TIntAttribute attrib) override;
    void set_color_attribute(TIntAttribute attrib, Color value) override;
    Color get_color_attribute(TIntAttribute attrib) override;
    std::list<TIntAttribute> get_supported_attributes() override;

};

//---------------------------------------------------------------------------------------
// StaffObj: An object attached to an Staff. Consume time
class ImoStaffObj : public ImoScoreObj
{
protected:
    int m_staff = 0;
    int m_nVoice = 0;       //1..n. voice==0 means not defined
    TimeUnits m_time = 0.0;
    ColStaffObjsEntry* m_pEntry = nullptr;  //entry in ColStaffObjs table associated to this staffobj

    ImoStaffObj(int objtype) : ImoScoreObj(objtype) {}
    ImoStaffObj(ImoId id, int objtype) : ImoScoreObj(id, objtype) {}

public:
    //the five special
    ~ImoStaffObj() override;
    ImoStaffObj(const ImoStaffObj& a) : ImoScoreObj(a) { clone(a); }
    ImoStaffObj& operator= (const ImoStaffObj& a) { clone(a); return *this; }
    ImoStaffObj(ImoStaffObj&&) = delete;
    ImoStaffObj& operator= (ImoStaffObj&&) = delete;

    //relations
    void include_in_relation(ImoRelObj* pRelObj, ImoRelDataObj* pData=nullptr);
    void remove_from_relation(ImoRelObj* pRelObj);
    void remove_but_not_delete_relation(ImoRelObj* pRelObj);

    void add_relation(ImoRelObj* pRO);
    ImoRelations* get_relations();
    bool has_relations();
    int get_num_relations();
    ImoRelObj* get_relation(int i);
    ImoRelObj* find_relation(int type);

    //getters
    inline TimeUnits get_time() { return m_time; }
    virtual TimeUnits get_duration() { return 0.0; }
    inline int get_staff() { return m_staff; }
    inline int get_voice() { return m_nVoice; }
    inline ColStaffObjsEntry* get_colstaffobjs_entry() { return m_pEntry; }

    //setters
    virtual void set_staff(int staff) { m_staff = staff; }
    inline void set_voice(int voice) { m_nVoice = voice; }
    virtual void set_time(TimeUnits rTime) { m_time = rTime; }
    inline void set_colstaffobjs_entry(ColStaffObjsEntry* pEntry) { m_pEntry = pEntry; }

    //other
    ImoInstrument* get_instrument();
    ImoScore* get_score();

    /** Non-timed staffobjs (e.g. ImoDirections) are stored in the IM maintaining the
        same order, in relation to note/rests and barlines, than the score order in
        source format. This ordering is important, in some cases, for re-exporting
        an score. This method returns the first noterest or barline that has been
        defined after this staffobj. If no barline or noterest, returns nullptr.
    */
    ImoStaffObj* get_next_noterest_or_barline();

    //IM attributes interface
    virtual void set_int_attribute(TIntAttribute attrib, int value) override;
    virtual int get_int_attribute(TIntAttribute attrib) override;
    virtual list<TIntAttribute> get_supported_attributes() override;

protected:

    ImoStaffObj& clone(const ImoStaffObj& a);

};

//---------------------------------------------------------------------------------------
// AuxObj: a ScoreObj that must be attached to other objects but not
//         directly to an staff. Do not consume time
class ImoAuxObj : public ImoScoreObj
{
protected:
    ImoAuxObj(int objtype) : ImoScoreObj(objtype) {}
    ImoAuxObj(ImoId id, int objtype)
        : ImoScoreObj(id, objtype)
    {
    }

public:
    //the five special
    ~ImoAuxObj() override {}
    ImoAuxObj(const ImoAuxObj&) = default;
    ImoAuxObj& operator= (const ImoAuxObj&) = default;
    ImoAuxObj(ImoAuxObj&&) = delete;
    ImoAuxObj& operator= (ImoAuxObj&&) = delete;

    ImoStaffObj* get_parent_staffobj();
};

//---------------------------------------------------------------------------------------
// AuxRelObj: a type of AuxObj that is related to other AuxObjs of the same type.
class ImoAuxRelObj : public ImoAuxObj
{
protected:
    ImoId m_prevId;     //Id for previous ImoAuxRelObj
    ImoId m_nextId;     //Id for next ImoAuxRelObj

    ImoAuxRelObj(int objtype)
        : ImoAuxObj(objtype)
        , m_prevId(k_no_imoid)
        , m_nextId(k_no_imoid)
    {
    }
    ImoAuxRelObj(ImoId id, int objtype)
        : ImoAuxObj(id, objtype)
        , m_prevId(k_no_imoid)
        , m_nextId(k_no_imoid)
    {
    }


public:

    //the five special
    ~ImoAuxRelObj() override;
    ImoAuxRelObj(const ImoAuxRelObj&) = default;
    ImoAuxRelObj& operator= (const ImoAuxRelObj&) = default;
    ImoAuxRelObj(ImoAuxRelObj&&) = delete;
    ImoAuxRelObj& operator= (ImoAuxRelObj&&) = delete;

    //information
    inline bool is_start_of_relation() { return m_prevId == k_no_imoid; }
    inline bool is_end_of_relation() { return m_nextId == k_no_imoid; }

protected:

    void link_to_next_ARO(ImoId idNext);
    void set_prev_ARO(ImoId idPrev);

    ImoAuxRelObj* get_prev_ARO() { return get_ARO_imo(m_prevId); }
    ImoAuxRelObj* get_next_ARO() { return get_ARO_imo(m_nextId); }
    ImoAuxRelObj* get_ARO_imo(ImoId id);
};

//---------------------------------------------------------------------------------------
//An abstract object containing the specific data for one node in a relation
class ImoRelDataObj : public ImoSimpleObj
{
protected:
    ImoRelDataObj(int objtype) : ImoSimpleObj(objtype) {}

public:
    //the five special
    ~ImoRelDataObj() override {}
    ImoRelDataObj(const ImoRelDataObj&) = default;
    ImoRelDataObj& operator= (const ImoRelDataObj&) = default;
    ImoRelDataObj(ImoRelDataObj&&) = delete;
    ImoRelDataObj& operator= (ImoRelDataObj&&) = delete;

};

//---------------------------------------------------------------------------------------
/** %ImoRelObj: An abstract object relating two or more StaffObjs. Exception: All grace
    notes preceeding a regular note are tied by a ImoGraceRelObj relationship. But if
    only one grace note preceedes the regular note, the ImoGraceRelObj is maintained.
    Thus, grace notes can have a ImoGraceRelObj with only one participant: the grace
    note itself.
*/
# define LOMSE_RELOBJ_USES_ID    1

class ImoRelObj : public ImoScoreObj
{
private:
    //AWARE: Elements of this list can never be children of this node, as any instance
    //of ImoRelObj is linked to two or more nodes
#if (LOMSE_RELOBJ_USES_ID == 1)
    std::list< std::pair<ImoId, ImoRelDataObj*> > m_relatedObjects;
    std::list< std::pair<ImoStaffObj*, ImoRelDataObj*> >* m_pointersList = nullptr;
#else
    std::list< pair<ImoStaffObj*, ImoRelDataObj*> > m_relatedObjects;
#endif

protected:
    ImoRelObj(int objtype) : ImoScoreObj(objtype) {}

public:
    //the five special
    ~ImoRelObj() override;
    ImoRelObj(const ImoRelObj& a) : ImoScoreObj(a) { clone(a); }
    ImoRelObj& operator= (const ImoRelObj& a) { clone(a); return *this; }
    ImoRelObj(ImoRelObj&&) = delete;
    ImoRelObj& operator= (ImoRelObj&&) = delete;

    virtual void push_back(ImoStaffObj* pSO, ImoRelDataObj* pData);
    void remove(ImoStaffObj* pSO);
    void remove_all();
    inline int get_num_objects() { return int( m_relatedObjects.size() ); }
#if (LOMSE_RELOBJ_USES_ID == 1)
    std::list< std::pair<ImoStaffObj*, ImoRelDataObj*> >& get_related_objects();
    ImoStaffObj* get_start_object();
    ImoStaffObj* get_end_object();
#else
    std::list< std::pair<ImoStaffObj*, ImoRelDataObj*> >& get_related_objects() { return m_relatedObjects; }
    ImoStaffObj* get_start_object() { return m_relatedObjects.front().first; }
    ImoStaffObj* get_end_object() { return m_relatedObjects.back().first; }
#endif
    ImoRelDataObj* get_start_data() { return m_relatedObjects.front().second; }
    ImoRelDataObj* get_end_data() { return m_relatedObjects.back().second; }
    ImoRelDataObj* get_data_for(ImoStaffObj* pSO);

    virtual void reorganize_after_object_deletion()=0;
    virtual int get_min_number_for_autodelete() { return 2; }

    void accept_visitor_for_data(BaseVisitor& v, ImoStaffObj* pSO);

protected:
    ImoRelObj& clone(const ImoRelObj& a);

};

//---------------------------------------------------------------------------------------
class ImoAttachments : public ImoCollection
{
protected:
    friend class ImFactory;
    friend class ImoContentObj;
    ImoAttachments() : ImoCollection(k_imo_attachments) {}

public:
    //the five special
    ~ImoAttachments() override {};
    ImoAttachments(const ImoAttachments&) = default;
    ImoAttachments& operator= (const ImoAttachments&) = default;
    ImoAttachments(ImoAttachments&&) = delete;
    ImoAttachments& operator= (ImoAttachments&&) = delete;

    void remove(ImoAuxObj* pAO) { remove_child_imo(pAO); }
    void add(ImoAuxObj* pAO) { append_child_imo(pAO); }
    ImoAuxObj* find_item_of_type(int type)
    {
        return static_cast<ImoAuxObj*>( get_child_of_type(type) );
    }
};


//=======================================================================================
// Simple objects
//=======================================================================================

//---------------------------------------------------------------------------------------
class ImoBeamData : public ImoRelDataObj
{
protected:
    int m_beamType[6];
    bool m_repeat[6];

    friend class ImFactory;
    ImoBeamData(ImoBeamDto* pDto);
    ImoBeamData();

public:
    //the five special
    ~ImoBeamData() override {}
    ImoBeamData(const ImoBeamData&) = default;
    ImoBeamData& operator= (const ImoBeamData&) = default;
    ImoBeamData(ImoBeamData&&) = delete;
    ImoBeamData& operator= (ImoBeamData&&) = delete;

    //getters
    inline int get_beam_type(int level) { return m_beamType[level]; }
    inline bool get_repeat(int level) { return m_repeat[level]; }

    //setters
    inline void set_beam_type(int level, int type) { m_beamType[level] = type; }

    //properties
    bool is_end_of_beam();
    bool is_start_of_beam();
};

//---------------------------------------------------------------------------------------
class ImoBeamDto : public ImoSimpleObj
{
protected:
    int m_beamType[6];
    int m_beamNum;
    bool m_repeat[6];
    LdpElement* m_pBeamElm;
    ImoNoteRest* m_pNR;
    int m_lineNum;

public:
    ImoBeamDto();

    //getters
    inline int get_beam_number()
    {
        return m_beamNum;
    }
    inline ImoNoteRest* get_note_rest()
    {
        return m_pNR;
    }
    int get_line_number()
    {
        return m_lineNum;
    }
    int get_beam_type(int level);
    bool get_repeat(int level);

    //setters
    inline void set_beam_number(int num)
    {
        m_beamNum = num;
    }
    inline void set_note_rest(ImoNoteRest* pNR)
    {
        m_pNR = pNR;
    }
    void set_beam_type(int level, int type);
    void set_beam_type(string& segments);
    void set_repeat(int level, bool value);
    inline void set_line_number(int value)
    {
        m_lineNum = value;
    }

    //properties
    bool is_end_of_beam();
    bool is_start_of_beam();

    //required by RelationBuilder
    int get_item_number()
    {
        return get_beam_number();
    }
    bool is_start_of_relation()
    {
        return is_start_of_beam();
    }
    bool is_end_of_relation()
    {
        return is_end_of_beam();
    }
    ImoStaffObj* get_staffobj();

};

//---------------------------------------------------------------------------------------
class ImoBezierInfo : public ImoSimpleObj
{
protected:
    TPoint m_tPoints[4];   //start, end, ctrol1, ctrol2

    friend class ImFactory;
    ImoBezierInfo() : ImoSimpleObj(k_imo_bezier_info) {}

public:
    ImoBezierInfo(ImoBezierInfo* pBezier);

    //the five special
    ~ImoBezierInfo() override {}
    ImoBezierInfo(const ImoBezierInfo&) = default;
    ImoBezierInfo& operator= (const ImoBezierInfo&) = default;
    ImoBezierInfo(ImoBezierInfo&&) = delete;
    ImoBezierInfo& operator= (ImoBezierInfo&&) = delete;

    enum { k_start=0, k_end, k_ctrol1, k_ctrol2, k_max};     // point number

    //points
    inline void set_point(int i, TPoint& value)
    {
        m_tPoints[i] = value;
    }
    inline TPoint& get_point(int i)
    {
        return m_tPoints[i];
    }

};

//---------------------------------------------------------------------------------------
class ImoBorderDto : public ImoSimpleObj
{
    Color         m_color;
    Tenths        m_width;
    ELineStyle    m_style;

public:
    ImoBorderDto() : ImoSimpleObj(k_imo_border_dto)
        , m_color(Color(0,0,0,255)), m_width(1.0f), m_style(k_line_solid) {}

    //getters
    inline Color get_color()
    {
        return m_color;
    }
    inline Tenths get_width()
    {
        return m_width;
    }
    inline ELineStyle get_style()
    {
        return m_style;
    }

    //setters
    inline void set_color(Color value)
    {
        m_color = value;
    }
    inline void set_width(Tenths value)
    {
        m_width = value;
    }
    inline void set_style(ELineStyle value)
    {
        m_style = value;
    }

};

//---------------------------------------------------------------------------------------
class ImoChord : public ImoRelObj
{
protected:
    bool m_fCrossStaff;        //it is a cross-staff chord
    int m_stemDirection;       //value from EComputedStem enum

    friend class ChordEngraver;
    friend class BeamedChordHelper;
    inline void set_stem_direction(int value) { m_stemDirection = value; }

    friend class ImFactory;
    ImoChord()
        : ImoRelObj(k_imo_chord)
        , m_fCrossStaff(false)
        , m_stemDirection(k_computed_stem_undecided)
    {
    }

public:
    //the five special
    ~ImoChord() override = default;
    ImoChord(const ImoChord&) = default;
    ImoChord& operator= (const ImoChord&) = default;
    ImoChord(ImoChord&&) = delete;
    ImoChord& operator= (ImoChord&&) = delete;

    inline bool is_cross_staff() { return m_fCrossStaff; }
    void update_cross_staff_data();
    ImoNote* get_start_note();
    ImoNote* get_end_note();
    inline bool is_stem_up() { return m_stemDirection == k_computed_stem_up
                                      || m_stemDirection == k_computed_stem_forced_up; }
    inline bool is_stem_down() { return m_stemDirection == k_computed_stem_down
                                        || m_stemDirection == k_computed_stem_forced_down; }
    inline bool is_stem_forced_up() { return m_stemDirection == k_computed_stem_forced_up; }
    inline bool is_stem_forced_down() { return m_stemDirection == k_computed_stem_forced_down; }
    inline bool is_stem_direction_decided() { return m_stemDirection != k_computed_stem_undecided; }
    inline bool is_stem_forced() { return m_stemDirection == k_computed_stem_forced_down
                                          || m_stemDirection == k_computed_stem_forced_up; }

    //required override for ImoRelObj
    void reorganize_after_object_deletion() override;

    //override for ImoRelObj
    void push_back(ImoStaffObj* pSO, ImoRelDataObj* pData) override;

};

//---------------------------------------------------------------------------------------
class ImoCursorInfo : public ImoSimpleObj
{
protected:
    int m_instrument;
    int m_staff;
    TimeUnits m_time;
    ImoId m_id;

    friend class ImFactory;
    ImoCursorInfo() : ImoSimpleObj(k_imo_cursor_info)
        , m_instrument(0), m_staff(0), m_time(0.0), m_id(k_no_imoid) {}

public:

    //getters
    inline int get_instrument()
    {
        return m_instrument;
    }
    inline int get_staff()
    {
        return m_staff;
    }
    inline TimeUnits get_time()
    {
        return m_time;
    }
    inline ImoId get_id()
    {
        return m_id;
    }

    //setters
    inline void set_instrument(int value)
    {
        m_instrument = value;
    }
    inline void set_staff(int value)
    {
        m_staff = value;
    }
    inline void set_time(TimeUnits value)
    {
        m_time = value;
    }
    inline void set_id(ImoId value)
    {
        m_id = value;
    }
};

//---------------------------------------------------------------------------------------
class ImoLineStyleDto : public ImoDto
{
protected:
    TypeLineStyle m_style;

    friend class ImFactory;
    friend class ImoTextBox;
    friend class ImoLine;
    friend class ImoScoreLine;
    ImoLineStyleDto() : ImoDto(k_imo_line_style_dto) {}

public:

    TypeLineStyle& get_data() { return m_style; }

    //getters
    inline ELineStyle get_line_style() const { return m_style.lineStyle; }
    inline ELineEdge get_start_edge() const { return m_style.startEdge; }
    inline ELineEdge get_end_edge() const { return m_style.endEdge; }
    inline ELineCap get_start_cap() const { return m_style.startStyle; }
    inline ELineCap get_end_cap() const { return m_style.endStyle; }
    inline Color get_color() const { return m_style.color; }
    inline Tenths get_width() const { return m_style.width; }
    inline TPoint get_start_point() const { return m_style.startPoint; }
    inline TPoint get_end_point() const { return m_style.endPoint; }

    //setters
    inline void set_line_style(ELineStyle value) { m_style.lineStyle = value; }
    inline void set_start_edge(ELineEdge value) { m_style.startEdge = value; }
    inline void set_end_edge(ELineEdge value) { m_style.endEdge = value; }
    inline void set_start_cap(ELineCap value) { m_style.startStyle = value; }
    inline void set_end_cap(ELineCap value) { m_style.endStyle = value; }
    inline void set_color(Color value) { m_style.color = value; }
    inline void set_width(Tenths value) { m_style.width = value; }
    inline void set_start_point(TPoint point) { m_style.startPoint = point; }
    inline void set_end_point(TPoint point) { m_style.endPoint = point; }
};

//---------------------------------------------------------------------------------------
//The <score-part> element allows for multiple score-instruments per
//score-part. ImoMidiInfo contains the MIDI information for one score-instrument
class ImoMidiInfo : public ImoSimpleObj
{
protected:
    std::string	m_soundId;          //id of the score-instrument

    //midi device
    int     m_port = -1;            //port: 0-15 (MIDI 1-16)
    std::string  m_midiDeviceName;  //DeviceName meta event

    //midi instrument
    std::string m_midiName; //name: ProgramName meta-events
    int m_bank = 0;			//bank: 0-16383 (MIDI 1-16,384)
    int m_channel = -1;		//channel: 0-15 (MIDI 1-16)
    int m_program = 0;		//patch: 0-127 (MIDI 1-128)
    int m_unpitched = 0;	//pitch number for percussion: 1-128

    //mixer information
    float m_volume = 1.0f;  //channel volume: 0.0 (muted) to 1.0
    int m_pan = 0;			//degrees: -180 to 180.
                            //      0 = in front of the listener, centered
                            //    -90 = hard left, 90 is hard right
                            //   -180 and 180 = behind the listener, centered
    int m_elevation = 0;    //degrees: -90 to 90.
                            //      0 = listener level
                            //     90 = directly above
                            //    -90 = directly below.

    //to control if variables contain inherited/default values or a new value has been set
    long m_modified = 0L;
    enum {
        k_modified_port =       0x00000001,
        k_modified_bank =       0x00000002,
        k_modified_channel =    0x00000004,
        k_modified_program =    0x00000008,
        k_modified_unpitched =  0x00000010,
        k_modified_volume =     0x00000020,
        k_modified_pan =        0x00000040,
        k_modified_elevation =  0x00000080,
        k_modified_name =       0x00000100,
    };

    friend class ImFactory;
    friend class ImoInstrument;
    ImoMidiInfo() : ImoSimpleObj(k_imo_midi_info) {}

public:
    //the five special
    ~ImoMidiInfo() override {}
    ImoMidiInfo(const ImoMidiInfo&) = default;
    ImoMidiInfo& operator= (const ImoMidiInfo& info)
    {
        if (&info == this)
            return *this;

        m_port = info.m_port;
        m_midiDeviceName = info.m_midiDeviceName;
        m_midiName = info.m_midiName;
        m_bank = info.m_bank;
        m_channel = info.m_channel;
        m_program = info.m_program;
        m_unpitched = info.m_unpitched;
        m_volume = info.m_volume;
        m_pan = info.m_pan;
        m_elevation = info.m_elevation;

        m_modified = 0L;
        return *this;
    }
    ImoMidiInfo(ImoMidiInfo&&) = delete;
    ImoMidiInfo& operator= (ImoMidiInfo&&) = delete;

    //getters
    inline std::string& get_score_instr_id() { return 	m_soundId; }
    inline int get_midi_port() { return m_port; }
    inline std::string& get_midi_device_name() { return m_midiDeviceName; }
    inline std::string& get_midi_name() { return m_midiName; }
    inline int get_midi_bank() { return m_bank; }
    inline int get_midi_channel() { return m_channel; }
    inline int get_midi_program() { return m_program; }
    inline int get_midi_unpitched() { return m_unpitched; }
    inline float get_midi_volume() { return m_volume; }
    inline int get_midi_pan() { return m_pan; }
    inline int get_midi_elevation() { return m_elevation; }

    //check if contains inherited/default values or a new value has been set
    inline bool only_contains_default_values()  { return m_modified == 0L; }
    inline bool is_port_modified() { return (m_modified & k_modified_port) != 0; }
    inline bool is_name_modified() { return (m_modified & k_modified_name) != 0; }
    inline bool is_bank_modified() { return (m_modified & k_modified_bank) != 0; }
    inline bool is_channel_modified() { return (m_modified & k_modified_channel) != 0; }
    inline bool is_program_modified() { return (m_modified & k_modified_program) != 0; }
    inline bool is_unpitched_modified() { return (m_modified & k_modified_unpitched) != 0; }
    inline bool is_volume_modified() { return (m_modified & k_modified_volume) != 0; }
    inline bool is_pan_modified() { return (m_modified & k_modified_pan) != 0; }
    inline bool is_elevation_modified() { return (m_modified & k_modified_elevation) != 0; }

    //setters
    inline void set_score_instr_id(const std::string& value) { m_soundId = value; }
    inline void set_midi_port(int value) { m_port = value; m_modified |= k_modified_port; }
    inline void set_midi_device_name(const std::string& value) { m_midiDeviceName = value; }
    inline void set_midi_name(const std::string& value) { m_midiName = value; m_modified |= k_modified_name; }
    inline void set_midi_bank(int value) { m_bank = value; m_modified |= k_modified_bank; }
    inline void set_midi_channel(int value) { m_channel = value; m_modified |= k_modified_channel; }
    inline void set_midi_program(int value) { m_program = value; m_modified |= k_modified_program; }
    inline void set_midi_unpitched(int value) { m_unpitched = value; m_modified |= k_modified_unpitched; }
    inline void set_midi_volume(float value) { m_volume = value; m_modified |= k_modified_volume; }
    inline void set_midi_pan(int value) { m_pan = value; m_modified |= k_modified_pan; }
    inline void set_midi_elevation(int value) { m_elevation = value; m_modified |= k_modified_elevation; }

protected:

    //used by MidiAssigner to assing values when info is not present in source file
    friend class MidiAssigner;
    inline void init_midi_port(int value) { m_port = value; }
    inline void init_midi_channel(int value) { m_channel = value; }
};

//---------------------------------------------------------------------------------------
class ImoTextBlockInfo : public ImoSimpleObj
{
//<location>[<size>][<color>][<border>]
protected:
    //block position and size
    TSize     m_size;
    TPoint    m_topLeftPoint;

    ////block position within a possible parent block
    //EHAlign       m_hAlign;
    //EVAlign       m_vAlign;

    //block looking
    Color        m_bgColor;
    Color        m_borderColor;
    Tenths        m_borderWidth;
    ELineStyle    m_borderStyle;

public:
    ImoTextBlockInfo()
        : ImoSimpleObj(k_imo_textblock_info)
        , m_size(TSize(160.0f, 100.0f))
        , m_topLeftPoint(TPoint(0.0f, 0.0f))
        , m_bgColor( Color(255, 255, 255, 255) )
        , m_borderColor( Color(0, 0, 0, 255) )
        , m_borderWidth(1.0f)
        , m_borderStyle(k_line_solid)
    {
    }

    //getters
    inline Tenths get_height()
    {
        return m_size.height;
    }
    inline Tenths get_width()
    {
        return m_size.width;
    }
    inline TPoint get_position()
    {
        return m_topLeftPoint;
    }
    inline Color get_bg_color()
    {
        return m_bgColor;
    }
    inline Color get_border_color()
    {
        return m_borderColor;
    }
    inline Tenths get_border_width()
    {
        return m_borderWidth;
    }
    inline ELineStyle get_border_style()
    {
        return m_borderStyle;
    }

    //setters
    inline void set_position_x(Tenths value)
    {
        m_topLeftPoint.x = value;
    }
    inline void set_position_y(Tenths value)
    {
        m_topLeftPoint.y = value;
    }
    inline void set_size(TSize size)
    {
        m_size = size;
    }
    inline void set_bg_color(Color color)
    {
        m_bgColor = color;
    }
    inline void set_border(ImoBorderDto* pBorder)
    {
        m_borderColor = pBorder->get_color();
        m_borderWidth = pBorder->get_width();
        m_borderStyle = pBorder->get_style();
    }
};

//---------------------------------------------------------------------------------------
//The score-instrument element allows for multiple instruments per
//score-part. Contains the information for an instrument in an score-part
class ImoSoundInfo : public ImoSimpleObj
{
protected:
    std::string	m_soundId;          //id of the score-instrument
    std::string	m_instrName;		//not oriented to appearing printed on the score
    std::string	m_instrAbbrev;		//not oriented to appearing printed on the score
    std::string	m_instrSound;		//describes the default timbre of the score-instrument
    //Allows playback to be shared more easily between
    //applications and libraries.

    //performance data: a solo instrument? or an ensemble?
    bool	m_fSolo;			//performance by a solo instrument
    bool	m_fEnsemble;		//performance by an ensemble
    int		m_ensembleSize;		//if ensemble, the size of the section, or
    //0 if the ensemble size is not specified.

    //defines the specific virtual instrument used for sound.
    std::string	m_virtualLibrary;		//virtual library name
    std::string	m_virtualName;			//virtual instrument to use

    //defines play technique to use for all notes played back with this instrument
    int     m_playTechnique;


    friend class ImFactory;
    friend class ImoInstrument;
    ImoSoundInfo();
    void initialize_object() override;

public:
    //the five special
    ~ImoSoundInfo() override {}
    ImoSoundInfo(const ImoSoundInfo&) = default;
    ImoSoundInfo& operator= (const ImoSoundInfo&) = default;
    ImoSoundInfo(ImoSoundInfo&&) = delete;
    ImoSoundInfo& operator= (ImoSoundInfo&&) = delete;

    //getters
    inline std::string& get_score_instr_id() { return m_soundId; }
    inline std::string& get_score_instr_name() { return m_instrName; }
    inline std::string& get_score_instr_abbrev() { return m_instrAbbrev; }
    inline std::string& get_score_instr_sound() { return m_instrSound; }
    inline bool	get_score_instr_solo() { return m_fSolo; }
    inline bool	get_score_instr_ensemble() { return m_fEnsemble; }
    inline int get_score_instr_ensemble_size() { return m_ensembleSize; }
    inline std::string& get_score_instr_virtual_library() { return m_virtualLibrary; }
    inline std::string& get_score_instr_virtual_name() { return m_virtualName; }

    //setters
    void set_score_instr_id(const std::string& value);
    inline void set_score_instr_name(const std::string& value) { m_instrName = value; }
    inline void set_score_instr_abbrev(const std::string& value) { m_instrAbbrev = value; }
    inline void set_score_instr_sound(const std::string& value) { m_instrSound = value; }
    inline void set_score_instr_solo(bool value) { m_fSolo = value; }
    inline void set_score_instr_ensemble(bool value) { m_fEnsemble = value; }
    inline void set_score_instr_ensemble_size(int value) { m_ensembleSize = value; }
    inline void set_score_instr_virtual_library(const std::string& value) { m_virtualLibrary = value; }
    inline void set_score_instr_virtual_name(const std::string& value) { m_virtualName = value; }

    //access to ImoMidiInfo child
    ImoMidiInfo* get_midi_info();

};

//---------------------------------------------------------------------------------------
class ImoPageInfo : public ImoSimpleObj
{
protected:
    //odd pages
    LUnits  m_uLeftMarginOdd;
    LUnits  m_uRightMarginOdd;
    LUnits  m_uTopMarginOdd;
    LUnits  m_uBottomMarginOdd;
    //even pages
    LUnits  m_uLeftMarginEven;
    LUnits  m_uRightMarginEven;
    LUnits  m_uTopMarginEven;
    LUnits  m_uBottomMarginEven;
    //common
    USize   m_uPageSize;
    bool    m_fPortrait;

    //to control if variables contain inherited/default values or a new value has been set
    long m_modified = 0L;
    enum {
        k_modified_left_margin_odd =    0x00000001,
        k_modified_left_margin_even =   0x00000002,
        k_modified_top_margin_odd =     0x00000004,
        k_modified_top_margin_even =    0x00000008,
        k_modified_right_margin_odd =   0x00000010,
        k_modified_right_margin_even =  0x00000020,
        k_modified_bottom_margin_odd =  0x00000040,
        k_modified_bottom_margin_even = 0x00000080,
        k_modified_page_size =          0x00000100,
    };

    friend class ImFactory;
    friend class ImoDocument;
    friend class ImoScore;
    ImoPageInfo();

public:
    //the five special
    ~ImoPageInfo() override {}
    ImoPageInfo(const ImoPageInfo&) = default;
    ImoPageInfo& operator= (const ImoPageInfo&) = default;
    ImoPageInfo(ImoPageInfo&&) = delete;
    ImoPageInfo& operator= (ImoPageInfo&&) = delete;

    //info about modified values (values imported from source file or modified by the user)
    inline bool is_page_layout_modified() { return m_modified != 0L; }
    inline bool is_left_margin_odd_modified() { return (m_modified & k_modified_left_margin_odd) != 0; }
    inline bool is_left_margin_even_modified() { return (m_modified & k_modified_left_margin_even) != 0; }
    inline bool is_top_margin_odd_modified() { return (m_modified & k_modified_top_margin_odd) != 0; }
    inline bool is_top_margin_even_modified() { return (m_modified & k_modified_top_margin_even) != 0; }
    inline bool is_right_margin_odd_modified() { return (m_modified & k_modified_right_margin_odd) != 0; }
    inline bool is_right_margin_even_modified() { return (m_modified & k_modified_right_margin_even) != 0; }
    inline bool is_bottom_margin_odd_modified() { return (m_modified & k_modified_bottom_margin_odd) != 0; }
    inline bool is_bottom_margin_even_modified() { return (m_modified & k_modified_bottom_margin_even) != 0; }
    inline bool is_page_size_modified() { return (m_modified & k_modified_page_size) != 0; }
    inline bool are_odd_page_margins_modified() { return (m_modified & k_modified_left_margin_odd) != 0
                                                         || (m_modified & k_modified_top_margin_odd) != 0
                                                         || (m_modified & k_modified_right_margin_odd) != 0
                                                         || (m_modified & k_modified_bottom_margin_odd) != 0; }
    inline bool are_even_page_margins_modified() { return (m_modified & k_modified_left_margin_even) != 0
                                                          || (m_modified & k_modified_top_margin_even) != 0
                                                          || (m_modified & k_modified_right_margin_even) != 0
                                                          || (m_modified & k_modified_bottom_margin_even) != 0; }
    inline bool are_page_margins_modified() { return are_odd_page_margins_modified()
                                                     || are_even_page_margins_modified(); }

    //margins, odd pages
    inline LUnits get_left_margin_odd() { return m_uLeftMarginOdd; }
    inline LUnits get_right_margin_odd() { return m_uRightMarginOdd; }
    inline LUnits get_top_margin_odd() { return m_uTopMarginOdd; }
    inline LUnits get_bottom_margin_odd() { return m_uBottomMarginOdd; }
    inline void set_left_margin_odd(LUnits value) { m_uLeftMarginOdd = value; m_modified |= k_modified_left_margin_odd; }
    inline void set_right_margin_odd(LUnits value) { m_uRightMarginOdd = value; m_modified |= k_modified_right_margin_odd; }
    inline void set_top_margin_odd(LUnits value) { m_uTopMarginOdd = value; m_modified |= k_modified_top_margin_odd; }
    inline void set_bottom_margin_odd(LUnits value) { m_uBottomMarginOdd = value; m_modified |= k_modified_bottom_margin_odd; }

    //margins, even pages
    inline LUnits get_left_margin_even() { return m_uLeftMarginEven; }
    inline LUnits get_right_margin_even() { return m_uRightMarginEven; }
    inline LUnits get_top_margin_even() { return m_uTopMarginEven; }
    inline LUnits get_bottom_margin_even() { return m_uBottomMarginEven; }
    inline void set_left_margin_even(LUnits value) { m_uLeftMarginEven = value; m_modified |= k_modified_left_margin_even; }
    inline void set_right_margin_even(LUnits value) { m_uRightMarginEven = value; m_modified |= k_modified_right_margin_even; }
    inline void set_top_margin_even(LUnits value) { m_uTopMarginEven = value; m_modified |= k_modified_top_margin_even; }
    inline void set_bottom_margin_even(LUnits value) { m_uBottomMarginEven = value; m_modified |= k_modified_bottom_margin_even; }

    //page size
    inline USize get_page_size() { return m_uPageSize; }
    inline LUnits get_page_width() { return m_uPageSize.width; }
    inline LUnits get_page_height() { return m_uPageSize.height; }
    inline void set_page_size(USize uPageSize) { m_uPageSize = uPageSize; m_modified |= k_modified_page_size; }
    inline void set_page_width(LUnits value) { m_uPageSize.width = value; m_modified |= k_modified_page_size; }
    inline void set_page_height(LUnits value) { m_uPageSize.height = value; m_modified |= k_modified_page_size; }

    //page orientation
    inline bool is_portrait() { return m_fPortrait; }
    inline void set_portrait(bool value) { m_fPortrait = value; }
};


//=======================================================================================
// Real objects
//=======================================================================================

//---------------------------------------------------------------------------------------
class ImoAnonymousBlock : public ImoInlinesContainer
{
protected:
    friend class ImoBlocksContainer;
    friend class Document;
    friend class ImFactory;
    ImoAnonymousBlock() : ImoInlinesContainer(k_imo_anonymous_block) {}

public:
    //the five special
    ~ImoAnonymousBlock() override {}
    ImoAnonymousBlock(const ImoAnonymousBlock&) = default;
    ImoAnonymousBlock& operator= (const ImoAnonymousBlock&) = default;
    ImoAnonymousBlock(ImoAnonymousBlock&&) = delete;
    ImoAnonymousBlock& operator= (ImoAnonymousBlock&&) = delete;

    //required by Visitable parent class
    void accept_visitor(BaseVisitor& v) override;
};

//---------------------------------------------------------------------------------------
/** %ImoBarline represents a barline for an instrument. Each instrument/staff has its
    own barline. Systemic barlines do not explicitly exist, they are just implicit when
    the score has barlines. The systemic barline can be supressed by invoking
    ImoScore::set_bool_option("Staff.DrawLeftBarline", false).
*/
class ImoBarline : public ImoStaffObj
{
protected:
    int m_barlineType;  //from enum EBarline
    bool m_fMiddle;     //true when it is in the middle of a measure (e.g.: dotted, dashed)
    bool m_fTKChange;   //true if Time and/or Key signature change after this barline

    int m_times;    //for k_barline_end_repetition and k_barline_double_repetition,
                    //indicates the number of times the repeated section must be
                    //played. If there are volta brackets m_times is updated with
                    //the number of times implied by the volta bracket.

    int m_winged;   //indicates whether the barline has winged extensions above and
                    //below. The k_winged_none value indicates no wings.

    TypeMeasureInfo* m_pMeasureInfo;    //ptr to info for measure ending with this barline.
                                        //nullptr when middle barline

    friend class ImFactory;
    ImoBarline()
        : ImoStaffObj(k_imo_barline)
        , m_barlineType(k_barline_simple)
        , m_fMiddle(false)
        , m_fTKChange(false)
        , m_times(0)
        , m_winged(k_wings_none)
        , m_pMeasureInfo(nullptr)
    {
    }

public:
    //the five special
    ~ImoBarline() override;
    ImoBarline(const ImoBarline& a) : ImoStaffObj(a) { clone(a); }
    ImoBarline& operator= (const ImoBarline& a) { clone(a); return *this; }
    ImoBarline(ImoBarline&&) = delete;
    ImoBarline& operator= (ImoBarline&&) = delete;

    //getters
    inline int get_type() { return m_barlineType; }
    inline bool is_middle() { return m_fMiddle; }
    inline int get_num_repeats() { return m_times; }
    inline int get_winged() { return m_winged; }
    inline TypeMeasureInfo* get_measure_info() { return m_pMeasureInfo; }
    inline bool tk_change_after() { return m_fTKChange; }


    //setters
    inline void set_type(int barlineType) { m_barlineType = barlineType; }
    inline void set_middle(bool value) { m_fMiddle = value; }
    inline void set_num_repeats(int times) { m_times = times; }
    inline void set_winged(int wingsType) { m_winged = wingsType; }
    inline void set_measure_info(TypeMeasureInfo* pInfo) { m_pMeasureInfo = pInfo; }

    //overrides: barlines always in staff 0
    void set_staff(int UNUSED(staff)) override { m_staff = 0; }

    //IM attributes interface
    virtual void set_int_attribute(TIntAttribute attrib, int value) override;
    virtual int get_int_attribute(TIntAttribute attrib) override;
    virtual list<TIntAttribute> get_supported_attributes() override;

protected:
    ImoBarline& clone(const ImoBarline& a);

    friend class ColStaffObjsBuilderEngine1x;
    friend class ColStaffObjsBuilderEngine2x;
    inline void set_tk_change() { m_fTKChange = true; }
};

//---------------------------------------------------------------------------------------
class ImoBeam : public ImoRelObj
{
protected:
    vector<int>* m_pStemsDir;   //engravers computed values for stem directions.
                                //Inmutable, no problem in cloning. Deleted in destructor.

    friend class BeamedChordHelper;
    void set_stems_direction(vector<int>* pStemsDir);

    friend class ImFactory;
    ImoBeam() : ImoRelObj(k_imo_beam), m_pStemsDir(nullptr) {}

public:
    //the five special
    ~ImoBeam() override { delete m_pStemsDir; }
    ImoBeam(const ImoBeam&) = default;
    ImoBeam& operator= (const ImoBeam&) = default;
    ImoBeam(ImoBeam&&) = delete;
    ImoBeam& operator= (ImoBeam&&) = delete;

    //type of beam
    enum { k_none = 0, k_begin, k_continue, k_end, k_forward, k_backward, };

    int get_max_staff();
    int get_min_staff();
    bool contains_chords();
    inline vector<int>* get_stems_direction() const { return m_pStemsDir; }

    //required override for ImoRelObj
    void reorganize_after_object_deletion() override;
};

//---------------------------------------------------------------------------------------
class ImoBlock : public ImoAuxObj
{
protected:
    ImoTextBlockInfo m_box;

    ImoBlock(int objtype) : ImoAuxObj(objtype) {}
    ImoBlock(int objtype, ImoTextBlockInfo& box) : ImoAuxObj(objtype), m_box(box) {}

public:
    //the five special
    ~ImoBlock() override {}
    ImoBlock(const ImoBlock&) = default;
    ImoBlock& operator= (const ImoBlock&) = default;
    ImoBlock(ImoBlock&&) = delete;
    ImoBlock& operator= (ImoBlock&&) = delete;

};

//---------------------------------------------------------------------------------------
class ImoTextBox : public ImoBlock
{
protected:
    std::string m_text;
    TypeLineStyle m_line;
    bool m_fHasAnchorLine;
    //TPoint m_anchorJoinPoint;     //point on the box rectangle

    friend class ImFactory;
    ImoTextBox() : ImoBlock(k_imo_text_box), m_fHasAnchorLine(false) {}
    ImoTextBox(ImoTextBlockInfo& box) : ImoBlock(k_imo_text_box, box), m_fHasAnchorLine(false) {}

public:
    //the five special
    ~ImoTextBox() override {}
    ImoTextBox(const ImoTextBox&) = default;
    ImoTextBox& operator= (const ImoTextBox&) = default;
    ImoTextBox(ImoTextBox&&) = delete;
    ImoTextBox& operator= (ImoTextBox&&) = delete;

    inline ImoTextBlockInfo* get_box_info() { return &m_box; }
    inline TypeLineStyle& get_anchor_line_info() { return m_line; }
    inline bool has_anchor_line() { return m_fHasAnchorLine; }

    inline const std::string& get_text() { return m_text; }
    inline void set_text(TypeTextInfo& pTI) { m_text = pTI.text; }
    inline void set_anchor_line(ImoLineStyleDto* pLine) {
        m_line = pLine->get_data();
        m_fHasAnchorLine = true;
    }
};

//---------------------------------------------------------------------------------------
// ImoControl: An inline wrapper for defining GUI controls (GUI interactive component).
class ImoControl : public ImoInlineLevelObj
{
protected:
    std::shared_ptr<Control> m_ctrol;

    friend class ImFactory;
    ImoControl(Control* ctrol);
    ImoControl(int type) : ImoInlineLevelObj(type), m_ctrol(nullptr) {}

    friend class ImoBoxInline;
    friend class ImoInlinesContainer;
    void attach_control(Control* ctrol);

public:
    //the five special
//    ~ImoControl() override;
    ImoControl(const ImoControl&) = default;
    ImoControl& operator= (const ImoControl&) = default;
    ImoControl(ImoControl&&) = delete;
    ImoControl& operator= (ImoControl&&) = delete;

    //delegates on its associated Control for determining its size
    USize measure();

    //delegates on its associated Control for generating its graphical model
    GmoBoxControl* layout(LibraryScope& libraryScope, UPoint pos);

    //other
    Control* get_control();

};

//---------------------------------------------------------------------------------------
class ImoButton : public ImoControl
{
protected:
    std::string m_text;
    std::string m_language;
    USize m_size;
    Color m_bgColor;
    bool m_fEnabled;

    friend class ImFactory;
    ImoButton();

public:
    //the five special
    ~ImoButton() override {}
    ImoButton(const ImoButton&) = default;
    ImoButton& operator= (const ImoButton&) = default;
    ImoButton(ImoButton&&) = delete;
    ImoButton& operator= (ImoButton&&) = delete;

    //getters
    inline std::string& get_label() { return m_text; }
    inline std::string& get_language() { return m_language; }
    inline USize get_size() { return m_size; }
    inline LUnits get_width() { return m_size.width; }
    inline LUnits get_height() { return m_size.height; }
    inline Color get_bg_color() { return m_bgColor; }
    inline bool is_enabled() { return m_fEnabled; }

    //setters
    inline void set_style(ImoStyle* pStyle)
    {
        ImoContentObj::set_style(pStyle);
        if (pStyle)
            m_bgColor = pStyle->background_color();
    }
    inline void set_bg_color(Color color) { m_bgColor = color; }
    inline void set_label(const std::string& text) { m_text = text; }
    inline void set_language(const std::string& value) { m_language = value; }
    inline void set_size(const USize& size) { m_size = size; }
    inline void set_width(LUnits value) { m_size.width = value; }
    inline void set_height(LUnits value) { m_size.height = value; }
    inline void enable(bool value) { m_fEnabled = value; }
};

//---------------------------------------------------------------------------------------
class ImoClef : public ImoStaffObj
{
protected:
    int m_sign = k_clef_sign_G;
    int m_line = 2;
    int m_octaveChange = 0;
    int m_symbolSize = k_size_default;

    friend class ImFactory;
    ImoClef() : ImoStaffObj(k_imo_clef) {}

public:
    //the five special
    ~ImoClef() override {};
    ImoClef(const ImoClef&) = default;
    ImoClef& operator= (const ImoClef&) = default;
    ImoClef(ImoClef&&) = delete;
    ImoClef& operator= (ImoClef&&) = delete;

    //building
    void set_clef(int sign, int line, int octaveChange);
    inline void set_symbol_size(int symbolSize) { m_symbolSize = symbolSize; }

    //deprecated, use set_clef()
    void set_clef_type(int type);

    //getters and info
    int get_clef_type() const;
    inline int get_sign() { return m_sign; }
    inline int get_line() { return m_line; }
    inline int get_octave_change() { return m_octaveChange; }
    inline int get_symbol_size() { return m_symbolSize; }
    int get_default_octave();
    int get_octave();
    DiatonicPitch get_first_ledger_line_pitch();
    bool can_generate_secondary_shapes() override { return true; }
    bool supports_accidentals();

};

//---------------------------------------------------------------------------------------
/** %ImoContent object is a generic block-level container, that is a wrapper container
    for ImoBlockLevelObj objects.

    %ImoContent object can be compared to a @<div@> element in HTML. It is used for
    wrapping content.

    In a well formed document this object is a child of ImoDocument, must always
    exist, and there must be only one %ImoContent child.

    The children of %ImoContent are ImoBlockLevelObj objects, representing the main
    blocks of the document content, such as paragraphs, headings, scores, lists or
    tables.

    For example:
@verbatim

                             ImoDocument
                                  |
                             ImoContent
                                  |
         +----------------+----------------+------------------+
         |                |                |                  |
     ImoHeading     ImoParagraph       ImoScore         ImoParagraph

.
@endverbatim
*/
class ImoContent : public ImoBlocksContainer
{
protected:
    friend class ImFactory;
    ImoContent() : ImoBlocksContainer(k_imo_content) {}
    ImoContent(int objtype) : ImoBlocksContainer(objtype) {}

public:
    //the five special
    ~ImoContent() override {}
    ImoContent(const ImoContent&) = default;
    ImoContent& operator= (const ImoContent&) = default;
    ImoContent(ImoContent&&) = delete;
    ImoContent& operator= (ImoContent&&) = delete;

    //contents
    inline int get_num_items() { return get_num_children(); }
    inline void remove_item(ImoContentObj* pItem) { remove_child_imo(pItem); }
        //iItem = 0..n-1
    ImoContentObj* get_item(int iItem) {
         return dynamic_cast<ImoContentObj*>( get_child(iItem) );
    }

};

//---------------------------------------------------------------------------------------
/** ImoDirection represents a discrete instruction in the score that applies to
    notated events.
*/
class ImoDirection : public ImoStaffObj
{
protected:
    Tenths  m_space = 0.0f;
    EPlacement m_placement = k_placement_default;

    // When the <direction> element contains a <sound> children related to repetition
    // marks or a <direction-type> children of type <segno>/<coda>, or a <words>
    // related to repetition marks, these members contains the type of mark.
    // Otherwise, contains k_repeat_none.
    int	    m_displayRepeat = k_repeat_none;
    int     m_soundRepeat = k_repeat_none;

    //to identify dynamics moved to notes (MusicXML import) so that they can be properly
    //exported
    ImoId m_idNR = k_no_imoid;


    friend class ImFactory;
    ImoDirection() : ImoStaffObj(k_imo_direction) {}

public:
    //the five special
    ~ImoDirection() override {}
    ImoDirection(const ImoDirection&) = default;
    ImoDirection& operator= (const ImoDirection&) = default;
    ImoDirection(ImoDirection&&) = delete;
    ImoDirection& operator= (ImoDirection&&) = delete;

    //getters
    inline Tenths get_width() { return m_space; }
    inline EPlacement get_placement() const { return m_placement; }
    inline int get_display_repeat() { return m_displayRepeat; }
    inline int get_sound_repeat() { return m_soundRepeat; }

    //setters
    inline void set_width(Tenths space) { m_space = space; }
    inline void set_placement(EPlacement placement) { m_placement = placement; }
    inline void set_display_repeat(int repeat) { m_displayRepeat = repeat; }
    inline void set_sound_repeat(int repeat) { m_soundRepeat = repeat; }

    //info
    inline bool is_display_repeat() { return m_displayRepeat != k_repeat_none; }
    inline bool is_sound_repeat() { return m_soundRepeat != k_repeat_none; }

    //in MusicXML, when importing a <direction> of type dynamics mark, the dynamics
    //mark is attached to next note. These cases are marked so that they can be
    //properly moved again when exporting to MusicXML
    void mark_as_dynamics_removed(ImoNoteRest* pNR);
    ImoNoteRest* get_noterest_for_transferred_dynamics();

};

//---------------------------------------------------------------------------------------
class ImoSymbolRepetitionMark : public ImoAuxObj
{
protected:
    int m_symbol = ImoSymbolRepetitionMark::k_undefined;       //a value from enum ESymbolRepetitionMark

    friend class ImFactory;
    ImoSymbolRepetitionMark() : ImoAuxObj(k_imo_symbol_repetition_mark) {}

public:
    //the five special
    ~ImoSymbolRepetitionMark() override {}
    ImoSymbolRepetitionMark(const ImoSymbolRepetitionMark&) = default;
    ImoSymbolRepetitionMark& operator= (const ImoSymbolRepetitionMark&) = default;
    ImoSymbolRepetitionMark(ImoSymbolRepetitionMark&&) = delete;
    ImoSymbolRepetitionMark& operator= (ImoSymbolRepetitionMark&&) = delete;

    enum ESymbolRepetitionMark
    {
        k_undefined = 0,
        k_coda,
        k_segno,
    };

    //getters and setters
    inline int get_symbol() { return m_symbol; }
    inline void set_symbol(int value) { m_symbol = value; }

};

//---------------------------------------------------------------------------------------
class ImoDynamic : public ImoContent
{
protected:
    std::string m_classid;

    friend class ImFactory;
    ImoDynamic() : ImoContent(k_imo_dynamic), m_classid("") {}

public:
    //the five special
    ~ImoDynamic() override {}
    ImoDynamic(const ImoDynamic&) = default;
    ImoDynamic& operator= (const ImoDynamic&) = default;
    ImoDynamic(ImoDynamic&&) = delete;
    ImoDynamic& operator= (ImoDynamic&&) = delete;

    //construction
    inline void set_classid(const std::string& value) { m_classid = value; }
    void add_param(ImoParamInfo* pParam);

    //accessors
    inline std::string& get_classid() { return m_classid; }
    std::list<ImoParamInfo*> get_params();

protected:
    ImoParameters* get_parameters();

};

//---------------------------------------------------------------------------------------
class ImoSystemBreak : public ImoStaffObj
{
protected:
    friend class ImFactory;
    ImoSystemBreak() : ImoStaffObj(k_imo_system_break) {}

public:
    //the five special
    ~ImoSystemBreak() override {}
    ImoSystemBreak(const ImoSystemBreak&) = default;
    ImoSystemBreak& operator= (const ImoSystemBreak&) = default;
    ImoSystemBreak(ImoSystemBreak&&) = delete;
    ImoSystemBreak& operator= (ImoSystemBreak&&) = delete;
};

//---------------------------------------------------------------------------------------
/** %ImoDocument is the root object of the internal model. It contains some data, such
    as the default language used in the document, the list of all styles defined
    in the document, and the information about the paper size with its margins.

    Children:
    - one ImoStyles, a container for all ImoStyle objects defined in the document.
      Default ImoStyle objects are automatically added when creating it ImoStyles.

    - one ImoPageInfo object, describing the document intended page size and margins.

    - one ImoContent object, wrapping all the document content. It is automatically
      created when storing the first content item (first invocation
      of ImoBlocksContainer::append_content_item() )

    @verbatim
                             ImoDocument
                                  │
               ┌──────────────────┼──────────────────┐
           ImoStyles         ImoPageInfo         ImoContent
               │                                     │
            ImoStyle (+)                     ImoBlockLevelObj (*)  [e.g. ImoScore]

    .
    @endverbatim
*/
class ImoDocument : public ImoBlocksContainer
{
protected:
    friend class Document;
    float m_scale;     //page content scaling factor
    std::string m_version;
    std::string m_language;
    std::list<ImoStyle*> m_privateStyles;

    friend class ImFactory;
    ImoDocument(const std::string& version="");
    void initialize_object() override;

public:
    //the five special
    ~ImoDocument() override;
    ImoDocument(const ImoDocument& a) : ImoBlocksContainer(a) { clone(a); }
    ImoDocument& operator= (const ImoDocument& a) { clone(a); return *this; }
    ImoDocument(ImoDocument&&) = delete;
    ImoDocument& operator= (ImoDocument&&) = delete;

    //special node
    void accept_visitor(BaseVisitor& v) override;
    bool has_visitable_children() override;

    //info
    inline std::string& get_version() { return m_version; }
    inline void set_version(const std::string& version) { m_version = version; }
    inline std::string& get_language() { return m_language; }
    inline void set_language(const std::string& language) { m_language = language; }
    inline float get_page_content_scale() { return m_scale; }
    inline void set_page_content_scale(float scale) { m_scale = scale; }

    //document intended paper size
    void add_page_info(ImoPageInfo* pPI);
    ImoPageInfo* get_page_info();
    inline LUnits get_paper_width() { return get_page_info()->get_page_width(); }
    inline LUnits get_paper_height() { return get_page_info()->get_page_height(); }

    //styles
    ImoStyles* get_styles();
    void add_style(ImoStyle* pStyle);
    ImoStyle* find_style(const std::string& name);
    ImoStyle* get_default_style();
    ImoStyle* get_style_or_default(const std::string& name);

    //user API
    ImoStyle* create_style(const std::string& name, const std::string& parent="Default style");
    ImoStyle* create_private_style(const std::string& parent="Default style");

    //support for edition commands
    void insert_block_level_obj(ImoBlockLevelObj* pAt, ImoBlockLevelObj* pImoNew);
    void delete_block_level_obj(ImoBlockLevelObj* pAt);

    //cursor
    //TODO: method add_cursor_info
    void add_cursor_info(ImoCursorInfo* UNUSED(pCursor)) {}

protected:
    ImoDocument& clone(const ImoDocument& a);

    void add_private_style(ImoStyle* pStyle);
    void add_default_styles();

};

//---------------------------------------------------------------------------------------
class ImoArpeggio : public ImoRelObj
{
protected:
    EArpeggio m_type;

    friend class ImFactory;
    ImoArpeggio()
        : ImoRelObj(k_imo_arpeggio)
        , m_type(k_arpeggio_standard)
    {
    }

public:
    //the five special
    ~ImoArpeggio() override = default;
    ImoArpeggio(const ImoArpeggio&) = default;
    ImoArpeggio& operator= (const ImoArpeggio&) = default;
    ImoArpeggio(ImoArpeggio&&) = delete;
    ImoArpeggio& operator= (ImoArpeggio&&) = delete;

    //getters
    EArpeggio get_type() const { return m_type; }

    //setters
    void set_type(EArpeggio value) { m_type = value; }

    //required override for ImoRelObj
    void reorganize_after_object_deletion() override;
};

//---------------------------------------------------------------------------------------
class ImoArpeggioDto : public ImoSimpleObj
{
protected:
    EArpeggio m_type;
    Color m_color;

public:
    ImoArpeggioDto() : ImoSimpleObj(k_imo_arpeggio_dto) {}

    //getters
    EArpeggio get_type() const { return m_type; }
    const Color& get_color() const { return m_color; }

    //setters
    void set_type(EArpeggio type) { m_type = type; }
    void set_color(const Color& color) { m_color = color; }

    void apply_properties_to(ImoArpeggio* pArpeggio) const
    {
        pArpeggio->set_type(m_type);
        pArpeggio->set_color(m_color);
    }
};

//---------------------------------------------------------------------------------------
class ImoFermata : public ImoAuxObj
{
protected:
    int m_placement;
    int m_symbol;

    friend class ImFactory;
    ImoFermata()
        : ImoAuxObj(k_imo_fermata)
        , m_placement(k_placement_default)
        , m_symbol(k_normal)
    {
    }

public:
    //the five special
    ~ImoFermata() override {}
    ImoFermata(const ImoFermata&) = default;
    ImoFermata& operator= (const ImoFermata&) = default;
    ImoFermata(ImoFermata&&) = delete;
    ImoFermata& operator= (ImoFermata&&) = delete;

    enum { k_normal, k_short, k_long, k_henze_short, k_henze_long,
           k_very_short, k_very_long, k_curlew,
         };

    //getters
    inline int get_placement()
    {
        return m_placement;
    }
    inline int get_symbol()
    {
        return m_symbol;
    }

    //setters
    inline void set_placement(int placement)
    {
        m_placement = placement;
    }
    inline void set_symbol(int symbol)
    {
        m_symbol = symbol;
    }

};

//---------------------------------------------------------------------------------------
class ImoArticulation : public ImoAuxObj
{
protected:
    int m_articulationType;
    int m_placement;

    ImoArticulation(int objtype)
        : ImoAuxObj(objtype)
        , m_articulationType(k_articulation_unknown)
        , m_placement(k_placement_default)
    {
    }

public:
    //the five special
    ~ImoArticulation() override {}
    ImoArticulation(const ImoArticulation&) = default;
    ImoArticulation& operator= (const ImoArticulation&) = default;
    ImoArticulation(ImoArticulation&&) = delete;
    ImoArticulation& operator= (ImoArticulation&&) = delete;

    //getters
    inline int get_placement()
    {
        return m_placement;
    }
    inline int get_articulation_type()
    {
        return m_articulationType;
    }

    //setters
    inline void set_placement(int placement)
    {
        m_placement = placement;
    }
    inline void set_articulation_type(int articulationType)
    {
        m_articulationType = articulationType;
    }

};

//---------------------------------------------------------------------------------------
class ImoArticulationSymbol : public ImoArticulation
{
protected:
    bool m_fUp;     //only for k_articulation_marccato
    int m_symbol;   //symbol to use when alternatives. For now only for breath_mark

    friend class ImFactory;
    ImoArticulationSymbol()
        : ImoArticulation(k_imo_articulation_symbol)
        , m_fUp(true)
        , m_symbol(k_default)
    {
    }

public:
    //the five special
    ~ImoArticulationSymbol() override {}
    ImoArticulationSymbol(const ImoArticulationSymbol&) = default;
    ImoArticulationSymbol& operator= (const ImoArticulationSymbol&) = default;
    ImoArticulationSymbol(ImoArticulationSymbol&&) = delete;
    ImoArticulationSymbol& operator= (ImoArticulationSymbol&&) = delete;

    enum
    {
        k_default=0,
        k_breath_comma, k_breath_tick, k_breath_v, k_breath_salzedo,
        k_caesura_normal, k_caesura_thick, k_caesura_short, k_caesura_curved,
    };

    //getters
    inline bool is_up()
    {
        return m_fUp;
    }
    inline int get_symbol()
    {
        return m_symbol;
    }

    //setters
    inline void set_up(bool value)
    {
        m_fUp = value;
    }
    inline void set_symbol(int value)
    {
        m_symbol = value;
    }

    //info
    inline bool is_accent()
    {
        return m_articulationType >= k_articulation_accent
               && m_articulationType <= k_articulation_tenuto;
    }
    inline bool is_stress()
    {
        return m_articulationType >= k_articulation_stress
               && m_articulationType <= k_articulation_unstress;
    }
    inline bool is_breath_mark()
    {
        return m_articulationType >= k_articulation_breath_mark
               && m_articulationType <= k_articulation_caesura;
    }
    inline bool is_jazz_pitch()
    {
        return m_articulationType >= k_articulation_scoop
               && m_articulationType <= k_articulation_falloff;
    }

};

//---------------------------------------------------------------------------------------
class ImoArticulationLine : public ImoArticulation
{
protected:
    int m_lineShape;        //straight | curved
    int m_lineType;         //solid | dashed | dotted | wavy
    Tenths m_dashLength;    //only for dashed lines
    Tenths m_dashSpace;     //only for dashed lines

    friend class ImFactory;
    ImoArticulationLine()
        : ImoArticulation(k_imo_articulation_line)
        , m_lineShape(k_line_shape_straight)
        , m_lineType(k_line_type_solid)
        , m_dashLength(4.0)
        , m_dashSpace(2.0)
    {
    }

public:
    //the five special
    ~ImoArticulationLine() override {}
    ImoArticulationLine(const ImoArticulationLine&) = default;
    ImoArticulationLine& operator= (const ImoArticulationLine&) = default;
    ImoArticulationLine(ImoArticulationLine&&) = delete;
    ImoArticulationLine& operator= (ImoArticulationLine&&) = delete;

    //getters
    inline int get_line_shape()
    {
        return m_lineShape;
    }
    inline int get_line_type()
    {
        return m_lineType;
    }
    inline Tenths get_dash_length()
    {
        return m_dashLength;
    }
    inline Tenths get_dash_space()
    {
        return m_dashSpace;
    }

    //setters
    inline void set_line_shape(int lineShape)
    {
        m_lineShape = lineShape;
    }
    inline void set_line_type(int lineType)
    {
        m_lineType = lineType;
    }
    inline void set_dash_length(Tenths length)
    {
        m_dashLength = length;
    }
    inline void set_dash_space(Tenths space)
    {
        m_dashSpace = space;
    }

};

//---------------------------------------------------------------------------------------
class ImoDynamicsMark : public ImoAuxObj
{
protected:
    std::string m_markType;
    int m_placement = k_placement_default;
    bool m_moved = false;       //to identify dynamics moved to notes (MusicXML import)
//TODO
//    %text-decoration;
//    %enclosure;

    friend class ImFactory;
    ImoDynamicsMark() : ImoAuxObj(k_imo_dynamics_mark) {}

public:
    //the five special
    ~ImoDynamicsMark() override {}
    ImoDynamicsMark(const ImoDynamicsMark&) = default;
    ImoDynamicsMark& operator= (const ImoDynamicsMark&) = default;
    ImoDynamicsMark(ImoDynamicsMark&&) = delete;
    ImoDynamicsMark& operator= (ImoDynamicsMark&&) = delete;

    //getters
    inline int get_placement() { return m_placement; }
    inline std::string get_mark_type() { return m_markType; }

    //setters
    inline void set_placement(int placement) { m_placement = placement; }
    inline void set_mark_type(const std::string& markType) { m_markType = markType; }

    //in MusicXML, when importing a <direction> of type dynamics mark, the dynamics
    //mark is attached to next note. These cases are marked so that they can be
    //properly moved again when exporting to MusicXML
    inline void mark_as_moved() { m_moved = true; }
    inline bool is_transferred_from_direction() { return m_moved; }
};

//---------------------------------------------------------------------------------------
class ImoOrnament : public ImoAuxObj
{
protected:
    int m_ornamentType;
    int m_placement;
//TODO
//    %text-decoration;
//    %enclosure;

    friend class ImFactory;
    ImoOrnament()
        : ImoAuxObj(k_imo_ornament)
        , m_ornamentType(k_ornament_unknown)
        , m_placement(k_placement_default)
    {
    }

public:
    //the five special
    ~ImoOrnament() override {}
    ImoOrnament(const ImoOrnament&) = default;
    ImoOrnament& operator= (const ImoOrnament&) = default;
    ImoOrnament(ImoOrnament&&) = delete;
    ImoOrnament& operator= (ImoOrnament&&) = delete;

    //getters
    inline int get_placement()
    {
        return m_placement;
    }
    inline int get_ornament_type()
    {
        return m_ornamentType;
    }

    //setters
    inline void set_placement(int placement)
    {
        m_placement = placement;
    }
    inline void set_ornament_type(int ornamentType)
    {
        m_ornamentType = ornamentType;
    }

};

//---------------------------------------------------------------------------------------
/** %ImoTechnical represents technical indications attached to a note/rest to give
    performance information. Most technical indications just only require the type of
    technical indication and, thus, they are represented by an ImoTechnical object.

    For technical indications requiring additional info, specific derived classes
    are created.
*/
class ImoTechnical : public ImoAuxObj
{
protected:
    int m_technicalType;
    int m_placement;

    friend class ImFactory;
    ImoTechnical()
        : ImoAuxObj(k_imo_technical)
        , m_technicalType(k_technical_unknown)
        , m_placement(k_placement_default)
    {
    }
    ImoTechnical(int type)
        : ImoAuxObj(type)
        , m_technicalType(k_technical_unknown)
        , m_placement(k_placement_default)
    {
    }

public:
    //the five special
    ~ImoTechnical() override {}
    ImoTechnical(const ImoTechnical&) = default;
    ImoTechnical& operator= (const ImoTechnical&) = default;
    ImoTechnical(ImoTechnical&&) = delete;
    ImoTechnical& operator= (ImoTechnical&&) = delete;

    //getters and setters
    inline int get_placement() { return m_placement; }
    inline int get_technical_type() { return m_technicalType; }
    inline void set_placement(int placement) { m_placement = placement; }
    inline void set_technical_type(int technicalType) { m_technicalType = technicalType; }

};

//---------------------------------------------------------------------------------------
/** %ImoFretString is a derived class from ImoTechnical to represent fret and string
    indications attached to a note/rest or to a chord diagram.
*/
class ImoFretString : public ImoTechnical
{
protected:
    int m_fret = 1;
    int m_string = 1;

    friend class ImFactory;
    ImoFretString() : ImoTechnical(k_imo_fret_string)
    {
        m_technicalType = k_technical_fret_string;
    }

public:
    //the five special
    ~ImoFretString() override {}
    ImoFretString(const ImoFretString&) = default;
    ImoFretString& operator= (const ImoFretString&) = default;
    ImoFretString(ImoFretString&&) = delete;
    ImoFretString& operator= (ImoFretString&&) = delete;

    inline int get_fret() const { return m_fret; }
    inline int get_string() const { return m_string; }
    inline void set_fret(int value) { m_fret = value; }
    inline void set_string(int value) { m_string = value; }

};

//---------------------------------------------------------------------------------------
/** Helper class for ImoFingering, with all the information for one fingering element.

    - 'value' is the fingering symbols, normally numbers 1 to 5 (e.g "2") but other
       values are possible.

    - 'attribs' is the list of optional attributes (position, color, font and placement)
      that each fingering element could have. If not present, the information from
      owner ImoFingering element is used.

    - 'flags' are boolean marks to indicate different type of fingering data:
        - bit 0: the fingering is a substitution for previous fingering data.
        - bit 1: it is an alternative fingering for previous fingering data.
*/
class FingerData
{
public:
    std::string value;              //fingering symbols
    AttrObj* attribs = nullptr;     //position, font, color, placement
    unsigned flags = 0;

    enum EFingeringFlags
    {
        k_fingering_substitution    = 0x0001,   //it is a substitution
        k_fingering_alternative     = 0x0002,   //it is an alternative
    };

    FingerData(const std::string& val) : value(val) {}

    //the five special
    ~FingerData() { delete attribs; }
    FingerData(const FingerData& a) { clone(a); }
    FingerData& operator= (const FingerData& a) { clone(a); return *this; }
    FingerData(FingerData&&) = delete;
    FingerData& operator= (FingerData&&) = delete;

    //helper for flags
    inline bool is_substitution() const { return (flags & k_fingering_substitution) != 0; }
    inline void set_substitution(bool data) { data ? flags |= k_fingering_substitution
                                                   : flags &= ~k_fingering_substitution; }
    inline bool is_alternative() const { return (flags & k_fingering_alternative) != 0; }
    inline void set_alternative(bool data) { data ? flags |= k_fingering_alternative
                                                  : flags &= ~k_fingering_alternative; }

protected:
    FingerData& clone(const FingerData& a)
    {
        value = a.value;
        flags = a.flags;
        if (a.attribs)
            attribs = a.attribs->clone();  //LOMSE_NEW AttrObj(*(a.attribs));
        else
            attribs = nullptr;

        return *this;
    }

};

//---------------------------------------------------------------------------------------
/** %ImoFingering represents fingering information. As multiple fingerings may be given,
    fingering information is stored in a list of fingerings.
*/
class ImoFingering : public ImoTechnical
{
protected:
    std::list<FingerData> m_fingerings;

    friend class ImFactory;
    ImoFingering() : ImoTechnical(k_imo_fingering)
    {
        m_technicalType = k_technical_fingering;
    }

public:
    //the five special
    ~ImoFingering() override {}
    ImoFingering(const ImoFingering&) = default;
    ImoFingering& operator= (const ImoFingering&) = default;
    ImoFingering(ImoFingering&&) = delete;
    ImoFingering& operator= (ImoFingering&&) = delete;

    //building
    FingerData& add_fingering(const std::string& value);

    //getters
    inline size_t num_fingerings() { return m_fingerings.size(); }
    inline const std::list<FingerData>& get_fingerings() { return m_fingerings; }

};

//---------------------------------------------------------------------------------------
class ImoGoBackFwd : public ImoStaffObj
{
protected:
    bool        m_fFwd;
    TimeUnits   m_rTimeShift;

    static constexpr TimeUnits k_shift_start_end = 100000000.0;     //any too big value

    friend class ImFactory;
    ImoGoBackFwd() : ImoStaffObj(k_imo_go_back_fwd), m_fFwd(true), m_rTimeShift(0.0) {}

public:
    //the five special
    ~ImoGoBackFwd() override {}
    ImoGoBackFwd(const ImoGoBackFwd&) = default;
    ImoGoBackFwd& operator= (const ImoGoBackFwd&) = default;
    ImoGoBackFwd(ImoGoBackFwd&&) = delete;
    ImoGoBackFwd& operator= (ImoGoBackFwd&&) = delete;

    //getters and setters
    inline bool is_forward() { return m_fFwd; }
    inline void set_forward(bool fFwd) { m_fFwd = fFwd; }
    inline bool is_to_start() { return !m_fFwd && (m_rTimeShift == -k_shift_start_end); }
    inline bool is_to_end() { return m_fFwd && (m_rTimeShift == k_shift_start_end); }
    inline TimeUnits get_time_shift() { return m_rTimeShift; }
    inline void set_to_start() { set_time_shift(k_shift_start_end); }
    inline void set_to_end() { set_time_shift(k_shift_start_end); }
    inline void set_time_shift(TimeUnits rTime) { m_rTimeShift = (m_fFwd ? rTime : -rTime); }

};

//---------------------------------------------------------------------------------------
/** When one or more consecutive grace notes appear in the score, this auxiliary RelObj
    is responsible for relating all the grace notes in a group
    Important: All grace notes preceeding a regular note are tied by a ImoGraceRelObj
    relationship. But if only one grace note preceedes the regular note, the
    ImoGraceRelObj is maintained. Thus, ImoGraceRelObj can have only one participant.
*/
class ImoGraceRelObj : public ImoRelObj
{
protected:
    int         m_graceType;    //a value from enum
    bool        m_fSlash;       //true when grace notes are notated with a diagonal stroke
    float       m_percentage;       //percentage of time to steal
    TimeUnits   m_makeTime;         //duration to assign

    friend class ImFactory;
    ImoGraceRelObj()
        : ImoRelObj(k_imo_grace_relobj)
        , m_graceType(k_grace_steal_previous)
        , m_fSlash(true)
    {
    }

public:
    //the five special
    ~ImoGraceRelObj() override = default;
    ImoGraceRelObj(const ImoGraceRelObj&) = default;
    ImoGraceRelObj& operator= (const ImoGraceRelObj&) = default;
    ImoGraceRelObj(ImoGraceRelObj&&) = delete;
    ImoGraceRelObj& operator= (ImoGraceRelObj&&) = delete;

    // grace notes behaviour
    enum EGraceType
    {
        k_grace_steal_previous = 0, ///< grace note occupies a time interval that ends before the
                                    ///< expected onset of the next non-grace event, shortening
                                    ///< the duration of the preceding non-grace event.
        k_grace_steal_following,    ///< grace note occupies a time interval starting at the
                                    ///< expected onset of the next non-grace event, both delaying
                                    ///< its onset and shortening its duration.
        k_grace_make_time,          ///< grace note delays the onset of the next non-grace event.
    };

    //getters
    inline int get_grace_type() { return m_graceType; }
    inline bool has_slash() { return m_fSlash; }
    inline float get_percentage() { return m_percentage; }
    inline TimeUnits get_time_to_make() { return m_makeTime; }

    //setters
    inline void set_grace_type(int graceType) { m_graceType = graceType; }
    inline void set_slash(bool value) { m_fSlash = value; }
    inline void set_percentage(float value) { m_percentage = value; }
    inline void set_time_to_make(TimeUnits value) { m_makeTime = value; }

    //overrides for ImoRelObj
    int get_min_number_for_autodelete() override { return 1; }
    void reorganize_after_object_deletion() override;
};

//---------------------------------------------------------------------------------------
class ImoScoreText : public ImoAuxObj
{
protected:
    TypeTextInfo m_text;

    friend class ImFactory;
    friend class ImoInstrument;
    friend class ImoInstrGroup;
    ImoScoreText() : ImoAuxObj(k_imo_score_text), m_text() {}
    ImoScoreText(int objtype) : ImoAuxObj(objtype), m_text() {}

public:
    //the five special
    ~ImoScoreText() override {}
    ImoScoreText(const ImoScoreText&) = default;
    ImoScoreText& operator= (const ImoScoreText&) = default;
    ImoScoreText(ImoScoreText&&) = delete;
    ImoScoreText& operator= (ImoScoreText&&) = delete;

    //getters
    inline std::string& get_text() { return m_text.text; }
    inline TypeTextInfo& get_text_info() { return m_text; }
    std::string& get_language();
    inline void set_language(const std::string& lang) { m_text.language = lang; }
    inline bool has_language() { return !(m_text.language).empty(); }

    //setters
    inline void set_text(const std::string& value) { m_text.text = value; }

};

//---------------------------------------------------------------------------------------
class ImoScoreTitle : public ImoScoreText
{
protected:
    int m_hAlign;

    friend class ImFactory;
    ImoScoreTitle() : ImoScoreText(k_imo_score_title), m_hAlign(k_halign_center) {}

public:
    //the five special
    ~ImoScoreTitle() override {}
    ImoScoreTitle(const ImoScoreTitle&) = default;
    ImoScoreTitle& operator= (const ImoScoreTitle&) = default;
    ImoScoreTitle(ImoScoreTitle&&) = delete;
    ImoScoreTitle& operator= (ImoScoreTitle&&) = delete;

    inline int get_h_align()
    {
        return m_hAlign;
    }
    inline void set_h_align(int value)
    {
        m_hAlign = value;
    }
//    inline void set_style(ImoStyle* pStyle) { m_text.set_style(pStyle); }
//    inline ImoStyle* get_style() { return m_text.get_style(); }
};

//---------------------------------------------------------------------------------------
class ImoSoundChange : public ImoStaffObj
{
protected:

    friend class ImFactory;
    ImoSoundChange()
        : ImoStaffObj(k_imo_sound_change)
    {
    }

public:
    //the five special
    ~ImoSoundChange() override {}
    ImoSoundChange(const ImoSoundChange&) = default;
    ImoSoundChange& operator= (const ImoSoundChange&) = default;
    ImoSoundChange(ImoSoundChange&&) = delete;
    ImoSoundChange& operator= (ImoSoundChange&&) = delete;

    ImoMidiInfo* get_midi_info(const std::string& soundId);

};

//---------------------------------------------------------------------------------------
/** ImoTranspose staffobj represents what must be added to the written pitch to get the
    correct sounding pitch. The transposition is represented by:

    - m_chromatic: the number of semitones needed to get from written to sounding
        pitch. It does not include octaves
    - m_octaveChange: how many octaves to add to get from written pitch to sounding pitch.
    - m_diatonic: number of steps, for correct spelling of enharmonic transpositions.
    - m_doubled: if @TRUE indicates that the music is doubled one octave down from what
        is currently written (as is the case for mixed cello / bass parts in
        orchestral literature).
    - m_numStaff. If -1, this transposition applies to all staves in the instrument.
      Otherwise, it applies only to the specified staff (0..n-1)
*/
class ImoTranspose : public ImoStaffObj
{
protected:
    int m_numStaff;
    int m_diatonic;
    int m_chromatic;
    int m_octaveChange;
    bool m_doubled;

    friend class ImFactory;
    ImoTranspose()
        : ImoStaffObj(k_imo_transpose)
    {
        init(-1, 0, 0, 0, false);
    }

public:
    //the five special
    ~ImoTranspose() override {}
    ImoTranspose(const ImoTranspose&) = default;
    ImoTranspose& operator= (const ImoTranspose&) = default;
    ImoTranspose(ImoTranspose&&) = delete;
    ImoTranspose& operator= (ImoTranspose&&) = delete;

    inline int get_applicable_staff() { return m_numStaff; }
    inline int get_diatonic() { return m_diatonic; }
    inline int get_chromatic() { return m_chromatic; }
    inline int get_octave_change() { return m_octaveChange; }
    inline bool get_doubled() { return m_doubled; }

protected:
    friend class TransposeMxlAnalyser;
    void init(int iStaff, int chromatic, int diatonic, int octaves, bool doubled)
    {
        m_numStaff = iStaff;
        m_diatonic = diatonic;
        m_chromatic = chromatic;
        m_octaveChange = octaves;
        m_doubled = doubled;
    }
};

//---------------------------------------------------------------------------------------
class ImoTextRepetitionMark : public ImoScoreText
{
protected:
    int m_repeatType;

    friend class ImFactory;
    ImoTextRepetitionMark()
        : ImoScoreText(k_imo_text_repetition_mark)
        , m_repeatType(0)
    {
    }

public:
    //the five special
    ~ImoTextRepetitionMark() override {}
    ImoTextRepetitionMark(const ImoTextRepetitionMark&) = default;
    ImoTextRepetitionMark& operator= (const ImoTextRepetitionMark&) = default;
    ImoTextRepetitionMark(ImoTextRepetitionMark&&) = delete;
    ImoTextRepetitionMark& operator= (ImoTextRepetitionMark&&) = delete;

    //getters
    inline int get_repeat_mark()
    {
        return m_repeatType;
    }

    //setters
    inline void set_repeat_mark(int repeat)
    {
        m_repeatType = repeat;
    }

};

//---------------------------------------------------------------------------------------
class ImoImage : public ImoInlineLevelObj
{
protected:
    SpImage m_image;

    friend class ImFactory;
    ImoImage() : ImoInlineLevelObj(k_imo_image), m_image( LOMSE_NEW Image() ) {}
    ImoImage(unsigned char* imgbuf, VSize bmpSize, EPixelFormat format, USize imgSize)
        : ImoInlineLevelObj(k_imo_image)
        , m_image( LOMSE_NEW Image(imgbuf, bmpSize, format, imgSize) )
    {
    }
    friend class ImageAnalyser;
    friend class ImageLmdAnalyser;
    friend class ImageMxlAnalyser;
    friend class ImageMnxAnalyser;
    inline void set_content(SpImage img) { m_image = img; }

public:
    //the five special
    ~ImoImage() override {}
    ImoImage(const ImoImage&) = default;
    ImoImage& operator= (const ImoImage&) = default;
    ImoImage(ImoImage&&) = delete;
    ImoImage& operator= (ImoImage&&) = delete;

    //accessors
    SpImage get_image() { return m_image; }
    inline unsigned char* get_buffer() { return m_image->get_buffer(); }
    inline LUnits get_image_width() { return m_image->get_image_width(); }
    inline LUnits get_image_height() { return m_image->get_image_height(); }
    inline USize& get_image_size() { return m_image->get_image_size(); }
    inline Pixels get_bitmap_width() { return m_image->get_bitmap_width(); }
    inline Pixels get_bitmap_height() { return m_image->get_bitmap_height(); }
    inline VSize& get_bitmap_size() { return m_image->get_bitmap_size(); }
    inline int get_stride() { return m_image->get_stride(); }
    inline int get_format() { return m_image->get_format(); }

    inline int get_bits_per_pixel() { return m_image->get_bits_per_pixel(); }
    inline bool has_alpha() { return m_image->has_alpha(); }

protected:
    inline void load(unsigned char* imgbuf, VSize bmpSize, EPixelFormat format,
                     USize imgSize)
    {
        m_image->load(imgbuf, bmpSize, format, imgSize);
    }

};

//---------------------------------------------------------------------------------------
/** %ImoInstrGroup is a terminal node containing info about a group of instruments,
    such as:
    - the instrument indexes for the instruments that form the group.
    - the name and abbreviation for the group.
    - applicable styles for name & abbreviation
    - the type of brace/bracket to display the group

    It is a child of ImoInstrGroups.
*/
class ImoInstrGroup : public ImoSimpleObj
{
protected:
    int m_joinBarlines ;                //value from enum EJoinBarlines
    int m_symbol;                       //value from enum EGroupSymbol
    TypeTextInfo m_name;
    TypeTextInfo m_abbrev;
    int m_numInstrs = 0;                //number of instruments in the group
    int m_iFirstInstr = -1;             //index to first instrument
    ImoId m_nameStyle = k_no_imoid;
    ImoId m_abbrevStyle = k_no_imoid;

    friend class ImFactory;
    ImoInstrGroup();

public:
    //the five special
    ~ImoInstrGroup() override {}
    ImoInstrGroup(const ImoInstrGroup&) = default;
    ImoInstrGroup& operator= (const ImoInstrGroup&) = default;
    ImoInstrGroup(ImoInstrGroup&&) = delete;
    ImoInstrGroup& operator= (ImoInstrGroup&&) = delete;

    //getters
    inline int join_barlines() { return m_joinBarlines; }
    inline int get_symbol() { return m_symbol; }
    inline const std::string& get_name_string() { return m_name.text; }
    inline const std::string& get_abbrev_string() { return m_abbrev.text; }
    inline TypeTextInfo& get_name() { return m_name; }
    inline TypeTextInfo& get_abbrev() { return m_abbrev; }
    inline ImoStyle* get_name_style() { return get_style_imo(m_nameStyle); }
    inline ImoStyle* get_abbrev_style() { return get_style_imo(m_abbrevStyle); }

    //setters
    void set_name(TypeTextInfo& text);
    void set_abbrev(TypeTextInfo& text);
    void set_name(const std::string& value);
    void set_abbrev(const std::string& value);
    inline void set_symbol(int symbol) { m_symbol = symbol; }
    inline void set_join_barlines(int value) { m_joinBarlines = value; }
    void set_name_style(ImoStyle* style);
    void set_abbrev_style(ImoStyle* style);

    //instruments
    ImoInstrument* get_first_instrument();
    ImoInstrument* get_last_instrument();
    inline int get_index_to_first_instrument() { return m_iFirstInstr; }
    inline int get_index_to_last_instrument() { return m_iFirstInstr + m_numInstrs - 1; }
    void set_range(int iFirstInstr, int iLastInstr);
    ImoInstrument* get_instrument(int iInstr);   //0..n-1
    inline int get_num_instruments() { return m_numInstrs; }
    bool contains_instrument(ImoInstrument* pInstr);

    //info
    ImoScore* get_score();
    inline bool has_name() { return m_name.text != ""; }
    inline bool has_abbrev() { return m_abbrev.text != ""; }

protected:

    friend class PartGroups;
    friend class MnxPartGroups;
    friend class Linker;
    void add_instrument(int iInstr);

    ImoStyle* get_style_imo(ImoId id);
};

//---------------------------------------------------------------------------------------
/** %ImoInstrument represents the musical content and attributes for an score part
    ('instrument', in lomse terminology).

    Layout options are modeled by specific member variables. Perhaps, in future, if
    there are many options, they could be modeled by an ImoOptions child
*/
class ImoInstrument : public ImoContainerObj
{
protected:
    TypeTextInfo m_name;
    TypeTextInfo m_abbrev;
    ImoId m_nameStyle = k_no_imoid;
    ImoId m_abbrevStyle = k_no_imoid;
    std::string m_partId;
    std::list<ImoStaffInfo*> m_staves;

    //layout options
    int     m_barlineLayout;        //from enum EBarlineLayout
    int     m_measuresNumbering;    //from enum EMeasuresNumbering

    //measures info
    ImMeasuresTable*  m_pMeasures;
    TypeMeasureInfo*  m_pLastMeasureInfo;   //for last measure if not closed or the score
                                            //has no metric. Otherwise it will be nullptr.

    friend class ImFactory;
    ImoInstrument();
    void initialize_object() override;

public:
    //the five special
    ~ImoInstrument() override;
    ImoInstrument(const ImoInstrument& a) : ImoContainerObj(a) { clone(a); }
    ImoInstrument& operator= (const ImoInstrument& a) { clone(a); return *this; }
    ImoInstrument(ImoInstrument&&) = delete;
    ImoInstrument& operator= (ImoInstrument&&) = delete;

    //special node
    void accept_visitor(BaseVisitor& v) override;
    bool has_visitable_children() override;

    //methods for accessing ImoSounds child
    LOMSE_DECLARE_IMOSOUNDS_INTERFACE

    //getters
    inline int get_num_staves() const { return static_cast<int>(m_staves.size()); }
    inline TypeTextInfo& get_name() { return m_name; }
    inline TypeTextInfo& get_abbrev() { return m_abbrev; }
    ImoMusicData* get_musicdata();
    ImoStaffInfo* get_staff(int iStaff);
    LUnits get_line_spacing_for_staff(int iStaff);
    LUnits get_line_thickness_for_staff(int iStaff);
    double get_notation_scaling_for_staff(int iStaff);
    inline const std::string& get_instr_id() const { return m_partId; }
    inline ImMeasuresTable* get_measures_table() const { return m_pMeasures; }
    inline TypeMeasureInfo* get_last_measure_info() { return m_pLastMeasureInfo; }
    inline ImoStyle* get_name_style() { return get_style_imo(m_nameStyle); }
    inline ImoStyle* get_abbrev_style() { return get_style_imo(m_abbrevStyle); }

    //setters
    ImoStaffInfo* add_staff();
    void set_name(TypeTextInfo& text);
    void set_abbrev(TypeTextInfo& text);
    void set_name(const std::string& value);
    void set_abbrev(const std::string& value);
    void replace_staff_info(ImoStaffInfo* pInfo);
    inline void set_instr_id(const std::string& id) { m_partId = id; }
    inline void set_last_measure_info(TypeMeasureInfo* pInfo) { m_pLastMeasureInfo = pInfo; }
    void set_name_style(ImoStyle* style);
    void set_abbrev_style(ImoStyle* style);

    //to change default settings at creation time (MusicXML importer)
    void set_staff_margin(int iStaff, LUnits distance);
    void mark_staff_margin_as_imported(int iStaff);

        //layout options

    /// Valid values for defining the policy to layout barlines.
    enum EBarlineLayout
    {
        k_isolated=0,       ///< Independent barlines for each score part
        k_joined,           ///< Barlines joined across all parts
        k_mensurstrich,     ///< Barlines only in the gaps between parts, but not on
                            ///< the staves of each part
        k_nothing,          ///< Do not display barlines (???)
    };

    /// Valid values for measure numbering global policy.
    enum EMeasuresNumbering
    {
        k_undefined=0,  ///< Measure numbering global policy not defined
        k_none,         ///< Measure numbers are never displayed
        k_system,       ///< Display measure numbers only at start of each system
        k_all,          ///< Display measure numbers in all measures
    };

    /** Barlines layout describes the policy for laying out barlines.
    */
    inline int get_barline_layout() const { return m_barlineLayout; }
    inline void set_barline_layout(int value) { m_barlineLayout = value; }

    /** Measures numbering describes the policy for displaying numbers in measures
        for this instrument. The value set in the policy can be overridden at each
        measure.
    */
    inline int get_measures_numbering() const { return m_measuresNumbering; }
    inline void set_measures_numbering(int value) { m_measuresNumbering = value; }


    //info
    inline bool has_name() { return m_name.text != ""; }
    inline bool has_abbrev() { return m_abbrev.text != ""; }
    LUnits tenths_to_logical(Tenths value, int iStaff=0);
    ImoScore* get_score();

    //direct creation API
    ImoBarline* add_barline(int type, bool fVisible=true);
    ImoClef* add_clef(int type, int nStaff=1, bool fVisible=true);
    ImoKeySignature* add_key_signature(int type, bool fVisible=true);
    ImoTimeSignature* add_time_signature(int top, int bottom, bool fVisible=true);
    ImoDirection* add_spacer(Tenths space);
    ImoObj* add_object(const std::string& ldpsource);
    void add_staff_objects(const std::string& ldpsource);

    //score edition API
    void delete_staffobj(ImoStaffObj* pImo);
    void insert_staffobj(ImoStaffObj* pPos, ImoStaffObj* pImo);
    void insert_staffobj_after(ImoStaffObj* pPos, ImoStaffObj* pImo);
    ImoStaffObj* insert_staffobj_at(ImoStaffObj* pAt, ImoStaffObj* pImo);
    ImoStaffObj* insert_staffobj_at(ImoStaffObj* pAt, const std::string& ldpsource,
                                    ostream& reporter=cout);
    list<ImoStaffObj*> insert_staff_objects_at(ImoStaffObj* pAt, ImoMusicData* pObjects);
    list<ImoStaffObj*> insert_staff_objects_at(ImoStaffObj* pAt, const std::string& ldpsource,
            ostream& reporter);

protected:
    ImoInstrument& clone(const ImoInstrument& a);

    friend class LdpAnalyser;
    friend class MxlAnalyser;
    friend class MnxAnalyser;
    friend class InstrumentAnalyser;

    //FIX: For lyrics space
    void reserve_space_for_lyrics(int iStaff, LUnits space);

    friend class MeasuresTableBuilder;
    void set_measures_table(ImMeasuresTable* pTable);

    ImoStyle* get_style_imo(ImoId id);

};

//---------------------------------------------------------------------------------------
class ImoInstruments : public ImoCollection
{
protected:
    friend class ImFactory;
    ImoInstruments() : ImoCollection(k_imo_instruments) {}

public:
    //the five special
    ~ImoInstruments() override {}
    ImoInstruments(const ImoInstruments&) = default;
    ImoInstruments& operator= (const ImoInstruments&) = default;
    ImoInstruments(ImoInstruments&&) = delete;
    ImoInstruments& operator= (ImoInstruments&&) = delete;

};

//---------------------------------------------------------------------------------------
class ImoInstrGroups : public ImoCollection
{
protected:
    friend class ImFactory;
    ImoInstrGroups() : ImoCollection(k_imo_instrument_groups) {}

public:
    //the five special
    ~ImoInstrGroups() override {}
    ImoInstrGroups(const ImoInstrGroups&) = default;
    ImoInstrGroups& operator= (const ImoInstrGroups&) = default;
    ImoInstrGroups(ImoInstrGroups&&) = delete;
    ImoInstrGroups& operator= (ImoInstrGroups&&) = delete;

};

//---------------------------------------------------------------------------------------
class ImoScoreTitles : public ImoCollection
{
protected:
    friend class ImFactory;
    ImoScoreTitles() : ImoCollection(k_imo_score_titles) {}

public:
    //the five special
    ~ImoScoreTitles() override {}
    ImoScoreTitles(const ImoScoreTitles&) = default;
    ImoScoreTitles& operator= (const ImoScoreTitles&) = default;
    ImoScoreTitles(ImoScoreTitles&&) = delete;
    ImoScoreTitles& operator= (ImoScoreTitles&&) = delete;
};

//---------------------------------------------------------------------------------------
class ImoParameters : public ImoCollection
{
protected:
    friend class ImFactory;
    ImoParameters() : ImoCollection(k_imo_parameters) {}

public:
    //the five special
    ~ImoParameters() override {}
    ImoParameters(const ImoParameters&) = default;
    ImoParameters& operator= (const ImoParameters&) = default;
    ImoParameters(ImoParameters&&) = delete;
    ImoParameters& operator= (ImoParameters&&) = delete;
};

//---------------------------------------------------------------------------------------
class ImoSounds : public ImoCollection
{
protected:
    friend class ImFactory;
    ImoSounds() : ImoCollection(k_imo_sounds) {}

public:
    //the five special
    ~ImoSounds() override {}
    ImoSounds(const ImoSounds&) = default;
    ImoSounds& operator= (const ImoSounds&) = default;
    ImoSounds(ImoSounds&&) = delete;
    ImoSounds& operator= (ImoSounds&&) = delete;

    void add_sound_info(ImoSoundInfo* pInfo);
    int get_num_sounds();
    ImoSoundInfo* get_sound_info(const std::string& soundId);
    ImoSoundInfo* get_sound_info(int iSound);

};

//---------------------------------------------------------------------------------------
/** Helper struct with all the information for a key signature accidental
*/
struct KeyAccidental
{
    int step;           //as step for notes pitch
    float alter;        //as alter for notes pitch
    int accidental;     //symbol to use, from EAccidentals, for disambiguating microtonal accidentals
    //bool cancel,      //default = no
    //smufl,            //SMuFl symbol to use

    KeyAccidental()
    {
        initialize();
    }

    KeyAccidental(int s, float alt, int acc)
        : step(s)
        , alter(alt)
        , accidental(acc)
    {
    }

    void initialize()
    {
        step = k_step_undefined;
        alter = 0.0f;
        accidental = k_no_accidentals;
    }
};

//---------------------------------------------------------------------------------------
class ImoKeySignature : public ImoStaffObj
{
protected:

    bool m_fStandard = true;        //true for standard key signatures
    bool m_fForAllStaves = true;    //true if it is common for all staves

        //Variables only valid for standard key signatures (is_standard() == true)

    //Standard key signatures are represented by the number of flats and sharps, plus an
    //optional mode for major/minor/other distinctions.
    int m_fifths = 0;                   //flats or sharps number. Negative: flats, positive: sharps
    int m_keyMode = k_key_mode_none;    //a value from enum EKeyMode
    bool m_fCancel = false;             //true to cancel any previous key before this one appears
    //int m_cancelLocation = (left | right | before-barline)


        //Variables only valid for non-standard key signatures (is_standard() == false)

    //Non-traditional key signatures are represented using a vector of altered tones.
	KeyAccidental m_accidentals[7];


        //Common variables, valid for all key signatures, standard and non-standard

    //octave for each accidental, from left to right, to place the accidental in
    //non-standard positions. Value -1 means 'not defined', that is, use standard
    //positions for standard keys; for non-standard keys the position is not defined:
    //do your best
	int m_octave[7] = {-1,-1,-1,-1,-1,-1,-1};



    friend class ImFactory;
    ImoKeySignature() : ImoStaffObj(k_imo_key_signature) {}


public:
    //the five special
    ~ImoKeySignature() override {}
    ImoKeySignature(const ImoKeySignature&) = default;
    ImoKeySignature& operator= (const ImoKeySignature&) = default;
    ImoKeySignature(ImoKeySignature&&) = delete;
    ImoKeySignature& operator= (ImoKeySignature&&) = delete;

    //building
    void set_standard_key(int fifths, bool fMajor);
    void set_non_standard_key(const KeyAccidental (&acc)[7]);
    void set_key_type(int type);    //TODO: remove. Still used in some methods
    inline void set_octave(int i, int value, bool UNUSED(fCancel)) { m_octave[i] = value; }
    void set_staff(int staff) override;
    void set_staff_common_for_all(int staff);


    //information and getters
    int get_key_type() const;
    inline int get_fifths() const { return m_fifths; }
    inline int get_mode() const { return m_keyMode; }
    inline bool is_standard() const { return m_fStandard; }
    inline KeyAccidental& get_accidental(int i) { return m_accidentals[i]; }
    bool has_accidentals() const;
    inline int get_octave(int i) const { return m_octave[i]; }
    inline bool is_common_for_all_staves() const { return m_fForAllStaves; }


    //operations
    void transpose(const int semitones);

    //properties
    bool can_generate_secondary_shapes() override { return true; }

protected:

};

//---------------------------------------------------------------------------------------
class ImoLine : public ImoAuxObj
{
protected:
    TypeLineStyle m_style;

    friend class ImFactory;
    ImoLine() : ImoAuxObj(k_imo_line) {}

public:
    virtual ~ImoLine() {}

    inline TypeLineStyle& get_line_info() { return m_style; }
    inline void set_line_style(ImoLineStyleDto* pStyle) { m_style = pStyle->get_data(); }

};

//---------------------------------------------------------------------------------------
class ImoListItem : public ImoBlocksContainer
{
protected:
    friend class Document;
    friend class ImFactory;
    ImoListItem();
    void initialize_object() override;

public:
    //the five special
    ~ImoListItem() override {}
    ImoListItem(const ImoListItem&) = default;
    ImoListItem& operator= (const ImoListItem&) = default;
    ImoListItem(ImoListItem&&) = delete;
    ImoListItem& operator= (ImoListItem&&) = delete;

};

//---------------------------------------------------------------------------------------
class ImoList : public ImoBlocksContainer
{
protected:
    int m_listType;

    friend class ImFactory;
    ImoList();
    void initialize_object() override;

public:
    //the five special
    ~ImoList() override {}
    ImoList(const ImoList&) = default;
    ImoList& operator= (const ImoList&) = default;
    ImoList(ImoList&&) = delete;
    ImoList& operator= (ImoList&&) = delete;

    enum { k_itemized=0, k_ordered, };

    inline void set_list_type(int type) { m_listType = type; }
    inline int get_list_type() { return m_listType; }

    //API
    ImoListItem* add_listitem(ImoStyle* pStyle=nullptr);

    //helper, to access content
    inline ImoListItem* get_list_item(int iItem)     //iItem = 0..n-1
    {
        return dynamic_cast<ImoListItem*>( get_content_item(iItem) );
    }

};

//---------------------------------------------------------------------------------------
class ImoMetronomeMark : public ImoAuxObj
{
protected:
    int     m_markType;
    int     m_ticksPerMinute;
    int     m_leftNoteType;
    int     m_leftDots;
    int     m_rightNoteType;
    int     m_rightDots;
    bool    m_fParenthesis;

    friend class ImFactory;
    ImoMetronomeMark()
        : ImoAuxObj(k_imo_metronome_mark), m_markType(k_value)
        , m_ticksPerMinute(60), m_leftNoteType(0), m_leftDots(0)
        , m_rightNoteType(0), m_rightDots(0), m_fParenthesis(false)
    {}

public:
    //the five special
    ~ImoMetronomeMark() override {}
    ImoMetronomeMark(const ImoMetronomeMark&) = default;
    ImoMetronomeMark& operator= (const ImoMetronomeMark&) = default;
    ImoMetronomeMark(ImoMetronomeMark&&) = delete;
    ImoMetronomeMark& operator= (ImoMetronomeMark&&) = delete;

    enum { k_note_value=0, k_note_note, k_value, };

    //getters
    inline int get_left_note_type()
    {
        return m_leftNoteType;
    }
    inline int get_right_note_type()
    {
        return m_rightNoteType;
    }
    inline int get_left_dots()
    {
        return m_leftDots;
    }
    inline int get_right_dots()
    {
        return m_rightDots;
    }
    inline int get_ticks_per_minute()
    {
        return m_ticksPerMinute;
    }
    inline int get_mark_type()
    {
        return m_markType;
    }
    inline bool has_parenthesis()
    {
        return m_fParenthesis;
    }

    //setters
    inline void set_left_note_type(int noteType)
    {
        m_leftNoteType = noteType;
    }
    inline void set_right_note_type(int noteType)
    {
        m_rightNoteType = noteType;
    }
    inline void set_left_dots(int dots)
    {
        m_leftDots = dots;
    }
    inline void set_right_dots(int dots)
    {
        m_rightDots = dots;
    }
    inline void set_ticks_per_minute(int ticks)
    {
        m_ticksPerMinute = ticks;
    }
    inline void set_mark_type(int type)
    {
        m_markType = type;
    }
    inline void set_parenthesis(bool fValue)
    {
        m_fParenthesis = fValue;
    }
};

//---------------------------------------------------------------------------------------
class ImoMultiColumn : public ImoBlocksContainer
{
protected:
    std::vector<float> m_widths;

    friend class ImFactory;
    ImoMultiColumn();
    void initialize_object() override;

public:
    //the five special
    ~ImoMultiColumn() override {}
    ImoMultiColumn(const ImoMultiColumn&) = default;
    ImoMultiColumn& operator= (const ImoMultiColumn&) = default;
    ImoMultiColumn(ImoMultiColumn&&) = delete;
    ImoMultiColumn& operator= (ImoMultiColumn&&) = delete;

    //contents
    inline int get_num_columns() { return get_num_content_items(); }
    inline ImoContent* get_column(int iCol)     //iCol = 0..n-1
    {
        return dynamic_cast<ImoContent*>( get_content_item(iCol) );
    }

    void set_column_width(int iCol, float percentage);
    float get_column_width(int iCol);

protected:
    friend class ImoBlocksContainer;
    void create_columns(int numCols);

};

//---------------------------------------------------------------------------------------
class ImoMusicData : public ImoCollection
{
protected:
    friend class ImFactory;
    ImoMusicData() : ImoCollection(k_imo_music_data) {}

public:
    //the five special
    ~ImoMusicData() override {}
    ImoMusicData(const ImoMusicData&) = default;
    ImoMusicData& operator= (const ImoMusicData&) = default;
    ImoMusicData(ImoMusicData&&) = delete;
    ImoMusicData& operator= (ImoMusicData&&) = delete;

    ImoInstrument* get_instrument();
};

//---------------------------------------------------------------------------------------
class ImoOptionInfo : public ImoSimpleObj
{
protected:
    int         m_type = k_boolean;
    std::string m_name;
    std::string m_sValue;
    bool        m_fValue = false;
    long        m_nValue = 0L;
    float       m_rValue = 0.0f;

    friend class ImFactory;
    ImoOptionInfo() : ImoSimpleObj(k_imo_option) {}

public:
    //the five special
    ~ImoOptionInfo() override {}
    ImoOptionInfo(const ImoOptionInfo&) = default;
    ImoOptionInfo& operator= (const ImoOptionInfo&) = default;
    ImoOptionInfo(ImoOptionInfo&&) = delete;
    ImoOptionInfo& operator= (ImoOptionInfo&&) = delete;

    enum { k_boolean=0, k_number_long, k_number_float, k_string };

    //getters
    inline std::string get_name()
    {
        return m_name;
    }
    inline int get_type()
    {
        return m_type;
    }
    inline bool get_bool_value()
    {
        return m_fValue;
    }
    inline long get_long_value()
    {
        return m_nValue;
    }
    inline float get_float_value()
    {
        return m_rValue;
    }
    inline std::string& get_string_value()
    {
        return m_sValue;
    }
    inline bool is_bool_option()
    {
        return m_type == k_boolean;
    }
    inline bool is_long_option()
    {
        return m_type == k_number_long;
    }
    inline bool is_float_option()
    {
        return m_type == k_number_float;
    }

    //setters
    inline void set_name(const std::string& name)
    {
        m_name = name;
    }
    inline void set_type(int type)
    {
        m_type = type;
    }
    inline void set_bool_value(bool value)
    {
        m_fValue = value;
        m_type = k_boolean;
    }
    inline void set_long_value(long value)
    {
        m_nValue = value;
        m_type = k_number_long;
    }
    inline void set_float_value(float value)
    {
        m_rValue = value;
        m_type = k_number_float;
    }
    inline void set_string_value(const std::string& value)
    {
        m_sValue = value;
    }

};

//---------------------------------------------------------------------------------------
class ImoOptions : public ImoCollection
{
protected:
    friend class ImFactory;
    ImoOptions() : ImoCollection(k_imo_options) {}

public:
    //the five special
    ~ImoOptions() override {}
    ImoOptions(const ImoOptions&) = default;
    ImoOptions& operator= (const ImoOptions&) = default;
    ImoOptions(ImoOptions&&) = delete;
    ImoOptions& operator= (ImoOptions&&) = delete;

};

//---------------------------------------------------------------------------------------
class ImoParagraph : public ImoInlinesContainer
{
protected:
    friend class ImoBlocksContainer;
    friend class Document;
    friend class ImFactory;
    ImoParagraph() : ImoInlinesContainer(k_imo_para) { set_edit_terminal(true); }

public:
    //the five special
    ~ImoParagraph() override {}
    ImoParagraph(const ImoParagraph&) = default;
    ImoParagraph& operator= (const ImoParagraph&) = default;
    ImoParagraph(ImoParagraph&&) = delete;
    ImoParagraph& operator= (ImoParagraph&&) = delete;

    //required by Visitable parent class
    void accept_visitor(BaseVisitor& v) override;
};

//---------------------------------------------------------------------------------------
class ImoParamInfo : public ImoSimpleObj
{
protected:
    std::string m_name;
    std::string m_value;

    friend class ImFactory;
    ImoParamInfo() : ImoSimpleObj(k_imo_param_info), m_name(), m_value() {}

public:
    //the five special
    ~ImoParamInfo() override {}
    ImoParamInfo(const ImoParamInfo&) = default;
    ImoParamInfo& operator= (const ImoParamInfo&) = default;
    ImoParamInfo(ImoParamInfo&&) = delete;
    ImoParamInfo& operator= (ImoParamInfo&&) = delete;

    //getters
    inline std::string& get_name() { return m_name; }
    inline std::string& get_value() { return m_value; }
    bool get_value_as_int(int* pNumber);

    //setters
    inline void set_name(const std::string& name) { m_name = name; }
    inline void set_value(const std::string& value) { m_value = value; }

};

//---------------------------------------------------------------------------------------
class ImoHeading : public ImoInlinesContainer
{
protected:
    int m_level = 1;

    friend class ImFactory;
    ImoHeading() : ImoInlinesContainer(k_imo_heading) { set_edit_terminal(true); }

public:
    //the five special
    ~ImoHeading() override {}
    ImoHeading(const ImoHeading&) = default;
    ImoHeading& operator= (const ImoHeading&) = default;
    ImoHeading(ImoHeading&&) = delete;
    ImoHeading& operator= (ImoHeading&&) = delete;

    //level
    inline int get_level() { return m_level; }
    inline void set_level(int level) { m_level = level; }

    //required by Visitable parent class
    void accept_visitor(BaseVisitor& v) override;

};

//---------------------------------------------------------------------------------------
class ImoPointDto : public ImoSimpleObj
{
    TPoint m_point;

public:
    ImoPointDto() : ImoSimpleObj(k_imo_point_dto) {}

    inline TPoint get_point()
    {
        return m_point;
    }

    inline void set_x(Tenths x)
    {
        m_point.x = x;
    }
    inline void set_y(Tenths y)
    {
        m_point.y = y;
    }

};

//---------------------------------------------------------------------------------------
// A graphical line
class ImoScoreLine : public ImoAuxObj
{
protected:
    TPoint m_startPoint;
    TPoint m_endPoint;
    TypeLineStyle m_style;

    friend class ImFactory;
    ImoScoreLine()
        : ImoAuxObj(k_imo_score_line)
        , m_startPoint(0.0f, 0.0f)
        , m_endPoint(0.0f, 0.0f)
        , m_style()
    {
        m_style.set_line_style(k_line_solid);
        m_style.set_start_edge(k_edge_normal);
        m_style.set_end_edge(k_edge_normal);
        m_style.set_start_cap(k_cap_none);
        m_style.set_end_cap(k_cap_none);
        m_style.set_color(Color(0,0,0));
        m_style.set_width(1.0f);
    }

public:
    //the five special
    ~ImoScoreLine() override {}
    ImoScoreLine(const ImoScoreLine&) = default;
    ImoScoreLine& operator= (const ImoScoreLine&) = default;
    ImoScoreLine(ImoScoreLine&&) = delete;
    ImoScoreLine& operator= (ImoScoreLine&&) = delete;

    //setters
    inline void set_start_point(TPoint point) { m_startPoint = point; }
    inline void set_end_point(TPoint point) { m_endPoint = point; }
    inline void set_x_start(Tenths value) { m_startPoint.x = value; }
    inline void set_y_start(Tenths value) { m_startPoint.y = value; }
    inline void set_x_end(Tenths value) { m_endPoint.x = value; }
    inline void set_y_end(Tenths value) { m_endPoint.y = value; }
    inline void set_line_style(ELineStyle value) { m_style.set_line_style(value); }
    inline void set_start_edge(ELineEdge value) { m_style.set_start_edge(value); }
    inline void set_end_edge(ELineEdge value) { m_style.set_end_edge(value); }
    inline void set_start_cap(ELineCap value) { m_style.set_start_cap(value); }
    inline void set_end_cap(ELineCap value) { m_style.set_end_cap(value); }
    inline void set_color(Color value) { m_style.set_color(value); }
    inline void set_width(Tenths value) { m_style.set_width(value); }

    //getters
    inline Tenths get_x_start() const { return m_startPoint.x; }
    inline Tenths get_y_start() const { return m_startPoint.y; }
    inline Tenths get_x_end() const { return m_endPoint.x; }
    inline Tenths get_y_end() const { return m_endPoint.y; }
    inline TPoint get_start_point() const { return m_startPoint; }
    inline TPoint get_end_point() const { return m_endPoint; }
    inline Tenths get_line_width() const { return m_style.get_width(); }
    inline Color get_color() const { return m_style.get_color(); }
    inline ELineEdge get_start_edge() const { return m_style.get_start_edge(); }
    inline ELineEdge get_end_edge() const { return m_style.get_end_edge(); }
    inline ELineStyle get_line_style() const { return m_style.get_line_style(); }
    inline ELineCap get_start_cap() const { return m_style.get_start_cap(); }
    inline ELineCap get_end_cap() const { return m_style.get_end_cap(); }
};


#if (LOMSE_ENABLE_THREADS == 1)
//---------------------------------------------------------------------------------------
// ImoScorePlayer: A control for managing score playback
class ImoScorePlayer : public ImoControl
{
protected:
    ImoId m_idScore;                //the score to be managed by the player
    std::string m_playLabel;
    std::string m_stopLabel;

    friend class ImFactory;
    ImoScorePlayer();

    friend class ScorePlayerAnalyser;
    friend class ScorePlayerLmdAnalyser;
    void attach_score(ImoScore* pScore);
    void attach_player(ScorePlayerCtrl* pPlayer);
    void set_metronome_mm(int value);
    void set_play_label(const std::string& value);
    inline void set_stop_label(const std::string& value) { m_stopLabel = value; }

public:
    //the five special
    ~ImoScorePlayer() override {}
    ImoScorePlayer(const ImoScorePlayer&) = default;
    ImoScorePlayer& operator= (const ImoScorePlayer&) = default;
    ImoScorePlayer(ImoScorePlayer&&) = delete;
    ImoScorePlayer& operator= (ImoScorePlayer&&) = delete;

    ImoScore* get_score();
    ScorePlayerCtrl* get_player();
    int get_metronome_mm();
    inline const std::string& get_play_label() { return m_playLabel; }
    inline const std::string& get_stop_label() { return m_stopLabel; }

};
#endif  //LOMSE_ENABLE_THREADS == 1

//---------------------------------------------------------------------------------------
class ImoSizeDto : public ImoSimpleObj
{
    TSize m_size;

public:
    ImoSizeDto() : ImoSimpleObj(k_imo_size_dto) {}

    inline TSize get_size()
    {
        return m_size;
    }

    inline void set_width(Tenths w)
    {
        m_size.width = w;
    }
    inline void set_height(Tenths h)
    {
        m_size.height = h;
    }

};

//---------------------------------------------------------------------------------------
class ImoSystemInfo : public ImoSimpleObj
{
protected:
    bool    m_fFirst = true;            //true=first system, false=other systems
    LUnits   m_leftMargin = 0.0f;
    LUnits   m_rightMargin = 0.0f;
    LUnits   m_systemDistance = 0.0f;
    LUnits   m_topSystemDistance = 0.0f;

    //to control if variables contain inherited/default values or a new value has been set
    long m_modified = 0L;
    enum {
        k_modified_left_margin =    0x00000001,
        k_modified_right_margin =   0x00000002,
        k_modified_distance =       0x00000004,
        k_modified_top_distance =   0x00000008,
    };

    friend class ImFactory;
    friend class ImoScore;
    ImoSystemInfo() : ImoSimpleObj(k_imo_system_info) {}

public:
    //the five special
    ~ImoSystemInfo() override {}
    ImoSystemInfo(const ImoSystemInfo&) = default;
    ImoSystemInfo& operator= (const ImoSystemInfo&) = default;
    ImoSystemInfo(ImoSystemInfo&&) = delete;
    ImoSystemInfo& operator= (ImoSystemInfo&&) = delete;

    //info about values modified (values imported from source file or modified by the user)
    inline bool is_system_layout_modified() { return m_modified != 0L; }
    inline bool is_left_margin_modified() { return (m_modified & k_modified_left_margin) != 0; }
    inline bool is_right_margin_modified() { return (m_modified & k_modified_right_margin) != 0; }
    inline bool is_system_distance_modified() { return (m_modified & k_modified_distance) != 0; }
    inline bool is_top_system_distance_modified() { return (m_modified & k_modified_top_distance) != 0; }

    //getters
    inline bool is_first() { return m_fFirst; }
    inline LUnits get_left_margin() { return m_leftMargin; }
    inline LUnits get_right_margin() { return m_rightMargin; }
    inline LUnits get_system_distance() { return m_systemDistance; }
    inline LUnits get_top_system_distance() { return m_topSystemDistance; }

    //setters
    inline void set_first(bool fValue) { m_fFirst = fValue; }
    inline void set_left_margin(LUnits rValue) { m_leftMargin = rValue; m_modified |= k_modified_left_margin; }
    inline void set_right_margin(LUnits rValue) { m_rightMargin = rValue; m_modified |= k_modified_right_margin; }
    inline void set_system_distance(LUnits rValue) { m_systemDistance = rValue; m_modified |= k_modified_distance; }
    inline void set_top_system_distance(LUnits rValue) { m_topSystemDistance = rValue; m_modified |= k_modified_top_distance; }

protected:
    inline void set_default_system_distance(LUnits rValue) { m_systemDistance = rValue; }
    inline void set_default_top_system_distance(LUnits rValue) { m_topSystemDistance = rValue; }
};

//---------------------------------------------------------------------------------------
class ImoScore : public ImoBlockLevelObj      //ImoBlocksContainer
{
protected:
    int m_version = 0;          //LDP source file version
    int m_sourceFormat = 0;     //original source file format ; value from  enum
    int m_accidentalsModel = k_only_notation_provided;  //how pitch//accidentals are initialized
    ColStaffObjs* m_pColStaffObjs = nullptr;
    SoundEventsTable* m_pMidiTable = nullptr;
    float m_scaling;                        //global scaling tenths -> LUnits
    ImoSystemInfo m_systemInfoFirst;
    ImoSystemInfo m_systemInfoOther;
    std::map<std::string, ImoStyle*> m_nameToStyle;
    int m_numLyricFonts = 1;        //count for fonts "Lyrics" and "Lyric-nn"

    //data saved only when importing MusicXML, to re-export it
    std::map<int, std::string> m_lyricLanguages;    //lyric languages defined in <defaults>
    Tenths m_staffDistance = 0.0f;          //default value for <staff-distance>

    //to control if variables contain inherited/default values or a new value has been set
    long m_modified = 0L;
    enum {
        k_modified_scaling =    0x00000001,     //global scaling has been modified
    };

    friend class ImFactory;
    ImoScore();
    void initialize_object() override;

public:
    //the five special
    ~ImoScore() override;
    ImoScore(const ImoScore& a) : ImoBlockLevelObj(a) { clone(a); }
    ImoScore& operator= (const ImoScore& a)  { clone(a); return *this; }
    ImoScore(ImoScore&&) = delete;
    ImoScore& operator= (ImoScore&&) = delete;

    //to support different models for encoding notes in source files
    enum {
        k_pitch_and_notation_provided = 0,  //both notated and actual acc provided. Nothing to compute (e.g. MusicXML).
        k_only_notation_provided,           //actual acc. has to be computed (e.g. LDP)
        k_pitch_provided_notation_optional, //notated acc. has to be computed when not provided (e.g. MNX).
    };

    //building
    inline void set_version(int version) { m_version = version; }
    void set_staffobjs_table(ColStaffObjs* pColStaffObjs);
    void set_global_scaling(float millimeters, float tenths);
    inline void set_accidentals_model(int value) { m_accidentalsModel = value; }
    inline void set_source_format(int format) { m_sourceFormat = format; }

    enum { k_empty=0, k_ldp, k_musicxml, k_mnx, };

    //getters and info
    std::string get_version_string();
    inline int get_version_major() const { return m_version/100; }
    inline int get_version_minor() const { return m_version % 100; }
    inline int get_version_number() const { return m_version; }
    inline int get_accidentals_model() const { return m_accidentalsModel; }
    inline ColStaffObjs* get_staffobjs_table() const { return m_pColStaffObjs; }
    SoundEventsTable* get_midi_table();
    inline bool was_created_from_musicxml() const { return m_sourceFormat == k_musicxml; }
    inline bool was_created_from_ldp() const { return m_sourceFormat == k_ldp; }
    inline bool was_created_from_mnx() const { return m_sourceFormat == k_mnx; }
    inline bool was_created_from_scratch() const { return m_sourceFormat == k_empty; }

    //scaling
    inline LUnits tenths_to_logical(Tenths value) const { return value * m_scaling; }
    inline Tenths logical_to_tenths(LUnits value) const { return value / m_scaling; }
    inline float get_global_scaling() const { return m_scaling; }
    inline bool is_global_scaling_modified() { return (m_modified & k_modified_scaling) != 0; }


    //required by Visitable parent class
    void accept_visitor(BaseVisitor& v) override;
    bool has_visitable_children() override;

    //instruments
    void add_instrument(ImoInstrument* pInstr, const std::string& partId="");
    ImoInstrument* get_instrument(int iInstr);   //0..n-1
    ImoInstrument* get_instrument(const std::string& partId);
    int get_num_instruments();
    ImoInstruments* get_instruments();
    int get_instr_number_for(ImoInstrument* pInstr);

    //instrument groups
    void add_instruments_group(ImoInstrGroup* pGroup);
    ImoInstrGroups* get_instrument_groups();
    std::list<ImoInstrGroup*> find_groups_containing_instrument(ImoInstrument* pInstr);

    //options
    ImoOptions* get_options();
    bool has_options();
    ImoOptionInfo* get_option(const std::string& name);
    void set_float_option(const std::string& name, float value);
    void set_bool_option(const std::string& name, bool value);
    void set_long_option(const std::string& name, long value);
    void add_or_replace_option(ImoOptionInfo* pOpt);
    bool has_default_value(ImoOptionInfo* pOpt);

    //score layout
    void add_sytem_info(ImoSystemInfo* pSL);
    inline ImoSystemInfo* get_first_system_info() { return &m_systemInfoFirst; }
    inline ImoSystemInfo* get_other_system_info() { return &m_systemInfoOther; }
    bool has_default_values(ImoSystemInfo* pInfo);


    //titles
    ImoScoreTitles* get_titles();
    void add_title(ImoScoreTitle* pTitle);

    //styles
    void add_style(ImoStyle* pStyle);
    void add_required_text_styles();
    ImoStyle* find_style(const std::string& name);
    ImoStyle* get_default_style();
    ImoStyle* get_style_or_default(const std::string& name);
    int get_number_of_lyric_fonts() { return m_numLyricFonts; }

    //MusicXML imported data, but no used. Only for MusicXML export
    void add_lyric_language(int number, const std::string& lang);
    std::string get_lyric_language(int number);
    void save_default_staff_distance(Tenths value) { m_staffDistance = value; }
    Tenths get_default_staff_distance() { return m_staffDistance; }
    bool default_staff_distance_modified() { return m_staffDistance != 0.0f; }


    //low level edition API
    /** Append a new empty instrument (score part) to the score. Returns a pointer to
        the created instrument. */
    ImoInstrument* add_instrument();

    /** When you modify the content of an score it is necessary to update associated
        structures, such as the staffobjs collection. For this it is mandatory to
        invoke this method. Alternatively, you can invoke Document::end_of_changes(),
        that will invoke this method on all scores. */
    void end_of_changes();


protected:
    ImoScore& clone(const ImoScore& a);

    void add_option(ImoOptionInfo* pOpt);
    void delete_text_styles();
    ImoStyle* create_default_style();
    void set_defaults_for_system_info();
    void set_defaults_for_options();

    friend class ScoreLdpGenerator;
    friend class ScoreMxlGenerator;
    inline std::map<std::string, ImoStyle*>& get_styles()
    {
        return m_nameToStyle;
    }

};


//---------------------------------------------------------------------------------------
class ImoSlur : public ImoRelObj
{
protected:
    int     m_slurNum = 0;
    int     m_orientation = k_orientation_default;

    friend class ImFactory;
    ImoSlur() : ImoRelObj(k_imo_slur) {}
    ImoSlur(int num) : ImoRelObj(k_imo_slur), m_slurNum(num) {}

public:
    //the five special
    ~ImoSlur() override = default;
    ImoSlur(const ImoSlur&) = default;
    ImoSlur& operator= (const ImoSlur&) = default;
    ImoSlur(ImoSlur&&) = delete;
    ImoSlur& operator= (ImoSlur&&) = delete;

    //getters
    inline int get_slur_number() { return m_slurNum; }
    inline int get_orientation() { return m_orientation; }
    ImoNote* get_start_note();
    ImoNote* get_end_note();

    //setters
    inline void set_slur_number(int num) { m_slurNum = num; }
    inline void set_orientation(int value) { m_orientation = value; }

    //access to data objects
    ImoBezierInfo* get_start_bezier();
    ImoBezierInfo* get_stop_bezier() ;
    ImoBezierInfo* get_start_bezier_or_create();
    ImoBezierInfo* get_stop_bezier_or_create();

    //required override for ImoRelObj
    void reorganize_after_object_deletion() override;
};

//---------------------------------------------------------------------------------------
/** When not empty (no children) its first child is a ImoBezierInfo node.
*/
class ImoSlurData : public ImoRelDataObj
{
protected:
    bool    m_fStart;
    int     m_slurNum;
    int     m_orientation;

    friend class ImFactory;
    ImoSlurData(ImoSlurDto* pDto);

public:
    //the five special
    ~ImoSlurData() override {}
    ImoSlurData(const ImoSlurData&) = default;
    ImoSlurData& operator= (const ImoSlurData&) = default;
    ImoSlurData(ImoSlurData&&) = delete;
    ImoSlurData& operator= (ImoSlurData&&) = delete;

    //getters
    inline bool is_start() { return m_fStart; }
    inline int get_slur_number() { return m_slurNum; }
    inline int get_orientation() { return m_orientation; }
    ImoBezierInfo* get_bezier();

    //building
    ImoBezierInfo* add_bezier();
};

// raw info about a pending slur
//---------------------------------------------------------------------------------------
class ImoSlurDto : public ImoSimpleObj
{
protected:
    bool m_fStart;
    int m_slurNum;
    int m_orientation;
    ImoNote* m_pNote;
    ImoBezierInfo* m_pBezier;
    Color m_color;
    int m_lineNum;

public:
    ImoSlurDto()
        : ImoSimpleObj(k_imo_slur_dto)
        , m_fStart(true)
        , m_slurNum(0)
        , m_orientation(k_orientation_default)
        , m_pNote(nullptr)
        , m_pBezier(nullptr)
        , m_lineNum(0)
    {
    }
    virtual ~ImoSlurDto();

    //getters
    inline bool is_start()
    {
        return m_fStart;
    }
    inline int get_slur_number()
    {
        return m_slurNum;
    }
    inline int get_orientation()
    {
        return m_orientation;
    }
    inline ImoNote* get_note()
    {
        return m_pNote;
    }
    inline ImoBezierInfo* get_bezier()
    {
        return m_pBezier;
    }
    int get_line_number()
    {
        return m_lineNum;
    }
    inline Color get_color()
    {
        return m_color;
    }

    //setters
    inline void set_start(bool value)
    {
        m_fStart = value;
    }
    inline void set_slur_number(int num)
    {
        m_slurNum = num;
    }
    inline void set_orientation(int value)
    {
        m_orientation = value;
    }
    inline void set_note(ImoNote* pNote)
    {
        m_pNote = pNote;
    }
    inline void set_bezier(ImoBezierInfo* pBezier)
    {
        m_pBezier = pBezier;
    }
    inline void set_color(Color value)
    {
        m_color = value;
    }
    inline void set_line_number(int value)
    {
        m_lineNum = value;
    }

    //required by RelationBuilder
    int get_item_number()
    {
        return get_slur_number();
    }
    bool is_start_of_relation()
    {
        return is_start();
    }
    bool is_end_of_relation()
    {
        return !is_start();
    }
    ImoStaffObj* get_staffobj();
};

//---------------------------------------------------------------------------------------
class ImoStaffInfo : public ImoSimpleObj
{
protected:
    int m_numStaff;
    int m_nNumLines;
    int m_staffType;
    LUnits m_uSpacing;      //between line centers
    LUnits m_uLineThickness;
    LUnits m_uMarging;              //distance from the bottom line of the previous staff
    bool m_fTablature = false;      //This staff is for tablature notation
    double m_notationScaling = 1.0;

    //to control if variables contain default values or a new value has been set
    long m_modified = 0L;
    enum {
        k_modified_size =       0x00000001,
        k_modified_margin =     0x00000002,
    };

    friend class ImFactory;
    friend class ImoInstrument;
    ImoStaffInfo(int numStaff=0, int lines=5, int type=k_staff_regular,
                 LUnits spacing=LOMSE_STAFF_LINE_SPACING,
                 LUnits thickness=LOMSE_STAFF_LINE_THICKNESS,
                 LUnits margin=LOMSE_STAFF_TOP_MARGIN)
        : ImoSimpleObj(k_imo_staff_info)
        , m_numStaff(numStaff)
        , m_nNumLines(lines)
        , m_staffType(type)
        , m_uSpacing(spacing)
        , m_uLineThickness(thickness)
        , m_uMarging(margin)
    {
    }

public:
    //the five special
    ~ImoStaffInfo() override = default;
    ImoStaffInfo(const ImoStaffInfo&) = default;
    ImoStaffInfo& operator= (const ImoStaffInfo&) = default;
    ImoStaffInfo(ImoStaffInfo&&) = delete;
    ImoStaffInfo& operator= (ImoStaffInfo&&) = delete;

    enum { k_staff_ossia=0, k_staff_cue, k_staff_editorial, k_staff_regular,
           k_staff_alternate,
         };

    //checks, for exporting modified data
    bool is_staff_size_modified() const { return m_uSpacing != LOMSE_STAFF_LINE_SPACING; }
    bool is_staff_margin_modified() const { return (m_modified & k_modified_margin) != 0; }
    void mark_staff_margin_as_imported() { m_modified |= k_modified_margin; }

    //staff number
    inline int get_staff_number() const { return m_numStaff; }
    inline void set_staff_number(int num) { m_numStaff = num; }

    //staff type
    //TODO: Lomse accepts this but doesn't use it. Only for MusicXML import/export
    inline int get_staff_type() const { return m_staffType; }
    inline void set_staff_type(int type) { m_staffType = type; }

    //margins
    inline LUnits get_staff_margin() const { return m_uMarging; }
    inline void set_staff_margin(LUnits uSpace) { m_uMarging = uSpace; }

    //spacing and size
    inline LUnits get_line_spacing() const { return m_uSpacing; }
    inline void set_line_spacing(LUnits uSpacing) { m_uSpacing = uSpacing; }
    LUnits get_height() const;
    double get_applied_spacing_factor() const;

    //scaling
    inline double get_notation_scaling() const { return m_notationScaling; }
    inline void set_notation_scaling(double value) { m_notationScaling = value; }

    //lines
    inline LUnits get_line_thickness() const { return m_uLineThickness; }
    inline void set_line_thickness(LUnits uTickness) { m_uLineThickness = uTickness; }
    inline int get_num_lines() const { return m_nNumLines; }
    inline void set_num_lines(int nLines) { m_nNumLines = nLines; }
    bool is_line_visible(int iLine) const;

    //tablature
    inline bool is_for_tablature() const { return m_fTablature; }
    inline void set_tablature(bool value) { m_fTablature = value; }

protected:
    friend class MxlAnalyser;
    friend class LdpAnalyser;
    friend class MnxAnalyser;
    inline void add_space_for_lyrics(LUnits space) { m_uMarging += space; }

};

//---------------------------------------------------------------------------------------
class ImoStyles : public ImoSimpleObj
{
protected:
    std::map<std::string, ImoStyle*> m_nameToStyle;

    friend class ImFactory;
    ImoStyles();
    void initialize_object() override;

public:
    ~ImoStyles() override;
    ImoStyles(const ImoStyles& a) : ImoSimpleObj(a) { clone(a); }
    ImoStyles& operator= (const ImoStyles& a) { clone(a); return *this; }
    ImoStyles(ImoStyles&&) = delete;
    ImoStyles& operator= (ImoStyles&&) = delete;

    //overrides, to traverse this special node
    void accept_visitor(BaseVisitor& v) override;
    bool has_visitable_children() override { return m_nameToStyle.size() > 0; }

    //styles
    void add_style(ImoStyle* pStyle);     //AWARE: style ownership transferred to this
    ImoStyle* find_style(const std::string& name);
    ImoStyle* get_default_style();
    ImoStyle* get_style_or_default(const std::string& name);

protected:
    ImoStyles& clone(const ImoStyles& a);

    void delete_styles();
    void create_default_styles();

    friend class StylesLmdGenerator;
    friend class StylesMnxGenerator;
    friend class StylesMxlGenerator;
    inline std::map<std::string, ImoStyle*>& get_styles_collection()
    {
        return m_nameToStyle;
    }

};

//---------------------------------------------------------------------------------------
class ImoTable : public ImoBlocksContainer
{
protected:
    std::list<ImoId> m_colStyles;

    friend class ImFactory;
    ImoTable() : ImoBlocksContainer(k_imo_table) {}

public:
    //the five special
    ~ImoTable() override {}
    ImoTable(const ImoTable&) = default;
    ImoTable& operator= (const ImoTable&) = default;
    ImoTable(ImoTable&&) = delete;
    ImoTable& operator= (ImoTable&&) = delete;

    //contents
    ImoTableHead* get_head();
    ImoTableBody* get_body();
    std::list<ImoStyle*> get_column_styles();
    inline int get_num_columns() { return int( m_colStyles.size() ); }

protected:
    friend class ImoBlocksContainer;
    friend class TableColumnAnalyser;
    friend class TableColumnLmdAnalyser;
    void add_column_style(ImoStyle* pStyle);

    ImoStyle* get_style_imo(ImoId id);
};

//---------------------------------------------------------------------------------------
class ImoTableCell : public ImoBlocksContainer
{
protected:
    int m_rowspan;
    int m_colspan;

    friend class Document;
    friend class ImFactory;
    ImoTableCell();
    void initialize_object() override;

public:
    //the five special
    ~ImoTableCell() override {}
    ImoTableCell(const ImoTableCell&) = default;
    ImoTableCell& operator= (const ImoTableCell&) = default;
    ImoTableCell(ImoTableCell&&) = delete;
    ImoTableCell& operator= (ImoTableCell&&) = delete;

    //accessors
    inline void set_rowspan(int value) { m_rowspan = value; }
    inline int get_rowspan() { return m_rowspan; }
    inline void set_colspan(int value) { m_colspan = value; }
    inline int get_colspan() { return m_colspan; }

};

//---------------------------------------------------------------------------------------
class ImoTableRow : public ImoBlocksContainer
{
protected:

    friend class ImFactory;
    ImoTableRow();
    void initialize_object() override;

public:
    //the five special
    ~ImoTableRow() override {}
    ImoTableRow(const ImoTableRow&) = default;
    ImoTableRow& operator= (const ImoTableRow&) = default;
    ImoTableRow(ImoTableRow&&) = delete;
    ImoTableRow& operator= (ImoTableRow&&) = delete;

    //contents
    inline int get_num_cells()
    {
        return get_num_content_items();
    }
    inline void add_cell(ImoTableCell* pCell)
    {
        append_content_item(pCell);
    }
    inline ImoTableCell* get_cell(int iItem)     //iItem = 0..n-1
    {
        return dynamic_cast<ImoTableCell*>( get_content_item(iItem) );
    }

    //overrides: as ImoTableRow is stored in an ImoCollection, some general methods
    //doesn't work and must be overrinden
    ImoStyle* get_style(bool fInherit=true) override;

};

//---------------------------------------------------------------------------------------
class ImoTableSection : public ImoCollection
{
protected:
    ImoTableSection(int type) : ImoCollection(type) {}

public:
    //the five special
    ~ImoTableSection() override {}
    ImoTableSection(const ImoTableSection&) = default;
    ImoTableSection& operator= (const ImoTableSection&) = default;
    ImoTableSection(ImoTableSection&&) = delete;
    ImoTableSection& operator= (ImoTableSection&&) = delete;

    void remove(ImoTableRow* pTR) { remove_child_imo(pTR); }
    void add(ImoTableRow* pTR) { append_child_imo(pTR); }
};

//---------------------------------------------------------------------------------------
class ImoTableBody : public ImoTableSection
{
protected:
    friend class ImFactory;
    friend class ImoContentObj;
    ImoTableBody() : ImoTableSection(k_imo_table_body) {}

public:
    //the five special
    ~ImoTableBody() override {}
    ImoTableBody(const ImoTableBody&) = default;
    ImoTableBody& operator= (const ImoTableBody&) = default;
    ImoTableBody(ImoTableBody&&) = delete;
    ImoTableBody& operator= (ImoTableBody&&) = delete;
};

//---------------------------------------------------------------------------------------
class ImoTableHead : public ImoTableSection
{
protected:
    friend class ImFactory;
    friend class ImoContentObj;
    ImoTableHead() : ImoTableSection(k_imo_table_head) {}

public:
    //the five special
    ~ImoTableHead() override {}
    ImoTableHead(const ImoTableHead&) = default;
    ImoTableHead& operator= (const ImoTableHead&) = default;
    ImoTableHead(ImoTableHead&&) = delete;
    ImoTableHead& operator= (ImoTableHead&&) = delete;
};

//---------------------------------------------------------------------------------------
class ImoTextItem : public ImoInlineLevelObj
{
private:
    std::string m_text;
    std::string m_language;

protected:
    friend class ImFactory;
    friend class TextItemAnalyser;
    friend class TextItemLmdAnalyser;

    ImoTextItem() : ImoInlineLevelObj(k_imo_text_item), m_text("") {}

public:
    //the five special
    ~ImoTextItem() override {}
    ImoTextItem(const ImoTextItem&) = default;
    ImoTextItem& operator= (const ImoTextItem&) = default;
    ImoTextItem(ImoTextItem&&) = delete;
    ImoTextItem& operator= (ImoTextItem&&) = delete;

    //getters
    inline std::string& get_text() { return m_text; }
    std::string& get_language();

    //setters
    inline void set_text(const std::string& text) { m_text = text; }
    inline void set_language(const std::string& value) { m_language = value; }

};

//---------------------------------------------------------------------------------------
/** Although a tie relates only two objects, it could be necessary to have information
    associated to each note. For this %ImoTieData stores that specific information for
    each note/rest.

    When ImoTieData node is not empty (no children) its first child is a ImoBezierInfo
    node.
*/
class ImoTieData : public ImoRelDataObj
{
protected:
    bool    m_fStart;
    int     m_tieNum;
    int     m_orientation;

    friend class ImFactory;
    ImoTieData(ImoTieDto* pDto);
    ImoTieData();

public:
    //the five special
    ~ImoTieData() override {}
    ImoTieData(const ImoTieData&) = default;
    ImoTieData& operator= (const ImoTieData&) = default;
    ImoTieData(ImoTieData&&) = delete;
    ImoTieData& operator= (ImoTieData&&) = delete;

    //getters
    inline bool is_start() { return m_fStart; }
    inline int get_tie_number() { return m_tieNum; }
    inline int get_orientation() { return m_orientation; }
    ImoBezierInfo* get_bezier();

    //edition
    ImoBezierInfo* add_bezier();
};

//---------------------------------------------------------------------------------------
class ImoTie : public ImoRelObj
{
protected:
    int     m_tieNum = 0;
    int     m_orientation = k_orientation_default;

    friend class ImFactory;
    ImoTie() : ImoRelObj(k_imo_tie) {}
    ImoTie(int num) : ImoRelObj(k_imo_tie), m_tieNum(num) {}

public:
    //the five special
    ~ImoTie() override = default;
    ImoTie(const ImoTie&) = default;
    ImoTie& operator= (const ImoTie&) = default;
    ImoTie(ImoTie&&) = delete;
    ImoTie& operator= (ImoTie&&) = delete;

    //getters
    inline int get_tie_number() { return m_tieNum; }
    inline int get_orientation() { return m_orientation; }
    ImoNote* get_start_note();
    ImoNote* get_end_note();

    //setters
    inline void set_tie_number(int num) { m_tieNum = num; }
    inline void set_orientation(int value) { m_orientation = value; }

    //access to data objects
    ImoBezierInfo* get_start_bezier();
    ImoBezierInfo* get_stop_bezier() ;
    ImoBezierInfo* get_start_bezier_or_create();
    ImoBezierInfo* get_stop_bezier_or_create();

    //required override for ImoRelObj
    void reorganize_after_object_deletion() override;
};

// raw info about a pending tie
//---------------------------------------------------------------------------------------
class ImoTieDto : public ImoSimpleObj
{
protected:
    bool m_fStart = true;
    int m_tieNum = 0;
    int m_orientation = k_orientation_default;
    ImoNote* m_pNote = nullptr;
    ImoBezierInfo* m_pBezier = nullptr;
    Color m_color = Color(0,0,0);
    int m_lineNum = 0;

public:
    ImoTieDto() : ImoSimpleObj(k_imo_tie_dto) {}
    virtual ~ImoTieDto();

    //getters
    inline bool is_start()
    {
        return m_fStart;
    }
    inline int get_tie_number()
    {
        return m_tieNum;
    }
    inline int get_orientation()
    {
        return m_orientation;
    }
    inline ImoNote* get_note()
    {
        return m_pNote;
    }
    inline ImoBezierInfo* get_bezier()
    {
        return m_pBezier;
    }
    int get_line_number()
    {
        return m_lineNum;
    }
    inline Color get_color()
    {
        return m_color;
    }

    //setters
    inline void set_start(bool value)
    {
        m_fStart = value;
    }
    inline void set_tie_number(int num)
    {
        m_tieNum = num;
    }
    inline void set_orientation(int value)
    {
        m_orientation = value;
    }
    inline void set_note(ImoNote* pNote)
    {
        m_pNote = pNote;
    }
    inline void set_bezier(ImoBezierInfo* pBezier)
    {
        m_pBezier = pBezier;
    }
    inline void set_color(Color value)
    {
        m_color = value;
    }
    inline void set_line_number(int value)
    {
        m_lineNum = value;
    }

    //required by RelationBuilder
    int get_item_number()
    {
        return get_tie_number();
    }
    bool is_start_of_relation()
    {
        return is_start();
    }
    bool is_end_of_relation()
    {
        return !is_start();
    }
    ImoStaffObj* get_staffobj();
};

//---------------------------------------------------------------------------------------
class ImoTimeModificationDto : public ImoDto
{
protected:
    int m_top = 1;
    int m_bottom = 1;

public:
    ImoTimeModificationDto() : ImoDto(k_imo_time_modification_dto) {}

    //getters & setters
    inline int get_top_number()
    {
        return m_top;
    }
    inline void set_top_number(int num)
    {
        m_top = num;
    }
    inline int get_bottom_number()
    {
        return m_bottom;
    }
    inline void set_bottom_number(int num)
    {
        m_bottom = num;
    }
};

//---------------------------------------------------------------------------------------
class ImoTimeSignature : public ImoStaffObj
{
protected:
    int     m_top;
    int     m_bottom;
    int     m_type;

    friend class ImFactory;
    ImoTimeSignature()
        : ImoStaffObj(k_imo_time_signature)
        , m_top(2)
        , m_bottom(4)
        , m_type(ImoTimeSignature::k_normal)
    {
    }

public:
    //the five special
    ~ImoTimeSignature() override {}
    ImoTimeSignature(const ImoTimeSignature&) = default;
    ImoTimeSignature& operator= (const ImoTimeSignature&) = default;
    ImoTimeSignature(ImoTimeSignature&&) = delete;
    ImoTimeSignature& operator= (ImoTimeSignature&&) = delete;

    //getters and setters
    inline int get_top_number()
    {
        return m_top;
    }
    inline void set_top_number(int num)
    {
        m_top = num;
    }
    inline int get_bottom_number()
    {
        return m_bottom;
    }
    inline void set_bottom_number(int num)
    {
        m_bottom = num;
    }
    inline int get_type()
    {
        return m_type;
    }
    inline void set_type(int type)
    {
        m_type = type;
    }

    //overrides: time signatures always in staff 0
    void set_staff(int UNUSED(staff)) override { m_staff = 0; }

    //properties
    bool can_generate_secondary_shapes() override { return true; }

    //other
    bool is_compound_meter();
    inline bool is_normal()
    {
        return m_type == k_normal;
    }
    inline bool is_senza_misura()
    {
        return m_type == k_senza_misura;
    }
    inline bool is_single_number()
    {
        return m_type == k_single_number;
    }
    inline bool is_common()
    {
        return m_type == k_common;
    }
    inline bool is_cut()
    {
        return m_type == k_cut;
    }
    int get_num_pulses();
    TimeUnits get_ref_note_duration();
    TimeUnits get_measure_duration();
    TimeUnits get_beat_duration();

    //time signature type
    enum { k_normal=0, k_common, k_cut, k_single_number, k_senza_misura, };

};

//---------------------------------------------------------------------------------------
// raw info about a tuplet
class ImoTupletDto : public ImoSimpleObj
{
protected:
    int m_tupletType;
    int m_tupletNum;
    int m_nActualNum;
    int m_nNormalNum;
    int m_nShowBracket;
    int m_nPlacement;
    int m_nShowNumber;
    int m_lineNum;
    bool m_fOnlyGraphical;
    ImoNoteRest* m_pNR;

public:
    ImoTupletDto();

    enum { k_unknown = 0, k_start, k_continue, k_stop, };

    //getters
    inline ImoNoteRest* get_note_rest()
    {
        return m_pNR;
    }
    inline bool is_start_of_tuplet()
    {
        return m_tupletType == ImoTupletDto::k_start;
    }
    inline bool is_end_of_tuplet()
    {
        return m_tupletType == ImoTupletDto::k_stop;;
    }
    inline int get_actual_number()
    {
        return m_nActualNum;
    }
    inline int get_normal_number()
    {
        return m_nNormalNum;
    }
    inline int get_show_bracket()
    {
        return m_nShowBracket;
    }
    inline int get_show_number()
    {
        return m_nShowNumber;
    }
    inline int get_placement()
    {
        return m_nPlacement;
    }
    int get_line_number()
    {
        return m_lineNum;
    }
    inline bool is_only_graphical()
    {
        return m_fOnlyGraphical;
    }

    //setters
    inline void set_note_rest(ImoNoteRest* pNR)
    {
        m_pNR = pNR;
    }
    inline void set_tuplet_type(int value)
    {
        m_tupletType = value;
    }
    inline void set_tuplet_number(int value)
    {
        m_tupletNum = value;
    }
    inline void set_actual_number(int value)
    {
        m_nActualNum = value;
    }
    inline void set_normal_number(int value)
    {
        m_nNormalNum = value;
    }
    inline void set_show_bracket(int value)
    {
        m_nShowBracket = value;
    }
    inline void set_show_number(int value)
    {
        m_nShowNumber = value;
    }
    inline void set_placement(int value)
    {
        m_nPlacement = value;
    }
    inline void set_only_graphical(bool value)
    {
        m_fOnlyGraphical = value;
    }
    inline void set_line_number(int value)
    {
        m_lineNum = value;
    }

    //required by RelationBuilder
    int get_item_number()
    {
        return m_tupletNum;
    }
    bool is_start_of_relation()
    {
        return is_start_of_tuplet();
    }
    bool is_end_of_relation()
    {
        return is_end_of_tuplet();
    }
    ImoStaffObj* get_staffobj();
};

//---------------------------------------------------------------------------------------
class ImoTuplet : public ImoRelObj
{
protected:
    int m_nActualNum = 0;
    int m_nNormalNum = 0;
    int m_nShowBracket = k_yesno_default;       //a value from enum EYesNo
    int m_nShowNumber = k_number_actual;        //a value from ImoTuplet enum
    int m_nPlacement = k_placement_default;     //a value from enum EPlacement

    friend class ImFactory;
    ImoTuplet() : ImoRelObj(k_imo_tuplet) {}
    ImoTuplet(ImoTupletDto* dto);

public:
    //the five special
    ~ImoTuplet() override = default;
    ImoTuplet(const ImoTuplet&) = default;
    ImoTuplet& operator= (const ImoTuplet&) = default;
    ImoTuplet(ImoTuplet&&) = delete;
    ImoTuplet& operator= (ImoTuplet&&) = delete;

    enum { k_straight = 0, k_curved, k_slurred, };
    enum { k_number_actual=0, k_number_both, k_number_none, };

    //getters
    inline int get_actual_number() { return m_nActualNum; }
    inline int get_normal_number() { return m_nNormalNum; }
    inline int get_show_bracket() { return m_nShowBracket; }
    inline int get_show_number() { return m_nShowNumber; }
    inline int get_placement() { return m_nPlacement; }

    //setters
    inline void set_actual_number(int value) { m_nActualNum = value; }
    inline void set_normal_number(int value) { m_nNormalNum = value; }
    inline void set_show_bracket(int value) { m_nShowBracket = value; }
    inline void set_show_number(int value) { m_nShowNumber = value; }
    inline void set_placement(int value) { m_nPlacement = value; }

    //required override for ImoRelObj
    void reorganize_after_object_deletion() override;

};

//---------------------------------------------------------------------------------------
// ImoLyric represents the lyrics info for one note
class ImoLyric : public ImoAuxRelObj
{
protected:
    int m_number;
    int m_placement;
    int m_numTextItems;

    bool m_fLaughing;
    bool m_fHumming;
    bool m_fEndLine;
    bool m_fEndParagraph;
    bool m_fMelisma;
    bool m_fHyphenation;

    //children
    // ImoLyricsTextInfo[]

    friend class ImFactory;
    ImoLyric()
        : ImoAuxRelObj(k_imo_lyric)
        , m_number(0)
        , m_placement(k_placement_default)
        , m_numTextItems(0)
        , m_fLaughing(false)
        , m_fHumming(false)
        , m_fEndLine(false)
        , m_fEndParagraph(false)
        , m_fMelisma(false)
        , m_fHyphenation(false)
    {
    }

public:

    //the five special
    ~ImoLyric() override {}
    ImoLyric(const ImoLyric&) = default;
    ImoLyric& operator= (const ImoLyric&) = default;
    ImoLyric(ImoLyric&&) = delete;
    ImoLyric& operator= (ImoLyric&&) = delete;

    //getters
    inline int get_number() { return m_number; }
    inline int get_placement() { return m_placement; }
    inline bool is_laughing() { return m_fLaughing; }
    inline bool is_humming() { return m_fHumming; }
    inline bool is_end_line() { return m_fEndLine; }
    inline bool is_end_paragraph() { return m_fEndParagraph; }
    inline bool has_hyphenation() { return m_fHyphenation; }
    inline ImoLyric* get_prev_lyric() { return static_cast<ImoLyric*>(get_prev_ARO()); }
    inline ImoLyric* get_next_lyric() { return static_cast<ImoLyric*>(get_next_ARO()); }

    //setters
    inline void set_number(int number) { m_number = number; }
    inline void set_placement(int placement) { m_placement = placement; }
    inline void set_laughing(bool value) { m_fLaughing = value; }
    inline void set_humming(bool value) { m_fHumming = value; }
    inline void set_end_line(bool value) { m_fEndLine = value; }
    inline void set_end_paragraph(bool value) { m_fEndParagraph = value; }
    inline void set_melisma(bool value) { m_fMelisma = value; }
    inline void set_hyphenation(bool value) { m_fHyphenation = value; }

    //information
    inline int get_num_text_items() { return m_numTextItems; }
    inline bool has_melisma() { return m_fMelisma; }

    //data
    ImoLyricsTextInfo* get_text_item(int i);

protected:

    friend class LyricMxlAnalyser;
    friend class LyricAnalyser;
    friend class LyricMnxAnalyser;
    friend class LdpAnalyser;
    friend class MxlAnalyser;
    friend class MnxAnalyser;
    void add_text_item(ImoLyricsTextInfo* pText);
    void link_to_next_lyric(ImoLyric* pNext) { link_to_next_ARO(pNext->get_id()); }
    void set_prev_lyric(ImoLyric* pPrev) { set_prev_ARO(pPrev->get_id()); }

};

//---------------------------------------------------------------------------------------
class ImoLyricsTextInfo : public ImoSimpleObj
{
protected:
    int m_syllableType = k_single;
    TypeTextInfo m_text;
    ImoId m_styleId = k_no_imoid;
    std::string m_elision;       //after this syllable
//    std::string m_elisionFont;
//    Color m_elisionColor;

    friend class ImFactory;
    ImoLyricsTextInfo() : ImoSimpleObj(k_imo_lyrics_text_info) {}


public:
    //the five special
    ~ImoLyricsTextInfo() override {}
    ImoLyricsTextInfo(const ImoLyricsTextInfo&) = default;
    ImoLyricsTextInfo& operator= (const ImoLyricsTextInfo&) = default;
    ImoLyricsTextInfo(ImoLyricsTextInfo&&) = delete;
    ImoLyricsTextInfo& operator= (ImoLyricsTextInfo&&) = delete;

    //syllable type
    enum { k_single, k_begin, k_end, k_middle, };

    //getters
    inline int get_syllable_type() { return m_syllableType; }
    inline std::string& get_syllable_text() { return m_text.text; }
    inline std::string& get_syllable_language() { return m_text.language; }
    ImoStyle* get_syllable_style();
    inline bool has_elision() { return !m_elision.empty(); }
    inline std::string& get_elision_text() { return m_elision; }

    //setters
    inline void set_syllable_type(int value) { m_syllableType = value; }
    inline void set_syllable_text(const std::string& text) { m_text.text = text; }
    void set_syllable_style(ImoStyle* pStyle);
    inline void set_syllable_language(const std::string& language) { m_text.language = language; }
    inline void set_elision_text(const std::string& text) { m_elision = text; }

};


//---------------------------------------------------------------------------------------
// Represents an octave-shift line
class ImoOctaveShift : public ImoRelObj
{
protected:
    int     m_steps;
    int     m_octaveShiftNum;

    friend class ImFactory;
    ImoOctaveShift(int num=0)
        : ImoRelObj(k_imo_octave_shift)
        , m_steps(0)
        , m_octaveShiftNum(num)
    {
    }

public:
    //the five special
    ~ImoOctaveShift() override = default;
    ImoOctaveShift(const ImoOctaveShift&) = default;
    ImoOctaveShift& operator= (const ImoOctaveShift&) = default;
    ImoOctaveShift(ImoOctaveShift&&) = delete;
    ImoOctaveShift& operator= (ImoOctaveShift&&) = delete;

    //setters
    inline void set_shift_steps(int value) { m_steps = value; }
    inline void set_octave_shift_number(int value) { m_octaveShiftNum = value; }

    //getters
    inline int get_shift_steps() { return m_steps; }
    inline int get_octave_shift_number() { return m_octaveShiftNum; }

    //required override for ImoRelObj
    void reorganize_after_object_deletion() override;
};

//---------------------------------------------------------------------------------------
// raw info about a pending octave-shift line
class ImoOctaveShiftDto : public ImoSimpleObj
{
protected:
    bool m_fStart;
    int m_steps;
    int m_octaveShiftNum;
    ImoNoteRest* m_pNR;
    Color m_color;
    int m_lineNum;
    int m_iStaff;

public:
    ImoOctaveShiftDto()
        : ImoSimpleObj(k_imo_octave_shift_dto)
        , m_fStart(true)
        , m_steps(0)
        , m_octaveShiftNum(0)
        , m_pNR(nullptr)
        , m_color(Color(0,0,0))
        , m_lineNum(0)
        , m_iStaff(0)
    {
    }

    //setters
    inline void set_line_number(int value) { m_lineNum = value; }
    inline void set_staffobj(ImoNoteRest* pNR) { m_pNR = pNR; }
    inline void set_shift_steps(int value) { m_steps = value; }
    inline void set_start(bool value) { m_fStart = value; }
    inline void set_octave_shift_number(int value) { m_octaveShiftNum = value; }
    inline void set_color(Color value) { m_color = value; }
    inline void set_staff(int value) { m_iStaff = value; }

    //getters
    inline int get_line_number() { return m_lineNum; }
    inline int get_shift_steps() { return m_steps; }
    inline bool is_start() { return m_fStart; }
    inline int get_octave_shift_number() { return m_octaveShiftNum; }
    inline Color get_color() { return m_color; }
    inline int get_staff() { return m_iStaff; }

    //required by RelationBuilder
    inline int get_item_number() { return get_octave_shift_number(); }
    bool is_start_of_relation() { return is_start(); }
    bool is_end_of_relation() { return !is_start(); }
    inline ImoNoteRest* get_staffobj() { return m_pNR; }
};


//---------------------------------------------------------------------------------------
// Represents a pedal mark
class ImoPedalMark : public ImoAuxObj
{
protected:
    EPedalMark m_type = k_pedal_mark_start;
    bool m_fAbbreviated = false;

    friend class ImFactory;
    ImoPedalMark() : ImoAuxObj(k_imo_pedal_mark) {}

public:
    //the five special
    ~ImoPedalMark() override {}
    ImoPedalMark(const ImoPedalMark&) = default;
    ImoPedalMark& operator= (const ImoPedalMark&) = default;
    ImoPedalMark(ImoPedalMark&&) = delete;
    ImoPedalMark& operator= (ImoPedalMark&&) = delete;

    //setters
    void set_type(EPedalMark value) { m_type = value; }
    void set_abbreviated(bool value) { m_fAbbreviated = value; }

    //getters
    EPedalMark get_type() const { return m_type; }
    bool is_abbreviated() const { return m_fAbbreviated; }
    EPlacement get_placement();

    bool is_start_type() const { return m_type == k_pedal_mark_start || m_type == k_pedal_mark_sostenuto_start; }
    bool is_end_type() const { return m_type == k_pedal_mark_stop; }
};

//---------------------------------------------------------------------------------------
// Represents a pedal line
class ImoPedalLine : public ImoRelObj
{
    bool m_fDrawStartCorner = true;
    bool m_fDrawEndCorner = true;
    bool m_fDrawContinuationText = false;
    bool m_fSostenuto = false;

protected:
    friend class ImFactory;
    ImoPedalLine() : ImoRelObj(k_imo_pedal_line) {}

public:
    //the five special
    ~ImoPedalLine() override = default;
    ImoPedalLine(const ImoPedalLine&) = default;
    ImoPedalLine& operator= (const ImoPedalLine&) = default;
    ImoPedalLine(ImoPedalLine&&) = delete;
    ImoPedalLine& operator= (ImoPedalLine&&) = delete;

    //setters
    void set_draw_start_corner(bool value) { m_fDrawStartCorner = value; }
    void set_draw_end_corner(bool value) { m_fDrawEndCorner = value; }
    void set_draw_continuation_text(bool value) { m_fDrawContinuationText = value; }
    void set_sostenuto(bool value) { m_fSostenuto = value; }

    //getters
    bool get_draw_start_corner() const { return m_fDrawStartCorner; }
    bool get_draw_end_corner() const { return m_fDrawEndCorner; }
    bool get_draw_continuation_text() const { return m_fDrawContinuationText; }
    bool is_sostenuto() const { return m_fSostenuto; }

    //required override for ImoRelObj
    void reorganize_after_object_deletion() override;
};

//---------------------------------------------------------------------------------------
// raw info about a pending pedal line
class ImoPedalLineDto : public ImoSimpleObj
{
protected:
    int m_pedalNum = 0;
    ImoDirection* m_pDirection = nullptr;
    Color m_color = Color(0,0,0);
    int m_lineNum = 0;
    bool m_fDrawCorner = true;
    bool m_fDrawContinuationText = false;
    bool m_fSostenuto = false;
    bool m_fStart = true;
    bool m_fEnd = false;

public:
    ImoPedalLineDto() : ImoSimpleObj(k_imo_pedal_line_dto) {}

    //setters
    void set_line_number(int value) { m_lineNum = value; }
    void set_staffobj(ImoDirection* pDirection) { m_pDirection = pDirection; }
    void set_pedal_number(int value) { m_pedalNum = value; }
    void set_color(Color value) { m_color = value; }
    void set_draw_corner(bool value) { m_fDrawCorner = value; }
    void set_draw_continuation_text(bool value) { m_fDrawContinuationText = value; }
    void set_sostenuto(bool value) { m_fSostenuto = value; }
    void set_start(bool value) { m_fStart = value; }
    void set_end(bool value) { m_fEnd = value; }

    //getters
    int get_line_number() const { return m_lineNum; }
    int get_pedal_number() const { return m_pedalNum; }
    Color get_color() const { return m_color; }
    bool get_draw_corner() const { return m_fDrawCorner; }
    bool get_draw_continuation_text() const { return m_fDrawContinuationText; }
    bool is_sostenuto() const { return m_fSostenuto; }
    bool is_start() const { return m_fStart; }
    bool is_end() const { return m_fEnd; }

    //required by RelationBuilder
    int get_item_number() const { return m_pedalNum; }
    bool is_start_of_relation() const { return m_fStart; }
    bool is_end_of_relation() const { return m_fEnd; }
    ImoDirection* get_staffobj() const { return m_pDirection; }
};


//---------------------------------------------------------------------------------------
// AWARE: ImoPlayInfo object is not required until playback techniques are implemented.
////ImoPlayInfo specifies the playback technique to use.
//// - when attached to ImoSoundInfo it applies to all notes after the ImoSoundInfo.
//// - When attached to ImoNote, it applies only to that note.
//
//class ImoPlayInfo : public ImoSimpleObj
//{
//protected:
//    EMuteType m_mute = k_mute_off;
//
//    friend class ImFactory;
//    ImoPlayInfo() : ImoSimpleObj(k_imo_play_info) {}
//
//public:
//    ImoPlayInfo(const ImoPlayInfo&) = delete;
//    ImoPlayInfo& operator= (const ImoPlayInfo&) = delete;
//    ImoPlayInfo(ImoPlayInfo&&) = delete;
//    ImoPlayInfo& operator= (ImoPlayInfo&&) = delete;
//
//    //setters
//
//    //getters
//};


//---------------------------------------------------------------------------------------
class ImoVoltaBracket : public ImoRelObj
{
protected:
    bool    m_fStopJog;
    //False when there is no downward jog, as is typical for
    //second endings that do not conclude a piece.
    std::string  m_voltaNum;
    //The numeric values of the repetitions for the measure associated to this volta.
    //Single values such as "1" or comma-separated multiple endings such as "1, 2"
    //may be used.
    std::string  m_voltaText;
    //If not empty, this text is used to be displayed in the volta bracket instead
    //the volta numbers, i.e.: "First time" instead of "1".
    vector<int> m_repetitions;      //repetition numbers extracted from m_voltaNum

    //data valid only in first volta of each set of voltas for a repetition
    int m_numVoltas;                //number of voltas in the set

    friend class ImFactory;
    ImoVoltaBracket()
        : ImoRelObj(k_imo_volta_bracket)
        , m_fStopJog(true)
        , m_voltaNum()
        , m_voltaText()
        , m_numVoltas(1)
    {
    }

public:
    //the five special
    ~ImoVoltaBracket() override = default;
    ImoVoltaBracket(const ImoVoltaBracket&) = default;
    ImoVoltaBracket& operator= (const ImoVoltaBracket&) = default;
    ImoVoltaBracket(ImoVoltaBracket&&) = delete;
    ImoVoltaBracket& operator= (ImoVoltaBracket&&) = delete;

    //getters
    inline bool has_final_jog()
    {
        return m_fStopJog;
    }
    inline std::string& get_volta_number()
    {
        return m_voltaNum;
    }
    inline std::string& get_volta_text()
    {
        return m_voltaText;
    }
    ImoBarline* get_start_barline();
    ImoBarline* get_stop_barline();

    //setters
    inline void set_final_jog(bool value)
    {
        m_fStopJog = value;
    }
    inline void set_volta_number(const std::string& num)
    {
        m_voltaNum = num;
    }
    inline void set_volta_text(const std::string& text)
    {
        m_voltaText = text;
    }
    inline void increment_total_voltas()
    {
        ++m_numVoltas;
    }

    //info
    inline bool is_first_repeat()
    {
        return m_repetitions[0] == 1;
    }
    inline int get_number_of_repetitions()
    {
        return int(m_repetitions.size());
    }
    inline void set_repetitions(vector<int>& repetitions)
    {
        m_repetitions = repetitions;
    }
    inline int get_total_voltas()
    {
        return m_numVoltas;
    }


    //required override for ImoRelObj
    void reorganize_after_object_deletion() override;

};


//---------------------------------------------------------------------------------------
// raw info about a volta bracket
class ImoVoltaBracketDto : public ImoSimpleObj
{
protected:
    int         m_lineNum;
    int         m_voltaId;
    bool        m_fStopJog;
    int         m_type;
    std::string      m_voltaNum;
    std::string      m_voltaText;
    ImoBarline* m_pBarline;
    vector<int> m_repetitions;      //repetition numbers extracted from m_voltaNum
    //only for type k_start

public:
    ImoVoltaBracketDto()
        : ImoSimpleObj(k_imo_volta_bracket_dto)
        , m_lineNum(0)
        , m_voltaId(0)
        , m_fStopJog(true)
        , m_type(k_unknown)
        , m_voltaNum()
        , m_voltaText()
        , m_pBarline(nullptr)
    {
    }

    enum { k_unknown = 0, k_start, k_stop, k_discontinue,  };

    //getters
    inline int get_volta_type()
    {
        return m_type;
    }
    inline std::string& get_volta_number()
    {
        return m_voltaNum;
    }
    inline std::string& get_volta_text()
    {
        return m_voltaText;
    }
    inline ImoBarline* get_barline()
    {
        return m_pBarline;
    }
    inline bool get_final_jog()
    {
        return m_fStopJog;
    }
    inline int get_volta_id()
    {
        return m_voltaId;
    }
    int get_line_number()
    {
        return m_lineNum;
    }
    inline vector<int>& get_repetitions()
    {
        return m_repetitions;
    }

    //setters
    inline void set_volta_type(int value)
    {
        m_type = value;
    }
    inline void set_volta_number(const std::string& num)
    {
        m_voltaNum = num;
    }
    inline void set_volta_text(const std::string& text)
    {
        m_voltaText = text;
    }
    inline void set_barline(ImoBarline* pSO)
    {
        m_pBarline = pSO;
    }
    inline void set_final_jog(bool value)
    {
        m_fStopJog = value;
    }
    inline void set_volta_id(int value)
    {
        m_voltaId = value;
    }
    inline void set_line_number(int value)
    {
        m_lineNum = value;
    }
    inline void set_repetitions(vector<int>& repetitions)
    {
        m_repetitions = repetitions;
    }

    //required by RelationBuilder
    int get_item_number()
    {
        return get_volta_id();
    }
    bool is_start_of_relation()
    {
        return m_type == k_start;
    }
    bool is_end_of_relation()
    {
        return m_type == k_stop;
    }
    ImoStaffObj* get_staffobj()
    {
        return m_pBarline;
    }
};

//---------------------------------------------------------------------------------------
class ImoWedge : public ImoRelObj
{
protected:
    Tenths  m_startSpread = 0.0f;       //spread at start point
    Tenths  m_endSpread = 0.0f;         //spread at end point
    bool    m_fNiente = false;          //at start for crescendo or at end for diminuendo
    bool    m_fCrescendo = false;       //crescendo (true) or diminuendo (false)
    int     m_wedgeNum;

    //to control if variables contain default values or a new value has been set
    long m_modified = 0L;
    enum {
        k_modified_start_spread =   0x00000001,
        k_modified_end_spread =     0x00000002,
    };

    friend class ImFactory;
    ImoWedge(int num=0) : ImoRelObj(k_imo_wedge), m_wedgeNum(num) {}

public:
    //the five special
    ~ImoWedge() override = default;
    ImoWedge(const ImoWedge&) = default;
    ImoWedge& operator= (const ImoWedge&) = default;
    ImoWedge(ImoWedge&&) = delete;
    ImoWedge& operator= (ImoWedge&&) = delete;

    //setters
    inline void set_start_spread(Tenths value) { m_startSpread = value; m_modified |= k_modified_start_spread; }
    inline void set_end_spread(Tenths value) { m_endSpread = value;  m_modified |= k_modified_end_spread; }
    inline void set_default_spreads(Tenths start, Tenths end) { m_startSpread = start; m_endSpread = end; }
    inline void set_niente(bool value) { m_fNiente = value; }
    inline void set_crescendo(bool value) { m_fCrescendo = value; }
    inline void set_wedge_number(int value) { m_wedgeNum = value; }

    //getters
    inline Tenths get_start_spread() { return m_startSpread; }
    inline Tenths get_end_spread() { return m_endSpread; }
    inline bool is_niente() { return m_fNiente; }
    inline bool is_crescendo() { return m_fCrescendo; }
    inline int get_wedge_number() { return m_wedgeNum; }

    //check for modified values, for exporters
    bool is_start_spread_modified() const { return (m_modified & k_modified_start_spread) != 0; }
    bool is_end_spread_modified() const { return (m_modified & k_modified_end_spread) != 0; }

    //required override for ImoRelObj
    void reorganize_after_object_deletion() override;
};

// raw info about a pending wedge
//---------------------------------------------------------------------------------------
class ImoWedgeDto : public ImoSimpleObj
{
protected:
    Tenths m_spread;
    bool m_fNiente;
    bool m_fStart;
    bool m_fCrescendo;
    int m_wedgeNum;
    ImoDirection* m_pDirection;
    Color m_color;
    int m_lineNum;

public:
    ImoWedgeDto()
        : ImoSimpleObj(k_imo_wedge_dto)
        , m_spread(0.0f)
        , m_fNiente(false)
        , m_fStart(true)
        , m_fCrescendo(false)
        , m_wedgeNum(0)
        , m_pDirection(nullptr)
        , m_color(Color(0,0,0))
        , m_lineNum(0)
    {
    }
    virtual ~ImoWedgeDto();

    //setters
    inline void set_line_number(int value) { m_lineNum = value; }
    inline void set_spread(Tenths value) { m_spread = value; }
    inline void set_niente(bool value) { m_fNiente = value; }
    inline void set_staffobj(ImoDirection* pDirection) { m_pDirection = pDirection; }
    inline void set_crescendo(bool value) { m_fCrescendo = value; }
    inline void set_start(bool value) { m_fStart = value; }
    inline void set_wedge_number(int value) { m_wedgeNum = value; }
    inline void set_color(Color value) { m_color = value; }

    //getters
    inline int get_line_number() { return m_lineNum; }
    inline Tenths get_spread() { return m_spread; }
    inline bool is_niente() { return m_fNiente; }
    inline bool is_crescendo() { return m_fCrescendo; }
    inline bool is_start() { return m_fStart; }
    inline int get_wedge_number() { return m_wedgeNum; }
    inline Color get_color() { return m_color; }

    //required by RelationBuilder
    inline int get_item_number() { return get_wedge_number(); }
    bool is_start_of_relation() { return is_start(); }
    bool is_end_of_relation() { return !is_start(); }
    inline ImoDirection* get_staffobj() { return m_pDirection; }
};


//---------------------------------------------------------------------------------------
// A tree of ImoObj objects
typedef Tree<ImoObj>          ImoTree;
typedef TreeNode<ImoObj>      ImoNode;

//---------------------------------------------------------------------------------------
// Visitors for ImoObj objects
typedef Tree<ImoObj>          ImoTree;
//typedef Visitor<ImoAttachments> ImVisitor;
//typedef Visitor<ImoAuxObj> ImVisitor;
//typedef Visitor<ImoBeamData> ImVisitor;
//typedef Visitor<ImoBeamDto> ImVisitor;
//typedef Visitor<ImoBeam> ImVisitor;
//typedef Visitor<ImoBlocksContainer> ImVisitor;
//typedef Visitor<ImoInlinesContainer> ImVisitor;
//typedef Visitor<ImoBoxInline> ImVisitor;
//typedef Visitor<ImoButton> ImVisitor;
//typedef Visitor<ImoChord> ImVisitor;
//typedef Visitor<ImoContentObj> ImVisitor;
//typedef Visitor<ImoDocument> ImVisitor;
//typedef Visitor<ImoDynamic> ImVisitor;
typedef Visitor<ImoHeading> ImHeadingVisitor;
//typedef Visitor<ImoInlineLevelObj> ImVisitor;
//typedef Visitor<ImoInstrument> ImVisitor;
//typedef Visitor<ImoMusicData> ImVisitor;
//typedef Visitor<ImoNote> ImVisitor;
//typedef Visitor<ImoNoteRest> ImVisitor;
typedef Visitor<ImoObj> ImObjVisitor;
//typedef Visitor<ImoOptionInfo> ImVisitor;
typedef Visitor<ImoParagraph> ImParagraphVisitor;
//typedef Visitor<ImoParamInfo> ImVisitor;
//typedef Visitor<ImoRelDataObj> ImVisitor;
//typedef Visitor<ImoRelObj> ImVisitor;
//typedef Visitor<ImoScoreText> ImVisitor;
//typedef Visitor<ImoSimpleObj> ImVisitor;
//typedef Visitor<ImoSlurDto> ImVisitor;
//typedef Visitor<ImoStaffInfo> ImVisitor;
//typedef Visitor<ImoStaffObj> ImVisitor;
//typedef Visitor<ImoStyles> ImVisitor;
//typedef Visitor<ImoTextItem> ImVisitor;
//typedef Visitor<ImoTextStyle> ImVisitor;
//typedef Visitor<ImoStyle> ImVisitor;
//typedef Visitor<ImoTieData> ImVisitor;
//typedef Visitor<ImoTieDto> ImVisitor;
//typedef Visitor<ImoTupletDto> ImVisitor;
//typedef Visitor<ImoTuplet> ImVisitor;
//typedef Visitor<ImoWrapperBox> ImVisitor;


//---------------------------------------------------------------------------------------
// global functions

extern int to_note_type(const char& letter);
extern NoteTypeAndDots ldp_duration_to_components(const std::string& duration);
extern TimeUnits to_duration(int nNoteType, int nDots);


}   //namespace lomse
///@endcond

#endif    //__LOMSE_INTERNAL_MODEL_P__
