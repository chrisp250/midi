// Controller module for launchpad
#include <arduino.h>
#include <usbh_midi.h>
#include <usbhub.h>
#include <MIDI.h>

#include "launchpad_midi.h"

// Draw one row of the launchpad
void LaunchpadController::DrawRow(uint8_t row, uint8_t rowData[8]){
    uint8_t message[3] = {0x90};
    for (uint8_t counterCol=0;counterCol<8;counterCol++){
        message[1]=RowStart[row]+counterCol;
        message[2]=rowData[counterCol];
        USBMidi->SendData(message);
    }
}

// Initialise the Launchpad to the base layout
void LaunchpadController::Initialise() {

    USBMidi->SendSysEx(daw_mode,sizeof(daw_mode));
    for (uint8_t countRow=0;countRow<3;countRow=countRow+2){
        DrawRow(countRow,(uint8_t*)onWhite);
    }
    for (uint8_t countRow=1;countRow<4;countRow=countRow+2){
        DrawRow(countRow,(uint8_t*)onBlack);
    }
    for (uint8_t countRow=4;countRow<8;countRow++){
        uint8_t data[8] = {0x4c, 0x4c, 0x4c, 0x4c, 0x4c, 0x4c, 0x4c, 0x4c};
        DrawRow(countRow,data);
    }

    return;
}

// Process Session message
void LaunchpadController::ProcessSession(uint8_t msgSize ,uint8_t *message) {
    

}

// Process message received from the Launchpad
void LaunchpadController::ProcessInput(uint8_t msgSize, uint8_t *message){
    uint8_t msgType=message[0] & 0xf;
    uint8_t msgPort=message[0]>>4;

    Serial.printf("Received Size:%d, Type:%x, Port: %d\n", msgSize, msgType, msgPort);
    if (message[1] == 0x90) {
        Serial.printf("\tNote on:%d,Vel:%x\n", message[2], message[3]);
    }

    // Session message
    if (msgPort == 0) {
        ProcessSession(msgSize, message+1);
    }
}

//Set launchpad to DAW mode and draw initial setup-
LaunchpadController::LaunchpadController(USBH_MIDI *eMidi) {
    if (eMidi == NULL) {
        return;
    }
    USBMidi=eMidi;
    SequencerState=Stop;
}
