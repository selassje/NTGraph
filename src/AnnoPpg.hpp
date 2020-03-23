/*
MIT License

Copyright(c) 2020 Przemyslaw Koziol

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

#if !defined(AFX_ANNOPPG_H__C05F14EB_EEFC_4EDB_9D6F_F86547275571__INCLUDED_)
#define AFX_ANNOPPG_H__C05F14EB_EEFC_4EDB_9D6F_F86547275571__INCLUDED_

#include "ColourPicker.hpp"

class CAnnoPropPage : public COlePropertyPage {
    DECLARE_DYNCREATE(CAnnoPropPage)
    DECLARE_OLECREATE_EX(CAnnoPropPage)
public:
    CAnnoPropPage();

    enum {
        IDD = IDD_PROPPAGE_ANNOTATION
    };
    CColourPicker m_btnBkColor;
    CColourPicker m_btnColor;
    CButton m_btnVisible;
    CComboBox m_cbOrientation;
    CListBox m_lbAnnotations;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    LPDISPATCH GetControlDispatch();
    void AddAnnotation();
    void DeleteAnnotation(short annoID);
    void UpdateControls();

protected:
    afx_msg void OnAddbutton();
    afx_msg void OnDelbutton();
    afx_msg void OnSelchangeAnnotations();
    virtual BOOL OnInitDialog();
    afx_msg void OnChangeEditCaption();
    afx_msg void OnChangeEditX();
    afx_msg void OnChangeEditY();
    afx_msg void OnCloseupComboOrientation();
    afx_msg void OnCheckVisible();
    afx_msg LRESULT OnColorChange(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnBkColorChange(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};

#endif
