#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "amcom.h"

/// Start of packet character
const uint8_t  AMCOM_SOP         = 0xA1;
const uint16_t AMCOM_INITIAL_CRC = 0xFFFF;

static uint16_t AMCOM_UpdateCRC(uint8_t byte, uint16_t crc)
{
	byte ^= (uint8_t)(crc & 0x00ff);
	byte ^= (uint8_t)(byte << 4);
	return ((((uint16_t)byte << 8) | (uint8_t)(crc >> 8)) ^ (uint8_t)(byte >> 4) ^ ((uint16_t)byte << 3));
}


void AMCOM_InitReceiver(AMCOM_Receiver* receiver, AMCOM_PacketHandler packetHandlerCallback, void* userContext) {
    receiver->payloadCounter = 0;
    receiver->receivedPacketState = AMCOM_PACKET_STATE_EMPTY;
	receiver->packetHandler=packetHandlerCallback;
	receiver->userContext = userContext;
}

size_t AMCOM_Serialize(uint8_t packetType, const void* payload, size_t payloadSize, uint8_t* destinationBuffer) {
	
	AMCOM_PacketHeader packet_header;
	packet_header.sop=AMCOM_SOP;
	packet_header.type=packetType;
	packet_header.length=payloadSize;
	
	packet_header.crc=AMCOM_INITIAL_CRC;
	
	packet_header.crc=AMCOM_UpdateCRC(packetType, packet_header.crc);
    packet_header.crc=AMCOM_UpdateCRC(payloadSize, packet_header.crc);
    
    size_t i=0;
    while(i<payloadSize){
        uint8_t *payload_=(uint8_t*)payload;
        packet_header.crc=AMCOM_UpdateCRC(payload_[i], packet_header.crc);
        i++;
    }
    
    memcpy(destinationBuffer, &packet_header, sizeof(packet_header));
    memcpy(destinationBuffer+sizeof(packet_header), payload, payloadSize);
    
	return payloadSize+sizeof(packet_header);
}

void AMCOM_Deserialize(AMCOM_Receiver* receiver, const void* data, size_t dataSize) {
    uint8_t* data_ = (uint8_t*)data;

    for(size_t i=0;i<dataSize;i++){
        
        if(receiver->receivedPacketState==AMCOM_PACKET_STATE_EMPTY){

            if(*data_==AMCOM_SOP){
                receiver->receivedPacket.header.sop=*data_++;
                receiver->receivedPacketState=AMCOM_PACKET_STATE_GOT_SOP;
            }
        }
        
        else if(receiver->receivedPacketState==AMCOM_PACKET_STATE_GOT_SOP){
            receiver->receivedPacket.header.type=*data_++;
            receiver->receivedPacketState=AMCOM_PACKET_STATE_GOT_TYPE;
        }
        
        else if(receiver->receivedPacketState==AMCOM_PACKET_STATE_GOT_TYPE){
            
            if(*data_<=200){
                receiver->receivedPacket.header.length=*data_++;
                receiver->receivedPacketState=AMCOM_PACKET_STATE_GOT_LENGTH;
            }
            else{
                receiver->receivedPacketState=AMCOM_PACKET_STATE_EMPTY;
            }
            
        }
        
        else if(receiver->receivedPacketState==AMCOM_PACKET_STATE_GOT_LENGTH){
            receiver->receivedPacket.header.crc=*data_++;
            receiver->receivedPacketState=AMCOM_PACKET_STATE_GOT_CRC_LO;
        }
        
        else if(receiver->receivedPacketState==AMCOM_PACKET_STATE_GOT_CRC_LO){
            receiver->receivedPacket.header.crc|=((*data_++)<<8);
            if(receiver->receivedPacket.header.length > 0){
                receiver->receivedPacketState=AMCOM_PACKET_STATE_GETTING_PAYLOAD;
            }
            else{
                receiver->receivedPacketState=AMCOM_PACKET_STATE_GOT_WHOLE_PACKET;
                dataSize++;
            }
            
        }
        
        else if(receiver->receivedPacketState==AMCOM_PACKET_STATE_GETTING_PAYLOAD){
            
            receiver->receivedPacket.payload[receiver->payloadCounter]=*data_++;
            receiver->payloadCounter++;
            
            if(receiver->payloadCounter==receiver->receivedPacket.header.length){
                receiver->receivedPacketState=AMCOM_PACKET_STATE_GOT_WHOLE_PACKET;
                dataSize++;
            }
        }
        else{
            uint16_t crc;
	        crc=AMCOM_INITIAL_CRC;
	        crc=AMCOM_UpdateCRC(receiver->receivedPacket.header.type, crc);
            crc=AMCOM_UpdateCRC(receiver->receivedPacket.header.length, crc);
            
            size_t k=0;
            while(k<receiver->payloadCounter){
                crc = AMCOM_UpdateCRC(receiver->receivedPacket.payload[k], crc);
                k++;
            }
            
            if(receiver->receivedPacket.header.crc == crc){
                receiver->packetHandler(&(receiver->receivedPacket), receiver->userContext);
            }
            receiver->payloadCounter = 0;
            receiver->receivedPacketState = AMCOM_PACKET_STATE_EMPTY;
        }
    }
}

