#include "stdafx.h"
#include "HexIntegerEdit.h"


CHexIntegerEdit::CHexIntegerEdit()
{
}


CHexIntegerEdit::~CHexIntegerEdit()
{
}
BEGIN_MESSAGE_MAP(CHexIntegerEdit, CEdit)
    ON_WM_CHAR()
END_MESSAGE_MAP()


void CHexIntegerEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default
    if (m_bIsUseHex) {
        if ((nChar >= '0' && nChar <= '9') || (nChar >= 'a' && nChar <= 'f')
            || (nChar >= 'A' && nChar <= 'F') || (VK_BACK == nChar)
            || (VK_DELETE == nChar) || (VK_SPACE == nChar)) {
            CEdit::OnChar(nChar, nRepCnt, nFlags);
        }
    }
    else {
        CEdit::OnChar(nChar, nRepCnt, nFlags);
    }
}
