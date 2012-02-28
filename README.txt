Project Description
===================

This is a C++ library for communication with XBee (standard and pro) ZNet 2.5
devices. Essentially, it provides driver-like functionality allowing you to
issue commands to XBee wireless chipsets using the XBee API Protocol.

Requirements
============

To add USB-to-serial communication support, pair this library with the ftdi 
drivers provided at http://www.ftdichip.com/FTDrivers.htm.  As a convenience,
this project includes ftd2xx.lib, ftd2xx.dll, and ftd2xx.h for inclusion in
your projects.

Several parameters must be configured on every XBee unit to be used with 
this library using X-CTU (or comparable software):

For the Co-ordinator

-- Set a PAN ID (ID field in XCTU), a node identifier (NI field), and the 
   AP = 2 option to enable API Mode with escape characters

-- Ensure that the NJ field is set to 0xFF and the SC field to 1FFE

-- Ensure that the baud rate of the XBee matches the baud rate of your COM 
   port, this can be done by appropriately setting the BD field

For each end device or router

-- Set the same PAN ID as the co-ordinator to put them all in the same 
   network, also set some desired NI field and set AP = 2 as well

-- Set JV = 1 to have correct channel selection

-- Ensure that the DH and DL fields are set to 0 (if you want the device to
   talk directly to the Co-ordinator) and that the SC is 1FFE

Usage
=====

The general usage pattern is as follows (read code comments to find more
specific functionality):

1. Create an instance of the aUARTDevice class.  Use this instance to configure 
   the USB to serial interface between your computer and XBee device.

2. If you wish to send data to a XBee device, create the appropriate packet by 
   using an appropriate class defined in xbee_lib.h. 
   Use the class member functions to populate the contents of the packet 
   (length, API ID, etc). 

3. Dump the contents of your class into the aUARTDevice instance you have 
   created (use the PtrToTXDataBuffer function of aUARTDevice class).
   Call the SendData function of aUARTDevice class to send the packet,
   remembering to specify the size of your packet as an argument to this 
   function as well.

4. If you wish to receive a packet from a XBee device, call the getData() 
   function of aUARTDevice class. NOTE that this will block your process 
   until a packet is received, so make sure to do this in a seperate thread 
   or create a dedicated receive process. 
   The getData() function will return a code specifying what type of packet
   was received. Use this code in your application to create an appropriate 
   class defined in xbee_lib.h to hold the XBee packet. You can assign the 
   data to the class created by using the "ConvertTo" functions in aUARTDevice 
   class (e.g. ConvertToTXStatus).

License
=======

The code is released under the ISC license (http://www.opensource.org/licenses/ISC).

If you need help, have feedback, or got questions, shoot me (Ebrahim Ashrafizadeh) 
a message on github.
