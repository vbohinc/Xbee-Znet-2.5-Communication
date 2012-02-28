/*

XBee (ZNet 2.5 Spec) Communication Library
------------------------------------------

Copyright (c) 2012, Ebrahim Ashrafizadeh

Permission to use, copy, modify, and/or distribute this software for any purpose with or 
without fee is hereby granted, provided that the above copyright notice and this permission 
notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS 
SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL 
THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES 
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, 
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
OF THIS SOFTWARE.

*/

#pragma once

//General XBee Constants (mainly used for setting up operation in API mode)
#define API_MODE 2
#define XON 0x11
#define XOFF 0x13
#define START_BYTE 0x7E
#define ESCAPE_BYTE 0x7D
#define MAX_NUMBYTES_PACKET 100
#define COORDINATOR_ADDR 0x0

//Command IDs (XBee 2.5) supported by API Mode
#define MODEM_STATUS 0x8A
#define AT_COMMAND 0x08
#define AT_COMMAND_QUEUE 0x09
#define AT_COMMAND_RESPONSE 0x88
#define REMOTE_AT_COMMAND_REQUEST 0x17
#define REMOTE_AT_COMMAND_RESPONSE 0x97
#define ZIGBEE_TX_REQUEST 0x10
#define EXPLICIT_XBEE_ADDR_COM_FRAME 0x11
#define ZIGBEE_TX_STATUS 0x8B
#define ZIGBEE_RX_PACKET 0x90
#define ZIGBEE_EXPLICIT_RX 0x91
#define ZIGBEE_IO_RX 0x92
#define ZIGBEE_SENSOR_READ 0x94
#define NODE_ID 0x95

//Options for RX Packets
#define PACKET_ACKED 0x1
#define PACKET_WAS_BROADCAST 0x2

//Options for TX Packets
#define BROADCAST_RADIUS_MAX_HOPS 0x0
#define UNICAST 0x0

//Remote Command Status Indicators
#define OK 0x0
#define STAT_ERROR 0x1
#define INVALID_COMMAND 0x2
#define INVALID_PARAMETER 0x3

//Transmit Status Indicators
#define	SUCCESS 0x0
#define CCA_FAILURE 0x2
#define INVALID_DESTINATION_ENDPOINT 0x15
#define	NETWORK_ACK_FAILURE 0x21
#define NOT_JOINED_TO_NETWORK 0x22
#define	SELF_ADDRESSED 0x23
#define ADDRESS_NOT_FOUND 0x24
#define ROUTE_NOT_FOUND 0x25

//XBee Modem Status Codes
#define HARDWARE_RESET 0x0
#define WATCHDOG_TIMER_RESET 0x1
#define ASSOCIATED 0x2
#define DISASSOCIATED 0x3
#define SYNCHRONIZATION_LOST 0x4
#define COORDINATOR_REALIGNMENT 0x5
#define COORDINATOR_STARTED 0x6

//A class implementing core/basic xbee operations (for all packets - 
//do not instantiate this directly)
class BasicXBee
{
protected:
	unsigned int MSB64BitAddr;
	unsigned int LSB64BitAddr;
	unsigned char RXcommandID;
	unsigned short LengthField;
	unsigned char checksum;
	unsigned char frameLength;
	unsigned char* RXframeData;
	unsigned char TXcommandID;
	unsigned char TXFrameID;
	unsigned char TXframeLength;
	unsigned char* TXframeData;

public:
	BasicXBee();

	unsigned int get64BitAddrMSB();
	int set64BitAddrMSB(unsigned int msb);
	unsigned int get64BitAddrLSB();
	int set64BitAddrLSB(unsigned int lsb);

	//prepare a default response packet format
	int PrepareRX();
	
	//functions for manipulating the command ID of RX packet
	unsigned char getRXCommandID();
	int setRXCommandID(unsigned char id);
	
	//get/set the length field of a packet [INCLUDING the framedata field]
	unsigned short getRXLength();
	int setRXLength(unsigned short size);
	//SPECIAL SUBSET: get/set the framedata field of packet
	unsigned char getRXFrameDataLength();
	int setRXFrameDataLength(unsigned char flength);
	int setRXFrameData(unsigned char* Data);
	unsigned char* getRXFrameData();
	
	//set/get the checksum field
	unsigned char getCSum();
	int setCSum(unsigned char csum);

	//prepare a default transmit packet format
	int PrepareTX(unsigned char commID, unsigned char frID);
	
	//set/get the command ID for TX Packet
	unsigned char getTXCommandID();
	int setTXCommandID(unsigned char id);
	
	//get/set the frame ID (1-255)
	unsigned char getTXFrameID();
	int setTXFrameID(unsigned char frameId);

