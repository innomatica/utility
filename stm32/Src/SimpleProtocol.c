/**
 * \file
 */
#include "SimpleProtocol.h"
#include <stdbool.h>

// packet decoding state machine states
#define PKT_STATE_HDR           0
#define PKT_STATE_LEN           1
#define PKT_STATE_PLD           2
#define PKT_STATE_CSM           3

/**
 * This function runs the packet decoding state machine. It takes stream of 
 * UART data one byte * at a time and returns the current state of the state
 * machine. Upon successful retrieval of a packet, it fills the array packet[] 
 * with the payload.
 *
 * To use this function, define an uint8 array for the packet storage and call
 * this function whenever new byte is arrived. The best place to put this
 * function is inside of the UART check routine (not inside the interrupt
 * service callback function).
 *
\code
uint8 buffer[MAX_PKTSIZE];
uint16 size;
pkt_status pstatus;
    
if(UART_RX_flag_is_set)
{
    // new data byte is in new_byte
    if(PKT_RECEIVED == SimplePacketDecoder(new_byte, buffer, &size))
    {
        // your packet is in the buffer whose length is size byte
    }
}
\endcode
 *
 * \param	byte received byte
 * \param   packet retrieved packet
 * \param   size size of the packet
 * \return  pkt_status status of the state machine
 * \todo    checksum is ignored for now.
 */
pkt_status SimplePacketDecoder(uint8 byte, uint8 *packet, int *size)
{
    static uint8 state = PKT_STATE_HDR;
    static uint8 len = 0;
    static uint8 index = 0;
    static uint8 payload[MAX_PAYLOAD] = {0};
    
    uint8 i;
    
    // waiting for the header byte
    if(state == PKT_STATE_HDR)
    {
        if(byte == PKT_HEADR)
        {
            // proceed to the next state
            state = PKT_STATE_LEN;
        }
        else if(byte == PKT_ACK)
        {
            // ACK received but do not change the state
            return ACK_RECEIVED;
        }
        else if(byte == PKT_NAK)
        {
            // NAK received but do not change the state
            return NAK_RECEIVED;
        }
        else if(byte == PKT_IAM)
        {
            // IAM received but do not change the state
            return IAM_RECEIVED;
        }        
    }
    // waiting for the length byte
    else if(state == PKT_STATE_LEN)
    {
        // save the length of the payload
        len = byte;
        
        // invalid size
        if(len > MAX_PAYLOAD)
        {
            // start all over
            state = PKT_STATE_HDR;
            // report size error
            return PKT_SIZE_ERR;
        }
        // length byte is valid
        else
        {
            // clear index
            index = 0;
            // clear payload buffer
            for(i = 0; i < MAX_PAYLOAD; i++)
            {
                payload[i] = 0;
            }
            // proceed to the next state
            state = PKT_STATE_PLD;
        }
    }
    // waiting for the payload
    else if(state == PKT_STATE_PLD)
    {
        // collecte data
        payload[index++] = byte;
        // proceed to the next if all payload is collected
        if(index == len)
        {
            state = PKT_STATE_CSM;
        }
    }
    // waiting for the checksum byte
    else if(state == PKT_STATE_CSM)
    {
        /// \todo process the checksum byte
        byte = byte;
        
        // TODO: do something about the checksum
        if(1)
        {
            // copy payload to the packet buffer
            for(i = 0; i < len; i++)
            {
                packet[i] = payload[i];
            }
            // size byte
            *size = (int)len;
            // start all over
            state = PKT_STATE_HDR;
            // valid packet arrived
            return PKT_RECEIVED;
        }
        else
        {
            // start all over
            state = PKT_STATE_HDR;
            // checksum error
            return PKT_CSUM_ERR;            
        }
    }
    
    return NON_RECEIVED;
}

/**
 * Construct a packet from given command class, command, and data
 *
\code
int size;
uint8 dat[4];
utin8 packet[MAX_PKTSIZE];

# pack signed int32 value in big endian mode
dat[0] = (value >> 24) & 0xff;
dat[1] = (value >> 16) & 0xff;
dat[2] = (value >> 8) & 0xff;
dat[3] = (value) & 0xff;

# construct a packet
size = SimplePacketEncoder(CCL_GR_HOST, RPT_S32XXX, dat, 4, packet);
\endcode
 *
 * \param   ccl command class code
 * \param   cmd command code
 * \param   dat data array
 * \param   size size of the data array
 * \param   packet resulting packet
 * \return  int the size of the resulting packet
 * \todo    checksum is ignored for now
 */
int SimplePacketEncoder(uint8 ccl, uint8 cmd, uint8 *dat, int size, uint8 *packet)
{
    uint8 csum = 0;
    int i;
    
    // header
    packet[0] = PKT_HEADR;
    // payload size
    packet[1] = size + 2;
    // command type
    packet[2] = ccl;
    // command
    packet[3] = cmd;
    // data
    for(i = 0; i < size; i++)
    {
        packet[4+i] = dat[i];
    }
    // checksum
    packet[4 + size] = csum;
    
    return (size + 5);
}
