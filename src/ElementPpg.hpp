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

#if !defined(AFX_ELEMENTPPG_H__517E4C68_E695_4AA4_AE50_B34B19B34076__INCLUDED_)
#define AFX_ELEMENTPPG_H__517E4C68_E695_4AA4_AE50_B34B19B34076__INCLUDED_

#include "ColourPicker.hpp"


class CElementPropPage : public COlePropertyPage {
    DECLARE_DYNCREATE(CElementPropPage)
    DECLARE_OLECREATE_EX(CElementPropPage)

public:
    CElementPropPage();
    BOOL OnHelp(LPCTSTR helpdir);

    enum { IDD = IDD_PROPPAGE_ELEMENTS };
    CButton m_btnSolid;
    CButton m_btnShow;
    CComboBox m_cbLineSymbol;
    CComboBox m_cbLineType;
    CListBox m_lbElementList;
    CColourPicker m_btnPointColor;
    CColourPicker m_btnLineColor;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    LPDISPATCH GetControlDispatch();
    void AddElement();
    void DeleteElement(long ElementID);
    void UpdateControls();

    afx_msg void OnSelchangeElementlist();
    virtual BOOL OnInitDialog();
    afx_msg void OnAddbutton();
    afx_msg void OnDelbutton();
    afx_msg void OnCheckSolid();
    afx_msg void OnCheckVisible();
    afx_msg LRESULT OnLineColorChange(WPARAM lParam, LPARAM wParam);
    afx_msg LRESULT OnPointColorChange(WPARAM lParam, LPARAM wParam);
    afx_msg void OnCloseupComboSymbol();
    afx_msg void OnCloseupComboType();
    afx_msg void OnDeltaposSpinWidth(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChangeEditName();
    afx_msg void OnChangeEditWidth();

    DECLARE_MESSAGE_MAP()
};

#endif
