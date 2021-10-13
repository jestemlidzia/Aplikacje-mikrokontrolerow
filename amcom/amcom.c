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
        //initialization
       receiver->packetHandler = packetHandlerCallback;
       receiver->userContext = userContext;
    return;

}

size_t AMCOM_Serialize(uint8_t packetType, const void* payload, size_t payloadSize, uint8_t* destinationBuffer) {
	
	//header
	AMCOM_PacketHeader header;
    header.sop = AMCOM_SOP;
    header.type = packetType;
    header.length = payloadSize;
    
    //header crc
    header.crc = AMCOM_INITIAL_CRC;
    header.crc = AMCOM_UpdateCRC(packetType, header.crc);
    header.crc = AMCOM_UpdateCRC(payloadSize, header.crc);
    
    //payload crc
    size_t i = 0;
    uint8_t* payLoad = (uint8_t*)payload;
    while(i != payloadSize){
        header.crc = AMCOM_UpdateCRC(*payLoad++, header.crc);
        i++;
    };
    
    //copy the header and data to the destination buffer
    memcpy(destinationBuffer, &header, sizeof(header));
    memcpy(destinationBuffer+sizeof(header), (uint8_t*)payload, payloadSize);
    
	return sizeof(header)+payloadSize;
}

void AMCOM_Deserialize(AMCOM_Receiver* receiver, const void* data, size_t dataSize) {
    
    uint8_t* byte = (uint8_t*)data;
    
    for(size_t i = 0; i < dataSize; i++){
        
        switch(receiver->receivedPacketState){
            //the structure is empty
            case AMCOM_PACKET_STATE_EMPTY:
                
                if(*byte == AMCOM_SOP){
                    receiver->receivedPacket.header.sop = *byte++;
                    receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_SOP;
                }
                
            break;
            //received 0xA1
            case AMCOM_PACKET_STATE_GOT_SOP:
                
                receiver->receivedPacket.header.type = *byte++;
                receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_TYPE;
                
            break;
            //received type byte
            case AMCOM_PACKET_STATE_GOT_TYPE:
                
                if(*byte > 200){ //if the value is too large return to the empty state
                    receiver->receivedPacketState = AMCOM_PACKET_STATE_EMPTY;
                    break;
                }
                
                receiver->receivedPacket.header.length = *byte++;
                receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_LENGTH;
                
            break;
            //received byte = 0...200
            case AMCOM_PACKET_STATE_GOT_LENGTH:
            
                receiver->receivedPacket.header.crc = *byte++; //crc not complete, only low byte
                receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_CRC_LO;
                
            break;
            //received crc low byte
            case AMCOM_PACKET_STATE_GOT_CRC_LO:
                
                receiver->receivedPacket.header.crc |= ((*byte++)<<8); //crc complete
                
                if(receiver->receivedPacket.header.length == 0){ //LENGTH = 0
                    receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_WHOLE_PACKET;
                    i--;
                }
                else{ //LENGTH > 0
                    receiver->receivedPacketState = AMCOM_PACKET_STATE_GETTING_PAYLOAD;
                }
            break;
            //getting payload
            case AMCOM_PACKET_STATE_GETTING_PAYLOAD:
                
                receiver->receivedPacket.payload[receiver->payloadCounter] = *byte++;
                receiver->payloadCounter++;
                
                //checking if the end of data
                if (receiver->payloadCounter == receiver->receivedPacket.header.length)
                {
                    receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_WHOLE_PACKET;
                    i--;
                }
                
            break;
            //got whole packet
            case AMCOM_PACKET_STATE_GOT_WHOLE_PACKET: ;
                
                //crc
                uint16_t crc = AMCOM_INITIAL_CRC;
                crc = AMCOM_UpdateCRC( receiver->receivedPacket.header.type, crc);
                crc = AMCOM_UpdateCRC( receiver->receivedPacket.header.length, crc);
                
                size_t j = 0;
                while(j != receiver->payloadCounter){
                    crc = AMCOM_UpdateCRC(receiver->receivedPacket.payload[j++], crc);
                };
                
                if(receiver->receivedPacket.header.crc == crc){ //comparing crc
                    receiver->packetHandler(&(receiver->receivedPacket), receiver->userContext);
                }
                
                //reset
                receiver->payloadCounter = 0;
                receiver->receivedPacketState = AMCOM_PACKET_STATE_EMPTY;

            break;
        
        }
    }

    
}
