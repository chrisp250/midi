// Controller module for launchpad
#include <arduino.h>
#include <usbh_midi.h>
#include <usbhub.h>
#include <MIDI.h>

#include "launchpad_midi.h"

void LaunchpadController::Initialise() {
    USBMidi->SendSysEx(daw_mode,sizeof(daw_mode));
    return;
}

//Set launchpad to DAW mode and draw initial setup-
LaunchpadController::LaunchpadController(USBH_MIDI *eMidi) {
    if (eMidi == NULL) {
        return;
    }
    USBMidi=eMidi;
    SequencerState=Stop;
}
