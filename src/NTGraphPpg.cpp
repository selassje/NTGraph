// NTGraphPpg.cpp : Implementation of the CNTGraphPropPage property page class.

#include "stdafx.h"
#include "NTGraph.h"
#include "NTGraphPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CNTGraphPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CNTGraphPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CNTGraphPropPage)
	ON_WM_HELPINFO()
	ON_MESSAGE(CPN_CLOSEUP , OnColorChange)
	ON_EN_CHANGE(IDC_CAPTION, OnChangeCaption)
	ON_BN_CLICKED(IDC_CHECK_SHOWGRID, OnCheckShowgrid)
	ON_BN_CLICKED(IDC_CHECK_XLOG, OnCheckXlog)
	ON_BN_CLICKED(IDC_CHECK_YLOG, OnCheckYlog)
	ON_CBN_CLOSEUP(IDC_COMBO_FRAME, OnCloseupComboFrame)
	ON_CBN_CLOSEUP(IDC_COMBO_MODE, OnCloseupComboMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CNTGraphPropPage, "NTGRAPH.NTGraphPropPage.1",
	0x2dd5d509, 0xe89e, 0x4825, 0x87, 0xd3, 0xa9, 0x39, 0xe6, 0x89, 0xbd, 0x25)


/////////////////////////////////////////////////////////////////////////////
// CNTGraphPropPage::CNTGraphPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CNTGraphPropPage

BOOL CNTGraphPropPage::CNTGraphPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_NTGRAPH_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CNTGraphPropPage::CNTGraphPropPage - Constructor

CNTGraphPropPage::CNTGraphPropPage() :
	COlePropertyPage(IDD, IDS_NTGRAPH_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CNTGraphPropPage)
	//}}AFX_DATA_INIT

	SetHelpInfo(_T("Names to appear in the control"), _T("NTGRAPH.HLP"), 0);
}


/////////////////////////////////////////////////////////////////////////////
// CNTGraphPropPage::DoDataExchange - Moves data between page and properties

void CNTGraphPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CNTGraphPropPage)
	DDX_Control(pDX, IDC_CHECK_YLOG, m_btnYLog);
	DDX_Control(pDX, IDC_CHECK_XLOG, m_btnXLog);
	DDX_Control(pDX, IDC_CHECK_SHOWGRID, m_btnGrid);
	DDX_Control(pDX, IDC_FRAME_COLOR, m_btnFrameColor);
	DDX_Control(pDX, IDC_PLOT_COLOR, m_btnPlotColor);
	DDX_Control(pDX, IDC_GRID_COLOR, m_btnGridColor);
	DDX_Control(pDX, IDC_AXIS_COLOR, m_btnAxisColor);
	DDX_Control(pDX, IDC_LABEL_COLOR, m_btnLabelColor);
	DDP_CBIndex(pDX, IDC_COMBO_FRAME, m_nFrame, _T("FrameStyle") );
	DDX_CBIndex(pDX, IDC_COMBO_FRAME, m_nFrame);
	DDX_Control(pDX, IDC_COMBO_FRAME, m_cbFrame);
	DDP_CBIndex(pDX, IDC_COMBO_FRAME, m_nMode, _T("TrackMode") );
	DDX_CBIndex(pDX, IDC_COMBO_FRAME, m_nMode);
	DDX_Control(pDX, IDC_COMBO_MODE, m_cbTrackMode);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);

}


/////////////////////////////////////////////////////////////////////////////
// CNTGraphPropPage message handlers
///////////////////////////////////////////////////////////////////////////


BOOL CNTGraphPropPage::OnHelpInfo(HELPINFO* pHelpInfo) 
{

	// OnHelp is called by IPropertyPage::Help.  When someone requests
	// it, we supply help by calling CWinApp::WinHelp
	 AfxGetApp()->WinHelp(0,HELP_CONTENTS);
	
	return COlePropertyPage::OnHelpInfo(pHelpInfo);
}

