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

#include "ColourPicker.hpp"
#include "ColourPopup.hpp"
#include "stdafx.h"

void AFXAPI DDX_ColourPicker(CDataExchange* pDX, int nIDC, COLORREF& crColour)
{
    HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
    ASSERT(hWndCtrl != NULL);

    CColourPicker* pColourPicker = (CColourPicker*)CWnd::FromHandle(hWndCtrl);
    if (pDX->m_bSaveAndValidate) {
        crColour = pColourPicker->GetColour();
    } else {
        pColourPicker->SetColour(crColour);
    }
}

CColourPicker::CColourPicker()
{
    SetBkColour(GetSysColor(COLOR_3DFACE));
    SetTextColour(GetSysColor(COLOR_BTNTEXT));

    m_bTrackSelection = FALSE;
    m_nSelectionMode = CP_MODE_BK;
    m_bActive = FALSE;

    m_strDefaultText = _T("Automatic");
    m_strCustomText = _T("More Colours...");
}

CColourPicker::~CColourPicker()
{
}

IMPLEMENT_DYNCREATE(CColourPicker, CButton)

BEGIN_MESSAGE_MAP(CColourPicker, CButton)

ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
ON_WM_CREATE()
ON_MESSAGE(CPN_SELENDOK, OnSelEndOK)
ON_MESSAGE(CPN_SELENDCANCEL, OnSelEndCancel)
ON_MESSAGE(CPN_SELCHANGE, OnSelChange)

END_MESSAGE_MAP()

LRESULT CColourPicker::OnSelEndOK(WPARAM wParam, LPARAM)
{
    COLORREF crNewColour = (COLORREF)wParam;
    m_bActive = FALSE;
    SetColour(crNewColour);

    CWnd* pParent = GetParent();
    if (pParent) {
        pParent->SendMessage(CPN_CLOSEUP, wParam, (WPARAM)GetDlgCtrlID());
        pParent->SendMessage(CPN_SELENDOK, wParam, (WPARAM)GetDlgCtrlID());
    }

    if (crNewColour != GetColour())
        if (pParent)
            pParent->SendMessage(CPN_SELCHANGE, wParam, (WPARAM)GetDlgCtrlID());

    return TRUE;
}

LRESULT CColourPicker::OnSelEndCancel(WPARAM wParam, LPARAM)
{
    m_bActive = FALSE;
    SetColour((COLORREF)wParam);

    CWnd* pParent = GetParent();
    if (pParent) {
        pParent->SendMessage(CPN_CLOSEUP, wParam, (WPARAM)GetDlgCtrlID());
        pParent->SendMessage(CPN_SELENDCANCEL, wParam, (WPARAM)GetDlgCtrlID());
    }

    return TRUE;
}

LRESULT CColourPicker::OnSelChange(WPARAM wParam, LPARAM)
{
    if (m_bTrackSelection)
        SetColour((COLORREF)wParam);

    CWnd* pParent = GetParent();
    if (pParent)
        pParent->SendMessage(CPN_SELCHANGE, wParam, (WPARAM)GetDlgCtrlID());

    return TRUE;
}

int CColourPicker::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CButton::OnCreate(lpCreateStruct) == -1)
        return -1;

    SetWindowSize();
    return 0;
}

BOOL CColourPicker::OnClicked()
{
    m_bActive = TRUE;
    CRect rect;
    GetWindowRect(rect);
    new CColourPopup(CPoint(rect.left, rect.bottom),
        GetColour(),
        this,
        m_strDefaultText,
        m_strCustomText);

    CWnd* pParent = GetParent();
    if (pParent)
        pParent->SendMessage(CPN_DROPDOWN, (LPARAM)GetColour(), (WPARAM)GetDlgCtrlID());

    return TRUE;
}

