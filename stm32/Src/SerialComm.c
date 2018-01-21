/**
 * \file
 * \author	<a href="http://www.innomatic.ca">innomatic</a>
 * \copyright <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons Licence" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.
 */
#include "SerialComm.h"
#include <stdbool.h>

// packet decoding state machine states
#define PKT_STATE_HDR			0
#define PKT_STATE_LEN			1
#define PKT_STATE_PLD			2
#define PKT_STATE_CSM			3


/**
 * This function runs packet decoding state machine. It takes stream of serial
 * data one byte at a time and returns the current state of the state machine.
 * Upon successful retrieval of a full packet, it fills the array buffer[]
 * with the received payload.
 *
 * To use this function, define an uint8_t array for the payload data storage
 * and call this function whenever new byte is arrived. The best place to put
 * this function is inside the UART check routine (not inside the interrupt
 * service callback function).
 *
\code
uint8_t buffer[MAX_PKTSIZE];
pkt_status pstatus;

if(UART_RX_flag_is_set)
{
    // new data byte is in new_byte
    if(PKT_RECEIVED == SerialPacketDecoder(new_byte, buffer))
    {
        // packet data is in the buffer
    }
}
\endcode
 *
 * \param	byte received byte
 * \param   buffer packet will be retured here
 * \return  pkt_status status of the state machine
 */
pkt_status SerialComm_Decoder(uint8_t byte, uint8_t *buffer)
{
	static uint8_t state = PKT_STATE_HDR;
	static uint8_t index = 0;
	static uint8_t packet[MAX_PKTSIZE] = {0};
	static uint8_t csum = 0;

	int i;

	// waiting for the header byte
	if(state == PKT_STATE_HDR)
	{
		if(byte == PKT_HEADR)
		{
			// store the header byte
			packet[0] = byte;
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
		// store the length byte
		packet[1] = byte;

		// invalid payload size
		if(byte > MAX_PAYLOAD)
		{
			// start all over
			state = PKT_STATE_HDR;
			// report size error
			return PKT_SIZE_ERR;
		}
		// length byte is valid
		else
		{
			// reset index
			index = 2;
			// clear chesum byte
			csum = 0;
			// proceed to the next state
			state = PKT_STATE_PLD;
		}
	}
	// waiting for the payload
	else if(state == PKT_STATE_PLD)
	{
		// collect data
		packet[index++] = byte;
		// process checksum
		csum ^= byte;
		// proceed to the next if all payload is collected
		if(index == (packet[1] + 2))
		{
			state = PKT_STATE_CSM;
		}
	}
	// waiting for the checksum byte
	else if(state == PKT_STATE_CSM)
	{
		// collect data
		packet[index] = byte;
		// checksum matches
		if(byte == csum)
		{
			// copy packet to the buffer
			for(i = 0; i < packet[1] + 3; i++)
			{
				buffer[i] = packet[i];
			}
			// start all over again
			state = PKT_STATE_HDR;
			// valid packet arrived
			return PKT_RECEIVED;
		}
		// checksum does not match
		else
		{
			// start all over
			state = PKT_STATE_HDR;
			// checksum error
			return PKT_CSUM_ERR;
		}
	}

	return PKT_INPROCES;
}

/**
 * Construct a packet from a give payload
 *
\code
uint8_t payload[5];

# pack signed int32 value in big endian order
payload[0] = RPT_S32XXX;
payload[1] = (value >> 24) & 0xff;
payload[2] = (value >> 16) & 0xff;
payload[3] = (value >> 8) & 0xff;
payload[4] = (value) & 0xff;

# send the packet
SerialComm_SendPacket(payload, 5, packet);
\endcode
 *
 * \param   payload payload data
 * \param   size size of the payload
 * \param   packet resulting packet
 * \return  int the size of the packet
 */
void SerialComm_SendPacket(uint8_t *payload, int size)
{
	uint8_t csum = 0;
	uint8_t packet[MAX_PKTSIZE];
	int i;

	// header
	packet[0] = PKT_HEADR;
	// payload size
	packet[1] = size;
	// data
	for(i = 0; i < size; i++)
	{
		csum ^= payload[i];
		packet[2+i] = payload[i];
	}
	// checksum
	packet[2 + size] = csum;

	// send the array of bytes
	SerialComm_SendByteArray(packet, size+3);
}
