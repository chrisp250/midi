// Track definition
#include <arduino.h>

#define TRACK_STEPS 32

class TrackNote {
        uint8_t note[TRACK_STEPS];
        uint8_t duration[TRACK_STEPS];
        uint8_t veloticy[TRACK_STEPS];

};

class track {
    TrackNote note[32];

    public:
        void add_note(uint8_t step, TrackNote note) {

        }

        void remote_note(uint8_t step, TrackNote note) {

        }

        void get_note(uint8_t step, TrackNote note) {

        }
};