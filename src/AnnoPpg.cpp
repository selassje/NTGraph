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

#include "AnnoPpg.hpp"
#include "NTGraph.hpp"
#include "stdafx.h"
#include <AFXPRIV.H.>

IMPLEMENT_DYNCREATE(CAnnoPropPage, COlePropertyPage)

BEGIN_MESSAGE_MAP(CAnnoPropPage, COlePropertyPage)

ON_BN_CLICKED(IDC_ADDBUTTON, OnAddbutton)
ON_BN_CLICKED(IDC_DELBUTTON, OnDelbutton)
ON_LBN_SELCHANGE(IDC_ANNOTATIONS, OnSelchangeAnnotations)
ON_EN_CHANGE(IDC_EDIT_CAPTION, OnChangeEditCaption)
ON_EN_CHANGE(IDC_EDIT_X, OnChangeEditX)
ON_EN_CHANGE(IDC_EDIT_Y, OnChangeEditY)
ON_CBN_CLOSEUP(IDC_COMBO_ORIENTATION, OnCloseupComboOrientation)
ON_BN_CLICKED(IDC_CHECK_VISIBLE, OnCheckVisible)
ON_MESSAGE(CPN_SELENDOK, OnColorChange)
ON_MESSAGE(CPN_CLOSEUP, OnBkColorChange)

END_MESSAGE_MAP()

IMPLEMENT_OLECREATE_EX(CAnnoPropPage,
    "NTGraph.CAnnoPropPage",
    0x2d3b02a,
    0xa146,
    0x4969,
    0xa5,
    0xc1,
    0x27,
    0xb8,
    0x70,
    0xb9,
    0xb4,
    0x10)

BOOL CAnnoPropPage::CAnnoPropPageFactory::UpdateRegistry(BOOL bRegister)
{
    if (bRegister)
        return AfxOleRegisterPropertyPageClass(
            AfxGetInstanceHandle(), m_clsid, IDS_ANNOTATION_PPG);
    else
        return AfxOleUnregisterClass(m_clsid, NULL);
}

CAnnoPropPage::CAnnoPropPage()
    : COlePropertyPage(IDD, IDS_ANNOTATION_PPG_CAPTION)

{
}

void CAnnoPropPage::DoDataExchange(CDataExchange* pDX)
{
    DDX_Control(pDX, IDC_LABEL_BGCOLOR, m_btnBkColor);
    DDX_Control(pDX, IDC_LABEL_COLOR, m_btnColor);
    DDX_Control(pDX, IDC_CHECK_VISIBLE, m_btnVisible);
    DDX_Control(pDX, IDC_COMBO_ORIENTATION, m_cbOrientation);
    DDX_Control(pDX, IDC_ANNOTATIONS, m_lbAnnotations);
    DDP_PostProcessing(pDX);
}

LPDISPATCH CAnnoPropPage::GetControlDispatch()
{
    ULONG ulObjects;
    LPDISPATCH* lpObjectArray = GetObjectArray(&ulObjects);
    ASSERT(lpObjectArray != NULL);

    return (lpObjectArray[0]);
}

