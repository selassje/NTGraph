// ElementPpg.cpp : implementation file
//

#include "stdafx.h"
#include "NTGraph.hpp"
#include "ElementPpg.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CElementPropPage dialog

IMPLEMENT_DYNCREATE(CElementPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CElementPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CElementPropPage)
	ON_LBN_SELCHANGE(IDC_ELEMENTLIST, OnSelchangeElementlist)
	ON_BN_CLICKED(IDC_ADDBUTTON, OnAddbutton)
	ON_BN_CLICKED(IDC_DELBUTTON, OnDelbutton)
	ON_BN_CLICKED(IDC_CHECK_SOLID, OnCheckSolid)
	ON_BN_CLICKED(IDC_CHECK_VISIBLE, OnCheckVisible)
	ON_MESSAGE(CPN_SELENDOK, OnLineColorChange)
	ON_MESSAGE(CPN_CLOSEUP , OnPointColorChange)
	ON_CBN_CLOSEUP(IDC_COMBO_SYMBOL, OnCloseupComboSymbol)
	ON_CBN_CLOSEUP(IDC_COMBO_TYPE, OnCloseupComboType)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WIDTH, OnDeltaposSpinWidth)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, OnChangeEditWidth)
	//}}AFX_MSG_MAP
	
	
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

// {67B5E3BF-901C-4C69-8D1F-301EDBDA2885}
IMPLEMENT_OLECREATE_EX(CElementPropPage, "NTGraph.CElementPropPage",
	0x67b5e3bf, 0x901c, 0x4c69, 0x8d, 0x1f, 0x30, 0x1e, 0xdb, 0xda, 0x28, 0x85)


/////////////////////////////////////////////////////////////////////////////
// CElementPropPage::CElementPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CElementPropPage

BOOL CElementPropPage::CElementPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Define string resource for page type; replace '0' below with ID.

	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ELEMENTS_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CElementPropPage::CElementPropPage - Constructor

// TODO: Define string resource for page caption; replace '0' below with ID.

CElementPropPage::CElementPropPage() :
// ****** Add your code below this line ********** //
     COlePropertyPage(IDD,  IDS_ELEMENTS_PPG_CAPTION)
// ****** Add your code above this line ********** //

{
	//{{AFX_DATA_INIT(CElementPropPage)
	//}}AFX_DATA_INIT

	SetHelpInfo(_T("Names to appear in the control"), _T("NTGRAPH.HLP"), 0);

}

/////////////////////////////////////////////////////////////////////////////
// CElementPropPage::DoDataExchange - Moves data between page and properties

void CElementPropPage::DoDataExchange(CDataExchange* pDX)
{
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//{{AFX_DATA_MAP(CElementPropPage)
	DDX_Control(pDX, IDC_CHECK_SOLID, m_btnSolid);
	DDX_Control(pDX, IDC_CHECK_VISIBLE, m_btnShow);
	DDX_Control(pDX, IDC_COMBO_SYMBOL, m_cbLineSymbol);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cbLineType);
	DDX_Control(pDX, IDC_ELEMENTLIST, m_lbElementList);
	DDX_Control(pDX, IDC_POINT_COLOR, m_btnPointColor);
	DDX_Control(pDX, IDC_LINE_COLOR, m_btnLineColor);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}

/////////////////////////////////////////////////////////////////////////////
#include <AFXPRIV.H.> 
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
LPDISPATCH CElementPropPage::GetControlDispatch()
{
   // Get the property page's IDispatch array
   ULONG ulObjects;
   LPDISPATCH* lpObjectArray = GetObjectArray( &ulObjects );
   ASSERT( lpObjectArray != NULL );

   // I'm assuming there is but one control, ours.
   // This is a pretty straightforward assumption
   // Most containers don’t even support multi-control
   // selection of custom property pages, yet.
   return( lpObjectArray[0] );
}

void CElementPropPage::AddElement()
{
   // Needed for Unicode conversion functions
   USES_CONVERSION;

   // Get the dispatch of the control
   LPDISPATCH lpControl = GetControlDispatch();

   // Update the control using automation calls
   COleDispatchDriver PropDispDriver;
   DISPID dwDispID;

   // Get a Unicode string
   LPCOLESTR lpOleStr = T2COLE( "AddElement" );

   if (SUCCEEDED( lpControl->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr,
                        1, 0, &dwDispID)))
   {
        PropDispDriver.AttachDispatch( lpControl, FALSE);
		PropDispDriver.InvokeHelper( dwDispID, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
		PropDispDriver.DetachDispatch();
   }
}

void CElementPropPage::DeleteElement(long ElementID) 
{
   // Needed for Unicode conversion functions
   USES_CONVERSION;

   // Get the dispatch of the control
   LPDISPATCH lpControl = GetControlDispatch();

   // Update the control using automation calls
   COleDispatchDriver PropDispDriver;
   DISPID dwDispID;
   // Get a Unicode string
   LPCOLESTR lpOleStr = T2COLE( "DeleteElement" );

   if (SUCCEEDED( lpControl->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr,
                        1, 0, &dwDispID)))
   {
     PropDispDriver.AttachDispatch( lpControl, FALSE);
	 static BYTE parms[] = VTS_I4; 
	 PropDispDriver.InvokeHelper( dwDispID, DISPATCH_METHOD, 
		                          VT_EMPTY, NULL, parms, ElementID); 
	 PropDispDriver.DetachDispatch();
   }
}

