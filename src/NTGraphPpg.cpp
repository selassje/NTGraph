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

#include "NTGraphPpg.hpp"
#include "stdafx.h"

IMPLEMENT_DYNCREATE(CNTGraphPropPage, COlePropertyPage)

BEGIN_MESSAGE_MAP(CNTGraphPropPage, COlePropertyPage)

ON_WM_HELPINFO()
ON_MESSAGE(CPN_CLOSEUP, OnColorChange)
ON_EN_CHANGE(IDC_CAPTION, OnChangeCaption)
ON_BN_CLICKED(IDC_CHECK_SHOWGRID, OnCheckShowgrid)
ON_BN_CLICKED(IDC_CHECK_XLOG, OnCheckXlog)
ON_BN_CLICKED(IDC_CHECK_YLOG, OnCheckYlog)
ON_CBN_CLOSEUP(IDC_COMBO_FRAME, OnCloseupComboFrame)
ON_CBN_CLOSEUP(IDC_COMBO_MODE, OnCloseupComboMode)

END_MESSAGE_MAP()

IMPLEMENT_OLECREATE_EX(CNTGraphPropPage, "NTGRAPH.NTGraphPropPage.1",
    0x2dd5d509, 0xe89e, 0x4825, 0x87, 0xd3, 0xa9, 0x39, 0xe6, 0x89, 0xbd, 0x25)

BOOL CNTGraphPropPage::CNTGraphPropPageFactory::UpdateRegistry(BOOL bRegister)
{
    if (bRegister)
        return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
            m_clsid, IDS_NTGRAPH_PPG);
    else
        return AfxOleUnregisterClass(m_clsid, NULL);
}

CNTGraphPropPage::CNTGraphPropPage()
    : COlePropertyPage(IDD, IDS_NTGRAPH_PPG_CAPTION)
{
    SetHelpInfo(_T("Names to appear in the control"), _T("NTGRAPH.HLP"), 0);
}

void CNTGraphPropPage::DoDataExchange(CDataExchange* pDX)
{

    DDX_Control(pDX, IDC_CHECK_YLOG, m_btnYLog);
    DDX_Control(pDX, IDC_CHECK_XLOG, m_btnXLog);
    DDX_Control(pDX, IDC_CHECK_SHOWGRID, m_btnGrid);
    DDX_Control(pDX, IDC_FRAME_COLOR, m_btnFrameColor);
    DDX_Control(pDX, IDC_PLOT_COLOR, m_btnPlotColor);
    DDX_Control(pDX, IDC_GRID_COLOR, m_btnGridColor);
    DDX_Control(pDX, IDC_AXIS_COLOR, m_btnAxisColor);
    DDX_Control(pDX, IDC_LABEL_COLOR, m_btnLabelColor);
    DDP_CBIndex(pDX, IDC_COMBO_FRAME, m_nFrame, _T("FrameStyle"));
    DDX_CBIndex(pDX, IDC_COMBO_FRAME, m_nFrame);
    DDX_Control(pDX, IDC_COMBO_FRAME, m_cbFrame);
    DDP_CBIndex(pDX, IDC_COMBO_FRAME, m_nMode, _T("TrackMode"));
    DDX_CBIndex(pDX, IDC_COMBO_FRAME, m_nMode);
    DDX_Control(pDX, IDC_COMBO_MODE, m_cbTrackMode);

    DDP_PostProcessing(pDX);
}

BOOL CNTGraphPropPage::OnHelpInfo(HELPINFO* pHelpInfo)
{
    AfxGetApp()->WinHelp(0, HELP_CONTENTS);

    return COlePropertyPage::OnHelpInfo(pHelpInfo);
}