void CAnnoPropPage::AddAnnotation()
{
    USES_CONVERSION;

    LPDISPATCH lpControl = GetControlDispatch();

    COleDispatchDriver PropDispDriver;
    DISPID dwDispID;

    LPCOLESTR lpOleStr = T2COLE("AddAnnotation");

    if (SUCCEEDED(lpControl->GetIDsOfNames(
            IID_NULL, (LPOLESTR*)&lpOleStr, 1, 0, &dwDispID))) {
        PropDispDriver.AttachDispatch(lpControl, FALSE);
        PropDispDriver.InvokeHelper(
            dwDispID, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
        PropDispDriver.DetachDispatch();
    }
}

void CAnnoPropPage::DeleteAnnotation(short annoID)
{
    USES_CONVERSION;

    LPDISPATCH lpControl = GetControlDispatch();

    COleDispatchDriver PropDispDriver;
    DISPID dwDispID;

    LPCOLESTR lpOleStr = T2COLE("DeleteAnnotation");

    if (SUCCEEDED(lpControl->GetIDsOfNames(
            IID_NULL, (LPOLESTR*)&lpOleStr, 1, 0, &dwDispID))) {
        PropDispDriver.AttachDispatch(lpControl, FALSE);
        static BYTE parms[] = VTS_I2;
        PropDispDriver.InvokeHelper(
            dwDispID, DISPATCH_METHOD, VT_EMPTY, NULL, parms, annoID);
        PropDispDriver.DetachDispatch();
    }
}

void CAnnoPropPage::OnAddbutton()
{
    CString str;
    AddAnnotation();

    int index;
    GetPropText("Annotation", &index);

    str.Format("Annotation-%d", index);
    m_lbAnnotations.AddString(str);
    m_lbAnnotations.SetCurSel(index);

    SetPropText("Annotation", index);
    UpdateControls();
}

void CAnnoPropPage::OnDelbutton()
{
    int index = m_lbAnnotations.GetCurSel();
    int count = m_lbAnnotations.GetCount();

    DeleteAnnotation(index);
    m_lbAnnotations.DeleteString(index);

    index = m_lbAnnotations.GetCount() - 1;

    m_lbAnnotations.SetCurSel(index);

    if (index >= 0)
        SetPropText("Annotation", index);

    UpdateControls();
}

void CAnnoPropPage::OnSelchangeAnnotations()
{
    int index;
    index = m_lbAnnotations.GetCurSel();
    SetPropText("Annotation", index);
    UpdateControls();
}

void CAnnoPropPage::UpdateControls()
{
    CString caption;
    GetPropText("AnnoLabelCaption", &caption);
    SetDlgItemText(IDC_EDIT_CAPTION, caption);

    CString x, y;
    GetPropText("AnnoLabelX", &x);
    GetPropText("AnnoLabelY", &y);

    SetDlgItemText(IDC_EDIT_X, x);
    SetDlgItemText(IDC_EDIT_Y, y);

    COLORREF color;
    GetPropText("AnnoLabelColor", &color);
    m_btnColor.SetColour(color);

    GetPropText("AnnoLabelBkColor", &color);
    m_btnBkColor.SetColour(color);

    BOOL orientation;
    GetPropText("AnnoLabelHorizontal", &orientation);
    m_cbOrientation.SetCurSel(((int)orientation) + 1);

    BOOL visible;
    GetPropText("AnnoVisible", &visible);

    if (visible)
        CheckDlgButton(IDC_CHECK_VISIBLE, BST_CHECKED);
    else
        CheckDlgButton(IDC_CHECK_VISIBLE, BST_UNCHECKED);

    int count;
    GetPropText("AnnoCount", &count);

    CWnd* pWnd = GetDlgItem(IDC_DELBUTTON);
    if (pWnd)
        if (count > 0)
            pWnd->EnableWindow(TRUE);
        else
            pWnd->EnableWindow(FALSE);

    COLORREF colour;
    GetPropText("AnnoLabelColor", &colour);
    m_btnColor.SetColour(colour);
}

BOOL CAnnoPropPage::OnInitDialog()
{
    COlePropertyPage::OnInitDialog();

    int count;
    GetPropText("AnnoCount", &count);

    if (count > 0) {
        m_lbAnnotations.ResetContent();
        for (int i = 0; i < count; i++) {
            CString str;
            str.Format("Annotation-%d", i);
            m_lbAnnotations.AddString(str);
        }

        CString str;
        str.Format("%d", count - 1);
        SetPropText("Annotation", str);
        m_lbAnnotations.SetCurSel(count - 1);
    }

    UpdateControls();

    return TRUE;
}

void CAnnoPropPage::OnChangeEditCaption()
{
    int count;
    GetPropText("AnnoCount", &count);

    if (count > 0) {
        CString str;
        GetDlgItemText(IDC_EDIT_CAPTION, str);
        SetPropText("AnnoLabelCaption", str);
    }
}

void CAnnoPropPage::OnChangeEditX()
{
    int count;
    GetPropText("AnnoCount", &count);

    if (count > 0) {
        CString str;
        GetDlgItemText(IDC_EDIT_X, str);
        SetPropText("AnnoLabelX", str);
    }
}

void CAnnoPropPage::OnChangeEditY()
{
    int count;
    GetPropText("AnnoCount", &count);

    if (count > 0) {
        CString str;
        GetDlgItemText(IDC_EDIT_Y, str);
        SetPropText("AnnoLabelY", str);
    }
}

void CAnnoPropPage::OnCloseupComboOrientation()
{
    int count;
    GetPropText("AnnoCount", &count);

    if (count > 0) {
        CString str("TRUE");
        int horizontal = m_cbOrientation.GetCurSel();
        if (horizontal == 0)
            SetPropText("AnnoLabelHorizontal", str);
        else {
            str = "FALSE";
            SetPropText("AnnoLabelHorizontal", str);
        }
    }
}

void CAnnoPropPage::OnCheckVisible()
{
    int count;
    GetPropText("AnnoCount", &count);

    if (count > 0) {
        CString str("TRUE");

        if (m_btnVisible.GetCheck())
            SetPropText("AnnoVisible", str);
        else {
            str = "FALSE";
            SetPropText("AnnoVisible", str);
        }
    }
}

LRESULT CAnnoPropPage::OnColorChange(WPARAM, LPARAM)
{
    int count;
    GetPropText("AnnoCount", &count);

    if (count > 0) {
        CString str;
        str.Format("%u", m_btnColor.GetColour());
        SetPropText("AnnoLabelColor", str);
    }

    TRACE0("Selection changed= %d\n");
    return TRUE;
}

LRESULT CAnnoPropPage::OnBkColorChange(WPARAM, LPARAM)
{
    int count;
    GetPropText("AnnoCount", &count);

    if (count > 0) {
        CString str;
        str.Format("%u", m_btnBkColor.GetColour());
        SetPropText("AnnoLabelBkColor", str);
    }

    TRACE0("Selection changed= %d\n");
    return TRUE;
}
