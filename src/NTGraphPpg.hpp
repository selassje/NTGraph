#if !defined(AFX_NTGRAPHPPG_H__EE963BF3_61C1_420C_AAAC_CF89712455CC__INCLUDED_)
#define AFX_NTGRAPHPPG_H__EE963BF3_61C1_420C_AAAC_CF89712455CC__INCLUDED_

// NTGraphPpg.h : Declaration of the CNTGraphPropPage property page class.
#include "ColourPicker.hpp"
////////////////////////////////////////////////////////////////////////////
// CNTGraphPropPage : See NTGraphPpg.cpp.cpp for implementation.

class CNTGraphPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CNTGraphPropPage)
	DECLARE_OLECREATE_EX(CNTGraphPropPage)

// Constructor
public:
	CNTGraphPropPage();

	BOOL OnHelp(LPCTSTR helpdir);

// Dialog Data
	//{{AFX_DATA(CNTGraphPropPage)
	enum { IDD = IDD_PROPPAGE_NTGRAPH };
	CButton	m_btnYLog;
	CButton	m_btnXLog;
	CButton	m_btnGrid;
	CComboBox	m_cbFrame;
	CComboBox	m_cbTrackMode;
	BOOL	m_bShowGrid;
	CString m_caption;
	BOOL    m_bXLog;
	BOOL    m_bYLog;
	COLORREF	m_FrameColor;
	CColourPicker	m_btnFrameColor;
	COLORREF	m_PlotColor;
	CColourPicker	m_btnPlotColor;
	COLORREF	m_GridColor;
	CColourPicker	m_btnGridColor;
	COLORREF	m_AxisColor;
	CColourPicker	m_btnAxisColor;
	COLORREF	m_LabelColor;
	CColourPicker	m_btnLabelColor;
	int	m_nFrame;
	int	m_nMode;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void UpdateControls();

// Message maps
protected:
	//{{AFX_MSG(CNTGraphPropPage)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg LRESULT OnColorChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnChangeCaption();
	afx_msg void OnCheckShowgrid();
	afx_msg void OnCheckXlog();
	afx_msg void OnCheckYlog();
	afx_msg void OnCloseupComboFrame();
	afx_msg void OnCloseupComboMode();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NTGRAPHPPG_H__EE963BF3_61C1_420C_AAAC_CF89712455CC__INCLUDED)
