/**
 * \file
 * \author	<a href="http://www.innomatic.ca">innomatic</a>
 * \brief	Serial communication utility
 * \copyright <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons Licence" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.
 */

#ifndef __SERIAL_COMM_H__
#define __SERIAL_COMM_H__

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

// System control
#define SYS_SRESET			0x00    ///< perform software reset
#define SYS_WRESET			0x01	///< perform watchdog reset
#define SYS_STPMOD			0x02	///< enter stop mode
#define SYS_DSLMOD			0x03	///< enter deep sleep mode
#define SYS_SLPMOD			0x04	///< enter sleep mode

// DIO control
#define DIO_SETVAL			0x10	///< DIO set value
#define DIO_GETVAL			0x11	///< DIO get value
#define DIO_TGLVAL			0x12	///< DIO toggle value

// ADC control
#define ADC_SETMAG			0x20	///< ADC set magnitude
#define ADC_SETFRQ			0x21	///< ADC set frequency
#define ADC_CONSTV			0x23	///< ADC dc output
#define ADC_SINEWV			0x24	///< ADC sine waveform
#define ADC_SWTHWV			0x25	///< ADC sawtooth wavefrom
#define ADC_TRNGWV			0x26	///< ADC triangle waveform

// DAC control
#define DAC_SETFRQ			0x30	///< DAC set sample frequency
#define DAC_CAPSGL			0x31	///< DAC capture single
#define DAC_CAPCNT			0x32	///< DAC capture continuous

// Report data
#define RPT_FINISH			0x80    ///< no more data to report
#define RPT_U08XXX			0x81    ///< report with uint8_t data
#define RPT_S08XXX			0x82    ///< report with int8_t data
#define RPT_U16XXX			0x83    ///< report with uint16_t data
#define RPT_S16XXX			0x84    ///< report with int16_t data
#define RPT_U32XXX			0x85    ///< report with uint16_t data
#define RPT_S32XXX			0x86    ///< report with int16_t data

/// Initializie SerialComm
void SerialComm_Init(void);
/// Packet decoding state machine
pkt_status SeriaComm_Decoder(uint8_t byte, uint8_t *buffer, int *size);
/// Packet encoder
int SerialComm_Encoder(uint8_t *buffer, int size, uint8_t *packet);

#endif // __SERIAL_COMM_H__