void CColourPicker::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    ASSERT(lpDrawItemStruct);

    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect = lpDrawItemStruct->rcItem;
    UINT state = lpDrawItemStruct->itemState;
    CString m_strText;

    CSize Margins(::GetSystemMetrics(SM_CXEDGE), ::GetSystemMetrics(SM_CYEDGE));

    if (m_bActive)
        state |= ODS_SELECTED;
    pDC->DrawFrameControl(&m_ArrowRect, DFC_SCROLL, DFCS_SCROLLDOWN | ((state & ODS_SELECTED) ? DFCS_PUSHED : 0) | ((state & ODS_DISABLED) ? DFCS_INACTIVE : 0));

    pDC->DrawEdge(rect, EDGE_RAISED, BF_RECT);

    rect.DeflateRect(Margins.cx + 3, Margins.cy + 2);

    rect.right -= m_ArrowRect.Width();

    CBrush brush(((state & ODS_DISABLED) || m_crColourBk == CLR_DEFAULT) ? ::GetSysColor(COLOR_3DFACE) : m_crColourBk);
    CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brush);
    pDC->SelectStockObject(BLACK_PEN);
    pDC->Rectangle(rect);
    pDC->SelectObject(pOldBrush);

    GetWindowText(m_strText);
    if (m_strText.GetLength()) {
        pDC->SetBkMode(TRANSPARENT);
        if (state & ODS_DISABLED) {
            rect.OffsetRect(1, 1);
            pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
            pDC->DrawText(m_strText, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            rect.OffsetRect(-1, -1);
            pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
            pDC->DrawText(m_strText, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        } else {
            pDC->SetTextColor((m_crColourText == CLR_DEFAULT) ? 0 : m_crColourText);
            pDC->DrawText(m_strText, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        }
    }

    if (state & ODS_FOCUS) {
        rect.DeflateRect(1, 1);
        pDC->DrawFocusRect(rect);
    }
}

void CColourPicker::PreSubclassWindow()
{
    ModifyStyle(0, BS_OWNERDRAW);
    CButton::PreSubclassWindow();
    SetWindowSize();
}

COLORREF CColourPicker::GetColour()
{
    return (m_nSelectionMode == CP_MODE_TEXT) ? GetTextColour() : GetBkColour();
}

void CColourPicker::SetColour(COLORREF crColour)
{
    (m_nSelectionMode == CP_MODE_TEXT) ? SetTextColour(crColour) : SetBkColour(crColour);
}

void CColourPicker::SetBkColour(COLORREF crColourBk)
{
    m_crColourBk = crColourBk;
    if (IsWindow(m_hWnd))
        RedrawWindow();
}

void CColourPicker::SetTextColour(COLORREF crColourText)
{
    m_crColourText = crColourText;
    if (IsWindow(m_hWnd))
        RedrawWindow();
}

void CColourPicker::SetDefaultText(LPCTSTR szDefaultText)
{
    m_strDefaultText = (szDefaultText) ? szDefaultText : _T("");
}

void CColourPicker::SetCustomText(LPCTSTR szCustomText)
{
    m_strCustomText = (szCustomText) ? szCustomText : _T("");
}

void CColourPicker::SetWindowSize()
{
    CSize MarginSize(::GetSystemMetrics(SM_CXEDGE), ::GetSystemMetrics(SM_CYEDGE));

    int nArrowWidth = max(::GetSystemMetrics(SM_CXHTHUMB), 5 * MarginSize.cx);
    int nArrowHeight = max(::GetSystemMetrics(SM_CYVTHUMB), 5 * MarginSize.cy);
    CSize ArrowSize(max(nArrowWidth, nArrowHeight), max(nArrowWidth, nArrowHeight));

    CRect rect;
    GetWindowRect(rect);

    CWnd* pParent = GetParent();
    if (pParent)
        pParent->ScreenToClient(rect);

    int nWidth = max(rect.Width(), 2 * ArrowSize.cx + 2 * MarginSize.cx);
    MoveWindow(rect.left, rect.top, nWidth, ArrowSize.cy + 2 * MarginSize.cy, TRUE);

    GetWindowRect(rect);
    ScreenToClient(rect);

    m_ArrowRect.SetRect(rect.right - ArrowSize.cx - MarginSize.cx,
        rect.top + MarginSize.cy, rect.right - MarginSize.cx,
        rect.bottom - MarginSize.cy);
}
