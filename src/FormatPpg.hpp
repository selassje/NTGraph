/*
MIT License

Copyright(c) 1998-2020 Przemyslaw Koziol
                       Chris Maunder (chrismaunder@codeguru.com)
                       Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#if !defined(AFX_FORMATPPG_H__4BD4059B_DF9B_4741_954D_651485B3EB6A__INCLUDED_)
#define AFX_FORMATPPG_H__4BD4059B_DF9B_4741_954D_651485B3EB6A__INCLUDED_

#include "stdafx.h"

class CFormatPropPage : public COlePropertyPage {
    DECLARE_DYNCREATE(CFormatPropPage)
    DECLARE_OLECREATE_EX(CFormatPropPage)

public:
    CFormatPropPage();
    enum { IDD = IDD_PROPAGE_FORMAT };
    CListBox m_lbFormats;
    CComboBox m_cbType;
    CComboBox m_cbAxis;

    virtual void DoDataExchange(CDataExchange* pDX);
    void SetAxisTimeFormat(CString str);

protected:
    void UpdateControls();
    afx_msg void OnChangeEditFormat();
    virtual BOOL OnInitDialog();
    afx_msg void OnButtonDefault();
    afx_msg void OnSelchangeFormatList();
    afx_msg void OnCloseupComboType();
    afx_msg void OnCloseupComboAxis();

    DECLARE_MESSAGE_MAP()
};

#endif
