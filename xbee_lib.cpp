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

#include "stdafx.h"

using namespace std;

BasicXBee::BasicXBee()
{
}

unsigned int BasicXBee::get64BitAddrMSB()
{
	return MSB64BitAddr;
}

int BasicXBee::set64BitAddrMSB(unsigned int msb)
{
	MSB64BitAddr = msb;
	return 1;
}

unsigned int BasicXBee::get64BitAddrLSB()
{
	return LSB64BitAddr;
}

int BasicXBee::set64BitAddrLSB(unsigned int lsb)
{
	LSB64BitAddr = lsb;
	return 1;
}

int BasicXBee::PrepareRX()
{
	checksum = 0;
	RXcommandID = 0;
	LengthField = 0;
	frameLength = 0;
	return 1;
}

unsigned char BasicXBee::getRXCommandID()
{
	return RXcommandID;
}

int BasicXBee::setRXCommandID(unsigned char id)
{
	RXcommandID = id;
	return 1;
}

unsigned short BasicXBee::getRXLength()
{
	return LengthField;
}

int BasicXBee::setRXLength(unsigned short size)
{
	LengthField = size;
	return 1;
}

unsigned char BasicXBee::getRXFrameDataLength()
{
	return frameLength;
}

int BasicXBee::setRXFrameDataLength(unsigned char flength)
{
	frameLength = flength;
	return 1;
}

unsigned char* BasicXBee::getRXFrameData()
{
	return RXframeData;
}

int BasicXBee::setRXFrameData(unsigned char* Data)
{
	RXframeData = Data;
	return 1;
}

unsigned char BasicXBee::getCSum()
{
	return checksum;
}

int BasicXBee::setCSum(unsigned char csum)
{
	checksum = csum;
	return 1;
}

int BasicXBee::PrepareTX(unsigned char commID, unsigned char frID)
{
	TXcommandID = commID;
	TXFrameID = frID;
	return 1;
}

unsigned char BasicXBee::getTXCommandID()
{
	return TXcommandID;
}

int BasicXBee::setTXCommandID(unsigned char id)
{
	TXcommandID = id;
	return 1;
}

unsigned char BasicXBee::getTXFrameID()
{
	return TXFrameID;
}

int BasicXBee::setTXFrameID(unsigned char frameId)
{
	TXFrameID = frameId;
	return 1;
}

unsigned char BasicXBee::getTXFrameDataLength()
{
	return TXframeLength;
}

int BasicXBee::setTXFrameDataLength(unsigned char flength)
{
	TXframeLength = flength;
	return 1;
}

unsigned char* BasicXBee::getTXFrameData()
{
	return TXframeData;
}

int BasicXBee::setTXFrameData(unsigned char* Data)
{
	TXframeData = Data;
	return 1;
}

TXStatusResponse::TXStatusResponse()
{
}

unsigned char TXStatusResponse::getFrameID()
{
	return getRXFrameData()[0];
}

unsigned short TXStatusResponse::getRemoteAddress()
{
	unsigned short temp = getRXFrameData()[1] << 8;
	return (temp + getRXFrameData()[2]);
}

unsigned char TXStatusResponse::getTxRetryCount()
{
	return getRXFrameData()[3];
}

unsigned char TXStatusResponse::getDeliveryStatus()
{
	return getRXFrameData()[4];
}

unsigned char TXStatusResponse::getDiscoveryStatus()
{
	return getRXFrameData()[5];
}

bool TXStatusResponse::isResponseHealthy()
{
	if (getDeliveryStatus() == SUCCESS)
		return TRUE;
	else
		return FALSE;
}

RXResponse::RXResponse()
{
}

unsigned short RXResponse::getRemoteAddress()
{
	unsigned short temp = getRXFrameData()[8] << 8;
	return (temp + getRXFrameData()[9]);
}

unsigned char RXResponse::getOptions()
{
	return getRXFrameData()[10];
}

unsigned char RXResponse::getLengthOfData()
{
	return getRXLength() - 12;	
}

unsigned char* RXResponse::getRFDataField()
{
	return getRXFrameData() + 11;
}

