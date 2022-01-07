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
  delay(500);
  lp_controller.Initialise();
  delay(500);
  if ( Midi1 ) {
    Serial.printf("Got it\n");
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
      lp_controller.ProcessInput(size,outBuf);
      //Serial.printf("Byte: (s)%d, %x, %x, %x, %x\n",size, outBuf[0],outBuf[1],outBuf[2],outBuf[3]);
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
