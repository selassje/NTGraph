// CursorPpg.cpp : implementation file
//

#include "stdafx.h"
#include "NTGraph.h"
#include "CursorPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCursorPropPage dialog

IMPLEMENT_DYNCREATE(CCursorPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCursorPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CCursorPropPage)
	ON_BN_CLICKED(IDC_ADDBUTTON, OnAddbutton)
	ON_BN_CLICKED(IDC_DELBUTTON, OnDelbutton)
	ON_LBN_SELCHANGE(IDC_CURSORS, OnSelchangeCursors)
	ON_EN_CHANGE(IDC_EDIT_X, OnChangeEditX)
	ON_EN_CHANGE(IDC_EDIT_Y, OnChangeEditY)
	ON_BN_CLICKED(IDC_CHECK_VISIBLE, OnCheckVisible)
	ON_MESSAGE(CPN_SELENDOK, OnColorChange)
	ON_CBN_CLOSEUP(IDC_COMBO_STYLE, OnCloseupComboStyle)
	ON_CBN_CLOSEUP(IDC_COMBO_SNAP, OnCloseupComboSnap)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

// {F8B1D8D6-10AB-42F4-AEAC-6436CBE62171}
IMPLEMENT_OLECREATE_EX(CCursorPropPage, "NTGraph.CCursorPropPage",
	0xf8b1d8d6, 0x10ab, 0x42f4, 0xae, 0xac, 0x64, 0x36, 0xcb, 0xe6, 0x21, 0x71)


/////////////////////////////////////////////////////////////////////////////
// CCursorPropPage::CCursorPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CCursorPropPage

BOOL CCursorPropPage::CCursorPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Define string resource for page type; replace '0' below with ID.

	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CURSOR_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CCursorPropPage::CCursorPropPage - Constructor

// TODO: Define string resource for page caption; replace '0' below with ID.

CCursorPropPage::CCursorPropPage() :
// ****** Add your code below this line ********** //
     COlePropertyPage(IDD,  IDS_CURSOR_PPG_CAPTION)
// ****** Add your code above this line ********** //
{
	//{{AFX_DATA_INIT(CCursorPropPage)
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CCursorPropPage::DoDataExchange - Moves data between page and properties

void CCursorPropPage::DoDataExchange(CDataExchange* pDX)
{
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//{{AFX_DATA_MAP(CCursorPropPage)
	DDX_Control(pDX, IDC_CHECK_VISIBLE, m_btnVisible);
	DDX_Control(pDX, IDC_COMBO_SNAP, m_cbSnap);
	DDX_Control(pDX, IDC_COMBO_STYLE, m_cbStyle);
	DDX_Control(pDX, IDC_CURSOR_COLOR, m_btnColor);
	DDX_Control(pDX, IDC_CURSORS, m_lbCursors);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}
/////////////////////////////////////////////////////////////////////////////
#include <AFXPRIV.H.> 
///////////////////////////////////////////////////////////////////////
LPDISPATCH CCursorPropPage::GetControlDispatch()
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

void CCursorPropPage::AddCursor()
{
   // Needed for Unicode conversion functions
   USES_CONVERSION;

   // Get the dispatch of the control
   LPDISPATCH lpControl = GetControlDispatch();

   // Update the control using automation calls
   COleDispatchDriver PropDispDriver;
   DISPID dwDispID;

   // Get a Unicode string
   LPCOLESTR lpOleStr = T2COLE( "AddCursor" );

   if (SUCCEEDED( lpControl->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr,
                        1, 0, &dwDispID)))
   {
        PropDispDriver.AttachDispatch( lpControl, FALSE);
		PropDispDriver.InvokeHelper( dwDispID, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
		PropDispDriver.DetachDispatch();
   }
}

void CCursorPropPage::DeleteCursor(short annoID) 
{
   // Needed for Unicode conversion functions
   USES_CONVERSION;

   // Get the dispatch of the control
   LPDISPATCH lpControl = GetControlDispatch();

   // Update the control using automation calls
   COleDispatchDriver PropDispDriver;
   DISPID dwDispID;
   // Get a Unicode string
   LPCOLESTR lpOleStr = T2COLE( "DeleteCursor" );

   if (SUCCEEDED( lpControl->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr,
                        1, 0, &dwDispID)))
   {
     PropDispDriver.AttachDispatch( lpControl, FALSE);
	 static BYTE parms[] = VTS_I2; 
	 PropDispDriver.InvokeHelper( dwDispID, DISPATCH_METHOD, 
		                          VT_EMPTY, NULL, parms, annoID); 
	 PropDispDriver.DetachDispatch();
   }
}

/////////////////////////////////////////////////////////////////////////////
// CCursorPropPage message handlers

