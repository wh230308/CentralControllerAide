#include "stdafx.h"
#include "DataUtil.h"

int Char2Decimal(char data)
{
	if (data >= '0' && data <= '9') {
		return data - '0';
	}
	else if (data >= 'a' && data <= 'f') {
		return data - 'a' + 10;
	}
	else if (data >= 'A' && data <= 'F') {
		return data - 'A' + 10;
	}
	else {
		return -1;
	}
}

void String2ByteArray(const CString& strInput, BYTE* byHexArray, ULONG& uSize)
{
	ASSERT(byHexArray != NULL);
	uSize = 0;
	for (int i = 0; i < strInput.GetLength() - 1; i++) {
		if (strInput[i] != ' ' && strInput[i + 1] != ' ') {
			*(byHexArray + uSize) = (BYTE)(Char2Decimal(strInput[i]) * 16 + Char2Decimal(strInput[i + 1]));
			uSize++;
		}
	}
}

void ByteArray2String(const BYTE* byHexArray, ULONG& uSize, CString& strOpcode)
{
	for (ULONG i = 0; i < uSize; i++) {
		CString strTemp;
		strTemp.Format(_T("%02x "), *(byHexArray + i));
		strOpcode += strTemp;
	}

	strOpcode.TrimRight();
}

void BuildSerialData(BYTE byIndex, BYTE byFunction, const BYTE* byInput, ULONG uLength, BYTE* byHexArray, ULONG& uSize)
{
	ASSERT(byHexArray != NULL);
	uSize = 0;

	// fill header $dwt#
	byHexArray[uSize++] = '$';
	byHexArray[uSize++] = 'd';
	byHexArray[uSize++] = 'w';
	byHexArray[uSize++] = 't';
	byHexArray[uSize++] = '#';
	// fill index
    byHexArray[uSize++] = byIndex;
	// fill function
	byHexArray[uSize++] = byFunction;
	// fill data length
	byHexArray[uSize++] = (BYTE)uLength;
	// fill data
	for (ULONG i = 0; i < uLength; i++) {
		byHexArray[uSize++] = *(byInput + i);
	}
}

void CharacterSequence2HexSequence(const CString& strCharacterSequence, CString& strHexSequence)
{
    for (int i = 0; i < strCharacterSequence.GetLength(); i++) {
        CString strTemp;
        strTemp.Format(_T("%02X "), strCharacterSequence[i] - 0x00);
        strHexSequence += strTemp;
    }
}

void HexSequence2CharacterSequence(const CString& strHexSequence, CString& strCharacterSequence)
{
    for (int i = 0; i < strHexSequence.GetLength() - 1; i++) {
        if (strHexSequence[i] != ' ' && strHexSequence[i + 1] != ' ') {
            BYTE byCode = (BYTE)(Char2Decimal(strHexSequence[i]) * 16 + Char2Decimal(strHexSequence[i + 1]));
            CString strTemp;
            strTemp.Format(_T("%c"), byCode);
            strCharacterSequence += strTemp;
        }
    }
}