IORXResponse::IORXResponse()
{
}

int IORXResponse::PullMasks(unsigned short digi, unsigned char ana)
{
	Digital_Mask = digi;
	Analog_Mask = ana;
	return 1;
}

unsigned short IORXResponse::getRemoteAddress()
{
	unsigned short temp = getRXFrameData()[8] << 8;
	return (temp + getRXFrameData()[9]);
}

unsigned char IORXResponse::getOptions()
{
	return getRXFrameData()[10];
}

int IORXResponse::what_IO_is_enabled()
{
	//first check if the number of samples field is set to 1, if not, return error (0)
	if (getRXFrameData()[11] != 0x1)
		return 0;

	if (Digital_Mask > 0 && Analog_Mask > 0) //digital and analog IO
		return 1;

	if (Digital_Mask > 0) //digital IO
		return 2;

	if (Analog_Mask > 0) //analog IO
		return 3;
}

unsigned char* IORXResponse::sampledIOData()
{
	return getRXFrameData() + 15;
}

unsigned short IORXResponse::getDigMask()
{
	return Digital_Mask;
}

unsigned char IORXResponse::getAnalogMask()
{
	return Analog_Mask;
}

RXModemStatus::RXModemStatus()
{
}

unsigned char RXModemStatus::getModuleStatus()
{
	return getRXFrameData()[0];
}

ATCommandRX::ATCommandRX()
{
}

unsigned char ATCommandRX::getFrameID()
{
	return getRXFrameData()[0];
}

unsigned short ATCommandRX::getATCommand()
{
	unsigned short temp = getRXFrameData()[1] << 8;
	return (temp + getRXFrameData()[2]); 
}

unsigned char ATCommandRX::getStatus()
{
	return getRXFrameData()[3];
}

unsigned char ATCommandRX::getValueFieldLength()
{
	return getRXFrameDataLength() - 4;
}

unsigned char* ATCommandRX::getValueField()
{
	if (getValueFieldLength() > 0)
		return getRXFrameData() + 4;
	else
		return NULL;
}

bool ATCommandRX::isResponseHealthy()
{
	if (getStatus() == OK)
		return TRUE;
	else
		return FALSE;
}

RemoteATCommandRX::RemoteATCommandRX()
{
}

unsigned char RemoteATCommandRX::getFrameID()
{
	return getRXFrameData()[0];
}

unsigned short RemoteATCommandRX::getRemoteAddress()
{
	unsigned short temp = getRXFrameData()[9] << 8;
	return (temp + getRXFrameData()[10]); 
}

unsigned short RemoteATCommandRX::getATCommand()
{
	unsigned short temp = getRXFrameData()[11] << 8;
	return (temp + getRXFrameData()[12]); 
}

unsigned char RemoteATCommandRX::getStatus()
{
	return getRXFrameData()[13];
}

unsigned char RemoteATCommandRX::getValueFieldLength()
{
	return getRXFrameDataLength() - 14;
}

unsigned char* RemoteATCommandRX::getValueField()
{
	if (getValueFieldLength() > 0)
		return getRXFrameData() + 14;
	else
		return NULL;
}

bool RemoteATCommandRX::isResponseHealthy()
{
	if (getStatus() == OK)
		return TRUE;
	else
		return FALSE;
}

TXRequest::TXRequest()
{
}

unsigned short TXRequest::getRemoteAddress()
{
	return Network16BitAddr;
}

int TXRequest::setRemoteAddress(unsigned short addr)
{
	Network16BitAddr = addr;
	return 1;
}

unsigned char TXRequest::getBroadcastRadius()
{
	return BRadius;
}

int TXRequest::setBroadcastRadius(unsigned char radius)
{
	BRadius = radius;
	return 1;
}

unsigned char TXRequest::getOptions()
{
	return OptionsField;
}

int TXRequest::setOptions(unsigned char ops)
{
	OptionsField = ops;
	return 1;
}

unsigned char TXRequest::getDataLength()
{
	return DataLength;
}

int TXRequest::setDataLength(unsigned char length)
{
	DataLength = length;
	return 1;
}

