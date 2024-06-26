//---------------------------------------------------------------------------------------
// This file is part of the Lomse library.
// Copyright (c) 2010-present, Lomse Developers
//
// Licensed under the MIT license.
//
// See LICENSE and NOTICE.md files in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#ifndef __LOMSE_IM_NOTE_H__        //to avoid nested includes
#define __LOMSE_IM_NOTE_H__

#include "lomse_internal_model.h"
#include "lomse_pitch.h"

using namespace std;

namespace lomse
{


//---------------------------------------------------------------------------------------
//noteheads
enum ENoteHeads
{
    k_notehead_longa = 1,
    k_notehead_breve,
    k_notehead_whole,              //Whole note
    k_notehead_half,               //Half note
    k_notehead_quarter,            //Quarter note
    //other noteheads
    k_notehead_cross,              //Cross (for percussion)
    k_notehead_fret,               //fret number, for tablature
};

//---------------------------------------------------------------------------------------
//stem notated values
enum ENoteStem
{
    k_stem_default = 0,     ///< No notated value. Stem should follow engraving rules
    k_stem_up,              ///< Stem must go up
    k_stem_down,            ///< Stem must go down
    k_stem_double,          ///< Two stems, one up and the other down
    k_stem_none,            ///< No stem
};

//---------------------------------------------------------------------------------------
// actual accidentals are, normally, not specified in LDP. Therefore, note pitch is
// computed from notated accidentals. But for this, it is necessary to know when it
// is specified and when not.
#define k_acc_not_computed      10000.0f    //an absurd value


//---------------------------------------------------------------------------------------
class ImoNoteRest : public ImoStaffObj
{
protected:
    bool        m_fUnpitched = false;           //unpitched note
    int         m_nNoteType = k_quarter;
    int         m_step = k_step_undefined;      //step and octave are for note's pitch. But
    int         m_octave = k_octave_undefined;  //also for placement on the staff when
                                                //unpitched notes and for rests
    int         m_nDots = 0;
    int         m_timeModifierTop = 1;          //for tuplets
    int         m_timeModifierBottom = 1;
    TimeUnits   m_duration = k_duration_quarter;        //nominal duration implied by note type and dots
    TimeUnits   m_playDuration = k_duration_quarter;    //playback duration: nominal duration for playback
    TimeUnits   m_eventDuration = k_duration_quarter;   //event duration: real duration for playback
    TimeUnits   m_playTime = 0.0;                       //playback time: on-set time for playback

public:
    ImoNoteRest(int objtype) : ImoStaffObj(objtype) { m_nVoice = 1; }

    //the five special
    ~ImoNoteRest() override {}
    ImoNoteRest(const ImoNoteRest&) = default;
    ImoNoteRest& operator= (const ImoNoteRest&) = default;
    ImoNoteRest(ImoNoteRest&&) = delete;
    ImoNoteRest& operator= (ImoNoteRest&&) = delete;

    //overrides to ImoStaffObj
    TimeUnits get_duration() override { return m_duration; }
    void set_time(TimeUnits rTime) override {
        m_time = rTime;
        m_playTime = rTime;
    }

    //getters
    inline int get_note_type() { return m_nNoteType; }
    inline int get_dots() { return m_nDots; }
    inline int get_time_modifier_top() { return m_timeModifierTop; }
    inline int get_time_modifier_bottom() { return m_timeModifierBottom; }
    inline TimeUnits get_playback_duration() { return m_playDuration; }
    inline TimeUnits get_event_duration() { return m_eventDuration; }
    inline TimeUnits get_playback_time() { return m_playTime; }

    //setters
    inline void set_note_type(int noteType) { m_nNoteType = noteType; }
    inline void set_dots(int dots) { m_nDots = dots; }
    void set_note_type_and_dots(int noteType, int dots);
    void set_time_modifiers_and_duration(int numerator, int denominator);
    void set_time_modifiers(int numerator, int denominator);
    void set_type_dots_duration(int noteType, int dots, TimeUnits duration);
    inline void set_playback_duration(TimeUnits value) { m_playDuration = value; }
    inline void set_event_duration(TimeUnits value) { m_eventDuration = value; }
    inline void set_playback_time(TimeUnits value) { m_playTime = value; }
    inline void set_unpitched() { m_fUnpitched = true; }
    inline void reset_playback_duration() { m_playDuration = m_duration; }


