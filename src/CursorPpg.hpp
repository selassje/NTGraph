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

#if !defined(AFX_CURSORPPG_H__D40C9F8C_0136_43F0_857D_F287AFA497D1__INCLUDED_)
#define AFX_CURSORPPG_H__D40C9F8C_0136_43F0_857D_F287AFA497D1__INCLUDED_


#include "ColourPicker.hpp"

class CCursorPropPage : public COlePropertyPage {
    DECLARE_DYNCREATE(CCursorPropPage)
    DECLARE_OLECREATE_EX(CCursorPropPage)

public:
    CCursorPropPage();

    enum { IDD = IDD_PROPPAGE_CURSOR };
    CButton m_btnVisible;
    CComboBox m_cbSnap;
    CComboBox m_cbStyle;
    CColourPicker m_btnColor;
    CListBox m_lbCursors;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    LPDISPATCH GetControlDispatch();
    void AddCursor();
    void DeleteCursor(short annoID);
    void UpdateControls();

    virtual BOOL OnInitDialog();
    afx_msg void OnAddbutton();
    afx_msg void OnDelbutton();
    afx_msg void OnSelchangeCursors();
    afx_msg void OnChangeEditX();
    afx_msg void OnChangeEditY();
    afx_msg void OnCheckVisible();
    afx_msg LRESULT OnColorChange(WPARAM wParam, LPARAM lParam);
    afx_msg void OnCloseupComboStyle();
    afx_msg void OnCloseupComboSnap();

    DECLARE_MESSAGE_MAP()
};


#endif