unsigned char* TXRequest::getDataPtr()
{
	return Data;
}

int TXRequest::setDataPtr(unsigned char *ptr)
{
	Data = ptr;
	return 1;
}

ATCommandTX::ATCommandTX()
{
	ParameterValueLength = 0;
	ParameterValue = NULL;
}

unsigned short ATCommandTX::getATCommand()
{
	return AT_Command;
}

int ATCommandTX::setATCommand(unsigned short atcom)
{
	AT_Command = atcom;
	return 1;
}

unsigned char ATCommandTX::getParameterLength()
{
	return ParameterValueLength;
}

int ATCommandTX::setParameterLength(unsigned char length)
{
	ParameterValueLength = length;
	return 1;
}

unsigned char* ATCommandTX::getParameterField()
{
	return ParameterValue;
}

int ATCommandTX::setParameterField(unsigned char *param)
{
	ParameterValue = param;
	return 1;
}

RemoteATCommandTX::RemoteATCommandTX()
{
	ParameterValueLength = 0;
	ParameterValue = NULL;
}

unsigned short RemoteATCommandTX::get16RemoteAddr()
{
	return The16BitAddr;
}

int RemoteATCommandTX::set16RemoteAddr(unsigned short addr)
{
	The16BitAddr = addr;
	return 1;
}

unsigned char RemoteATCommandTX::getCommandOps()
{
	return commandOptions;
}

int RemoteATCommandTX::setCommandOps(unsigned char comops)
{
	commandOptions = comops;
	return 1;
}

aUARTDevice::aUARTDevice()
{
	FrameIDCounter = 1;
	temp_CSum = 0;
	current_read_position = 0;
	ReadTimer = 5000;
	WriteTimer = 2000;
	packet_outstanding = FALSE;
}

int aUARTDevice::CompileDeviceInfo()
{ 
	//Start off by creating a device information list 
	status = FT_CreateDeviceInfoList(&NumberOfConnectedDevices); 
	
	if (status != FT_OK) 
		return 0;

	if (NumberOfConnectedDevices > 0) 
	{ 
		DeviceInfoArray = new FT_DEVICE_LIST_INFO_NODE[NumberOfConnectedDevices]; 
		
		//populate the device info nodes with data
		status = FT_GetDeviceInfoList(DeviceInfoArray, &NumberOfConnectedDevices); 

		if (status != FT_OK) 
		return 0;
	}

	return 1;
}

unsigned int aUARTDevice::getNumberofConnectedDevices()
{
	return (unsigned int) NumberOfConnectedDevices;
}

int aUARTDevice::clearDeviceInfoNodes()
{
	delete [] DeviceInfoArray;
	return 1;
}

int aUARTDevice::getDeviceFlags(int deviceindex, DWORD* storage)
{
	if ((unsigned int) deviceindex >= NumberOfConnectedDevices)
		return 0;
	else
	{
		(*storage) = DeviceInfoArray[deviceindex].Flags; 
		return 1;
	}
}

int aUARTDevice::getDeviceID(int deviceindex, DWORD* storage)
{
	if ((unsigned int) deviceindex >= NumberOfConnectedDevices)
		return 0;
	else
	{
		(*storage) = DeviceInfoArray[deviceindex].ID; 
		return 1;
	}
}

int aUARTDevice::getDeviceType(int deviceindex, DWORD* storage)
{
	if ((unsigned int) deviceindex >= NumberOfConnectedDevices)
		return 0;
	else
	{
		(*storage) = DeviceInfoArray[deviceindex].Type; 
		return 1;
	}
}

int aUARTDevice::getDeviceLocID(int deviceindex, DWORD* storage)
{
	if ((unsigned int) deviceindex >= NumberOfConnectedDevices)
		return 0;
	else
	{
		(*storage) = DeviceInfoArray[deviceindex].LocId; 
		return 1;
	}
}

int aUARTDevice::getDeviceSerialNum(int deviceindex, char* storage)
{
	if ((unsigned int) deviceindex >= NumberOfConnectedDevices)
		return 0;
	else
	{
		for (int i = 0; i < 16; i++)
		{
			storage[i] = DeviceInfoArray[deviceindex].SerialNumber[i];	
		}
		return 1;
	}
}

