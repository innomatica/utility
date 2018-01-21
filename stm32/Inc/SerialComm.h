/**
 * \file
 * \author	<a href="http://www.innomatic.ca">innomatic</a>
 * \brief	Serial communication utility
 * \copyright <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons Licence" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.
 */

#ifndef __SERIAL_COMM_H
#define __SERIAL_COMM_H

#include <stdint.h>

#define MAX_PAYLOAD			10					///< max size of the payload
#define MAX_PKTSIZE			(MAX_PAYLOAD + 3)   ///< max size of the packet
#define MAX_DATSIZE			(MAX_PAYLOAD - 1)   ///< max data bytes

#define PKT_HEADR			0xf5				///< packet header signature
#define PKT_ACK				0xf6				///< ACK packet
#define PKT_NAK				0xf7				///< NAK packet
#define PKT_IAM				0xf8                ///< IAM packet

/// Packet state machine return value
typedef enum
{
	PKT_INPROCES = 0,		///< packet decoding in process
	PKT_RECEIVED,			///< valid packet received
	ACK_RECEIVED,			///< ACK packet received
	NAK_RECEIVED,			///< NAK packet received
	IAM_RECEIVED,           ///< IAM packet received
	PKT_SIZE_ERR,           ///< packet size error detected
	PKT_CSUM_ERR			///< checksum error detected
} pkt_status;

/// Initialize UART module
void SerialComm_Init(void) __attribute((weak));
/// RX interrupt handler
void SerialComm_RxRoutine(void) __attribute((weak));
/// Send a single byte
void SerialComm_SendByte(uint8_t byte) __attribute((weak));
/// Send a stream of bytes
void SerialComm_SendByteArray(uint8_t *buffer, int size) __attribute((weak));
/// Send a packet
void SerialComm_SendPacket(uint8_t *payload, int size);
/// Packet decoding state machine
pkt_status SerialComm_Decoder(uint8_t byte, uint8_t *buffer);

#endif // __SERIAL_COMM_H
