#pragma once

#define BUFFER_SIZE		(120)
#define HEADER_SIZE		(5)
#define INDEX_SIZE      (1)
#define FUNCTION_SIZE	(1)
#define ERR_SIZE		(1)
#define DATA_LENGTH		(1)
#define PACKET_HEADER_SIZE (HEADER_SIZE + INDEX_SIZE + FUNCTION_SIZE + ERR_SIZE + DATA_LENGTH)

#define FUNCTION_RSET       (0x01)
#define FUNCTION_RON        (0x02) 
#define FUNCTION_ROFF       (0x03) 
#define FUNCTION_RRGB       (0x04) 
#define FUNCTION_RVIDEO     (0x05)
#define FUNCTION_RDELAY     (0x06)
#define FUNCTION_RSERIAL    (0x07)

#define FUNCTION_WSET       (0x08)
#define FUNCTION_WON        (0x09)
#define FUNCTION_WOFF       (0x10)
#define FUNCTION_WRGB       (0x11)
#define FUNCTION_WVIDEO     (0x12)
#define FUNCTION_WDELAY     (0x13)
#define FUNCTION_WSERIAL    (0x14)

void String2ByteArray(const CString& strInput, BYTE* byHexArray, ULONG& uSize);
void ByteArray2String(const BYTE* byHexArray, ULONG& uSize, CString& strOpcode);
void BuildSerialData(BYTE byIndex, BYTE byFunction, const BYTE* byInput, ULONG uLength, BYTE* byHexArray, ULONG& uSize);
void CharacterSequence2HexSequence(const CString& strCharacterSequence, CString& strHexSequence);
void HexSequence2CharacterSequence(const CString& strHexSequence, CString& strCharacterSequence);