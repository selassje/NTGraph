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

#include "CursorPpg.hpp"
#include "NTGraph.hpp"
#include "stdafx.h"
#include <AFXPRIV.H.>

IMPLEMENT_DYNCREATE(CCursorPropPage, COlePropertyPage)

BEGIN_MESSAGE_MAP(CCursorPropPage, COlePropertyPage)

ON_BN_CLICKED(IDC_ADDBUTTON, OnAddbutton)
ON_BN_CLICKED(IDC_DELBUTTON, OnDelbutton)
ON_LBN_SELCHANGE(IDC_CURSORS, OnSelchangeCursors)
ON_EN_CHANGE(IDC_EDIT_X, OnChangeEditX)
ON_EN_CHANGE(IDC_EDIT_Y, OnChangeEditY)
ON_BN_CLICKED(IDC_CHECK_VISIBLE, OnCheckVisible)
ON_MESSAGE(CPN_SELENDOK, OnColorChange)
ON_CBN_CLOSEUP(IDC_COMBO_STYLE, OnCloseupComboStyle)
ON_CBN_CLOSEUP(IDC_COMBO_SNAP, OnCloseupComboSnap)

END_MESSAGE_MAP()

IMPLEMENT_OLECREATE_EX(CCursorPropPage, "NTGraph.CCursorPropPage",
    0xf8b1d8d6, 0x10ab, 0x42f4, 0xae, 0xac, 0x64, 0x36, 0xcb, 0xe6, 0x21, 0x71)

BOOL CCursorPropPage::CCursorPropPageFactory::UpdateRegistry(BOOL bRegister)
{
    if (bRegister)
        return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
            m_clsid, IDS_CURSOR_PPG);
    else
        return AfxOleUnregisterClass(m_clsid, NULL);
}

CCursorPropPage::CCursorPropPage()
    : COlePropertyPage(IDD, IDS_CURSOR_PPG_CAPTION)

{
}

void CCursorPropPage::DoDataExchange(CDataExchange* pDX)
{
    DDX_Control(pDX, IDC_CHECK_VISIBLE, m_btnVisible);
    DDX_Control(pDX, IDC_COMBO_SNAP, m_cbSnap);
    DDX_Control(pDX, IDC_COMBO_STYLE, m_cbStyle);
    DDX_Control(pDX, IDC_CURSOR_COLOR, m_btnColor);
    DDX_Control(pDX, IDC_CURSORS, m_lbCursors);
    DDP_PostProcessing(pDX);
}

LPDISPATCH CCursorPropPage::GetControlDispatch()
{
    ULONG ulObjects;
    LPDISPATCH* lpObjectArray = GetObjectArray(&ulObjects);
    ASSERT(lpObjectArray != NULL);
    return (lpObjectArray[0]);
}

void CCursorPropPage::AddCursor()
{
    USES_CONVERSION;
    LPDISPATCH lpControl = GetControlDispatch();

    COleDispatchDriver PropDispDriver;
    DISPID dwDispID;

    LPCOLESTR lpOleStr = T2COLE("AddCursor");

    if (SUCCEEDED(lpControl->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr,
            1, 0, &dwDispID))) {
        PropDispDriver.AttachDispatch(lpControl, FALSE);
        PropDispDriver.InvokeHelper(dwDispID, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
        PropDispDriver.DetachDispatch();
    }
}

void CCursorPropPage::DeleteCursor(short annoID)
{
    USES_CONVERSION;

    LPDISPATCH lpControl = GetControlDispatch();

    COleDispatchDriver PropDispDriver;
    DISPID dwDispID;
    LPCOLESTR lpOleStr = T2COLE("DeleteCursor");

    if (SUCCEEDED(lpControl->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr,
            1, 0, &dwDispID))) {
        PropDispDriver.AttachDispatch(lpControl, FALSE);
        static BYTE parms[] = VTS_I2;
        PropDispDriver.InvokeHelper(dwDispID, DISPATCH_METHOD,
            VT_EMPTY, NULL, parms, annoID);
        PropDispDriver.DetachDispatch();
    }
}