    //pitch (notes) or placement on the staff (rests & unpitched notes)
    inline int get_step() { return m_step; }
    inline int get_octave() { return m_octave; }
    inline bool is_pitch_defined() { return m_step != k_step_undefined; }
    inline bool is_unpitched() { return m_fUnpitched; }

    /** Methods for doing atomic changes in ImoNote/ImoRest internal variables.
        For rests these methods are used to control placement on the staff and there are
        no problems in using them.
        For notes, these methods are intended for notes's construction/modification and
        should not be used by your application unless you understand well what you are doing.
    */
    inline void set_step(int step) { m_step = step; }
    inline void set_octave(int octave) { m_octave = octave; }

    //beam
    ImoBeam* get_beam();
    void set_beam_type(int level, int type);
    int get_beam_type(int level);
    bool is_end_of_beam();
    /** Method ImoNoteRest::is_beamed() informs if the note has an attached beam
        relation, and not if the note is in a beamed group. Take into account that
        in beamed chords, the beam relation is only placed on the base note
        for each chord. Therefore, for notes in chord, method ImoNoteRest::is_beamed()
        is not reliable.
        Method ImoNote::has_been() is reliable in all circumstances and cases.
    */
    inline bool is_beamed() { return find_relation(k_imo_beam) != nullptr; }

    //tuplets
    bool is_in_tuplet();
    ImoTuplet* get_first_tuplet();

    //IM attributes interface
//    void set_attribute(TIntAttribute attrib, int value) override;
    void set_int_attribute(TIntAttribute attrib, int value) override;
    int get_int_attribute(TIntAttribute attrib) override;
    list<TIntAttribute> get_supported_attributes() override;

protected:

};

//---------------------------------------------------------------------------------------
/** ImoRest represents a rest in the score.
**/
class ImoRest : public ImoNoteRest
{
protected:
    bool m_fGoFwd = false;
    bool m_fFullMeasureRest = false;

    friend class ImFactory;
    ImoRest() : ImoNoteRest(k_imo_rest) {}

    friend class GoBackFwdAnalyser;
    friend class GoBackFwdLmdAnalyser;
    friend class FwdBackMxlAnalyser;
    friend class MxlAnalyser;
    friend class MxlTimeKeeper;
    inline void mark_as_go_fwd() { m_fGoFwd = true; }

    friend class RestMxlAnalyser;
    friend class NoteRestMxlAnalyser;
    friend class EventMnxAnalyser;
    inline void mark_as_full_measure(bool value) { m_fFullMeasureRest = value; }

    //IM attributes interface
    void set_int_attribute(TIntAttribute attrib, int value) override;
    int get_int_attribute(TIntAttribute attrib) override;
    list<TIntAttribute> get_supported_attributes() override;

public:
    //the five special
    ~ImoRest() override {}
    ImoRest(const ImoRest&) = default;
    ImoRest& operator= (const ImoRest&) = default;
    ImoRest(ImoRest&&) = delete;
    ImoRest& operator= (ImoRest&&) = delete;

    inline bool is_go_fwd() { return m_fGoFwd; }
    inline bool is_full_measure() { return m_fFullMeasureRest; }

};

//---------------------------------------------------------------------------------------
/** ImoNote represents a note in the score.
**/
class ImoNote : public ImoNoteRest
{
protected:
    //pitch (step, octave, actual_acc) represents the sound, not what is notated.
    float   m_actual_acc;           //number of semitones (i.e, -1 for flat). Decimal
                                    //values like 0.5 (quarter tone sharp) are also valid.

    //notated accidentals. Accidentals to display are deduced from key signature and
    //context, but can be overridden
    EAccidentals m_notated_acc;
    int          m_options;

