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

#if !defined(AFX_NTGRAPHPPG_H__EE963BF3_61C1_420C_AAAC_CF89712455CC__INCLUDED_)
#define AFX_NTGRAPHPPG_H__EE963BF3_61C1_420C_AAAC_CF89712455CC__INCLUDED_

#include "ColourPicker.hpp"

class CNTGraphPropPage : public COlePropertyPage {
    DECLARE_DYNCREATE(CNTGraphPropPage)
    DECLARE_OLECREATE_EX(CNTGraphPropPage)

public:
    CNTGraphPropPage();

    BOOL OnHelp(LPCTSTR helpdir);

    enum { IDD = IDD_PROPPAGE_NTGRAPH };
    CButton m_btnYLog;
    CButton m_btnXLog;
    CButton m_btnGrid;
    CComboBox m_cbFrame;
    CComboBox m_cbTrackMode;
    BOOL m_bShowGrid;
    CString m_caption;
    BOOL m_bXLog;
    BOOL m_bYLog;
    COLORREF m_FrameColor;
    CColourPicker m_btnFrameColor;
    COLORREF m_PlotColor = 0;
    CColourPicker m_btnPlotColor;
    COLORREF m_GridColor;
    CColourPicker m_btnGridColor;
    COLORREF m_AxisColor;
    CColourPicker m_btnAxisColor;
    COLORREF m_LabelColor;
    CColourPicker m_btnLabelColor;
    int m_nFrame;
    int m_nMode;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    void UpdateControls();

    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    afx_msg LRESULT OnColorChange(WPARAM wParam, LPARAM lParam);
    afx_msg void OnChangeCaption();
    afx_msg void OnCheckShowgrid();
    afx_msg void OnCheckXlog();
    afx_msg void OnCheckYlog();
    afx_msg void OnCloseupComboFrame();
    afx_msg void OnCloseupComboMode();
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()
};

#endif
