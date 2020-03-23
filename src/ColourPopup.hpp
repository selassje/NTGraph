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

#if !defined(AFX_COLOURPOPUP_H__D0B75902_9830_11D1_9C0F_00A0243D1382__INCLUDED_)
#define AFX_COLOURPOPUP_H__D0B75902_9830_11D1_9C0F_00A0243D1382__INCLUDED_

#include "stdafx.h"

#define CPN_SELCHANGE WM_USER + 1001
#define CPN_DROPDOWN WM_USER + 1002
#define CPN_CLOSEUP WM_USER + 1003
#define CPN_SELENDOK WM_USER + 1004
#define CPN_SELENDCANCEL WM_USER + 1005

class CColourPicker;

typedef struct {
    COLORREF crColour;
    TCHAR* szName;
} ColourTableEntry;

class CColourPopup : public CWnd {
public:
    CColourPopup();
    CColourPopup(CPoint p, COLORREF crColour, CWnd* pParentWnd,
        LPCTSTR szDefaultText = NULL, LPCTSTR szCustomText = NULL);
    void Initialise();

    BOOL Create(CPoint p, COLORREF crColour, CWnd* pParentWnd,
        LPCTSTR szDefaultText = NULL, LPCTSTR szCustomText = NULL);

    virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
    virtual ~CColourPopup();

protected:
    BOOL GetCellRect(int nIndex, const LPRECT& rect);
    void FindCellFromColour(COLORREF crColour);
    void SetWindowSize();
    void CreateToolTips();
    void ChangeSelection(int nIndex);
    void EndSelection(int nMessage);
    void DrawCell(CDC* pDC, int nIndex);

    COLORREF GetColour(size_t nIndex) { return m_crColours[nIndex].crColour; }
    LPCTSTR GetColourName(size_t nIndex) { return m_crColours[nIndex].szName; }
    int GetIndex(int row, int col) const;
    int GetRow(int nIndex) const;
    int GetColumn(int nIndex) const;

protected:
    static ColourTableEntry m_crColours[];
    int m_nNumColours;
    int m_nNumColumns, m_nNumRows;
    int m_nBoxSize, m_nMargin;
    int m_nCurrentSel;
    int m_nChosenColourSel;
    CString m_strDefaultText;
    CString m_strCustomText;
    CRect m_CustomTextRect, m_DefaultTextRect, m_WindowRect;
    CFont m_Font;
    CPalette m_Palette;
    COLORREF m_crInitialColour, m_crColour;
    CToolTipCtrl m_ToolTip;
    CWnd* m_pParent;

    BOOL m_bChildWindowVisible;

protected:
    afx_msg void OnNcDestroy();
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg BOOL OnQueryNewPalette();
    afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
    DECLARE_MESSAGE_MAP()
};

#endif