BOOL CNTGraphPropPage::OnHelp(LPCTSTR)
{
    AfxGetApp()->WinHelp(0, HELP_CONTENTS);

    return TRUE;
}
LRESULT CNTGraphPropPage::OnColorChange(WPARAM, LPARAM)
{

    CString str;

    str.Format("%u", m_btnFrameColor.GetColour());
    SetPropText("ControlFrameColor", str);

    str.Format("%u", m_btnPlotColor.GetColour());
    SetPropText("PlotAreaColor", str);

    str.Format("%u", m_btnGridColor.GetColour());
    SetPropText("GridColor", str);

    str.Format("%u", m_btnAxisColor.GetColour());
    SetPropText("AxisColor", str);

    str.Format("%u", m_btnLabelColor.GetColour());
    SetPropText("LabelColor", str);

    TRACE0("Selection changed= %d\n");
    return TRUE;
}

void CNTGraphPropPage::OnChangeCaption()
{
    CString str;
    GetDlgItemText(IDC_CAPTION, str);
    SetPropText("Caption", str);
}

void CNTGraphPropPage::OnCheckShowgrid()
{
    CString str("TRUE");

    if (m_btnGrid.GetCheck())
        SetPropText("ShowGrid", str);
    else {
        str = "FALSE";
        SetPropText("ShowGrid", str);
    }
}

void CNTGraphPropPage::OnCheckXlog()
{
    CString str("TRUE");

    if (m_btnXLog.GetCheck())
        SetPropText("XLog", str);
    else {
        str = "FALSE";
        SetPropText("XLog", str);
    }
}

void CNTGraphPropPage::OnCheckYlog()
{
    CString str("TRUE");

    if (m_btnYLog.GetCheck())
        SetPropText("YLog", str);
    else {
        str = "FALSE";
        SetPropText("YLog", str);
    }
}

void CNTGraphPropPage::OnCloseupComboFrame()
{
    CString str;
    str.Format("%d", m_cbFrame.GetCurSel());
    SetPropText("FrameStyle", str);
}

void CNTGraphPropPage::OnCloseupComboMode()
{
    CString str;
    str.Format("%d", m_cbTrackMode.GetCurSel());
    SetPropText("TrackMode", str);
}

void CNTGraphPropPage::UpdateControls()
{
    CString caption;
    GetPropText("Caption", &caption);

    SetDlgItemText(IDC_CAPTION, caption);

    COLORREF color;

    GetPropText("AxisColor", &color);
    m_btnAxisColor.SetColour(color);

    GetPropText("ControlFrameColor", &color);
    m_btnFrameColor.SetColour(color);

    GetPropText("GridColor", &color);
    m_btnGridColor.SetColour(color);

    GetPropText("PlotAreaColor", &color);
    m_btnPlotColor.SetColour(color);

    GetPropText("LabelColor", &color);
    m_btnLabelColor.SetColour(color);

    int frame;
    GetPropText("FrameStyle", &frame);
    m_cbFrame.SetCurSel(frame);

    int mode;
    GetPropText("TrackMode", &mode);
    m_cbTrackMode.SetCurSel(mode);

    BOOL grid;
    GetPropText("ShowGrid", &grid);

    if (grid)
        CheckDlgButton(IDC_CHECK_SHOWGRID, BST_CHECKED);
    else
        CheckDlgButton(IDC_CHECK_SHOWGRID, BST_UNCHECKED);

    BOOL xlog;
    GetPropText("XLog", &xlog);

    if (xlog)
        CheckDlgButton(IDC_CHECK_XLOG, BST_CHECKED);
    else
        CheckDlgButton(IDC_CHECK_XLOG, BST_UNCHECKED);

    BOOL ylog;
    GetPropText("YLog", &ylog);

    if (ylog)
        CheckDlgButton(IDC_CHECK_YLOG, BST_CHECKED);
    else
        CheckDlgButton(IDC_CHECK_YLOG, BST_UNCHECKED);
}

BOOL CNTGraphPropPage::OnInitDialog()
{
    COlePropertyPage::OnInitDialog();

    UpdateControls();
    return TRUE;
}
