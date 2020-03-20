// AnnoPpg.cpp : implementation file
//

#include "stdafx.h"
#include "NTGraph.hpp"
#include "AnnoPpg.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnnoPropPage dialog

IMPLEMENT_DYNCREATE(CAnnoPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CAnnoPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CAnnoPropPage)
	ON_BN_CLICKED(IDC_ADDBUTTON, OnAddbutton)
	ON_BN_CLICKED(IDC_DELBUTTON, OnDelbutton)
	ON_LBN_SELCHANGE(IDC_ANNOTATIONS, OnSelchangeAnnotations)
	ON_EN_CHANGE(IDC_EDIT_CAPTION, OnChangeEditCaption)
	ON_EN_CHANGE(IDC_EDIT_X, OnChangeEditX)
	ON_EN_CHANGE(IDC_EDIT_Y, OnChangeEditY)
	ON_CBN_CLOSEUP(IDC_COMBO_ORIENTATION, OnCloseupComboOrientation)
	ON_BN_CLICKED(IDC_CHECK_VISIBLE, OnCheckVisible)
	ON_MESSAGE(CPN_SELENDOK, OnColorChange)
	ON_MESSAGE(CPN_CLOSEUP , OnBkColorChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

// {02D3B02A-A146-4969-A5C1-27B870B9B410}
IMPLEMENT_OLECREATE_EX(CAnnoPropPage, "NTGraph.CAnnoPropPage",
	0x2d3b02a, 0xa146, 0x4969, 0xa5, 0xc1, 0x27, 0xb8, 0x70, 0xb9, 0xb4, 0x10)


/////////////////////////////////////////////////////////////////////////////
// CAnnoPropPage::CAnnoPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CAnnoPropPage

BOOL CAnnoPropPage::CAnnoPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Define string resource for page type; replace '0' below with ID.

	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ANNOTATION_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CAnnoPropPage::CAnnoPropPage - Constructor

// TODO: Define string resource for page caption; replace '0' below with ID.

CAnnoPropPage::CAnnoPropPage() :
// ****** Add your code below this line ********** //
     COlePropertyPage(IDD,  IDS_ANNOTATION_PPG_CAPTION)
// ****** Add your code above this line ********** //
{
	//{{AFX_DATA_INIT(CAnnoPropPage)
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CAnnoPropPage::DoDataExchange - Moves data between page and properties

void CAnnoPropPage::DoDataExchange(CDataExchange* pDX)
{
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//{{AFX_DATA_MAP(CAnnoPropPage)
	DDX_Control(pDX, IDC_LABEL_BGCOLOR, m_btnBkColor);
	DDX_Control(pDX, IDC_LABEL_COLOR, m_btnColor);
	DDX_Control(pDX, IDC_CHECK_VISIBLE, m_btnVisible);
	DDX_Control(pDX, IDC_COMBO_ORIENTATION, m_cbOrientation);
	DDX_Control(pDX, IDC_ANNOTATIONS, m_lbAnnotations);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}

/////////////////////////////////////////////////////////////////////////////
#include <AFXPRIV.H.> 
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
LPDISPATCH CAnnoPropPage::GetControlDispatch()
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

void CAnnoPropPage::AddAnnotation()
{
   // Needed for Unicode conversion functions
   USES_CONVERSION;

   // Get the dispatch of the control
   LPDISPATCH lpControl = GetControlDispatch();

   // Update the control using automation calls
   COleDispatchDriver PropDispDriver;
   DISPID dwDispID;

   // Get a Unicode string
   LPCOLESTR lpOleStr = T2COLE( "AddAnnotation" );

   if (SUCCEEDED( lpControl->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr,
                        1, 0, &dwDispID)))
   {
        PropDispDriver.AttachDispatch( lpControl, FALSE);
		PropDispDriver.InvokeHelper( dwDispID, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
		PropDispDriver.DetachDispatch();
   }
}

void CAnnoPropPage::DeleteAnnotation(short annoID) 
{
   // Needed for Unicode conversion functions
   USES_CONVERSION;

   // Get the dispatch of the control
   LPDISPATCH lpControl = GetControlDispatch();

   // Update the control using automation calls
   COleDispatchDriver PropDispDriver;
   DISPID dwDispID;
   // Get a Unicode string
   LPCOLESTR lpOleStr = T2COLE( "DeleteAnnotation" );

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
// CAnnoPropPage message handlers

void CAnnoPropPage::OnAddbutton() 
{
    CString str;
	AddAnnotation();

	int index;
	GetPropText("Annotation", &index);
	
	str.Format ("Annotation-%d",index);
	m_lbAnnotations.AddString(str);
    m_lbAnnotations.SetCurSel(index);
    
    SetPropText("Annotation", index);
	UpdateControls();
	  	
}

void CAnnoPropPage::OnDelbutton() 
{
	int index = m_lbAnnotations.GetCurSel ();
    int count = m_lbAnnotations.GetCount();

	DeleteAnnotation(index);
	m_lbAnnotations.DeleteString(index);
	
	index = m_lbAnnotations.GetCount()-1;
	
	m_lbAnnotations.SetCurSel(index);

	if(index>=0)
		SetPropText("Annotation", index);
	
	UpdateControls();
}

void CAnnoPropPage::OnSelchangeAnnotations() 
{
	int index;
	index = m_lbAnnotations.GetCurSel ();
	SetPropText( "Annotation", index ); 
	UpdateControls();
}

void CAnnoPropPage::UpdateControls()
{
	CString caption;
	GetPropText("AnnoLabelCaption", &caption);
	SetDlgItemText (IDC_EDIT_CAPTION, caption);

	CString x,y;
	GetPropText("AnnoLabelX", &x);
    GetPropText("AnnoLabelY", &y);

	SetDlgItemText (IDC_EDIT_X, x);
	SetDlgItemText (IDC_EDIT_Y, y);

	COLORREF color;
	GetPropText("AnnoLabelColor", &color);
    m_btnColor.SetColour(color);

	GetPropText("AnnoLabelBkColor", &color);
    m_btnBkColor.SetColour(color);

	
	BOOL orientation;
	GetPropText("AnnoLabelHorizontal", &orientation);
	m_cbOrientation.SetCurSel(((int)orientation)+1);

	BOOL visible;
	GetPropText("AnnoVisible", &visible);

	if(visible)
		CheckDlgButton(IDC_CHECK_VISIBLE,BST_CHECKED);
	else
		CheckDlgButton(IDC_CHECK_VISIBLE,BST_UNCHECKED);	

	int count;
    GetPropText("AnnoCount", &count);
	
	CWnd* pWnd = GetDlgItem(IDC_DELBUTTON);
    if (pWnd)
		if (count > 0)
			pWnd->EnableWindow(TRUE);
		else
			pWnd->EnableWindow(FALSE);

	COLORREF colour;
	GetPropText("AnnoLabelColor", &colour);
	m_btnColor.SetColour(colour);
} 

BOOL CAnnoPropPage::OnInitDialog() 
{
	COlePropertyPage::OnInitDialog();
	
	int count;
    GetPropText("AnnoCount", &count);

	if(count>0)  
	{
		m_lbAnnotations.ResetContent();
		for (int i=0; i < count; i++) 
		{
			CString str;
			str.Format ("Annotation-%d",i);
			m_lbAnnotations.AddString(str);
		}

		CString str;
		str.Format("%d",count-1);
		SetPropText("Annotation", str);
		m_lbAnnotations.SetCurSel(count-1);
	}

	UpdateControls();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAnnoPropPage::OnChangeEditCaption() 
{
	int count;
	GetPropText("AnnoCount", &count);
	
	if(count>0)
	{
		CString str ;
		GetDlgItemText(IDC_EDIT_CAPTION, str);
		SetPropText("AnnoLabelCaption", str);
	}
}

void CAnnoPropPage::OnChangeEditX() 
{
	int count;
	GetPropText("AnnoCount", &count);
	
	if(count>0)
	{
		CString str ;
		GetDlgItemText(IDC_EDIT_X, str);
		SetPropText("AnnoLabelX", str);
	}	
}

void CAnnoPropPage::OnChangeEditY() 
{
	int count;
	GetPropText("AnnoCount", &count);
	
	if(count>0)
	{
		CString str ;
		GetDlgItemText(IDC_EDIT_Y, str);
		SetPropText("AnnoLabelY", str);
	}
}

void CAnnoPropPage::OnCloseupComboOrientation() 
{
	int count;
	GetPropText("AnnoCount", &count);
	
	if(count>0)
	{
		CString str("TRUE");
		int horizontal = m_cbOrientation.GetCurSel();
		if(horizontal ==0)
			SetPropText("AnnoLabelHorizontal", str);
		else
		{
			str = "FALSE";
			SetPropText("AnnoLabelHorizontal", str);
		}
	}	
}

void CAnnoPropPage::OnCheckVisible() 
{
	int count;
	GetPropText("AnnoCount", &count);
	
	if(count>0)
	{
		CString str("TRUE");		

		if(m_btnVisible.GetCheck())
			SetPropText("AnnoVisible", str);
		else
		{   
			str = "FALSE";
            SetPropText("AnnoVisible", str);
		}

	}	
}


LRESULT CAnnoPropPage::OnColorChange(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	int count;
	GetPropText("AnnoCount", &count);
	
	if(count>0)
	{
		CString str;
		str.Format("%d",m_btnColor.GetColour());
		SetPropText("AnnoLabelColor", str);	
	}	

    TRACE0("Selection changed= %d\n");
    return TRUE;
}

LRESULT CAnnoPropPage::OnBkColorChange(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	int count;
	GetPropText("AnnoCount", &count);
	
	if(count>0)
	{
		CString str;
		str.Format("%d",m_btnBkColor.GetColour());
		SetPropText("AnnoLabelBkColor", str);	
	}	

    TRACE0("Selection changed= %d\n");
    return TRUE;
}
