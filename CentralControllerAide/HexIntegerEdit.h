#pragma once
#include "afxwin.h"

class CHexIntegerEdit : public CEdit {
public:
    CHexIntegerEdit();
    virtual ~CHexIntegerEdit();

    DECLARE_MESSAGE_MAP()
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

public:
    void EnableHexSwitch(BOOL bIsUseHex) {
        m_bIsUseHex = bIsUseHex;
    }

private:
    BOOL m_bIsUseHex = TRUE;
};