BOOL CNTGraphPropPage::OnHelp(LPCTSTR)
{

	// OnHelp is called by IPropertyPage::Help.  When someone requests
	// it, we supply help by calling CWinApp::WinHelp
	 AfxGetApp()->WinHelp(0,HELP_CONTENTS);


return TRUE;
}
LRESULT CNTGraphPropPage::OnColorChange(WPARAM /*lParam*/, LPARAM /*wParam*/)
{

	CString str;
	
	str.Format("%d",m_btnFrameColor.GetColour());
	SetPropText("ControlFrameColor", str);

	str.Format("%d",m_btnPlotColor.GetColour());
	SetPropText("PlotAreaColor", str);
	
	str.Format("%d",m_btnGridColor.GetColour());
	SetPropText("GridColor", str);
	
	str.Format("%d",m_btnAxisColor.GetColour());
	SetPropText("AxisColor", str);
	
	str.Format("%d",m_btnLabelColor.GetColour());
	SetPropText("LabelColor", str);


    TRACE0("Selection changed= %d\n");
    return TRUE;
}

void CNTGraphPropPage::OnChangeCaption() 
{
	CString str;
	GetDlgItemText(IDC_CAPTION, str);
	SetPropText("Caption", str);		
}


void CNTGraphPropPage::OnCheckShowgrid() 
{
	CString str("TRUE");		

	if(m_btnGrid.GetCheck())
		SetPropText("ShowGrid", str);
	else
	{   
		str = "FALSE";
        SetPropText("ShowGrid", str);
	}

}

void CNTGraphPropPage::OnCheckXlog() 
{
	CString str("TRUE");		

	if(m_btnXLog.GetCheck())
		SetPropText("XLog", str);
	else
	{   
		str = "FALSE";
        SetPropText("XLog", str);
	}
}

void CNTGraphPropPage::OnCheckYlog() 
{
	CString str("TRUE");		

	if(m_btnYLog.GetCheck())
		SetPropText("YLog", str);
	else
	{   
		str = "FALSE";
        SetPropText("YLog", str);
	}	
}

void CNTGraphPropPage::OnCloseupComboFrame() 
{
	CString str;
	str.Format("%d",m_cbFrame.GetCurSel());
   	SetPropText("FrameStyle", str);		
}

void CNTGraphPropPage::OnCloseupComboMode() 
{
	CString str;
	str.Format("%d",m_cbTrackMode.GetCurSel());
   	SetPropText("TrackMode", str);			
}

void CNTGraphPropPage::UpdateControls() 
{
	CString caption;
	GetPropText("Caption", &caption);

	SetDlgItemText (IDC_CAPTION, caption);

	COLORREF color;

	GetPropText("AxisColor", &color);
    m_btnAxisColor.SetColour(color);

	GetPropText("ControlFrameColor", &color);
    m_btnFrameColor.SetColour(color);

	GetPropText("GridColor", &color);
    m_btnGridColor.SetColour(color);

	GetPropText("PlotAreaColor", &color);
    m_btnPlotColor.SetColour(color);

	GetPropText("LabelColor", &color);
    m_btnLabelColor.SetColour(color);

	int frame;
	GetPropText("FrameStyle", &frame);
	m_cbFrame.SetCurSel(frame);

	int mode;
	GetPropText("TrackMode", &mode);
	m_cbTrackMode.SetCurSel(mode);

	BOOL grid;
	GetPropText("ShowGrid", &grid);

	if(grid)
		CheckDlgButton(IDC_CHECK_SHOWGRID,BST_CHECKED);
	else
		CheckDlgButton(IDC_CHECK_SHOWGRID,BST_UNCHECKED);	

	BOOL xlog;
	GetPropText("XLog", &xlog);

	if(xlog)
		CheckDlgButton(IDC_CHECK_XLOG,BST_CHECKED);
	else
		CheckDlgButton(IDC_CHECK_XLOG,BST_UNCHECKED);	

	BOOL ylog;
	GetPropText("YLog", &ylog);

	if(ylog)
		CheckDlgButton(IDC_CHECK_YLOG,BST_CHECKED);
	else
		CheckDlgButton(IDC_CHECK_YLOG,BST_UNCHECKED);	


}

BOOL CNTGraphPropPage::OnInitDialog() 
{
	COlePropertyPage::OnInitDialog();
	


	UpdateControls();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
