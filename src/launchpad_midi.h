enum InterfaceMode {Session, Custom1, Custom2, Custom3, Faders};
enum SequencerMode {Stop, Play, Record};

#define LowC 24 // First key of the keyboard
#define MONITOR_SMIDI //Turn on Serial MIDI monitoring

#define TRACK_NUMBER 4 // Start with 4 tracks
#define TRACK_STEPS 32 // Steps per track





// Main controller for the Launchpad
class LaunchpadController {
    public:
        LaunchpadController(USBH_MIDI *);
        void Task();
        //Set launchpad to DAW mode and draw initial setup
        void Initialise(void);
        void ProcessInput(uint8_t, uint8_t *);
        void attachSendMIDI(void (*funcSendMIDI)(uint8_t, uint8_t, uint8_t)) {
                pFuncSendMIDI = funcSendMIDI;
        };

    private:
        void DrawRow(uint8_t, uint8_t *);
        void ProcessSession(uint8_t, uint8_t *);
        int8_t KeytoNote(uint8_t);
        void (*pFuncSendMIDI)(uint8_t, uint8_t, uint8_t) = nullptr; // Pointer to function called in onInit()
        
        // Messages
        uint8_t daw_mode[9] = {0xf0,0x00,0x20, 0x29, 0x02, 0x0d, 0x10, 0x01, 0xf7}; //Switch DAW mode on message
        const uint8_t device_inq[6] = {0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7}; //Device Inquire message
        const uint8_t defaultPad[10] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x0D, 0x14,0x52, 0x4f, 0xf7};
        const uint8_t lighton[11] =     {0xF0, 0x00, 0x20, 0x29, 0x02, 0x0D, 0x03, 0x00, 0x0b, 0x0d, 0xf7};

        // Key patterns
        const uint8_t onWhite[8] = {0x52, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x52}; // White keys pattern
        const uint8_t onBlack[8] = {0x00, 0x4f, 0x4f, 0x00, 0x4f, 0x4f, 0x4f, 0x00}; //Black keys pattern 
        
        const uint8_t RowStart[8] = {0x0b, 0x15, 0x1f, 0x29, 0x33, 0x3d, 0x47, 0x51}; // First key of each row

        USBH_MIDI *USBMidi;
        SequencerMode SequencerState;
        uint8_t Cnote; //C note on sequencer keyboard

};