/*
 *******************************************************************************
 * USB-MIDI Launchpad Mini mk3 test
 *
 *******************************************************************************
 */

#define SPI4TEENSY3 0
#include <Arduino.h>
#include <usbh_midi.h>
#include <usbhub.h>
#include <MIDI.h>

#define _MIDI_SERIAL_PORT Serial1


USB Usb;
USBH_MIDI  Midi1(&Usb);
USBH_MIDI  Midi2(&Usb);

void MIDI_poll();

// Send test messages
void onInit()
{
  //Switch DAW mode on message
  uint8_t daw_mode[9] = {0xf0,0x00,0x20, 0x29, 0x02, 0x0d, 0x10, 0x01, 0xf7};
  //Device Inquire message
  uint8_t device_inq[6] = {0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7};
  
  delay(100);
  
  Serial.printf("Sending device inquiry...\n");

  //Send switch to DAW mode message
  Midi1.SendSysEx(daw_mode, sizeof(daw_mode),0);
  
  //Send device inquiry message:
  
  //If sent with this function interface on cable 1 received messages normally
  //Midi1.SendSysEx(device_inq, sizeof(device_inq),0);

  //If sent with this function instead interface on cable 1 stops receiveing messsages
  //This is the case not only for SysEx messages but for noteon messages as well.
  //Midi1.SendData(device_inq);
  uint8_t msg[3] = {0x90, 0x24, 0x4f};
  Midi1.SendData(msg,0);
  uint8_t msg1[3] = {0xb0, 0x1d, 0x52};
  Midi1.SendData(msg1,0);

}

void setup()
{

  Serial.begin( 115200 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.printf("Start...");
  
  if (Usb.Init() == -1) {
    while (1); //halt
  }//if (Usb.Init() == -1...
  delay( 200 );
  Usb.Task();
  Serial.printf("init..\n");
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

// Poll USB MIDI Controler and print received data
void MIDI_poll()
{
  uint8_t outBuf[ 20 ];
  uint8_t size;

  //Serial.printf("Polling\n");
  do {
    if ( (size = Midi1.RecvData(outBuf,true)) > 0 ) {
      Serial.printf("Raw received: (s)%d, %x, %x, %x, %x\n",size, outBuf[0],outBuf[1],outBuf[2],outBuf[3]);
    }
  } while (size > 0);
}
