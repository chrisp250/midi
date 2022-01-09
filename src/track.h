#include <arduino.h>

#define TRACK_NUMBER 4 // Start with 4 tracks
#define TRACK_STEPS 32 // Steps per track


//Midi note definition
class TrackNote {
    public:
        uint8_t MidiChannel; //Midi channel for the track
        uint8_t note[TRACK_STEPS]; //Note value for each step
        uint8_t duration[TRACK_STEPS]; //Duration for each step
        uint8_t veloticy[TRACK_STEPS]; //Note velocity for each step

};

//Midi track definition
class MidiTrack {
    
    TrackNote note[TRACK_STEPS];

    public:
        MidiTrack(void);
        void add_note(uint8_t, TrackNote);
        void remote_note(uint8_t, TrackNote);     
        void get_note(uint8_t, TrackNote);
};