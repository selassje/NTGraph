#if !defined(AFX_ELEMENTPPG_H__517E4C68_E695_4AA4_AE50_B34B19B34076__INCLUDED_)
#define AFX_ELEMENTPPG_H__517E4C68_E695_4AA4_AE50_B34B19B34076__INCLUDED_

#include "ColourPicker.hpp"
/////////////////////////////////////////////////////////////////////////////
// CElementPropPage : Property page dialog

class CElementPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CElementPropPage)
	DECLARE_OLECREATE_EX(CElementPropPage)

// Constructors
public:
	CElementPropPage();
	BOOL OnHelp(LPCTSTR helpdir);

// Dialog Data
	//{{AFX_DATA(CElementPropPage)
	enum { IDD = IDD_PROPPAGE_ELEMENTS };
	CButton	m_btnSolid;
	CButton	m_btnShow;
	CComboBox	m_cbLineSymbol;
	CComboBox	m_cbLineType;
	CListBox	m_lbElementList;
	CColourPicker	m_btnPointColor;
	CColourPicker	m_btnLineColor;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support
	LPDISPATCH GetControlDispatch();
    void AddElement();
	void DeleteElement(long ElementID) ;
	void UpdateControls();
  	


// Message maps
protected:
	//{{AFX_MSG(CElementPropPage)
	afx_msg void OnSelchangeElementlist();
	virtual BOOL OnInitDialog();
	afx_msg void OnAddbutton();
	afx_msg void OnDelbutton();
	afx_msg void OnCheckSolid();
	afx_msg void OnCheckVisible();
	afx_msg LRESULT OnLineColorChange(WPARAM lParam, LPARAM wParam);
	afx_msg LRESULT OnPointColorChange(WPARAM lParam, LPARAM wParam);
	afx_msg void OnCloseupComboSymbol();
	afx_msg void OnCloseupComboType();
	afx_msg void OnDeltaposSpinWidth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditName();
	afx_msg void OnChangeEditWidth();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMENTPPG_H__517E4C68_E695_4AA4_AE50_B34B19B34076__INCLUDED_)