	//get/set the framedata field of TX packet
	unsigned char getTXFrameDataLength();
	int setTXFrameDataLength(unsigned char flength);
	int setTXFrameData(unsigned char* Data);
	unsigned char* getTXFrameData();
};

//Now classes for each specific packet type are defined

//===============================================================================
//PART I : Response Packet Types

//--------------- TX status response : sent after every TX request --------------
class TXStatusResponse : public BasicXBee 
{
public:
	TXStatusResponse();
	unsigned char getFrameID();
	unsigned short getRemoteAddress();
	unsigned char getTxRetryCount();
	unsigned char getDeliveryStatus();
	unsigned char getDiscoveryStatus();
	bool isResponseHealthy();
};
//-------------------------------------------------------------------------------

//--------------- RX response : a packet received through UART -----------------
class RXResponse : public BasicXBee 
{
public:
	RXResponse(); 
	unsigned short getRemoteAddress();
	unsigned char getOptions();
	unsigned char getLengthOfData(); //NOTE: data starts from the 12th element
	unsigned char* getRFDataField();
};
//------------------------------------------------------------------------------

//--------------- I/O RX response -----------------------------------------------
class IORXResponse : public BasicXBee 
{
protected:
	unsigned short Digital_Mask;
	unsigned char Analog_Mask;
public:
	IORXResponse(); 
	int PullMasks(unsigned short digi, unsigned char ana); 
	unsigned short getRemoteAddress();
	unsigned char getOptions();
	int what_IO_is_enabled();
	unsigned char* sampledIOData();
	unsigned short getDigMask();
	unsigned char getAnalogMask();
};
//----------------------------------------------------------------------------------

//------------------- Modem Status Response : Gives the status of XBee Module ---------
class RXModemStatus : public BasicXBee 
{
public:
	RXModemStatus(); 
	unsigned char getModuleStatus();
};
//------------------------------------------------------------------------------------

//------------------- AT Command Response : Sent in response to an AT command ----------
class ATCommandRX : public BasicXBee 
{
public:
	ATCommandRX(); 
	unsigned char getFrameID();
	unsigned short getATCommand();
	unsigned char getStatus();
	unsigned char getValueFieldLength();
	unsigned char* getValueField();
	bool isResponseHealthy();
};
//---------------------------------------------------------------------------------------

//------------------- Remote AT Command Response ----------------------------------------
class RemoteATCommandRX : public BasicXBee 
{
public:
	RemoteATCommandRX(); 
	unsigned char getFrameID();
	unsigned short getRemoteAddress();
	unsigned short getATCommand();
	unsigned char getStatus();
	unsigned char getValueFieldLength();
	unsigned char* getValueField();
	bool isResponseHealthy();
};
//-----------------------------------------------------------------------------------------

//===================================================================================================
//PART II : Transmit Packet Types

//------------------- TX Request: Used to transmit data to other XBee modules -------------
class TXRequest : public BasicXBee 
{
protected:
	unsigned short Network16BitAddr;
	unsigned char BRadius;
	unsigned char OptionsField;
	unsigned char DataLength;
	unsigned char* Data;
public:
	TXRequest();
	unsigned short getRemoteAddress(); //16-bit remote address
	int setRemoteAddress(unsigned short addr);
	unsigned char getBroadcastRadius();
	int setBroadcastRadius(unsigned char radius);
	unsigned char getOptions();
	int setOptions(unsigned char ops);

	//RF Data Field functions
	unsigned char getDataLength();
	int setDataLength(unsigned char length);
	unsigned char* getDataPtr();
	int setDataPtr(unsigned char* ptr);
};
//--------------------------------------------------------------------------------------------

//-------------------  AT Command TX: Used to set register values on a XBee module -------------
class ATCommandTX : public BasicXBee 
{
protected:
	unsigned short AT_Command;
	unsigned char ParameterValueLength;
	unsigned char* ParameterValue;
public:
	ATCommandTX();
	unsigned short getATCommand();
	int setATCommand(unsigned short atcom);
	unsigned char getParameterLength();
	int setParameterLength(unsigned char length);
	unsigned char* getParameterField();
	int setParameterField(unsigned char* param);
};
//-----------------------------------------------------------------------------------------------

//-------------------  Remote AT Command TX: Used to set register values on a remote XBee module ----------
class RemoteATCommandTX : public ATCommandTX 
{
protected:
	unsigned short The16BitAddr;
	unsigned char commandOptions;
public:
	RemoteATCommandTX();
	unsigned short get16RemoteAddr();
	int set16RemoteAddr(unsigned short addr);
	unsigned char getCommandOps();
	int setCommandOps(unsigned char comops);
};
//---------------------------------------------------------------------------------------------------
//============================================================================================================

