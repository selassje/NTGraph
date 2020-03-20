#if !defined(AFX_FORMATPPG_H__4BD4059B_DF9B_4741_954D_651485B3EB6A__INCLUDED_)
#define AFX_FORMATPPG_H__4BD4059B_DF9B_4741_954D_651485B3EB6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormatPpg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormatPropPage : Property page dialog

class CFormatPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CFormatPropPage)
	DECLARE_OLECREATE_EX(CFormatPropPage)

// Constructors
public:
	CFormatPropPage();

// Dialog Data
	//{{AFX_DATA(CFormatPropPage)
	enum { IDD = IDD_PROPAGE_FORMAT };
	CListBox	m_lbFormats;
	CComboBox	m_cbType;
	CComboBox	m_cbAxis;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support
	void SetAxisTimeFormat(CString str);

// Message maps
protected:
	void UpdateControls();
	//{{AFX_MSG(CFormatPropPage)
	afx_msg void OnChangeEditFormat();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDefault();
	afx_msg void OnSelchangeFormatList();
	afx_msg void OnCloseupComboType();
	afx_msg void OnCloseupComboAxis();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMATPPG_H__4BD4059B_DF9B_4741_954D_651485B3EB6A__INCLUDED_)