/////////////////////////////////////////////////////////////////////////////
// CElementPropPage message handlers
BOOL CElementPropPage::OnInitDialog() 
{
	COlePropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CSpinButtonCtrl* pSpin;
	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WIDTH);


	// Set the buddy control
	pSpin->SetBuddy (GetDlgItem(IDC_EDIT_WIDTH));

	// Set Spin Control Range
	pSpin->SetRange(0,5);

	int count;
    GetPropText("ElementCount",&count);

	CString str("0");

	if(count)
	{
		SetPropText("Element", str);
		str = "Element-0";
		m_lbElementList.AddString(str);
		m_lbElementList.SetCurSel(0);
		UpdateControls();
	}

	m_lbElementList.ResetContent();
	for (int i=0; i<count; i++) 
	{
		str.Format ("Element-%d",i);
		m_lbElementList.AddString(str);
		str.Format("%d",count-1); 
		SetPropText("Element", str);
		m_lbElementList.SetCurSel(count-1);
		UpdateControls();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CElementPropPage::OnSelchangeElementlist() 
{
	int index;
	index = m_lbElementList.GetCurSel ();
	SetPropText( "Element", index ); 
	UpdateControls();
}


void CElementPropPage::OnAddbutton() 
{
    CString str;
	AddElement();
    int index = m_lbElementList.GetCount ();
	str.Format("%d",index);
	SetPropText("Element",str);
	str.Format ("Element-%d",index++);
	m_lbElementList.AddString(str);
    m_lbElementList.SetCurSel(index);

}

void CElementPropPage::OnDelbutton() 
{
	int index = m_lbElementList.GetCurSel ();
    int count = m_lbElementList.GetCount();

	DeleteElement(index);
	m_lbElementList.DeleteString(index);

	if(index==0) {

		if(count==1)
		{
			m_lbElementList.AddString("Element-0");
			m_lbElementList.SetCurSel (index);
			SetPropText("Element",index);
		
		}
		else if (count>1)
		{
			m_lbElementList.SetCurSel (index);
			SetPropText("Element", index);
		
		}
	}
	else
	{	
	    index--;
		m_lbElementList.SetCurSel (index);
		SetPropText("Element", index);
	}
	UpdateControls();
}


void CElementPropPage::OnChangeEditWidth() 
{
 	int width=GetDlgItemInt(IDC_EDIT_WIDTH);
	SetPropText( "ElementWidth", width);
}

void CElementPropPage::OnDeltaposSpinWidth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	int width=GetDlgItemInt(IDC_EDIT_WIDTH);
	SetPropText( "ElementWidth", width);
	
	*pResult = 0;
}


void CElementPropPage::OnCheckSolid() 
{
	int solid = m_btnSolid.GetCheck();
	SetPropText("ElementSolidPoint", solid );
}

void CElementPropPage::OnCheckVisible() 
{
	int visible = m_btnShow.GetCheck();
	SetPropText("ElementShow", visible );

}
LRESULT CElementPropPage::OnLineColorChange(WPARAM, LPARAM)
{
	int count;
	GetPropText("ElementCount", &count);
	
	CString str;
	str.Format("%d", m_btnLineColor.GetColour());
	SetPropText("ElementLineColor", str);	
	
    TRACE0("Selection changed= %d\n");
    return TRUE;
}
LRESULT CElementPropPage::OnPointColorChange(WPARAM /*lParam*/, LPARAM /*wParam*/)
{
	int count;
	GetPropText("ElementCount", &count);
	
	CString str;
	str.Format("%d", m_btnPointColor.GetColour());
	SetPropText("ElementPointColor", str);	
	
    TRACE0("Selection changed= %d\n");
    return TRUE;
}

void CElementPropPage::OnCloseupComboSymbol() 
{
   int symbol = m_cbLineSymbol.GetCurSel();
   SetPropText("ElementPointSymbol", symbol );

}
void CElementPropPage::OnCloseupComboType() 
{
   int type = m_cbLineType.GetCurSel();
   SetPropText("ElementLinetype", type );
}


void CElementPropPage::OnChangeEditName() 
{
   CString str;
   GetDlgItemText(IDC_EDIT_NAME, str);
   SetPropText("ElementName",  str);
}

BOOL CElementPropPage::OnHelp(LPCTSTR)
{
   // OnHelp is called by IPropertyPage::Help.  When someone requests
   // it, we supply help by calling CWinApp::WinHelp
   AfxGetApp()->WinHelp(0,HELP_CONTENTS);
   return TRUE;
}

void CElementPropPage::UpdateControls()
{
	COLORREF colour;
	GetPropText("ElementLineColor", &colour);
	m_btnLineColor.SetColour(colour);

	GetPropText("ElementPointColor", &colour);
	m_btnPointColor.SetColour(colour);

	int width;
	GetPropText("ElementWidth",&width);
    SetDlgItemInt(IDC_EDIT_WIDTH,width);

	int type;
	GetPropText("ElementLinetype", &type);
	m_cbLineType.SetCurSel(type);

	int symbol;
	GetPropText("ElementPointSymbol", &symbol);
	m_cbLineSymbol.SetCurSel(symbol);

	BOOL visible;
	GetPropText("ElementShow", &visible);
	
	if(visible)
		m_btnShow.SetCheck (1);
	else
		m_btnShow.SetCheck (0);
	
	BOOL solid;
	GetPropText("ElementSolidPoint", &solid);
	
	if(solid)
		m_btnSolid.SetCheck (1);
	else
		m_btnSolid.SetCheck (0);


	CString ids;
	GetPropText("ElementName", &ids);
	SetDlgItemText(IDC_EDIT_NAME,ids);
}