//The following class implements an entire framework for communicating with FTDI UART devices. 
//Instances of this class provide an object that represents the interface between XBee and the 
//computer, as a result, every external program must declare at least one instance of this class.
class aUARTDevice
{
protected:
	unsigned char RXDataBuffer[MAX_NUMBYTES_PACKET];
	unsigned char TXDataBuffer[MAX_NUMBYTES_PACKET];
	unsigned char FrameIDCounter;
	unsigned char temp_CSum; //stores updating checksum values
	int current_read_position;
	bool packet_outstanding;
	
	FT_STATUS status; //retrieves status of executed FTDI DLL API commands (used for error checking)
	DWORD NumberOfConnectedDevices; //stores number of XBee UARTs connected
	FT_DEVICE_LIST_INFO_NODE* DeviceInfoArray; //this array stores all UART hardware device information
	FT_HANDLE USBHandle; //used to reference the USB to Serial port UART
	
	DWORD UARTBaudRate; //used to store the desired communication Baud Rate
	UCHAR UARTWordLength; //defines a WORD size for the UART device
	UCHAR UARTStopBits; //defines the number of stop bits for UART device
	UCHAR UARTParity; //defines the parity status of the UART device (odd, even, etc)
	DWORD ReadTimer; //defines the default reading timeout for the UART device (milliseconds)
	DWORD WriteTimer; //defines the default writing timeout for the UART device (milliseconds)
	USHORT UARTFlowControl; //defines the Flow Control mode of the UART device
	UCHAR ONChar; //defines the character used to signal Xon
	UCHAR OFFChar; //defines the character used to signal Xoff
	unsigned char LineStatus; //stores the UART line status
	unsigned char ModemStatus; //stores the UART Modem status
	DWORD BytesInRXBuffer; //stores the number of bytes currently in the RX buffer of the UART
	DWORD BytesInTXBuffer; //stores the number of bytes currently in the TX buffer of the UART
	DWORD UARTEvent; //stores the current state of the "event status" of the UART (see FTDI manual)
	DWORD PurgeMask; //when clearing out the RX and TX buffers of the UART, this variable determines HOW to do this

	DWORD DeviceAccessMethod; //used to store how to access the UART device
	PVOID AccessMethodValue; //depending on DeviceAccessMethod, the contents of this variable will change
	DWORD BytesToWrite; //stores the DESIRED number of bytes to write to UART device
	DWORD BytesToRead; //stores the DESIRED number of bytes to read from UART device
	DWORD BytesWritten; //stores the ACTUAL number of bytes written to UART device 
	DWORD BytesReceived; //stores the ACTUAL number of bytes received from UART device

public:
	aUARTDevice();

	int CompileDeviceInfo(); //this function retrieves ALL device info regarding connected UARTs (return of 0: Error, return of 1: Success)

	//the following functions can ONLY be used if CompileDeviceInfo has already been called
	unsigned int getNumberofConnectedDevices();
	int clearDeviceInfoNodes(); //call this function when you wish to terminate your application, it will clear up used memory (return of 0: Error, return of 1: Success)
	int getDeviceFlags(int deviceindex, DWORD* storage);
    int getDeviceID(int deviceindex, DWORD* storage);
	int getDeviceType(int deviceindex, DWORD* storage);
	int getDeviceLocID(int deviceindex, DWORD* storage);
	int getDeviceSerialNum(int deviceindex, char* storage);
	int getDeviceDescription(int deviceindex, char* storage);

	//the following function establishes a connection to a UART device. Connection MUST be made before any reading or writing can occur (or the setting of any
	//device parameters). Connection can be made using several methods, choose one by passing the following input for "type": 0 - by serial number,
	//1 - by description, 2 - by location ID.  Consistent with whatever the 'type' parameter you choose, you must also pass a void pointer to the 
	//relevant serial number, description, or location ID.  As before, return of 0: Error, return of 1: Success.
	int InitiateConnectionToDevice(int type, PVOID value); 
	int TerminateConnectionToDevice(); //closes whatever "USBHandle" is connected to.  As a result, MAKE SURE you only connect to ONE device at a time.

	int SetDeviceBaudRate(DWORD rate);
	
    //the following function sets the serial communication options.  As before, return of 0: Error, return of 1: Success.
	//Users are allowed to choose the following inputs for type1, type2, and type3 parameters: 
	//type1 (word length) : 0 - 8 bits      1 - 7 bits
	//type2 (stop bits) : 0 - 1 stop bit  1 - 2 stop bits
	//type3 (parity) : 0 - no parity   1 - odd    2 - even    3 - marked parity   4 - space parity
	int SetSerialCOMSpecs(int type1, int type2, int type3);

