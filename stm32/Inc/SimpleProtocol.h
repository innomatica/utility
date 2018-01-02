/**
 * \file
 */

#ifndef __SIMPLE_PROTOCOL_H__
#define __SIMPLE_PROTOCOL_H__
    
    
#define MAX_PAYLOAD             10					///< max size of the payload
#define MAX_PKTSIZE             (MAX_PAYLOAD + 3)   ///< max size of the packet
#define MAX_DATSIZE             (MAX_PAYLOAD - 2)   ///< max data bytes

#define PKT_HEADR               0xf5				///< packet header signature
#define PKT_ACK                 0xf6				///< ACK packet
#define PKT_NAK                 0xf7				///< NAK packet
#define PKT_IAM                 0xf8                ///< IAM packet

/// Packet state machine return value
typedef enum
{
    NON_RECEIVED = 0,		///< no packet received
    PKT_RECEIVED,			///< valid packet received
    ACK_RECEIVED,			///< ACK packet received
    NAK_RECEIVED,			///< NAK packet received
    IAM_RECEIVED,           ///< IAM packet received
    PKT_SIZE_ERR,           ///< packet size error detected
    PKT_CSUM_ERR			///< checksum error detected
} pkt_status;

// Command Classes: THERE SHOULD BE NO GAP IN THE NUMBERS
#define CCL_GR_HOSTPC           0x00    ///< command class handled by host PC
#define CCL_BROADCAST           0x00    ///< broadcast or no specific target
#define CCL_GR_MONITR           0x01    ///< command class handled by gear monitor
#define CCL_GR_CTRLER			0x02	///< command class for gear controller
#define CCL_ZW_MODULE			0x03	///< command class handled by zwave module

// Command Codes shared by All Command Classes (0x00 and 0x80 - 0x9F)
#define SYS_SRESET              0x00    ///< system reset
#define RPT_DONE                0x80    ///< no more data to report
#define RPT_U08XXX              0x81    ///< report with uint8 data
#define RPT_S08XXX              0x82    ///< report with int8 data
#define RPT_U16XXX              0x83    ///< report with uint16 data
#define RPT_S16XXX              0x84    ///< report with int16 data
#define RPT_U32XXX              0x85    ///< report with uint16 data
#define RPT_S32XXX              0x86    ///< report with int16 data

// Command Codes belongs to CCL_GR_MONITR (0x01 - 0x3F)
#define GRM_QDC_SETVAL          0x10    ///< set QuadDec value    
#define GRM_QDC_GETVAL          0x11    ///< get QuadDec value
#define GRM_QDC_BGNRPT          0x12    ///< start reporting QuadDec count
#define GRM_QDC_ENDRPT          0x13    ///< stop reporting QuadDec count
#define GRM_ADC_BGNCAP          0x14    ///< start ADC capture
#define GRM_ADC_GETDAT          0x15    ///< get ADC dat

// Command Codes belongs to CCL_GR_CTRLER (0x01 - 0x3F)
#define CTR_MOT_SETPOS          0x10    ///< set position (ZWave style)
#define CTR_MOT_GETPOS          0x11    ///< get position (ZWave style)
#define CTR_MOT_SETSPD          0x12    ///< set speed
#define CTR_MOT_GETSPD          0x13    ///< get speed
#define CTR_MOT_SWTDIR          0x14    ///< switch motor direction
#define CTR_MOT_RUNPRF          0x15    ///< perform profile run
#define CTR_MOT_GETPRF          0x16    ///< get profile data
#define CTR_ENC_SETVAL          0x20    ///< set encoder value    
#define CTR_ENC_GETVAL          0x21    ///< get encoder value
#define CTR_MLS_GETRPT          0x11    ///< get zwave mls report

// Command Codes belongs to CCL_ZW_MODULE (0x01 - 0x3F)
#define ZWC_STR_TFRAM			0x01	///< start sending test frame
#define ZWC_END_TFRAM			0x02	///< stop sending test frame
#define ZWC_STR_LEARN			0x20	///< start learn mode
#define ZWC_END_LEARN			0x21	///< end learn mode
#define ZWC_STR_OTAFW			0x24	///< start OTA firmware update
#define ZWC_RST_WCHDG			0x25	///< reset by watchdog

/// Packet decoding state machine
pkt_status SimplePacketDecoder(uint8 byte, uint8 *packet, int *size);
/// Packet encoding utility
int SimplePacketEncoder(uint8 ccl,uint8 cmd,uint8 *dat,int size,uint8 *packet);
    
#endif // __SIMPLE_PROTOCOL_H__