int aUARTDevice::getDeviceDescription(int deviceindex, char* storage)
{
	if ((unsigned int) deviceindex >= NumberOfConnectedDevices)
		return 0;
	else
	{
		for (int i = 0; i < 64; i++)
		{
			storage[i] = DeviceInfoArray[deviceindex].Description[i];	
		}
		return 1;
	}
}

int aUARTDevice::InitiateConnectionToDevice(int type, PVOID value)
{
	if (type == 0)
	{
		DeviceAccessMethod = FT_OPEN_BY_SERIAL_NUMBER;
	    AccessMethodValue = value;
		status = FT_OpenEx(AccessMethodValue, DeviceAccessMethod, &USBHandle);

		if (status == FT_OK)
			return 1;
		else
			return 0;
	}

	if (type == 1)
	{
		DeviceAccessMethod = FT_OPEN_BY_DESCRIPTION;
	    AccessMethodValue = value;
		status = FT_OpenEx(AccessMethodValue, DeviceAccessMethod, &USBHandle);

		if (status == FT_OK)
			return 1;
		else
			return 0;
	}

	if (type == 2)
	{
		DeviceAccessMethod = FT_OPEN_BY_LOCATION;
	    AccessMethodValue = value;
		status = FT_OpenEx(AccessMethodValue, DeviceAccessMethod, &USBHandle);

		if (status == FT_OK)
			return 1;
		else
			return 0;
	}

	return 0;
}

int aUARTDevice::TerminateConnectionToDevice()
{
	status = FT_Close(USBHandle);

	if (status == FT_OK)
		return 1;
	else
		return 0;
}

int aUARTDevice::SetDeviceBaudRate(DWORD rate)
{
	UARTBaudRate = rate;
	status = FT_SetBaudRate(USBHandle, UARTBaudRate);

	if (status == FT_OK)
		return 1;
	else
		return 0;
}

int aUARTDevice::SetSerialCOMSpecs(int type1, int type2, int type3)
{
	if (type1 != 0)
	{
		if (type1 != 1)
			return 0;
		else
			UARTWordLength = FT_BITS_7;
	}
	else
	{
		UARTWordLength = FT_BITS_8;
	}

	if (type2 != 0)
	{
		if (type2 != 1)
			return 0;
		else
			UARTStopBits = FT_STOP_BITS_2;
	}
	else
	{
		UARTStopBits = FT_STOP_BITS_1;
	}

	if (type3 > 4 || type3 < 0)
		return 0;
	else
	{
		if (type3 == 0)
			UARTParity = FT_PARITY_NONE;

		if (type3 == 1)
			UARTParity = FT_PARITY_ODD;

		if (type3 == 2)
			UARTParity = FT_PARITY_EVEN;

		if (type3 == 3)
			UARTParity = FT_PARITY_MARK;

		if (type3 == 4)
			UARTParity = FT_PARITY_SPACE;
	}

	status = FT_SetDataCharacteristics(USBHandle, UARTWordLength, UARTStopBits, UARTParity);

	if (status == FT_OK)
		return 1;
	else
		return 0;
}

int aUARTDevice::SetRXTXTimeouts(DWORD rxtout, DWORD txtout)
{
	ReadTimer = rxtout;
	WriteTimer = txtout;
	
	status = FT_SetTimeouts(USBHandle, ReadTimer, WriteTimer);

	if (status == FT_OK)
		return 1;
	else
		return 0;
}

int aUARTDevice::SetFlowSpecs(int type1, unsigned char ONCh, unsigned char OFFCh)
{
	if (type1 > 3 || type1 < 0)
		return 0;
	else
	{
		if (type1 == 0)
			UARTFlowControl = FT_FLOW_NONE;
		
		if (type1 == 1)
			UARTFlowControl = FT_FLOW_RTS_CTS;

		if (type1 == 2)
			UARTFlowControl = FT_FLOW_DTR_DSR;

		if (type1 == 3)
			UARTFlowControl = FT_FLOW_XON_XOFF;
	}

	if (UARTFlowControl != FT_FLOW_XON_XOFF)
	{
		status = FT_SetFlowControl(USBHandle, UARTFlowControl, ONCh, OFFCh);
	}
	else
	{
		ONChar = ONCh;
		OFFChar = OFFCh;
		status = FT_SetFlowControl(USBHandle, UARTFlowControl, ONChar, OFFChar);
	}

	if (status == FT_OK)
		return 1;
	else
		return 0;
}

