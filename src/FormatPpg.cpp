// FormatPpg.cpp : implementation file
//

#include "stdafx.h"
#include "FormatPpg.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormatPropPage dialog

IMPLEMENT_DYNCREATE(CFormatPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CFormatPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CFormatPropPage)
	ON_EN_CHANGE(IDC_EDIT_FORMAT, OnChangeEditFormat)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_LBN_SELCHANGE(IDC_FORMAT_LIST, OnSelchangeFormatList)
	ON_CBN_CLOSEUP(IDC_COMBO_TYPE, OnCloseupComboType)
	ON_CBN_CLOSEUP(IDC_COMBO_AXIS, OnCloseupComboAxis)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

// {743A8F83-8596-4E55-92AF-CC48DA8BAA5E}
IMPLEMENT_OLECREATE_EX(CFormatPropPage, "NTGraph.CFormatPropPage",
	0x743a8f83, 0x8596, 0x4e55, 0x92, 0xaf, 0xcc, 0x48, 0xda, 0x8b, 0xaa, 0x5e)


/////////////////////////////////////////////////////////////////////////////
// CFormatPropPage::CFormatPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CFormatPropPage

BOOL CFormatPropPage::CFormatPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Define string resource for page type; replace '0' below with ID.

	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_FORMAT_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CFormatPropPage::CFormatPropPage - Constructor

// TODO: Define string resource for page caption; replace '0' below with ID.

CFormatPropPage::CFormatPropPage() :
// ****** Add your code below this line ********** //
     COlePropertyPage(IDD,  IDS_FORMAT_PPG_CAPTION)
