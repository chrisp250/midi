// Controller module for launchpad
#include <arduino.h>
#include <usbh_midi.h>
#include <usbhub.h>
#include <MIDI.h>

#include "launchpad_midi.h"

// Convert pressed key to note
int8_t LaunchpadController::KeytoNote(uint8_t key){
    int8_t whitemap[8] = {0,2,4,5,7,9,11,12};
    int8_t blackmap[8] = {-1,1,3,-1,6,8,10,-1};
    int8_t row=(key-10)/10;

    Serial.printf("Row: %d\n", row);
    switch (row) {
        case 0:
            return whitemap[key-RowStart[row]]+Cnote;
            break;
        case 1:
            if (blackmap[key-RowStart[row]] == -1) {
                return -1;
            } else {
                return blackmap[key-RowStart[row]]+Cnote;
            }
            break;
        case 2:
            return whitemap[key-RowStart[row]]+Cnote+12;
            break;
        case 3:
            if (blackmap[key-RowStart[row]] == -1) {
                return -1;
            } else {
                return blackmap[key-RowStart[row]]+Cnote+12;
            }
            break;
    }
    return -1;
//    }
//    {
//   case /* constant-expression */:
//        /* code */
//        break;
//   
//    default:
//        break;
//    }
//    if (key < RowStart[1]) {
 //       return key-0xb+Cnote;
//    }
}
// Draw one row of the launchpad
void LaunchpadController::DrawRow(uint8_t row, uint8_t rowData[8]){
    uint8_t message[3] = {0x90};
    for (uint8_t counterCol=0;counterCol<8;counterCol++){
        message[1]=RowStart[row]+counterCol;
        message[2]=rowData[counterCol];
        USBMidi->SendData(message);
        delay(10);
    }
}

// Initialise the Launchpad to the base layout
void LaunchpadController::Initialise() {
    uint8_t sessionMode[9] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x0D, 0x00, 0x00, 0xF7};
    // Enable DAW mode
    USBMidi->SendSysEx(daw_mode,sizeof(daw_mode));
    delay(500);
    USBMidi->SendSysEx(sessionMode,sizeof(sessionMode));
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

    SequencerState=Stop;
    Cnote=LowC;
    return;
}

// Process Session message
void LaunchpadController::ProcessSession(uint8_t msgSize ,uint8_t *message) {
    if (message[0] == 0x90) {
        
        if (message[1] < 0x32) { //Keyboard pressed
            //Serial.printf("Note pressed: %x Velocity: %x\n", message[1], message[2]);
            int8_t note = KeytoNote(message[1]);
            Serial.printf("\tNote: %d Velocity: %d\n",note,message[2]);
            if (note != -1) {
                pFuncSendMIDI(note,message[2],1);
            }
        } else { // Sequencer pressed
            Serial.printf("Sequencer pressed: %x Velocity: %x\n", message[1], message[2]);

        }
    }

}

// Process message received from the Launchpad
void LaunchpadController::ProcessInput(uint8_t msgSize, uint8_t *message){
    uint8_t msgType=message[0] & 0xf;
    uint8_t msgPort=message[0]>>4;

    Serial.printf("Received Size:%d, Type:%x, Port: %d\n", msgSize, msgType, msgPort);

    // Session message
    if (msgPort == 0) {
        ProcessSession(msgSize, message+1);
    } else {
        Serial.printf("\tPort1 %x, %x, %x\n",message[1], message[2], message[3]);
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
