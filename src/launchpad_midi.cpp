/*
 *******************************************************************************
 * USB-MIDI to Legacy Serial MIDI converter
 * Copyright (C) 2012-2021 Yuuichi Akagawa
 *
 * Idea from LPK25 USB-MIDI to Serial MIDI converter
 *   by Collin Cunningham - makezine.com, narbotic.com
 *
 * This is sample program. Do not expect perfect behavior.
 *******************************************************************************
 */

#define SPI4TEENSY3 0
#include <Arduino.h>
#include <usbh_midi.h>
#include <usbhub.h>
#include <MIDI.h>

#include "launchpad_midi.h"

#ifdef USBCON
#define _MIDI_SERIAL_PORT Serial1
#else
#define _MIDI_SERIAL_PORT Serial
#endif

// Set to 1 if you want to wait for the Serial MIDI transmission to complete.
// For more information, see https://github.com/felis/USB_Host_Shield_2.0/issues/570
#define ENABLE_MIDI_SERIAL_FLUSH 0

//////////////////////////
// MIDI Pin assign
// 2 : GND
// 4 : +5V(Vcc) with 220ohm
// 5 : TX
//////////////////////////

USB Usb;
USBH_MIDI  Midi1(&Usb);
USBH_MIDI  Midi2(&Usb);
LaunchpadController lp_controller(&Midi1);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, SMIDI);

void MIDI_poll();


void onInit()
{
  //Switch DAW mode on
  uint8_t daw_mode[9] = {0xf0,0x00,0x20, 0x29, 0x02, 0x0d, 0x10, 0x01, 0xf7};
  //Device Inquire
  uint8_t device_inq[6] = {0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7};
  uint8_t programMode[10] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x0D, 0x00, 0x7f, 0xf7};

  delay(500);
  lp_controller.Initialise();
  //Serial.printf("Sending SysEx...");
  //Midi1.SendSysEx(device_inq, sizeof(device_inq),0);
  Midi1.SendSysEx(daw_mode, sizeof(daw_mode),0);
  //Midi1.SendSysEx(programMode, sizeof(programMode),0);

  delay(500);
  if ( Midi1 ) {
    Serial.printf("Got it\n");
  }
}

void turnOn(uint8_t key, uint8_t state) {

  uint8_t on[3] = {0x90, key};

  if (state == 0) {
  //Serial.printf("Sending light change\n");
    on[2]=0x00;
    Midi1.SendData(on);
  } else {
    on[2]=0x4f;
    Midi1.SendData(on);
  }

}


void setup()
{

  Serial.begin( 115200 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.printf("Start...");
  SMIDI.begin();
  //_MIDI_SERIAL_PORT.begin(31250);
  
  if (Usb.Init() == -1) {
    while (1); //halt
  }//if (Usb.Init() == -1...
  delay( 200 );
  Usb.Task();
  Serial.printf("init..\n");
  //Midi1.SendData(device_inq);
  Midi1.attachOnInit(onInit);
}

void loop()
{
  Usb.Task();

  if ( Midi1 ) {
    MIDI_poll();
  }
  //delay(1ms) if you want
  //delayMicroseconds(1000);
}

// Poll USB MIDI Controler and send to serial MIDI
void MIDI_poll()
{
  uint8_t outBuf[ 20 ];
  uint8_t size;

  //Serial.printf("Polling\n");
  do {
    if ( (size = Midi1.RecvData(outBuf,true)) > 0 ) {
      Serial.printf("Byte: (s)%d, %x, %x, %x, %x\n",size, outBuf[0],outBuf[1],outBuf[2],outBuf[3]);
      if (outBuf[1] == 0x90) {
        //turnOn(outBuf[2], outBuf[3]);
      }
      //MIDI Output
      //_MIDI_SERIAL_PORT.write(outBuf, size);
#if ENABLE_MIDI_SERIAL_FLUSH
      //_MIDI_SERIAL_PORT.flush();
#endif
    }
  } while (size > 0);
}