BOOL CCursorPropPage::OnInitDialog() 
{
	COlePropertyPage::OnInitDialog();
	
	int count;
    GetPropText("CursorCount", &count);

	if(count>0)  
	{
		m_lbCursors.ResetContent();
		for (int i=0; i < count; i++) 
		{
			CString str;
			str.Format ("Cursor-%d",i);
			m_lbCursors.AddString(str);
		}

		CString str;
		str.Format("%d",count-1);
		SetPropText("Cursor", str);
		m_lbCursors.SetCurSel(count-1);
	}

	UpdateControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCursorPropPage::OnAddbutton() 
{
    CString str;
	AddCursor();

	int index;
	GetPropText("Cursor", &index);
	
	str.Format ("Cursor-%d",index);
	m_lbCursors.AddString(str);
    m_lbCursors.SetCurSel(index);
    
    SetPropText("Cursor", index);
	UpdateControls();
	  	
}

void CCursorPropPage::OnDelbutton() 
{
	int index = m_lbCursors.GetCurSel ();
    int count = m_lbCursors.GetCount();

	DeleteCursor(index);
	m_lbCursors.DeleteString(index);
	
	index = m_lbCursors.GetCount()-1;
	
	m_lbCursors.SetCurSel(index);

	if(index>=0)
		SetPropText("Cursor", index);
	
	UpdateControls();
}

void CCursorPropPage::UpdateControls()
{

	CString x,y;
	GetPropText("CursorX", &x);
    GetPropText("CursorY", &y);

	SetDlgItemText (IDC_EDIT_X, x);
	SetDlgItemText (IDC_EDIT_Y, y);

	COLORREF color;
	GetPropText("CursorColor", &color);
    m_btnColor.SetColour(color);

	int style;
	GetPropText("CursorStyle", &style);
	m_cbStyle.SetCurSel(style);

	int snap;
	GetPropText("CursorMode", &snap);
	m_cbSnap.SetCurSel(snap);


	BOOL visible;
	GetPropText("CursorVisible", &visible);

	if(visible)
		CheckDlgButton(IDC_CHECK_VISIBLE,BST_CHECKED);
	else
		CheckDlgButton(IDC_CHECK_VISIBLE,BST_UNCHECKED);	

	int count;
    GetPropText("CursorCount", &count);
	
	CWnd* pWnd = GetDlgItem(IDC_DELBUTTON);
    if (pWnd)
		if (count > 0)
			pWnd->EnableWindow(TRUE);
		else
			pWnd->EnableWindow(FALSE);

} 

void CCursorPropPage::OnSelchangeCursors() 
{
	int index;
	index = m_lbCursors.GetCurSel ();
	SetPropText( "Cursor", index ); 
	UpdateControls();	
}

void CCursorPropPage::OnChangeEditX() 
{
	int count;
	GetPropText("CursorCount", &count);
	
	if(count>0)
	{
		CString str ;
		GetDlgItemText(IDC_EDIT_X, str);
		SetPropText("CursorX", str);
	}	
}

void CCursorPropPage::OnChangeEditY() 
{
	int count;
	GetPropText("CursorCount", &count);
	
	if(count>0)
	{
		CString str ;
		GetDlgItemText(IDC_EDIT_Y, str);
		SetPropText("CursorY", str);
	}	
	
}

void CCursorPropPage::OnCheckVisible() 
{
	int count;
	GetPropText("CursorCount", &count);
	
	if(count>0)
	{
		CString str("TRUE");		

		if(m_btnVisible.GetCheck())
			SetPropText("CursorVisible", str);
		else
		{   
			str = "FALSE";
            SetPropText("CursorVisible", str);
		}

	}		
}
LRESULT CCursorPropPage::OnColorChange(WPARAM /*lParam*/, LPARAM /*wParam*/)
{
	int count;
	GetPropText("CursorCount", &count);
	
	if(count>0)
	{
		CString str;
		str.Format("%d",m_btnColor.GetColour());
		SetPropText("CursorColor", str);	
	}	

    TRACE0("Selection changed= %d\n");
    return TRUE;
}


void CCursorPropPage::OnCloseupComboStyle() 
{
	int count;
	GetPropText("CursorCount", &count);
	
	if(count>0)
	{
		CString str("TRUE");
		int style = m_cbStyle.GetCurSel();
		SetPropText("CursorStyle", style);
	}		
}

void CCursorPropPage::OnCloseupComboSnap() 
{
	int count;
	GetPropText("CursorCount", &count);
	
	if(count>0)
	{
		CString str("TRUE");
		int mode = m_cbSnap.GetCurSel();
		SetPropText("CursorMode", mode);
	}	
}