    //stem and ties
    int     m_stemDirection;        //value from ENoteStem
    ImoId m_idTieNext = k_no_imoid;
    ImoId m_idTiePrev = k_no_imoid;

    //computed values for layout
    int     m_computedStem;         //value from ENoteStem

    friend class ImFactory;
    ImoNote(int type);
    ImoNote(int step, int octave, int noteType, EAccidentals accidentals=k_no_accidentals,
            int dots=0, int staff=0, int voice=0, int stem=k_stem_default);

    //IM attributes interface
    void set_int_attribute(TIntAttribute attrib, int value) override;
    int get_int_attribute(TIntAttribute attrib) override;
    list<TIntAttribute> get_supported_attributes() override;

    //play techniques. AWARE: In future, when play techniques are implemented, these
    //variables must be replaced by a ImoPlayInfo child
    bool m_fMute = false;       //true: mute in playback, do not generate MIDI events

public:
    //the five special
    ~ImoNote() override;
    ImoNote(const ImoNote&) = default;
    ImoNote& operator= (const ImoNote&) = default;
    ImoNote(ImoNote&&) = delete;
    ImoNote& operator= (ImoNote&&) = delete;

    //options for notated accidentals
    enum ENotatedAcc
    {
        k_computed      = 0x0001, ///< Flag for detecting the first time notated accidentals are computed
        k_force         = 0x0002, ///< Force to display accidentals
        k_also_natural  = 0x0004, ///< When 'forced', this option will also display 'natural' accidentals.
        k_parenthesis   = 0x0008, ///< Add parentheses to displayed accidentals
        k_never         = 0x0010, ///< Do not display accidentals
        k_non_standard  = 0x0020, ///< Use non-standard acc. if necessary
    };


    /** For muting playback and determining mute state */
    void mute(EMuteType value);
    bool is_muted();

    //pitch
    inline float get_actual_accidentals() { return m_actual_acc; }

    /** Using this method could create inconsistencies between new pitch and notated
        accidentals unless it is used in conjunction with set_actual_accidentals()
        or PitchAssigner is later used for computing actual accidentals.
    */
    inline void set_notated_pitch(int step, int octave, EAccidentals accidentals)
    {
        m_step = step;
        m_octave = octave;
        m_notated_acc = accidentals;
        m_actual_acc = k_acc_not_computed;
    }

    /** Using this method could create inconsistencies between new pitch and notated
        accidentals unless it is used in conjunction with set_notated_accidentals()
        or PitchAssigner is later used for computing notated accidentals.
    */
    void set_pitch(FPitch fp) {
        m_step = fp.step();
        m_octave = fp.octave();
        m_actual_acc = float(fp.num_accidentals());
        m_notated_acc = k_invalid_accidentals;
    }

    /** Using this method could create inconsistencies between new pitch and notated
        accidentals unless it is used in conjunction with set_notated_accidentals()
        or PitchAssigner is later used for computing notated accidentals.
    */
    void set_pitch(int step, int octave, float actualAcc)
    {
        m_step = step;
        m_octave = octave;
        m_actual_acc = actualAcc;
    }


    ///@{
    /** Methods for doing atomic changes in ImoNote internal variables. They will create
        inconsistencies unless you know what you are doing.
        These methods are intended for notes's construction/modification and should not
        be used by your application unless you understand well what you are doing.
    */
    inline void set_actual_accidentals(float value) { m_actual_acc = value; }
    inline void set_notated_accidentals(EAccidentals accidentals) { m_notated_acc = accidentals; }
    inline void request_pitch_recomputation() { m_actual_acc = k_acc_not_computed; }
    ///@}

    //notated accidentals
    inline EAccidentals get_notated_accidentals() { return m_notated_acc; }
    inline void force_to_display_accidentals() { m_options |= k_force; }
    inline bool is_display_accidentals_forced() { return (m_options & k_force) != 0; }
    inline void force_to_display_naturals() { m_options |= k_also_natural; }
    inline bool is_display_naturals_forced() { return (m_options & k_also_natural) != 0; }
    inline void mark_notated_accidentals_as_computed() { m_options |= k_computed; }
    inline bool notated_accidentals_never_computed() { return !(m_options & k_computed); }

