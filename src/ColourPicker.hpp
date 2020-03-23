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

#if !defined(AFX_COLOURPICKER_H__D0B75901_9830_11D1_9C0F_00A0243D1382__INCLUDED_)
#define AFX_COLOURPICKER_H__D0B75901_9830_11D1_9C0F_00A0243D1382__INCLUDED_

#include "ColourPopup.hpp"

void AFXAPI DDX_ColourPicker(CDataExchange* pDX, int nIDC, COLORREF& crColour);

#define CP_MODE_TEXT 1
#define CP_MODE_BK 2

class CColourPicker : public CButton {

public:
    CColourPicker();
    DECLARE_DYNCREATE(CColourPicker);

    COLORREF GetColour();
    void SetColour(COLORREF crColour);

    void SetDefaultText(LPCTSTR szDefaultText);
    void SetCustomText(LPCTSTR szCustomText);

    void SetTrackSelection(BOOL bTracking = TRUE) { m_bTrackSelection = bTracking; }
    BOOL GetTrackSelection() { return m_bTrackSelection; }

    void SetSelectionMode(UINT nMode) { m_nSelectionMode = nMode; }
    UINT GetSelectionMode() { return m_nSelectionMode; };

    void SetBkColour(COLORREF crColourBk);
    COLORREF GetBkColour() { return m_crColourBk; }

    void SetTextColour(COLORREF crColourText);
    COLORREF GetTextColour() { return m_crColourText; }

    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual ~CColourPicker();


protected:
    BOOL m_bActive;
    BOOL m_bTrackSelection;
    COLORREF m_crColourBk;
    COLORREF m_crColourText;
    UINT m_nSelectionMode;
    CRect m_ArrowRect;
    CString m_strDefaultText;
    CString m_strCustomText;
    void SetWindowSize();
    virtual void PreSubclassWindow();
    afx_msg BOOL OnClicked();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT OnSelEndOK(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSelEndCancel(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSelChange(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()
};

#endif