int aUARTDevice::QueryModemAndLineStatus()
{
	DWORD temp;
	status = FT_GetModemStatus(USBHandle, &temp);

	if (status == FT_OK)
	{
		ModemStatus = (unsigned char) (temp & 0x000000FF);
		LineStatus = (unsigned char) ((temp >> 8) & 0x000000FF);
		return 1;
	}
	else
		return 0;
}

unsigned char aUARTDevice::getLineStatus()
{
	return LineStatus;
}

unsigned char aUARTDevice::getModemStatus()
{
	return ModemStatus;
}

int aUARTDevice::QueryUARTBuffers()
{
	status = FT_GetStatus(USBHandle, &BytesInRXBuffer, &BytesInTXBuffer, &UARTEvent);
	
	if (status == FT_OK)
		return 1;
	else
		return 0;
}

unsigned int aUARTDevice::getBytesInRXBuffer()
{
	return (unsigned int) BytesInRXBuffer;
}

unsigned int aUARTDevice::getBytesInTXBuffer()
{
	return (unsigned int) BytesInTXBuffer;
}

int aUARTDevice::ClearUARTBuffers(int type)
{
	if (type > 2 || type < 0)
		return 0;
	else
	{
		if (type == 0)
			PurgeMask = FT_PURGE_RX;

		if (type == 1)
			PurgeMask = FT_PURGE_TX;

		if (type == 2)
			PurgeMask = FT_PURGE_RX | FT_PURGE_TX;
	}

	status = FT_Purge(USBHandle, PurgeMask);

	if (status == FT_OK)
		return 1;
	else
		return 0;
}

unsigned char aUARTDevice::ProduceFrameID()
{
	//frame IDs can ONLY cycle between 1 to 255
	if (FrameIDCounter == 0xFF || FrameIDCounter == 0x0) 
		FrameIDCounter = 0x1;

	FrameIDCounter++;

	return FrameIDCounter;
}

unsigned char* aUARTDevice::PtrToTXDataBuffer()
{
	return TXDataBuffer;
}

int aUARTDevice::SendData(int size)
{
	int temp;
	temp = SetRXTXTimeouts(ReadTimer, WriteTimer);

	if (temp != 1)
		return 0;

	BytesToWrite = (DWORD) size;
	status = FT_Write(USBHandle, TXDataBuffer, BytesToWrite, &BytesWritten);

	if (status == FT_OK && BytesToWrite == BytesWritten)
	{
		temp = QueryUARTBuffers();
		
		if (temp != 1)
			return 0;

		while (getBytesInTXBuffer() != 0)
		{
			//wait for UART TX BUFFER to empty
		}

		temp = ClearUARTBuffers(1);
		if (temp != 1)
			return 0;
		else
			return 1;
	}	
	else
		return 0;
}

