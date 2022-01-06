# Launchpad / Launch key protocol

Terminal set up  
`Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope Process`

## Device query
**Host => Device**  
Hex: F0h 7Eh 7Fh 06h 01h F7h  
Dec: 240 126 127 6 1 247  


**Launchkey MK3 => Host (Application)**  
Hex: F0h 7Eh 00h 06h 02h 00h 20h 29h <dev_type> 01h 00h 00h <app_version> F7h  
Dec: 240 126 0 6 2 0 32 41 <dev_type> 1 0 0 <app_version> 247  

## DAW mode
All commands to be sent on the DAW midi interface  
**Enter DAW mode**  
9Fh 0Ch 7Fh (port 16)

**Exit DAW mode**  
9Fh 0Ch 00h (port 16)

#
#Device identification
#Host >> Launchpad MK2: F0h 7Eh 7Fh 06h 01h F7h
#(240,126,127, 6, 1, 247)
#Launchpad MK2 will respond:
#Launchpad MK2 >> Host:
#F0h 7Eh <device ID> 06h 02h 00h 20h 29h 69h 00h 00h 00h <firmware revision 4 Bytes> F7h
#(240,126,<device ID>, 6, 2, 0, 32, 41,105, 0, 0, 0,<firmware revision 4 Bytes>, 247)


#Change layout
#Change the button layout Host >> Launchpad MK2:

#Where the layout byte is:
#F0h 00h 20h 29h 02h 18h 22h <Layout> F7h (240,0, 32, 41, 2, 24, 34, <Layout>,247)
#00h – Session
#01h – User 1 (drum rack)
#02h – User 2
#03h – (reserved for Ableton Live) 04h – Volume
#05h – Pan


#Turn on a button
#Example: light top-left grid LED blue in Session layout
#Host >> Launchpad MK2: Channel 1: note-on A5/51h (81), velocity 2Dh (45)
#90h 51h 2Dh (144, 81, 45)