	int SetRXTXTimeouts(DWORD rxtout, DWORD txtout); //NOTE: time must be in milliseconds

	//the following function sets the flow control options.  As before, return of 0: Error, return of 1: Success.
	//Users are allowed to choose the following inputs for type1.  Please note that the function will ONLY consider the 
	//inputs for the on and off characters IF THE FLOW CONTROL IS XON XOFF mode, so they are OPTIONAL inputs.
	//type1 (flow mode) : 0 - no flow control   1 - RTS/CTS    2 - DTR/DSR    3 - Xon/Xoff
	int SetFlowSpecs(int type1, unsigned char ONCh = XON, unsigned char OFFCh = XOFF);
	
	//this function puts the modem and line status into the ModemStatus and LineStatus variables defined in the aUARTDevice class.
	//To interpret these statuses, please refer to the FTDI API manual.  This class does NOT provide functions for 
	//interpreting the contents of the ModemStatus and LineStatus variables.  As before, return of 0: Error, return of 1: Success.
	int QueryModemAndLineStatus();

	//the following functions should only be called after calling QueryModemAndLineStatus()
	unsigned char getLineStatus();
	unsigned char getModemStatus();
	
	//This function gets the number of bytes in the RX and TX buffers of the UART and stores them in BytesInTXBuffer and
	//BytesInRXBuffer variables. 
	int QueryUARTBuffers();

	//the following functions should only be called after calling QueryUARTBuffers()
	unsigned int getBytesInRXBuffer();
	unsigned int getBytesInTXBuffer();

	//the following function clears the UART buffers.  The "type" input is used to select the type of clearing:
	//0 - clear RX buffer    1 - clear TX buffer    2 - clear RX and TX buffers
	//As before, return of 0: Error, return of 1: Success. 
	int ClearUARTBuffers(int type);

	unsigned char ProduceFrameID(); //you MUST use this function to assign proper frame IDs to relevant TX packets before sending them out
	unsigned char* PtrToTXDataBuffer(); //use this to access the TXDataBuffer of this class (so use it BEFORE calling SendData)

	//this function is used to send data in the TXDataBuffer array (from index 0 to size - 1) to a XBee module.  
	//It is YOUR responsibility to fill the TX buffer of the aUARTDevice class with a properly formatted XBee Packet (this can easily be 
	//done by using the different TX classes defined in this header file). KEEP IN MIND THAT THE TX BUFFER MUST BE FILLED WITH ESCAPE 
	//CHARACTERS AS APPROPRIATE (again programmer's responsibility).  The "size" input is the number of bytes to write (your ENTIRE packet's 
	//size).  As before, return of 0: Error, return of 1: Success.
	int SendData(int size);	
	
	//this function is used to read data currently in the UART RX buffer. Since the UART RX buffer has a limited size, CALL THIS
	//FUNCTION FREQUENTLY!
	//THIS FUNCTION, WHEN CALLED, WILL CONTINUE TO READ UNTIL A FINISHED XBEE PACKET IS RETRIEVED.
	//A couple important notes:
    //-- function checks for packet errors, and if it finds any, it will FLUSH the RX buffer and reset the read sequence. 
	//-- The user MUST use the return codes of this function to decide HOW to pull data out for use by external programs.  
    //-- Note that getData WILL NOT RUN if a packet that is outstanding has not been pulled into an appropriate XBee class
	//-- function handles escape characters appropriately and removes them from the local RXDataBuffer, so do
	//   not worry about getting escape formatted RX packets!
	//return codes: 0 - Critical Error (means USB line down) OR checksum error  1 - Packet Outstanding (pull the packet out first)
	//2 - TX status response packet ready    3 - RX response packet ready     4 - I/O RX Response packet ready
	//5 - Modem Status response packet ready     6 - AT Command response packet ready     7 - Remote AT Command response packet ready
	int getData();

	//the following functions assemble the RXDataBuffer contents into XBee RX packet types.  
    //As before, return of 0: Error, return of 1: Success.
	int ConvertToTXStatus(TXStatusResponse& inputpacket, unsigned char* dataholder, int arrsize);  
	int ConvertToRXResponse(RXResponse& inputpacket, unsigned char* dataholder, int arrsize);
	int ConvertToIORX(IORXResponse& inputpacket, unsigned char* dataholder, int arrsize);
	int ConvertToRXModem(RXModemStatus& inputpacket, unsigned char* dataholder, int arrsize);
	int ConvertToATRX(ATCommandRX& inputpacket, unsigned char* dataholder, int arrsize);
	int ConvertToRemoteATRX(RemoteATCommandRX& inputpacket, unsigned char* dataholder, int arrsize);
};
//=============================================================================================================================