BOOL CCursorPropPage::OnInitDialog()
{
    COlePropertyPage::OnInitDialog();

    int count;
    GetPropText("CursorCount", &count);

    if (count > 0) {
        m_lbCursors.ResetContent();
        for (int i = 0; i < count; i++) {
            CString str;
            str.Format("Cursor-%d", i);
            m_lbCursors.AddString(str);
        }

        CString str;
        str.Format("%d", count - 1);
        SetPropText("Cursor", str);
        m_lbCursors.SetCurSel(count - 1);
    }

    UpdateControls();

    return TRUE;
}

void CCursorPropPage::OnAddbutton()
{
    CString str;
    AddCursor();

    int index;
    GetPropText("Cursor", &index);

    str.Format("Cursor-%d", index);
    m_lbCursors.AddString(str);
    m_lbCursors.SetCurSel(index);

    SetPropText("Cursor", index);
    UpdateControls();
}

void CCursorPropPage::OnDelbutton()
{
    int index = m_lbCursors.GetCurSel();
    int count = m_lbCursors.GetCount();

    DeleteCursor(index);
    m_lbCursors.DeleteString(index);

    index = m_lbCursors.GetCount() - 1;

    m_lbCursors.SetCurSel(index);

    if (index >= 0)
        SetPropText("Cursor", index);

    UpdateControls();
}

void CCursorPropPage::UpdateControls()
{

    CString x, y;
    GetPropText("CursorX", &x);
    GetPropText("CursorY", &y);

    SetDlgItemText(IDC_EDIT_X, x);
    SetDlgItemText(IDC_EDIT_Y, y);

    COLORREF color;
    GetPropText("CursorColor", &color);
    m_btnColor.SetColour(color);

    int style;
    GetPropText("CursorStyle", &style);
    m_cbStyle.SetCurSel(style);

    int snap;
    GetPropText("CursorMode", &snap);
    m_cbSnap.SetCurSel(snap);

    BOOL visible;
    GetPropText("CursorVisible", &visible);

    if (visible)
        CheckDlgButton(IDC_CHECK_VISIBLE, BST_CHECKED);
    else
        CheckDlgButton(IDC_CHECK_VISIBLE, BST_UNCHECKED);

    int count;
    GetPropText("CursorCount", &count);

    CWnd* pWnd = GetDlgItem(IDC_DELBUTTON);
    if (pWnd)
        if (count > 0)
            pWnd->EnableWindow(TRUE);
        else
            pWnd->EnableWindow(FALSE);
}

void CCursorPropPage::OnSelchangeCursors()
{
    int index;
    index = m_lbCursors.GetCurSel();
    SetPropText("Cursor", index);
    UpdateControls();
}

void CCursorPropPage::OnChangeEditX()
{
    int count;
    GetPropText("CursorCount", &count);

    if (count > 0) {
        CString str;
        GetDlgItemText(IDC_EDIT_X, str);
        SetPropText("CursorX", str);
    }
}

void CCursorPropPage::OnChangeEditY()
{
    int count;
    GetPropText("CursorCount", &count);

    if (count > 0) {
        CString str;
        GetDlgItemText(IDC_EDIT_Y, str);
        SetPropText("CursorY", str);
    }
}

void CCursorPropPage::OnCheckVisible()
{
    int count;
    GetPropText("CursorCount", &count);

    if (count > 0) {
        CString str("TRUE");

        if (m_btnVisible.GetCheck())
            SetPropText("CursorVisible", str);
        else {
            str = "FALSE";
            SetPropText("CursorVisible", str);
        }
    }
}
LRESULT CCursorPropPage::OnColorChange(WPARAM /*lParam*/, LPARAM /*wParam*/)
{
    int count;
    GetPropText("CursorCount", &count);

    if (count > 0) {
        CString str;
        str.Format("%u", m_btnColor.GetColour());
        SetPropText("CursorColor", str);
    }

    TRACE0("Selection changed= %d\n");
    return TRUE;
}

void CCursorPropPage::OnCloseupComboStyle()
{
    int count;
    GetPropText("CursorCount", &count);

    if (count > 0) {
        int style = m_cbStyle.GetCurSel();
        SetPropText("CursorStyle", style);
    }
}

void CCursorPropPage::OnCloseupComboSnap()
{
    int count;
    GetPropText("CursorCount", &count);

    if (count > 0) {
        int mode = m_cbSnap.GetCurSel();
        SetPropText("CursorMode", mode);
    }
}