int aUARTDevice::getData()
{
	int temp;
	unsigned char RX_byte_store[1];
	bool escape_next = FALSE;
	int return_type;
	unsigned short len;

	if (packet_outstanding == TRUE)
		return 1;
	else
	{
		current_read_position = 0;
		temp_CSum = 0;

		temp = SetRXTXTimeouts(ReadTimer, WriteTimer);

		if (temp != 1)
			return 0;

		BytesToRead = 0x1;

		while (packet_outstanding == FALSE)
		{
			status = FT_Read(USBHandle, RX_byte_store, BytesToRead, &BytesReceived);

			if (status == FT_OK)
			{
				if (BytesReceived == 0x1)
				{
					if (current_read_position >= 0 && RX_byte_store[0] == START_BYTE)
					{
						RXDataBuffer[0] = START_BYTE;
						current_read_position = 1;
						temp_CSum = 0;
					}
					else
					{
						if (current_read_position > 0 && RX_byte_store[0] == ESCAPE_BYTE)
						{	
							escape_next = TRUE;
						}
						else
						{
							if (escape_next == TRUE)
							{
								RXDataBuffer[current_read_position] = RX_byte_store[0] ^ 0x20;
							    escape_next = FALSE;

								if (current_read_position > 2)
									temp_CSum += RXDataBuffer[current_read_position];
							}
							else
							{
								RXDataBuffer[current_read_position] = RX_byte_store[0];

								if (current_read_position > 2)
									temp_CSum += RXDataBuffer[current_read_position];
							}

							if (current_read_position < 3)
								current_read_position++;
							else
							{
								//check for a completed packet
								len = RXDataBuffer[1] << 8;
								len = len + RXDataBuffer[2];
								if (len == (current_read_position - 3))
								{
									//verify checksum
									if (temp_CSum != 0xFF)
										return 0;
									
									packet_outstanding = TRUE;
									
									if (RXDataBuffer[3] == ZIGBEE_TX_STATUS)
										return_type = 2;
									if (RXDataBuffer[3] == ZIGBEE_RX_PACKET)
										return_type = 3;
									if (RXDataBuffer[3] == ZIGBEE_IO_RX)
										return_type = 4;
									if (RXDataBuffer[3] == MODEM_STATUS)
										return_type = 5;
									if (RXDataBuffer[3] == AT_COMMAND_RESPONSE)
										return_type = 6;
									if (RXDataBuffer[3] == REMOTE_AT_COMMAND_RESPONSE)
										return_type = 7;
								}
								else
									current_read_position++;
							}
						}
					}
				}
				else
				{
					//do NOTHING!
				}
			}
			else
				return 0;
		}

		return return_type;
	}
}

int aUARTDevice::ConvertToTXStatus(TXStatusResponse& inputpacket, unsigned char* dataholder, int arrsize)
{
	int temp;

	if (packet_outstanding == TRUE && RXDataBuffer[3] == ZIGBEE_TX_STATUS) 
	{
		int framesizetemp = (current_read_position + 1) - 5;

		if (arrsize < framesizetemp) //not enough space, more memory needed
			return 0;

		temp = inputpacket.setRXCommandID(RXDataBuffer[3]);

		if (temp != 1)
			return 0;

		unsigned short temp_length = RXDataBuffer[1] << 8;
		temp_length = temp_length + RXDataBuffer[2];
		temp = inputpacket.setRXLength(temp_length);

		if (temp != 1)
			return 0;

		temp = inputpacket.setRXFrameDataLength((unsigned char) framesizetemp);

		if (temp != 1)
			return 0;

		for (int i = 0; i < framesizetemp; i++)
		{
			dataholder[i] = RXDataBuffer[i + 4];
		}

		temp = inputpacket.setRXFrameData(dataholder);

		if (temp != 1)
			return 0;

		temp = inputpacket.setCSum(RXDataBuffer[current_read_position]);

		if (temp != 1)
			return 0;

		packet_outstanding = FALSE;
		return 1;
	}
	else
		return 0;
}

