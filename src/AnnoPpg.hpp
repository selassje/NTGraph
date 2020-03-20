#if !defined(AFX_ANNOPPG_H__C05F14EB_EEFC_4EDB_9D6F_F86547275571__INCLUDED_)
#define AFX_ANNOPPG_H__C05F14EB_EEFC_4EDB_9D6F_F86547275571__INCLUDED_

// AnnoPpg.h : header file
//
#include "ColourPicker.hpp"
/////////////////////////////////////////////////////////////////////////////
// CAnnoPropPage : Property page dialog

class CAnnoPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CAnnoPropPage)
	DECLARE_OLECREATE_EX(CAnnoPropPage)

// Constructors
public:
	CAnnoPropPage();

// Dialog Data
	//{{AFX_DATA(CAnnoPropPage)
	enum { IDD = IDD_PROPPAGE_ANNOTATION };
	CColourPicker	m_btnBkColor;
	CColourPicker	m_btnColor;
	CButton	m_btnVisible;
	CComboBox	m_cbOrientation;
	CListBox	m_lbAnnotations;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support
	LPDISPATCH GetControlDispatch();
	void AddAnnotation();
	void DeleteAnnotation(short annoID);
	void UpdateControls();


// Message maps
protected:
	//{{AFX_MSG(CAnnoPropPage)
	afx_msg void OnAddbutton();
	afx_msg void OnDelbutton();
	afx_msg void OnSelchangeAnnotations();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditCaption();
	afx_msg void OnChangeEditX();
	afx_msg void OnChangeEditY();
	afx_msg void OnCloseupComboOrientation();
	afx_msg void OnCheckVisible();
	afx_msg LRESULT OnColorChange(WPARAM wParam,   LPARAM lParam);
	afx_msg LRESULT OnBkColorChange(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANNOPPG_H__C05F14EB_EEFC_4EDB_9D6F_F86547275571__INCLUDED_)