// ****** Add your code above this line ********** //
{
	//{{AFX_DATA_INIT(CFormatPropPage)
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CFormatPropPage::DoDataExchange - Moves data between page and properties

void CFormatPropPage::DoDataExchange(CDataExchange* pDX)
{
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//{{AFX_DATA_MAP(CFormatPropPage)
	DDX_Control(pDX, IDC_FORMAT_LIST, m_lbFormats);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cbType);
	DDX_Control(pDX, IDC_COMBO_AXIS, m_cbAxis);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CFormatPropPage message handlers

void CFormatPropPage::OnChangeEditFormat() 
{
	CString str ;
	GetDlgItemText(IDC_EDIT_FORMAT, str);
	
	if(m_cbAxis.GetCurSel())
		SetPropText("FormatAxisLeft", str);
	else
		SetPropText("FormatAxisBottom", str);
}

void CFormatPropPage::UpdateControls()
{
	CString str;
	BOOL time;

	if(m_cbAxis.GetCurSel())
	{
		GetPropText("YTime",&time);
		GetPropText("FormatAxisLeft", &str);
	}
    else
	{
		GetPropText("XTime",&time);
		GetPropText("FormatAxisBottom", &str);
	}

	SetDlgItemText(IDC_EDIT_FORMAT, str);

	if(time)
		m_cbType.SetCurSel(3);
	else
		m_cbType.SetCurSel(0);

	OnCloseupComboType() ;
}


BOOL CFormatPropPage::OnInitDialog() 
{
	COlePropertyPage::OnInitDialog();
	
	m_cbAxis.SetCurSel(0);
	m_cbType.SetCurSel(0);
	UpdateControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFormatPropPage::OnButtonDefault() 
{
	SetDlgItemText(IDC_EDIT_FORMAT, "%g");
	m_cbType.SetCurSel(0);
	UpdateControls();

}

void CFormatPropPage::OnSelchangeFormatList() 
{
	CString str;
	switch(m_cbType.GetCurSel())
	{
		case 0:
			if(m_lbFormats.GetCaretIndex()==0)
				SetDlgItemText(IDC_EDIT_FORMAT, "%g");
			else if(m_lbFormats.GetCaretIndex()==1)
				SetDlgItemText(IDC_EDIT_FORMAT, "%.1f");
			else if(m_lbFormats.GetCaretIndex()==2)
				SetDlgItemText(IDC_EDIT_FORMAT, "%.2f");
			else if(m_lbFormats.GetCaretIndex()==3)
				SetDlgItemText(IDC_EDIT_FORMAT, "%.3f");
			else if(m_lbFormats.GetCaretIndex()==4)
				SetDlgItemText(IDC_EDIT_FORMAT, "%.4f");
			break;
		case 1:
			if(m_lbFormats.GetCaretIndex()==0)
				SetDlgItemText(IDC_EDIT_FORMAT, "%.e");
			else if(m_lbFormats.GetCaretIndex()==1)
				SetDlgItemText(IDC_EDIT_FORMAT, "%.1e");
			else if(m_lbFormats.GetCaretIndex()==2)
				SetDlgItemText(IDC_EDIT_FORMAT, "%.2e");
			else if(m_lbFormats.GetCaretIndex()==3)
				SetDlgItemText(IDC_EDIT_FORMAT, "%.3e");
			else if(m_lbFormats.GetCaretIndex()==4)
				SetDlgItemText(IDC_EDIT_FORMAT, "%.4e");
			else if(m_lbFormats.GetCaretIndex()==5)
				SetDlgItemText(IDC_EDIT_FORMAT, "%.E");
			else if(m_lbFormats.GetCaretIndex()==6)
				SetDlgItemText(IDC_EDIT_FORMAT, "%.1E");
			else if(m_lbFormats.GetCaretIndex()==7)
				SetDlgItemText(IDC_EDIT_FORMAT, "%.2E");
			else if(m_lbFormats.GetCaretIndex()==8)
				SetDlgItemText(IDC_EDIT_FORMAT, "%.3E");
			else if(m_lbFormats.GetCaretIndex()==9)
				SetDlgItemText(IDC_EDIT_FORMAT, "%.4E");
			break;
		case 2:
			if(m_lbFormats.GetCaretIndex()==0)
				SetDlgItemText(IDC_EDIT_FORMAT, "%g V");
			else if(m_lbFormats.GetCaretIndex()==1)
				SetDlgItemText(IDC_EDIT_FORMAT, "%g A");
			else if(m_lbFormats.GetCaretIndex()==2)
				SetDlgItemText(IDC_EDIT_FORMAT, "%g Hz");
			else if(m_lbFormats.GetCaretIndex()==3)
				SetDlgItemText(IDC_EDIT_FORMAT, "%g g");
			else if(m_lbFormats.GetCaretIndex()==4)
				SetDlgItemText(IDC_EDIT_FORMAT, "%g Deg");
			else if(m_lbFormats.GetCaretIndex()==5)
				SetDlgItemText(IDC_EDIT_FORMAT, "%g %%");
			else if(m_lbFormats.GetCaretIndex()==6)
				SetDlgItemText(IDC_EDIT_FORMAT, "%.1f %%");
			else if(m_lbFormats.GetCaretIndex()==7)
				SetDlgItemText(IDC_EDIT_FORMAT, "%.2f %%");
			else if(m_lbFormats.GetCaretIndex()==8)
				SetDlgItemText(IDC_EDIT_FORMAT, "$ %g");
			else if(m_lbFormats.GetCaretIndex()==9)
				SetDlgItemText(IDC_EDIT_FORMAT, "$ %.1f");
			else if(m_lbFormats.GetCaretIndex()==10)
				SetDlgItemText(IDC_EDIT_FORMAT, "$ %.2f");
			break;
		case 3:
			if(m_lbFormats.GetCaretIndex()==0)
				SetDlgItemText(IDC_EDIT_FORMAT, "%d %a/%B/%Y");
			else if(m_lbFormats.GetCaretIndex()==1)
				SetDlgItemText(IDC_EDIT_FORMAT, "%d/%b/%Y");
            else if(m_lbFormats.GetCaretIndex()==2)
				SetDlgItemText(IDC_EDIT_FORMAT, "%d/%m/%Y");
            else if(m_lbFormats.GetCaretIndex()==3)
				SetDlgItemText(IDC_EDIT_FORMAT, "%b/%Y");
            else if(m_lbFormats.GetCaretIndex()==4)
				SetDlgItemText(IDC_EDIT_FORMAT, "%d/%B");
			break;
		case 4:
			if(m_lbFormats.GetCaretIndex()==0)
				SetDlgItemText(IDC_EDIT_FORMAT, "%H:%M:%S");
			else if(m_lbFormats.GetCaretIndex()==1)
				SetDlgItemText(IDC_EDIT_FORMAT, "%H %p/%M");
			else if(m_lbFormats.GetCaretIndex()==2)
				SetDlgItemText(IDC_EDIT_FORMAT, "%H:%M");
            else if(m_lbFormats.GetCaretIndex()==3)
				SetDlgItemText(IDC_EDIT_FORMAT, "%M:%S");
			break;
	}

}


void CFormatPropPage::OnCloseupComboType() 
{

	switch(m_cbType.GetCurSel())
	{
		case 0:
			SetAxisTimeFormat("FALSE");
			m_lbFormats.ResetContent();
			m_lbFormats.AddString(".");
            m_lbFormats.AddString(".#");
			m_lbFormats.AddString(".##");
			m_lbFormats.AddString(".###");
			m_lbFormats.AddString(".####");
			break;
		case 1:
			SetAxisTimeFormat("FALSE");
			m_lbFormats.ResetContent();
			m_lbFormats.AddString("e");
            m_lbFormats.AddString(".#e");
			m_lbFormats.AddString(".##e");
			m_lbFormats.AddString(".###e");
			m_lbFormats.AddString(".####e");
			m_lbFormats.AddString("E");
            m_lbFormats.AddString(".#E");
			m_lbFormats.AddString(".##E");
			m_lbFormats.AddString(".###E");
			m_lbFormats.AddString(".####E");
			break;
		case 2:
			SetAxisTimeFormat("FALSE");
			m_lbFormats.ResetContent();
			m_lbFormats.AddString("V");
            m_lbFormats.AddString("A");
			m_lbFormats.AddString("Hz");
			m_lbFormats.AddString("g");
			m_lbFormats.AddString("Deg");
			m_lbFormats.AddString(". %");
			m_lbFormats.AddString(".# %");
            m_lbFormats.AddString(".## %");
			m_lbFormats.AddString("$ .");
			m_lbFormats.AddString("$ .#");
			m_lbFormats.AddString("$ .##");
			break;
		case 3:
			SetAxisTimeFormat("TRUE");
			m_lbFormats.ResetContent();
			m_lbFormats.AddString("ddd/mmm/yyy");
			m_lbFormats.AddString("dd/mm/yy");
			m_lbFormats.AddString("d/m/y");
			m_lbFormats.AddString("m/y");
			m_lbFormats.AddString("d/m");
			break;
		case 4:
			SetAxisTimeFormat("TRUE");
			m_lbFormats.ResetContent();
			m_lbFormats.AddString("h:m:s");
			m_lbFormats.AddString("hh:m");
			m_lbFormats.AddString("h:m");
			m_lbFormats.AddString("m:s");
			break;
	}	
}

void CFormatPropPage::OnCloseupComboAxis() 
{
	UpdateControls();	
}

void CFormatPropPage::SetAxisTimeFormat(CString str)
{

	if(m_cbAxis.GetCurSel())
	{
		if(m_cbAxis.GetCurSel())
			SetPropText("YTime", str);
		else
			SetPropText("YTime", str);
	}
	else
	{
		if(m_cbAxis.GetCurSel())
			SetPropText("XTime", str);
		else
			SetPropText("XTime", str);
	}


}