int aUARTDevice::ConvertToRXResponse(RXResponse &inputpacket, unsigned char *dataholder, int arrsize)
{
	int temp;

	if (packet_outstanding == TRUE && RXDataBuffer[3] == ZIGBEE_RX_PACKET) 
	{
		int framesizetemp = (current_read_position + 1) - 5;

		if (arrsize < framesizetemp) //not enough space, more memory needed
			return 0;

		temp = inputpacket.setRXCommandID(RXDataBuffer[3]);

		if (temp != 1)
			return 0;

		unsigned short temp_length = RXDataBuffer[1] << 8;
		temp_length = temp_length + RXDataBuffer[2];
		temp = inputpacket.setRXLength(temp_length);

		if (temp != 1)
			return 0;

		temp = inputpacket.setRXFrameDataLength((unsigned char) framesizetemp);

		if (temp != 1)
			return 0;

		for (int i = 0; i < framesizetemp; i++)
		{
			dataholder[i] = RXDataBuffer[i + 4];
		}

		temp = inputpacket.setRXFrameData(dataholder);

		if (temp != 1)
			return 0;

		temp = inputpacket.setCSum(RXDataBuffer[current_read_position]);

		if (temp != 1)
			return 0;

		unsigned int msb64 = (inputpacket.getRXFrameData()[0] << 24) + (inputpacket.getRXFrameData()[1] << 16) + (inputpacket.getRXFrameData()[2] << 8) + inputpacket.getRXFrameData()[3];
        unsigned int lsb64 = (inputpacket.getRXFrameData()[4] << 24) + (inputpacket.getRXFrameData()[5] << 16) + (inputpacket.getRXFrameData()[6] << 8) + inputpacket.getRXFrameData()[7];

		temp = inputpacket.set64BitAddrMSB(msb64);

		if (temp != 1)
			return 0;

		temp = inputpacket.set64BitAddrLSB(lsb64);

		if (temp != 1)
			return 0;

		packet_outstanding = FALSE;
		return 1;
	}
	else
		return 0;
}

int aUARTDevice::ConvertToIORX(IORXResponse &inputpacket, unsigned char *dataholder, int arrsize)
{
	int temp;

	if (packet_outstanding == TRUE && RXDataBuffer[3] == ZIGBEE_IO_RX) 
	{
		int framesizetemp = (current_read_position + 1) - 5;

		if (arrsize < framesizetemp) //not enough space, more memory needed
			return 0;

		temp = inputpacket.setRXCommandID(RXDataBuffer[3]);

		if (temp != 1)
			return 0;

		unsigned short temp_length = RXDataBuffer[1] << 8;
		temp_length = temp_length + RXDataBuffer[2];
		temp = inputpacket.setRXLength(temp_length);

		if (temp != 1)
			return 0;

		temp = inputpacket.setRXFrameDataLength((unsigned char) framesizetemp);

		if (temp != 1)
			return 0;

		for (int i = 0; i < framesizetemp; i++)
		{
			dataholder[i] = RXDataBuffer[i + 4];
		}

		temp = inputpacket.setRXFrameData(dataholder);

		if (temp != 1)
			return 0;

		temp = inputpacket.setCSum(RXDataBuffer[current_read_position]);

		if (temp != 1)
			return 0;

		unsigned int msb64 = (inputpacket.getRXFrameData()[0] << 24) + (inputpacket.getRXFrameData()[1] << 16) + (inputpacket.getRXFrameData()[2] << 8) + inputpacket.getRXFrameData()[3];
        unsigned int lsb64 = (inputpacket.getRXFrameData()[4] << 24) + (inputpacket.getRXFrameData()[5] << 16) + (inputpacket.getRXFrameData()[6] << 8) + inputpacket.getRXFrameData()[7];

		temp = inputpacket.set64BitAddrMSB(msb64);

		if (temp != 1)
			return 0;

		temp = inputpacket.set64BitAddrLSB(lsb64);

		if (temp != 1)
			return 0;

		unsigned short mask_d = ( (inputpacket.getRXFrameData()[12] << 8) + inputpacket.getRXFrameData()[13] ) & 0x1CFF;
		unsigned char mask_a = ( inputpacket.getRXFrameData()[14] ) & 0x8F;

		temp = inputpacket.PullMasks(mask_d, mask_a);

		if (temp != 1)
			return 0;

		packet_outstanding = FALSE;
		return 1;
	}
	else
		return 0;
}