    //ties
    ImoTie* get_tie_next();
    ImoTie* get_tie_prev();
    inline bool is_tied_next() { return m_idTieNext != k_no_imoid; }
    inline bool is_tied_prev() { return m_idTiePrev != k_no_imoid; }
    void set_tie_next(ImoTie* pStartTie);
    void set_tie_prev(ImoTie* pEndTie);

    //notated stem
    /** Stem direction, as notated in source file */
    inline bool has_stem() { return m_nNoteType >= k_half; }
    inline int get_stem_direction() { return m_stemDirection; }
    inline void set_stem_direction(int value) { m_stemDirection = value; }
    inline bool is_stem_up() { return m_stemDirection == k_stem_up; }
    inline bool is_stem_down() { return m_stemDirection == k_stem_down; }
    inline bool is_stem_default() { return m_stemDirection == k_stem_default; }
    inline bool is_stem_none() { return m_stemDirection == k_stem_none; }

    //computed stem
    /** Engravers decide the direction for the stem and set the value */
    inline int get_computed_stem() { return m_computedStem; }
    inline void set_computed_stem(int value) { m_computedStem = value; }
    inline bool is_computed_stem_up() { return m_computedStem == k_computed_stem_up
                                            || m_computedStem == k_computed_stem_forced_up; }
    inline bool is_computed_stem_down() { return m_computedStem == k_computed_stem_down
                                            || m_computedStem == k_computed_stem_forced_down; }
    inline bool is_computed_stem_forced_up() { return m_computedStem == k_computed_stem_forced_up; }
    inline bool is_computed_stem_forced_down() { return m_computedStem == k_computed_stem_forced_down; }
    inline bool is_computed_stem_forced() { return m_computedStem == k_computed_stem_forced_down
                                                || m_computedStem == k_computed_stem_forced_up; }
    inline bool is_computed_stem_none() { return m_computedStem == k_computed_stem_none; }

    //in chord
    bool is_in_chord();
    ImoChord* get_chord();
    bool is_start_of_chord();
    bool is_end_of_chord();
    bool is_cross_staff_chord();

    /** Method ImoNoteRest::is_beamed() informs if the note has an attached beam
        relation, and not if the note is in a beamed group. Take into account that
        in beamed chords, the beam relation is only placed on the base note
        for each chord. Therefore, for notes in chord, method ImoNoteRest::is_beamed()
        is not reliable.
        Method ImoNote::has_been() is reliable in all circumstances and cases.
    */
    bool has_beam();

    //grace notes related to this note
    ImoGraceRelObj* get_grace_relobj();

    //arpeggio
    ImoArpeggio* get_arpeggio();


    //pitch. Only valid when m_actual_acc is computed
    FPitch get_fpitch();            //FPitch. Ignores fractional part of actual accidentals
    float get_frequency();          //frequecy, in Herzs
    MidiPitch get_midi_pitch();     //MidiPitch
    int get_midi_bend();            //Midi bend (two bytes)
    float get_cents();              //deviation from diatonic pitch implied by step and octave, in cents.

};

//---------------------------------------------------------------------------------------
/** ImoGraceNote represents a grace note in the score.
**/
class ImoGraceNote : public ImoNote
{
protected:
    TimeUnits m_alignTime;  //to simplify spacing algorithm a pseudo-timepos is assigned

    friend class ImFactory;
    ImoGraceNote() : ImoNote(k_imo_note_grace), m_alignTime(0.0) {}

public:
    //the five special
    ~ImoGraceNote() override {}
    ImoGraceNote(const ImoGraceNote&) = default;
    ImoGraceNote& operator= (const ImoGraceNote&) = default;
    ImoGraceNote(ImoGraceNote&&) = delete;
    ImoGraceNote& operator= (ImoGraceNote&&) = delete;

    inline void set_align_timepos(TimeUnits value) { m_alignTime = value; }
    inline TimeUnits get_align_timepos() { return m_alignTime; }

};


}   //namespace lomse

#endif    // __LOMSE_IM_NOTE_H__

