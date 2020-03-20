#if !defined(AFX_CURSORPPG_H__D40C9F8C_0136_43F0_857D_F287AFA497D1__INCLUDED_)
#define AFX_CURSORPPG_H__D40C9F8C_0136_43F0_857D_F287AFA497D1__INCLUDED_

// CursorPpg.h : header file
//
#include "ColourPicker.hpp"
/////////////////////////////////////////////////////////////////////////////
// CCursorPropPage : Property page dialog

class CCursorPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCursorPropPage)
	DECLARE_OLECREATE_EX(CCursorPropPage)

// Constructors
public:
	CCursorPropPage();

// Dialog Data
	//{{AFX_DATA(CCursorPropPage)
	enum { IDD = IDD_PROPPAGE_CURSOR };
	CButton	m_btnVisible;
	CComboBox	m_cbSnap;
	CComboBox	m_cbStyle;
	CColourPicker	m_btnColor;
	CListBox	m_lbCursors;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support
	LPDISPATCH GetControlDispatch();
	void AddCursor();
	void DeleteCursor(short annoID);
	void UpdateControls();

// Message maps
protected:
	//{{AFX_MSG(CCursorPropPage)
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURSORPPG_H__D40C9F8C_0136_43F0_857D_F287AFA497D1__INCLUDED_)
