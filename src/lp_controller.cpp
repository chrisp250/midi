// Controller module for launchpad
#include <arduino.h>
#include <usbh_midi.h>
#include <usbhub.h>
#include <MIDI.h>

#include "launchpad_midi.h"

LaunchpadController::LaunchpadController(USBH_MIDI *eMidi) {
    if (eMidi == NULL) {
        return;
    }
    Midi=eMidi;
    SequencerState=Stop;
}