int aUARTDevice::ConvertToRXModem(RXModemStatus &inputpacket, unsigned char *dataholder, int arrsize)
{
	int temp;

	if (packet_outstanding == TRUE && RXDataBuffer[3] == MODEM_STATUS) 
	{
		int framesizetemp = (current_read_position + 1) - 5;

		if (arrsize < framesizetemp) //not enough space, more memory needed
			return 0;

		temp = inputpacket.setRXCommandID(RXDataBuffer[3]);

		if (temp != 1)
			return 0;

		unsigned short temp_length = RXDataBuffer[1] << 8;
		temp_length = temp_length + RXDataBuffer[2];
		temp = inputpacket.setRXLength(temp_length);

		if (temp != 1)
			return 0;

		temp = inputpacket.setRXFrameDataLength((unsigned char) framesizetemp);

		if (temp != 1)
			return 0;

		for (int i = 0; i < framesizetemp; i++)
		{
			dataholder[i] = RXDataBuffer[i + 4];
		}

		temp = inputpacket.setRXFrameData(dataholder);

		if (temp != 1)
			return 0;

		temp = inputpacket.setCSum(RXDataBuffer[current_read_position]);

		if (temp != 1)
			return 0;

		packet_outstanding = FALSE;
		return 1;
	}
	else
		return 0;
}

int aUARTDevice::ConvertToATRX(ATCommandRX &inputpacket, unsigned char *dataholder, int arrsize)
{
	int temp;

	if (packet_outstanding == TRUE && RXDataBuffer[3] == AT_COMMAND_RESPONSE) 
	{
		int framesizetemp = (current_read_position + 1) - 5;

		if (arrsize < framesizetemp) //not enough space, more memory needed
			return 0;

		temp = inputpacket.setRXCommandID(RXDataBuffer[3]);

		if (temp != 1)
			return 0;

		unsigned short temp_length = RXDataBuffer[1] << 8;
		temp_length = temp_length + RXDataBuffer[2];
		temp = inputpacket.setRXLength(temp_length);

		if (temp != 1)
			return 0;

		temp = inputpacket.setRXFrameDataLength((unsigned char) framesizetemp);

		if (temp != 1)
			return 0;

		for (int i = 0; i < framesizetemp; i++)
		{
			dataholder[i] = RXDataBuffer[i + 4];
		}

		temp = inputpacket.setRXFrameData(dataholder);

		if (temp != 1)
			return 0;

		temp = inputpacket.setCSum(RXDataBuffer[current_read_position]);

		if (temp != 1)
			return 0;

		packet_outstanding = FALSE;
		return 1;
	}
	else
		return 0;
}

int aUARTDevice::ConvertToRemoteATRX(RemoteATCommandRX &inputpacket, unsigned char *dataholder, int arrsize)
{
	int temp;

	if (packet_outstanding == TRUE && RXDataBuffer[3] == REMOTE_AT_COMMAND_RESPONSE) 
	{
		int framesizetemp = (current_read_position + 1) - 5;

		if (arrsize < framesizetemp) //not enough space, more memory needed
			return 0;

		temp = inputpacket.setRXCommandID(RXDataBuffer[3]);

		if (temp != 1)
			return 0;

		unsigned short temp_length = RXDataBuffer[1] << 8;
		temp_length = temp_length + RXDataBuffer[2];
		temp = inputpacket.setRXLength(temp_length);

		if (temp != 1)
			return 0;

		temp = inputpacket.setRXFrameDataLength((unsigned char) framesizetemp);

		if (temp != 1)
			return 0;

		for (int i = 0; i < framesizetemp; i++)
		{
			dataholder[i] = RXDataBuffer[i + 4];
		}

		temp = inputpacket.setRXFrameData(dataholder);

		if (temp != 1)
			return 0;

		temp = inputpacket.setCSum(RXDataBuffer[current_read_position]);

		if (temp != 1)
			return 0;

		unsigned int msb64 = (inputpacket.getRXFrameData()[1] << 24) + (inputpacket.getRXFrameData()[2] << 16) + (inputpacket.getRXFrameData()[3] << 8) + inputpacket.getRXFrameData()[4];
        unsigned int lsb64 = (inputpacket.getRXFrameData()[5] << 24) + (inputpacket.getRXFrameData()[6] << 16) + (inputpacket.getRXFrameData()[7] << 8) + inputpacket.getRXFrameData()[8];

		temp = inputpacket.set64BitAddrMSB(msb64);

		if (temp != 1)
			return 0;

		temp = inputpacket.set64BitAddrLSB(lsb64);

		if (temp != 1)
			return 0;

		packet_outstanding = FALSE;
		return 1;
	}
	else
		return 0;
}
