/////////////////////////////////////////////////////////////////////////////
// NTGraphCtl.cpp : Implementation of the CNTGraphCtrl ActiveX Control class.
//
// Autor : Nikolai Teofilov  nteofilov@yahoo.de
//			
// Comment	: Use with your own risk !
//
// Copyright (c) 2003.



#include "stdafx.h"
#include "NTGraphCtl.hpp"
#include "NTGraphPpg.hpp"
#include "ElementPpg.hpp"
#include "AnnoPpg.hpp"
#include "CursorPpg.hpp"
#include "FormatPpg.hpp"

void DrawDiamond(CDC* pDC, CPoint point, int symsz);
void DrawAsterisk(CDC* pDC, CPoint point, int symsz);
void DrawDownTriangle(CDC* pDC, CPoint point, int symsz);
void DrawRightTriangle(CDC* pDC, CPoint point, int symsz);
void DrawUpTriangle(CDC* pDC, CPoint point, int symsz);
void DrawLeftTriangle(CDC* pDC, CPoint point, int symsz);

#ifndef MAX
#define MAX 1
#endif

#ifndef MIN
#define MIN 0
#endif

#define PT2DBLX(x) (double)((x - m_axisRect.left)*dResX)+ dRangeX[MIN]
#define PT2DBLY(y) (double)((m_axisRect.bottom - y)*dResY)+ dRangeY[MIN]
#define ROUND(x) 	((int)((x) + (((x)<0.0) ? -0.5 : 0.5)))

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// This help id is also defined and used in the
// testhelp.hpj help project file.  It references
// a topic in the testhelp.hlp help file.

#define IDH_WHATISTHISTHING 18001


IMPLEMENT_DYNCREATE(CNTGraphCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CNTGraphCtrl, COleControl)
	//{{AFX_MSG_MAP(CNTGraphCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CNTGraphCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CNTGraphCtrl)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "AxisColor", m_axisColor, OnAxisColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "GridColor", m_gridColor, OnGridColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "LabelColor", m_labelColor, OnLabelColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "CursorColor", m_cursorColor, OnCursorColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "XGridNumber", m_nGridX, OnXGridNumberChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "YGridNumber", m_nGridY, OnYGridNumberChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "ShowGrid", m_showGrid, OnShowGridChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "XLabel", m_xLabel, OnXLabelChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "YLabel", m_yLabel, OnYLabelChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "ElementLineColor", m_elementLineColor, OnElementLineColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "ElementPointColor", m_elementPointColor, OnElementPointColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "ElementLinetype", m_elementLinetype, OnElementLinetypeChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "ElementWidth", m_elementWidth, OnElementWidthChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "ElementPointSymbol", m_elementPointSymbol, OnElementPointSymbolChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "ElementSolidPoint", m_elementSolidPoint, OnElementSolidPointChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "ElementShow", m_elementShow, OnElementShowChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "ElementName", m_elementName, OnElementNameChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "ElementIdentify", m_elementIdentify, OnElementIdentifyChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "XLog", m_xLog, OnXLogChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "YLog", m_yLog, OnYLogChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "ControlFrameColor", m_controlFrameColor, OnControlFrameColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "PlotAreaColor", m_plotAreaColor, OnPlotAreaColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "FrameStyle", m_frameStyle, OnFrameStyleChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "AnnoLabelCaption", m_annoLabelCaption, OnAnnoLabelCaptionChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "AnnoLabelX", m_annoLabelX, OnAnnoLabelXChanged, VT_R8)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "AnnoLabelY", m_annoLabelY, OnAnnoLabelYChanged, VT_R8)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "AnnoLabelColor", m_annoLabelColor, OnAnnoLabelColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "AnnoLabelHorizontal", m_annoLabelHorizontal, OnAnnoLabelHorizontalChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "Annotation", m_nAnnotation, OnAnnotationChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "AnnoVisible", m_annoVisible, OnAnnoVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "Element", m_nElementID, OnElementChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "AnnoLabelBkColor", m_annoLabelBkColor, OnAnnoLabelBkColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "Cursor", m_nCursorID, OnCursorChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "CursorX", m_cursorX, OnCursorXChanged, VT_R8)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "CursorY", m_cursorY, OnCursorYChanged, VT_R8)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "CursorStyle", m_cursorStyle, OnCursorStyleChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "CursorVisible", m_cursorVisible, OnCursorVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "CursorMode", m_cursorMode, OnCursorModeChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "FormatAxisBottom", m_formatAxisBottom, OnFormatAxisBottomChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "FormatAxisLeft", m_formatAxisLeft, OnFormatAxisLeftChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "YTime", m_yTime, OnYTimeChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CNTGraphCtrl, "XTime", m_xTime, OnXTimeChanged, VT_BOOL)
	DISP_PROPERTY_EX(CNTGraphCtrl, "LabelFont", GetLabelFont, SetLabelFont, VT_FONT)
	DISP_PROPERTY_EX(CNTGraphCtrl, "TickFont", GetTickFont, SetTickFont, VT_FONT)
	DISP_PROPERTY_EX(CNTGraphCtrl, "TitleFont", GetTitleFont, SetTitleFont, VT_FONT)
	DISP_PROPERTY_EX(CNTGraphCtrl, "IdentFont", GetIdentFont, SetIdentFont, VT_FONT)
	DISP_PROPERTY_EX(CNTGraphCtrl, "TrackMode", GetTrackMode, SetTrackMode, VT_I4)
	DISP_PROPERTY_EX(CNTGraphCtrl, "ControlFramePicture", GetControlFramePicture, SetControlFramePicture, VT_PICTURE)
	DISP_PROPERTY_EX(CNTGraphCtrl, "PlotAreaPicture", GetPlotAreaPicture, SetPlotAreaPicture, VT_PICTURE)
	DISP_PROPERTY_EX(CNTGraphCtrl, "AnnoCount", GetAnnoCount, SetNotSupported, VT_I2)
	DISP_PROPERTY_EX(CNTGraphCtrl, "ElementCount", GetElementCount, SetNotSupported, VT_I2)
	DISP_PROPERTY_EX(CNTGraphCtrl, "CursorCount", GetCursorCount, SetNotSupported, VT_I2)
	DISP_FUNCTION(CNTGraphCtrl, "SetRange", SetRange, VT_EMPTY, VTS_R8 VTS_R8 VTS_R8 VTS_R8)
	DISP_FUNCTION(CNTGraphCtrl, "AutoRange", AutoRange, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CNTGraphCtrl, "CopyToClipboard", CopyToClipboard, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CNTGraphCtrl, "PrintGraph", PrintGraph, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CNTGraphCtrl, "AddElement", AddElement, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CNTGraphCtrl, "DeleteElement", DeleteElement, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CNTGraphCtrl, "ClearGraph", ClearGraph, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CNTGraphCtrl, "PlotXY", PlotXY, VT_EMPTY, VTS_R8 VTS_R8 VTS_I2)
	DISP_FUNCTION(CNTGraphCtrl, "PlotY", PlotY, VT_EMPTY, VTS_R8 VTS_I2)
	DISP_FUNCTION(CNTGraphCtrl, "ShowProperties", ShowProperties, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CNTGraphCtrl, "SaveAs", SaveAs, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CNTGraphCtrl, "AddAnnotation", AddAnnotation, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CNTGraphCtrl, "DeleteAnnotation", DeleteAnnotation, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CNTGraphCtrl, "AddCursor", AddCursor, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CNTGraphCtrl, "DeleteCursor", DeleteCursor, VT_EMPTY, VTS_I2)
	DISP_PROPERTY_PARAM(CNTGraphCtrl, "ElementXValue", GetElementXValue, SetElementXValue, VT_R8, VTS_I4 VTS_I2)
	DISP_PROPERTY_PARAM(CNTGraphCtrl, "ElementYValue", GetElementYValue, SetElementYValue, VT_R8, VTS_I4 VTS_I2)
	DISP_STOCKPROP_APPEARANCE()
	DISP_STOCKPROP_CAPTION()
	DISP_STOCKPROP_ENABLED()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CNTGraphCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CNTGraphCtrl, COleControl)
	//{{AFX_EVENT_MAP(CNTGraphCtrl)
	EVENT_CUSTOM("CursorPosition", FireCursorPosition, VTS_R8  VTS_R8)
	EVENT_CUSTOM("TrackModeChanged", FireTrackModeChanged, VTS_I4)
	EVENT_STOCK_CLICK()
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// Include GUIDs for the new stock property dialogs 
// contained in the dll MSStkProp.DLL 
#include "msstkppg.h" 

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CNTGraphCtrl, 8)
	PROPPAGEID(CNTGraphPropPage::guid)
	PROPPAGEID(CElementPropPage::guid)
	PROPPAGEID(CAnnoPropPage::guid)
	PROPPAGEID(CCursorPropPage::guid)
	PROPPAGEID(CFormatPropPage::guid)
//	PROPPAGEID(CLSID_CFontPropPage)
//	PROPPAGEID(CLSID_CColorPropPage)
//	PROPPAGEID(CLSID_CPicturePropPage)
	PROPPAGEID(CLSID_StockFontPage)
	PROPPAGEID(CLSID_StockColorPage)
	PROPPAGEID(CLSID_StockPicturePage)
END_PROPPAGEIDS(CNTGraphCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CNTGraphCtrl, "NTGRAPH.NTGraphCtrl.1",
	0xc9fe01c2, 0x2746, 0x479b, 0x96, 0xab, 0xe0, 0xbe, 0x99, 0x31, 0xb0, 0x18)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CNTGraphCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DNTGraph =
		{ 0xac90a107, 0x78e8, 0x4ed8, { 0x99, 0x5a, 0x3a, 0xe8, 0xbb, 0x30, 0x44, 0xa7 } };
const IID BASED_CODE IID_DNTGraphEvents =
		{ 0xd59adfe5, 0x1b52, 0x45d5, { 0xae, 0x1d, 0x8f, 0xa1, 0x46, 0x15, 0x21, 0x54 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwNTGraphOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CNTGraphCtrl, IDS_NTGRAPH, _dwNTGraphOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CNTGraphCtrl::CNTGraphCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CNTGraphCtrl

BOOL CNTGraphCtrl::CNTGraphCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_NTGRAPH,
			IDB_NTGRAPH,
			afxRegApartmentThreading,
			_dwNTGraphOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


//////////////////////////////////////////////////////////////////////////////
// static FONTDESC structure containing default attributes of the fonts
static const FONTDESC _fontdescIdent =
  { sizeof(FONTDESC), OLESTR("Arial"), FONTSIZE( 8 ), FW_THIN, 
     ANSI_CHARSET, FALSE, FALSE, FALSE };

static const FONTDESC _fontdescTick =
  { sizeof(FONTDESC), OLESTR("Arial"), FONTSIZE( 8 ), FW_NORMAL, 
     ANSI_CHARSET, FALSE, FALSE, FALSE };

static const FONTDESC _fontdescLabel =
  { sizeof(FONTDESC), OLESTR("Arial"), FONTSIZE( 10 ), FW_NORMAL, 
     ANSI_CHARSET, FALSE, FALSE, FALSE };

static const FONTDESC _fontdescTitle =
  { sizeof(FONTDESC), OLESTR("Times New Roman"), FONTSIZE( 12 ), FW_BOLD, 
     ANSI_CHARSET, FALSE, FALSE, FALSE };


/////////////////////////////////////////////////////////////////////////////
// CNTGraphCtrl::CNTGraphCtrl - Constructor

CNTGraphCtrl::CNTGraphCtrl() : m_fontTick( &m_xFontNotification ), 
                               m_fontTitle( &m_xFontNotification ), 
							   m_fontLabel( &m_xFontNotification ),
							   m_fontIdent( &m_xFontNotification )
{
	InitializeIIDs(&IID_DNTGraph, &IID_DNTGraphEvents);


	COleControl::SetInitialSize(250,150);

	// Initialize the control's instance data.

 	//m_xLabel = "X Axis" ;
	//m_yLabel = "Y Axis" ;
	//SetText("XY Graph");
	m_nGridX = 5 ;
	m_nGridY = 5;
	
	bIsPlotAvailable= FALSE;
	bUnzoom = FALSE;

    SetRange(0.0,1.0,0.0,1.0);

	m_nTrackMode = None;
	m_frameStyle = Bitmap;

	m_panPoint = Corrdinate(0,0);

    AddElement(); // Add Element0
	m_elementIdentify=FALSE;

	m_picFrame.CreateFromBitmap(IDB_FRAME);

	m_formatAxisBottom = "%g";
	m_formatAxisLeft   = "%g";
}

/////////////////////////////////////////////////////////////////////////////
// CNTGraphCtrl::~CNTGraphCtrl - Destructor

CNTGraphCtrl::~CNTGraphCtrl()
{
 
	 // Get the position at the head of the list
	 POSITION aPosition = m_ElementList.GetHeadPosition();

	 // Now delete the element pointed to by each list entry
	 while(aPosition)
		delete m_ElementList.GetNext(aPosition);
 
 // Finally delete all pointers
 m_ElementList.RemoveAll();

}



/////////////////////////////////////////////////////////////////////////////
// CNTGraphCtrl::OnDraw - Drawing function

void CNTGraphCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	//DrawGraph(pdc, rcBounds);
	DrawGraphOffScreen(pdc, rcBounds);
}
void CNTGraphCtrl::OnDrawMetafile(CDC* pDC, const CRect& rcBounds) 
{
	DrawGraph(pDC, rcBounds);	
//	COleControl::OnDrawMetafile(pDC, rcBounds); // Generate debug assertion!?.
}


/////////////////////////////////////////////////////////////////////////////
// CNTGraphCtrl::DoPropExchange - Persistence support

void CNTGraphCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
	PX_Bool(pPX,_T("XLog"),m_xLog,FALSE);
	PX_Bool(pPX,_T("YLog"),m_yLog,FALSE);
	PX_Bool(pPX,_T("ShowGrid"),m_showGrid,FALSE);
	PX_Long(pPX,_T("TrackMode"),m_nTrackMode, (TrackModeState)None );
	PX_Short(pPX,_T("FrameStyle"),m_frameStyle, (FrameStyle)Bitmap );
	
	PX_Color(pPX,_T("PlotAreaColor"),  m_plotAreaColor, RGB(0x00, 0x00, 0x00));
	PX_Color(pPX,_T("ControlFrameColor"),  m_controlFrameColor, RGB(0xFF, 0xFF, 0xFF));
	PX_Color(pPX,_T("AxisColor"),  m_axisColor, RGB(0x00, 0x00, 0x00));
	PX_Color(pPX,_T("LabelColor"),  m_labelColor, RGB(0x00, 0x00, 0x00));
	PX_Color(pPX,_T("GridColor"),  m_gridColor, RGB(0x80,0x80,0x80));

	PX_Short(pPX,_T("Cursor"), m_nCursorID, 0 );
	PX_Short(pPX,_T("CursorStyle"), m_cursorStyle, 0 );
	PX_Short(pPX,_T("CursorMode"), m_cursorMode, 0 );
	PX_Double(pPX,_T("CursorX"),  m_cursorX, 0.0);
	PX_Double(pPX,_T("CursorY"),  m_cursorY, 0.0);
	PX_Color(pPX,_T("CursorColor"),  m_cursorColor, RGB(0xFF,0xFF,0xFF));
    
	PX_String(pPX, _T("FormatAxisLeft"), m_formatAxisLeft,  _T("%g") );
	PX_String(pPX, _T("FormatAxisBottom"), m_formatAxisBottom,  _T("%g") );
	PX_Bool(pPX,_T("XTime"),m_xTime ,FALSE);
	PX_Bool(pPX,_T("YTime"),m_yTime ,FALSE);

	PX_Short(pPX,_T("Element"), m_nElementID, 0);
	PX_Short(pPX,_T("ElementCount"), m_elementCount, 0);
	PX_Bool(pPX,_T("ElementShow"),m_elementShow ,TRUE);
	PX_Bool(pPX,_T("ElementSolidPoint"),m_elementSolidPoint ,TRUE);	
	PX_Color(pPX,_T("ElementLineColor"),  m_elementLineColor, RGB(0xFF, 0xFF, 0xFF));
	PX_Color(pPX,_T("ElementPointColor"),  m_elementPointColor, RGB(0xFF, 0xFF, 0xFF));
	PX_Long(pPX,_T("ElementWidth"), m_elementWidth, 0 );
	PX_Long(pPX,_T("ElementLinetype"), m_elementLinetype , 0 );
	PX_Long(pPX,_T("ElementPointSymbol"), m_elementPointSymbol , 0 );
	PX_String(pPX, _T("ElementName"), m_elementName, _T("") );

    PX_Short(pPX,_T("Annotation"), m_nAnnotation, 0 );
	PX_Short(pPX,_T("AnnoCount"),  m_nAnnoCount, 0 );
	PX_String(pPX, _T("AnnoLabelCaption"), m_annoLabelCaption,  _T("") );
	PX_Color(pPX,_T("AnnoLabelColor"),  m_annoLabelColor, RGB(0xFF,0xFF,0xFF));
	PX_Double(pPX,_T("AnnoLabelX"),  m_annoLabelX, 0);
	PX_Double(pPX,_T("AnnoLabelY"),  m_annoLabelY, 0);
	PX_Bool(pPX,_T("AnnoLabelHorizontal"),  m_annoLabelHorizontal, TRUE);
	PX_Bool(pPX,_T("AnnoVisible"),  m_annoVisible, TRUE);


	PX_Font(pPX, _T("TickFont"), m_fontTick, &_fontdescTick);
	PX_Font(pPX, _T("TitleFont"), m_fontTitle, &_fontdescTitle);
	PX_Font(pPX, _T("LabelFont"), m_fontLabel, &_fontdescLabel);
	PX_Font(pPX, _T("IdentFont"), m_fontIdent, &_fontdescIdent);

	PX_Picture(pPX, "PlotAreaPicture",m_picPlotArea);
	PX_Picture(pPX, "ControlFramePicture",m_picControlFrame);
}


/////////////////////////////////////////////////////////////////////////////
// CNTGraphCtrl::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
// For information on using these flags, please see MFC technical note
// #nnn, "Optimizing an ActiveX Control".
DWORD CNTGraphCtrl::GetControlFlags()
{
	DWORD dwFlags = COleControl::GetControlFlags();


	// The control will not be redrawn when making the transition
	// between the active and inactivate state.
	dwFlags |= noFlickerActivate;
	return dwFlags;
}


/////////////////////////////////////////////////////////////////////////////
// CNTGraphCtrl::OnResetState - Reset control to default state

void CNTGraphCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
	m_picPlotArea.CreateEmpty();
    m_picControlFrame.CreateEmpty();
}


/////////////////////////////////////////////////////////////////////////////
// CNTGraphCtrl::AboutBox - Display an "About" box to the user

void CNTGraphCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_NTGRAPH);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CNTGraphCtrl message handlers
/////////////////////////////////////////////////////////////////////////////
// CNTGraphCtrl message handlers
void CNTGraphCtrl::CreateFont(CDC *pDC)
{
  
  if (m_fontY.m_hObject == NULL) 
  {
 	//Create system font
	LOGFONT lf;
	CFont *pOldFont,*pLogFont;

	pOldFont  = SelectFontObject(pDC, m_fontLabel);
	pLogFont  = pDC->SelectObject(pOldFont);

	// Retrieve a copy of the LOGFONT structure 
	// for a currently selected stock font.
	if (pLogFont)
		pLogFont->GetLogFont(&lf);
		
	// Rotate font 90 degree for Y axis label
	SetGraphicsMode( pDC->m_hDC, GM_ADVANCED );
	lf.lfOrientation = 900 ; 
	lf.lfEscapement = 900 ;
		
	m_fontY.CreateFontIndirect(&lf);

	pOldFont  = SelectFontObject(pDC, m_fontIdent);
	pLogFont  = pDC->SelectObject(pOldFont);

		// Retrieve a copy of the LOGFONT structure 
	// for a currently selected stock font.
	if (pLogFont)
		pLogFont->GetLogFont(&lf);
		
	// Rotate font 90 degree for Y axis label
	lf.lfOrientation = 900 ; 
	lf.lfEscapement = 900 ;

	m_fontVLabel.CreateFontIndirect(&lf);

  }
}

////////////////////////////////////////////////////////
//  Implementation of the Custom Font Proprties
LPFONTDISP CNTGraphCtrl::GetTickFont() { return m_fontTick.GetFontDispatch( ); }

void CNTGraphCtrl::SetTickFont(LPFONTDISP newValue) 
{
	m_fontTick.InitializeFont( &_fontdescTick, newValue);
    OnFontChanged();    //notify any changes
    SetModifiedFlag( );
}

LPFONTDISP CNTGraphCtrl::GetTitleFont() { return m_fontTitle.GetFontDispatch( ); }

void CNTGraphCtrl::SetTitleFont(LPFONTDISP newValue) 
{
	m_fontTitle.InitializeFont( &_fontdescTitle, newValue);
    OnFontChanged();    //notify any changes
    SetModifiedFlag( );
}

LPFONTDISP CNTGraphCtrl::GetLabelFont() { return m_fontLabel.GetFontDispatch( ); }

void CNTGraphCtrl::SetLabelFont(LPFONTDISP newValue) 
{
	m_fontLabel.InitializeFont( &_fontdescLabel, newValue);
    OnFontChanged();    //notify any changes
    SetModifiedFlag( );

}

LPFONTDISP CNTGraphCtrl::GetIdentFont(){return m_fontIdent.GetFontDispatch( );   }

void CNTGraphCtrl::SetIdentFont(LPFONTDISP newValue) 
{
	m_fontIdent.InitializeFont( &_fontdescIdent, newValue);
    OnFontChanged();    //notify any changes
	SetModifiedFlag();
}

void CNTGraphCtrl::OnFontChanged() 
{
	m_fontY.DeleteObject();
	m_fontVLabel.DeleteObject();
	InvalidateControl();
	SetModifiedFlag( );
	COleControl::OnFontChanged();
}

void CNTGraphCtrl::PrepareForDrawing(CDC *pDC, CRect rect)
{

   pDC->SetMapMode(MM_TEXT) ;
   pDC->SetWindowOrg(0,0) ;
   pDC->SetWindowExt(rect.right , rect.bottom) ;
   pDC->SetViewportOrg(0,0) ;
   pDC->SetViewportExt (rect.right , rect.bottom );

   m_ctlRect = rect ;
   pDC->DPtoLP(&m_ctlRect);

   CreateFont(pDC); // Create system font for Y label  
   CalcRect(pDC) ;	// Compute rectangle 
   ResCalc();	// Compute resolution per dot.


}

void CNTGraphCtrl::CalcRect(CDC *pDC)
{
	int offset = 15;

	CSize txtXLabelSize,txtYLabelSize(0,0),txtTitleSize ;
	CString str ;
	CFont *pOldFont, *pSaveFont ;

	pOldFont = SelectFontObject(pDC, m_fontTick);

	// Calculate Y Label sizes.
	double res = (dRangeY[MAX] - dRangeY[MIN]) / m_nGridY ;
	CSize sz(0,0);

	for (int i = 0 ; i <= m_nGridY ; i++)
	{
		double y = dRangeY[MIN] + (res * (double)i) ;
           
		str = FormatYAxisLabel(y);
        
		sz = pDC->GetTextExtent(str);

		if (txtYLabelSize.cx < sz.cx) txtYLabelSize=sz;
	}

	//Calculate X Label sizes.
	str = FormatXAxisLabel(dRangeX[MAX]);

	txtXLabelSize = pDC->GetTextExtent(str);

	pDC->SelectObject(pOldFont);
	// AT 10.07.2003 original code: pSaveFont = SelectFontObject(pDC,m_fontTick);

	// AT 10.07.2003 Begin 
	// the old code selected the wrong font
	// and the wrong String to define the top-border
	pSaveFont = SelectFontObject(pDC,m_fontTitle);

	const CString& strCaption = InternalGetText();

	txtTitleSize = pDC->GetTextExtent(strCaption);

	// AT 10.07.2003 End
	// AT 10.07.2003 original: txtTitleSize = pDC->GetTextExtent(m_xLabel); 

	pDC->SelectObject(pOldFont);

	m_axisRect.left = m_ctlRect.left + txtTitleSize.cy+txtYLabelSize.cx + offset;
	m_axisRect.right = m_ctlRect.right - (txtXLabelSize.cx/2) - offset;
	m_axisRect.top = m_ctlRect.top + txtTitleSize.cy + offset;
	m_axisRect.bottom= m_ctlRect.bottom - (txtTitleSize.cy+txtXLabelSize.cy) - offset; 
}



////////////////////////////////
//Calculate resolution per dot
void CNTGraphCtrl::ResCalc()
{
	double dpixelx, dpixely ;

	dpixelx = (double)m_axisRect.Width() ;
	dpixely = (double)m_axisRect.Height() ;
	dResY = (dRangeY[MAX] - dRangeY[MIN]) / dpixely ;
	dResX = (dRangeX[MAX] - dRangeX[MIN]) / dpixelx ;

}

CPoint CNTGraphCtrl::Corrdinate(double x, double y)
{
	double rx , ry ;
	int xPixel , yPixel ;
	CPoint retPt ;

	rx = x - dRangeX[MIN] ; // Calculate horizontal offset from origin
	ry = y - dRangeY[MIN]; // Calculate vertical offset from origin .

	// Convert offset to be number of pixel on screen .
	xPixel = (int)(rx / dResX) ; 
	yPixel = (int)(ry / dResY) ;

	//Calulate point to be drawn .
	retPt.x= xPixel + m_axisRect.left ;
	retPt.y= m_axisRect.bottom - yPixel; 
	return retPt ;
}

CPoint CNTGraphCtrl::Corrdinate(CElementPoint pt)
{
	double rx , ry ;
	int xPixel , yPixel ;
	CPoint retPt ;

	rx = pt.x - dRangeX[MIN] ; // Calculate horizontal offset from origin
	ry = pt.y - dRangeY[MIN]; // Calculate vertical offset from origin .

	// Convert offset to be number of pixel on screen .
	xPixel = (int)(rx / dResX) ; 
	yPixel = (int)(ry / dResY) ;

	//Calulate point to be drawn .
	retPt.x= xPixel + m_axisRect.left ;
	retPt.y= m_axisRect.bottom - yPixel; 
	return retPt ;
}

double CNTGraphCtrl::RoundUp( double val, int ex ) 
{
    int exponent;
    int i;
	exponent = (int)(floor( log10(fabs(val)) ) );
	
	if (ex==0) {
	    if( exponent < 0 ) 
			for(i=exponent; i<0; i++) { val *= 10.0; }
		else 
			for(i=0; i<exponent; i++) { val /= 10.0; }
	} else	{

		if( ex < 0 ) 
			if (exponent<0)
				for(i=ex; i<0; i++) { val *= 10.0; }
			else 
			    return val;            
		else 
			if (exponent<0)
				return val;
			else
				for(i=0; i<ex; i++) { val /= 10.0; }
			
	}		
	
    return val;
}  
///////////////////////////////////////////////////////////////////////////////////////

void CNTGraphCtrl::DrawGraphOffScreen(CDC* pdc, const CRect& rcBounds)
{
   CDC dcMem;
   //CWindowDC screenDC(NULL);

   CBitmap bitOff;
   CRect rcBoundsDP(rcBounds) ;

   // Convert bounds to device units. 
   pdc->LPtoDP(&rcBoundsDP) ;

   // The bitmap bounds have 0,0 in the upper-left corner.
   CRect rcBitmapBounds(0,0,rcBoundsDP.Width(),rcBoundsDP.Height()) ;

   // Create a DC that is compatible with the screen.
   //dcMem.CreateCompatibleDC(&screenDC) ;
   dcMem.CreateCompatibleDC(pdc) ;

   // Create a really compatible bitmap.
   bitOff.CreateCompatibleBitmap(pdc, rcBitmapBounds.Width(), rcBitmapBounds.Height());

   // Select the bitmap into the memory DC.
   CBitmap* pOldBitmap = dcMem.SelectObject(&bitOff) ;

   // Save the memory DC state, since DrawMe might change it.
   int iSavedDC = dcMem.SaveDC();

   // Draw our control on the memory DC.
   DrawGraph(&dcMem, rcBitmapBounds) ;

   // Restore the DC, since DrawMe might have changed mapping modes.
   dcMem.RestoreDC(iSavedDC) ;

   // We don't know what mapping mode pdc is using.
   // BitBlt uses logical coordinates.
   // Easiest thing is to change to MM_TEXT.
   pdc->SetMapMode(MM_TEXT) ;
   pdc->SetWindowOrg(0,0) ;
   pdc->SetViewportOrg(0,0) ;

    // Blt the memory device context to the screen.
   pdc->BitBlt( rcBoundsDP.left,
                rcBoundsDP.top,
                rcBoundsDP.Width(),
                rcBoundsDP.Height(),
                &dcMem,
                0,
                0,
                SRCCOPY) ;


   // Clean up.
   dcMem.SelectObject(pOldBitmap) ;
}

void CNTGraphCtrl::DrawGraph(CDC* pDC, const CRect& rcBounds)
{
	// Set control and axis rectangles  
	PrepareForDrawing(pDC,rcBounds);
    
	DrawControlFrame(pDC);
  
	DrawTitle(pDC);	

	if (m_showGrid)
		if (m_xLog || m_yLog)
		{
            if(m_xLog)
				DrawXLogGrid(pDC);
		    if (m_yLog)
				DrawYLogGrid(pDC);

			DrawGrid(pDC);
		}
		else
			DrawGrid(pDC);
	else
		if (m_xLog || m_yLog)
		{
            if(m_xLog)
				DrawXLogGrid(pDC);
		    if (m_yLog)
				DrawYLogGrid(pDC);

			DrawTicks(pDC);
		}
		else
			DrawTicks(pDC);

	if(m_frameStyle==Flat)
			DrawAxis(pDC);

	DrawAxisLabel(pDC);

	if (bIsPlotAvailable)
			PlotElement(pDC);

	if (m_CursorList.GetCount() )
			DrawCursor(pDC);

	if(m_AnnotationList.GetCount() )
			Annotate(pDC);
}
//////////////////////////////////////////////////////////
// Draw Control Frame
void CNTGraphCtrl::DrawControlFrame(CDC* pDC)
{
   
	if (m_brushFrame.m_hObject == NULL)
			m_brushFrame.CreateSolidBrush(m_controlFrameColor);
		
	if (m_brushPlot.m_hObject == NULL)
			m_brushPlot.CreateSolidBrush(m_plotAreaColor);

	CBrush* pBrushSave = pDC->SelectObject(&m_brushFrame);
    pDC->Rectangle(m_ctlRect);

	// Draw frame border
	if(m_frameStyle==Scope)
		::DrawEdge( pDC->GetSafeHdc(), m_ctlRect, EDGE_RAISED, BF_RECT);

	// Draw Bitmap Frame
	// TODO To maket it transparent!
	if(m_frameStyle==Bitmap)
		m_picFrame.Render(pDC, m_ctlRect, m_ctlRect);


	// Render the frame picture
	m_picControlFrame.Render(pDC, m_ctlRect, m_ctlRect);

	// Draw plot area background
    pDC->SelectObject(&m_brushPlot);
	pDC->Rectangle(m_axisRect);

	// Draw plot area border
	if(m_frameStyle!= Flat )
		::DrawEdge( pDC->GetSafeHdc(), m_axisRect, EDGE_SUNKEN, BF_RECT);

	// Render the plot area picture
	m_picPlotArea.Render(pDC, m_axisRect,m_axisRect);


	if (! IsOptimizedDraw())
		pDC->SelectObject(pBrushSave);

}
void CNTGraphCtrl::DrawAxis(CDC *pDC)
{

   if (m_axisPen.m_hObject == NULL)
        m_axisPen.CreatePen(PS_SOLID, 2, m_axisColor);
	
	CPen* pPenSave = pDC->SelectObject(&m_axisPen);
    
	pDC->MoveTo(Corrdinate(dRangeX[MIN],dRangeY[MIN]));
	pDC->LineTo(Corrdinate(dRangeX[MAX],dRangeY[MIN]));
	pDC->MoveTo(Corrdinate(dRangeX[MIN],dRangeY[MIN]));
	pDC->LineTo(Corrdinate(dRangeX[MIN],dRangeY[MAX]));
    pDC->MoveTo(Corrdinate(dRangeX[MAX],dRangeY[MIN]));
	pDC->LineTo(Corrdinate(dRangeX[MAX],dRangeY[MAX]));
	pDC->MoveTo(Corrdinate(dRangeX[MIN],dRangeY[MAX]));
	pDC->LineTo(Corrdinate(dRangeX[MAX],dRangeY[MAX]));

    if (! IsOptimizedDraw())
        pDC->SelectObject(pPenSave);

}


//////////////////////////////////////////
// Draw Y log ticks and grid
void CNTGraphCtrl::DrawYLogGrid(CDC *pDC)
{
	// Change this number for changing number of fine scales.
	const int FINE_SCALE = 5 ; 
	CPoint m_start, m_stop ;
	int i  ;
	int j ;
	double y ;
	double step ;
	double Ymax , Ymin ;

	Ymax = dRangeY[MAX] ;
	Ymin = dRangeY[MIN] ;
	
	//Remap scale to 0 - nGridY
	dRangeY[MIN] = 0;
	dRangeY[MAX] = m_nGridY;
	ResCalc();
	
	if (m_gridPen.m_hObject == NULL)
        m_gridPen.CreatePen(PS_SOLID, 0, m_gridColor);
	
    CPen* pPenSave = pDC->SelectObject(&m_gridPen);
	
	
	for( j = (int)dRangeY[MIN] ; j < (int)dRangeY[MAX] ; j ++) {
	   y = (double)j;
	   step = (pow(10,y+1) - pow(10,y)) /(double)FINE_SCALE ;
		for(i = 0 ; i < FINE_SCALE ; i++ )
		{
		
			y = log10(pow(10,y) + step) ;
			m_start = Corrdinate(dRangeX[MIN],y) ;
			m_stop  = Corrdinate(dRangeX[MAX],y) ;

			if(!m_showGrid)
			{   
				m_stop = m_start;
				
				if(m_frameStyle==Flat)
			      m_stop.x+=5;  // Draw log ticks only
                else
                  m_stop.x-=5;   
            }

			pDC->MoveTo(m_start);
			pDC->LineTo(m_stop);

		}
	}
	if(m_frameStyle==Flat) // Draw log ticks on right axis
	{
		for( j = (int)dRangeY[MIN] ; j < (int)dRangeY[MAX] ; j ++) {
		   y = (double)j;
		   step = (pow(10,y+1) - pow(10,y)) /(double)FINE_SCALE ;
			for(i = 0 ; i < FINE_SCALE ; i++ )
			{
			
				y = log10(pow(10,y) + step) ;
				m_start = Corrdinate(dRangeX[MIN],y) ;
				m_stop  = Corrdinate(dRangeX[MAX],y) ;

				if(!m_showGrid)
				{   
					m_start = m_stop;
					m_stop.x-=5;
					pDC->MoveTo(m_start);
					pDC->LineTo(m_stop);

				}

			}
		}
	}

	dRangeY[MIN] = Ymin;
	dRangeY[MAX] = Ymax;
	ResCalc();

	if (! IsOptimizedDraw())
        pDC->SelectObject(pPenSave);

	
}
///////////////////////////////////////////
// Draw X log grid and ticks
void CNTGraphCtrl::DrawXLogGrid(CDC *pDC)
{
	// Change this number for changing number of fine scales.
	const int FINE_SCALE = 5 ; 
	CPoint m_start, m_stop ;
	int i  ;
	int j ;
	double x ;
	double step ;
	double Xmax , Xmin ;

	Xmax = dRangeX[MAX] ;
	Xmin = dRangeX[MIN] ;
	
	//Remap scale to 0 - nGridX
	dRangeX[MIN] = 0;
	dRangeX[MAX] = m_nGridX;
	ResCalc();
	
	if (m_gridPen.m_hObject == NULL)
        m_gridPen.CreatePen(PS_SOLID, 0, m_gridColor);
	
    CPen* pPenSave = pDC->SelectObject(&m_gridPen);
	
	
	for( j = (int)dRangeX[MIN] ; j < (int)dRangeX[MAX] ; j ++) {
	   x = (double)j;
	   step = (pow(10,x+1) - pow(10,x)) /(double)FINE_SCALE ;
		for(i = 0 ; i < FINE_SCALE ; i++ )
		{
		
			x = log10(pow(10,x) + step) ;
			m_start = Corrdinate(x,dRangeY[MIN]) ;
			m_stop  = Corrdinate(x,dRangeY[MAX]) ;
			
			if(!m_showGrid)
			{   
				m_stop = m_start;
				
				if(m_frameStyle==Flat)
			      m_stop.y-=5 ;  // Draw log ticks only
				else
				  m_stop.y+=5 ;  
			}

			pDC->MoveTo(m_start);
			pDC->LineTo(m_stop);

		}

	}
	if(m_frameStyle==Flat) // Draw log ticks on top axis
	{
		for( j = (int)dRangeX[MIN] ; j < (int)dRangeX[MAX] ; j ++) {
		   x = (double)j;
		   step = (pow(10,x+1) - pow(10,x)) /(double)FINE_SCALE ;
			for(i = 0 ; i < FINE_SCALE ; i++ )
			{
			
				x = log10(pow(10,x) + step) ;
				m_start = Corrdinate(x,dRangeY[MIN]) ;
				m_stop  = Corrdinate(x,dRangeY[MAX]) ;
				
				if(!m_showGrid)
				{   
					m_start = m_stop;
				    m_stop.y+=5 ;  
					pDC->MoveTo(m_start);
					pDC->LineTo(m_stop);
				}

			}

		}
	}
	dRangeX[MIN] = Xmin;
	dRangeX[MAX] = Xmax;
	ResCalc();

	if (! IsOptimizedDraw())
        pDC->SelectObject(pPenSave);

	
}

//////////////////////////////////////
// Draw linear scale ticks 
void CNTGraphCtrl::DrawTicks(CDC *pDC)
{

	CPoint m_start, m_stop ;
	int i  ;
	double x ,y ;
	double step ;

    if (m_gridPen.m_hObject == NULL)
        m_gridPen.CreatePen(PS_SOLID, 0, m_gridColor);
	
    CPen* pPenSave = pDC->SelectObject(&m_gridPen);
	
	// Draw vertical Major and Minor Ticks 
    if(!m_xLog)
	{
		step = (dRangeX[MAX] - dRangeX[MIN]) / (double)m_nGridX ;

		for( i = 1 ; i < m_nGridX ; i++ )		// Major Ticks on Botom
		{
			x = dRangeX[MIN] + (step * (double)i) ;
			m_start = Corrdinate(x,dRangeY[MIN]);
			m_stop = m_start; 

			if(m_frameStyle==Flat)
				m_stop.y -= 6;
			else
				m_stop.y += 6;	

			pDC->MoveTo(m_start);
			pDC->LineTo(m_stop);
		}

		for( i = 1 ; i < m_nGridX*3 ; i++ )		// Minor Ticks on Botom
		{
			x = dRangeX[MIN] + (step * (double)i)/3 ;
			m_start = Corrdinate(x,dRangeY[MIN]);
			m_stop = m_start;
			
			if(m_frameStyle==Flat)
				m_stop.y -= 4;
			else
				m_stop.y += 4;	

			pDC->MoveTo(m_start);
			pDC->LineTo(m_stop);
		}

		if(m_frameStyle==Flat)
		{
			for( i = 1 ; i < m_nGridX ; i++ )		// Major Ticks on Top
			{
				x = dRangeX[MIN] + (step * (double)i) ;
				m_start = Corrdinate(x,dRangeY[MAX]);
				m_stop = m_start; 
				m_stop.y += 6;	
				pDC->MoveTo(m_start);
				pDC->LineTo(m_stop);
			}
			
			for( i = 1 ; i < m_nGridX*3 ; i++ )		// Minor Ticks on Top
			{
				x = dRangeX[MIN] + (step * (double)i)/3 ;
				m_start = Corrdinate(x,dRangeY[MAX]);
				m_stop = m_start; 
				m_stop.y += 4;	
				pDC->MoveTo(m_start);
				pDC->LineTo(m_stop);
			}
		}
    }

	// Draw horizontal Ticks
	if(!m_yLog)
	{
		step = (dRangeY[MAX] - dRangeY[MIN]) / (double)m_nGridY ;

		for( i = 1 ; i < m_nGridY ; i++ )		// Major Ticks on Left
		{
			y = dRangeY[MIN] + (step * (double)i) ;
			m_start = Corrdinate(dRangeX[MIN],y) ;
			m_stop  = m_start;
			
			if (m_frameStyle==Flat)
				m_stop.x += 6;
			else
				m_stop.x -= 6;	 

			pDC->MoveTo(m_start);
			pDC->LineTo(m_stop);
		}
		
		for( i = 1 ; i < m_nGridY*3 ; i++ )		// Minor Ticks on Left
		{
			y = dRangeY[MIN] + (step * (double)i)/3 ;
			m_start = Corrdinate(dRangeX[MIN],y) ;
			m_stop  = m_start; 

			if (m_frameStyle==Flat)
				m_stop.x += 4;
			else
				m_stop.x -= 4;	

			pDC->MoveTo(m_start);
			pDC->LineTo(m_stop);
		}

		if(m_frameStyle==Flat) // only for plots
		{
			for( i = 1 ; i < m_nGridY ; i++ )		// Major Ticks on Right
			{
				y = dRangeY[MIN] + (step * (double)i) ;
				m_start = Corrdinate(dRangeX[MAX],y) ;
				m_stop  = m_start; 
				m_stop.x -= 6;	
				pDC->MoveTo(m_start);
				pDC->LineTo(m_stop);
			}
			for( i = 1 ; i < m_nGridY*3 ; i++ )		// Minor Ticks on Right
			{
				y = dRangeY[MIN] + (step * (double)i)/3 ;
				m_start = Corrdinate(dRangeX[MAX],y) ;
				m_stop  = m_start; 
				m_stop.x -= 4;	
				pDC->MoveTo(m_start);
				pDC->LineTo(m_stop);
			}
		}
	}

    if (! IsOptimizedDraw())
        pDC->SelectObject(pPenSave);
}
/////////////////////////////////////
// Draw linear grid 
void CNTGraphCtrl::DrawGrid(CDC *pDC)
{
	CPoint m_start, m_stop ;
	int i  ;
	double x ,y ;
	double step ;

    if (m_gridPen.m_hObject == NULL)
        m_gridPen.CreatePen(PS_SOLID, 0, m_gridColor);
	
    CPen* pPenSave = pDC->SelectObject(&m_gridPen);
	
	// Draw vertical grid 
	if(!m_xLog)
	{
		step = (dRangeX[MAX] - dRangeX[MIN]) / (double)m_nGridX ;
		for( i = 1 ; i < m_nGridX ; i++ )
		{
			x = dRangeX[MIN] + (step * (double)i) ;
			m_start = Corrdinate(x,dRangeY[MIN]);
			m_stop  = Corrdinate(x,dRangeY[MAX]);
			pDC->MoveTo(m_start);
			pDC->LineTo(m_stop);
		}
	}
	
	// Draw horizontal grid.
    if(!m_yLog)
	{
		step = (dRangeY[MAX] - dRangeY[MIN]) / (double)m_nGridY ;
		for( i = 1 ; i < m_nGridY ; i++ )
		{
			y = dRangeY[MIN] + (step * (double)i) ;
			m_start = Corrdinate(dRangeX[MIN],y) ;
			m_stop  = Corrdinate(dRangeX[MAX],y) ;
			pDC->MoveTo(m_start);
			pDC->LineTo(m_stop);
		}
	}
	
    if (! IsOptimizedDraw())
        pDC->SelectObject(pPenSave);
}

void CNTGraphCtrl::DrawTitle(CDC *pDC)
{

	CFont* pOldFont;
	TEXTMETRIC tm;
	const CString& strCaption = InternalGetText();

	pOldFont = SelectFontObject (pDC,m_fontTitle);
	pDC->SetTextColor (m_labelColor);
    pDC->GetTextMetrics(&tm);

	pDC->SetTextAlign(TA_TOP | TA_CENTER);
	pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut((m_ctlRect.left + m_ctlRect.right) / 2,
                 (m_ctlRect.top+m_axisRect.top - tm.tmHeight) / 2,
                  strCaption, strCaption.GetLength());

   pDC->SelectObject(pOldFont);

}

void CNTGraphCtrl::DrawAxisLabel(CDC *pDC)
{
	CFont *pOldFont;

	int i ;
	double res , x, y  ; 
	CPoint point ;
	CSize txtSize ;
	CString str;

	pDC->SetTextColor (m_labelColor);
	pDC->SetTextAlign(TA_RIGHT);
	pDC->SetBkMode(TRANSPARENT);

    ////////////////////////////////////////////////
	pOldFont  = SelectFontObject(pDC, m_fontTick);
	////////////////////////////////////////////////

	// Draw Y Tick Labels
	res = (dRangeY[MAX] - dRangeY[MIN]) / m_nGridY ;
	CSize sz(0,0);

	for ( i = 0 ; i <= m_nGridY ; i++)
	{
		y = dRangeY[MIN] + (res * (double)i) ;
		point = Corrdinate(dRangeX[MIN],y) ;

		str = FormatYAxisLabel(y);

		txtSize = pDC->GetTextExtent(str) ;

		if (sz.cx < txtSize.cx) sz=txtSize;

		point.x -= 5 ;
		point.y -= txtSize.cy/2 ;

		pDC->TextOut(point.x,point.y,str) ;

	}


	// Draw X Tick Labels
	res = (dRangeX[MAX] - dRangeX[MIN]) / m_nGridX ;

	for ( i = 0 ; i <= m_nGridX ; i ++ )
	{
		x = dRangeX[MIN] + ( res * (double)i);
		point = Corrdinate(x,dRangeY[MIN]);
		
		str = FormatXAxisLabel(x);

		txtSize = pDC->GetTextExtent(str) ;

		point.x += txtSize.cx/2;
		point.y += 5 ;

		pDC->TextOut(point.x,point.y,str);
	}
	 
    ////////////////////////////////////////////////
    SelectFontObject(pDC, m_fontLabel);
	////////////////////////////////////////////////

    // Draw X Axis Title
	txtSize = pDC->GetTextExtent(m_xLabel);
    point.x = m_axisRect.CenterPoint().x + (txtSize.cx/2); 
	point.y = (m_ctlRect.bottom + m_axisRect.bottom)/2;
	point.y -= (txtSize.cy - sz.cy)/2;
    
	pDC->TextOut(point.x,point.y,m_xLabel);

    //////////////////////////////////////////////
    pDC->SelectObject(&m_fontY);
	//////////////////////////////////////////////

	// Draw Y Axis Title
	txtSize = pDC->GetTextExtent(m_yLabel);
	point.x = (m_ctlRect.left+m_axisRect.left)/2;
	point.x -= ( txtSize.cy + sz.cx )/2 ;
	point.y = m_axisRect.CenterPoint().y  - (txtSize.cx/2);
	pDC->TextOut(point.x,point.y,m_yLabel);
	
	pDC->SelectObject(pOldFont);

}

/////////////////////////////////////////
// Plot all elements
void CNTGraphCtrl::PlotElement(CDC *pDC)
{    

	// Get the position at the head of the list
    POSITION element = m_ElementList.GetHeadPosition();
	ASSERT(element);

	int index=1; // offset for annotation strings

 	while(element)
	{
		 
		CGraphElement* pElement = m_ElementList.GetNext(element);

		// Prevent plotting of non-existing data
		if (! pElement->bIsPlotAvailable )
		   continue;
		 
		// Added by A.Hofmann
		//////////////////////////////////
		// Check show state of the element
		if (! pElement->m_bShow )
			continue;

		if (pElement->m_nType != Null)
				PlotElementLines( pDC, pElement );
		
		if (pElement->m_nSymbol != Nosym)
                PlotElementPoints( pDC, pElement );

		if (m_elementIdentify)
                DrawElementLabel( pDC, pElement, index );
        index++;
	}
}
////////////////////////////////////
// Draw element's annotation
void CNTGraphCtrl::DrawElementLabel(CDC *pDC, CGraphElement *pElement, int index)
{

	CFont* pOldFont;
	TEXTMETRIC tm;
	CString strCaption = pElement->m_strName;

	pOldFont = SelectFontObject (pDC,m_fontIdent);
	pDC->SetTextColor (pElement->m_LineColor);
    pDC->GetTextMetrics(&tm);

	pDC->SetTextAlign(TA_TOP | TA_CENTER);
	pDC->SetBkMode(TRANSPARENT);

	CRect rect(m_axisRect);

	rect.left += tm.tmAveCharWidth*strCaption.GetLength();
	rect.top  += tm.tmHeight*index;

    pDC->TextOut(rect.left, rect.top, strCaption, strCaption.GetLength());


   pDC->SelectObject(pOldFont);

}
////////////////////////////////////	
// Draw Lines
void CNTGraphCtrl::PlotElementLines(CDC *pDC, CGraphElement* pElement)
{
	// Create the new pen as the color of element line
	if (m_linePen.m_hObject == NULL) 
	m_linePen.CreatePen (pElement->m_nType, 
	                        pElement->m_nWidth, 
							pElement->m_LineColor);
		
    CPen* pPenSave = pDC->SelectObject(&m_linePen);

	// Create the new pen as the color of element symbol 
	if (m_pointPen.m_hObject == NULL)
		m_pointPen.CreatePen (0, 0, pElement->m_PointColor);

	// Create the new brush as the color of element symbol 
	if (m_lineBrush.m_hObject == NULL)
		m_lineBrush.CreateSolidBrush (pElement->m_PointColor);
		
	CBrush* pBrushSave = pDC->SelectObject(&m_lineBrush);

		
	CRect BoundingRect; // Object to store bounding rectangle
	BoundingRect=m_axisRect; // Store the bounding rectangle
	// Decrease the rectangle by the pen width
	BoundingRect.DeflateRect (pElement->m_nWidth,pElement->m_nWidth);
    //BoundingRect.NormalizeRect ();
	
	POSITION pos = pElement->m_PointList.GetHeadPosition ();
	CElementPoint pt = pElement->m_PointList.GetHead ();

	if(m_xLog)
		pt.x = log10(pt.x);
	if(m_yLog)
		pt.y = log10(pt.y);


	// calculate the corrdinate of ploting point.
	CPoint point = Corrdinate(pt);

	// Clip the ploting area if it exceed ranged .
	if (point.x<BoundingRect.left) point.x=BoundingRect.left;
	if (point.y<BoundingRect.top) point.y=BoundingRect.top;
	if (point.y>BoundingRect.bottom) point.y=BoundingRect.bottom;
   				
	pDC->MoveTo (point);		
	CPoint oldpt(point);

	//Start plot all available data.
	while(pos)
	{
		pt = pElement->m_PointList.GetNext (pos);

		if(m_xLog)
			pt.x = log10(pt.x);
		if(m_yLog)
			pt.y = log10(pt.y);

		point = Corrdinate(pt);

		CPoint p;
        CRect rect;

        // Clip the ploting area if it exceed ranged .
		if (point.x>BoundingRect.right) point.x=BoundingRect.right;
		if (point.x<BoundingRect.left) point.x=BoundingRect.left;
		if (point.y<BoundingRect.top) point.y=BoundingRect.top;
		if (point.y>BoundingRect.bottom) point.y=BoundingRect.bottom;

		if(
			(oldpt.x==BoundingRect.right && point.x==BoundingRect.right) ||
		    (oldpt.x==BoundingRect.left && point.x==BoundingRect.left) ||
			(oldpt.y==BoundingRect.top && point.y==BoundingRect.top) ||
		    (oldpt.y==BoundingRect.bottom && point.y==BoundingRect.bottom)
		   ) 
		{
						
			switch (pElement->m_nType){
			case Bars:
				rect=CRect(oldpt,point);
			    pDC->FillSolidRect(rect, pElement->m_LineColor );
				pDC->MoveTo (point);
			    break;
			case Stick:
                pDC->MoveTo (point);
				p=Corrdinate(pt.x,pElement->min.y);
				p.x=point.x;
				if (p.y>BoundingRect.bottom) p.y=BoundingRect.bottom;
				if (p.x>BoundingRect.right) p.y=BoundingRect.right;
				pDC->LineTo(p);
				pDC->MoveTo (point);
				break; 
			default:
				pDC->MoveTo (point);
				oldpt = point;
			    continue;
			}
			
		}

        switch (pElement->m_nType) {
		
		case XYStep: // Draw XY steps
			p.x = point.x;
			p.y = oldpt.y;
			pDC->LineTo(p);
            pDC->LineTo(point) ;
			break;
        
		case YXStep: // Draw YX steps
			p.y = point.y;
			p.x = oldpt.x;
			pDC->LineTo(p);
            pDC->LineTo(point) ;
            break;
			
        case Bars:  // Draw bars
			p=Corrdinate(pt.x,pElement->min.y);
			p.x=point.x;
			if (p.y<BoundingRect.top) p.y=BoundingRect.top;
			if (p.y>BoundingRect.bottom) p.y=BoundingRect.bottom;
            rect=CRect(oldpt,p);
			pDC->FillSolidRect(rect, pElement->m_LineColor );
			pDC->MoveTo (point);
		    break;

		case Stick: // Draw sticked points to the base line
		   	pDC->LineTo(point);
			p=Corrdinate(pt.x,pElement->min.y);
			p.x=point.x;
			if (p.y>BoundingRect.bottom) p.y=BoundingRect.bottom;
			if (p.x>BoundingRect.right) p.y=BoundingRect.right;
			pDC->LineTo(p);
			pDC->MoveTo (point);
			break;

		default:
			pDC->LineTo(point);
            break;
		}
		
		
			
		oldpt = point;			

	} 
	
	if (! IsOptimizedDraw())
		pDC->SelectObject(pPenSave);

	if (! IsOptimizedDraw())
		pDC->SelectObject(pBrushSave);

	m_linePen.DeleteObject();
	m_lineBrush.DeleteObject();
}
/////////////////////////////////////
// Draw Points
void CNTGraphCtrl::PlotElementPoints(CDC *pDC, CGraphElement* pElement)
{
	// Create the new pen as the color of element symbol 
	if (m_pointPen.m_hObject == NULL)
		m_pointPen.CreatePen (0, 0, pElement->m_PointColor);
	
	CPen* pPenSave = pDC->SelectObject(&m_pointPen);

	// Create the new brush as the color of element symbol 
	if (m_pointBrush.m_hObject == NULL)
		m_pointBrush.CreateSolidBrush (pElement->m_PointColor);

    CBrush* pBrushSave = pDC->SelectObject(&m_pointBrush);
		
		
	CRect BoundingRect; // Object to store bounding rectangle
	BoundingRect=m_axisRect; // Store the bounding rectangle
	// Decrease the rectangle by the pen width
	BoundingRect.DeflateRect (pElement->m_nWidth,pElement->m_nWidth);
    //BoundingRect.NormalizeRect ();
	
	POSITION pos = pElement->m_PointList.GetHeadPosition ();
	CElementPoint pt = pElement->m_PointList.GetHead ();

	if(m_xLog)
		pt.x = log10(pt.x);
	if(m_yLog)
		pt.y = log10(pt.y);


	// calculate the corrdinate of ploting point.
	CPoint point = Corrdinate(pt);

	// Clip the ploting area if it exceed ranged .
	if (point.x<BoundingRect.left) point.x=BoundingRect.left;
	if (point.y<BoundingRect.top) point.y=BoundingRect.top;
	if (point.y>BoundingRect.bottom) point.y=BoundingRect.bottom;
   			
	pDC->MoveTo (point);		
	CPoint oldpt(point);

	//Start plot all available data.
	while(pos)
	{
		pt = pElement->m_PointList.GetNext (pos);

		if(m_xLog)
			pt.x = log10(pt.x);
		if(m_yLog)
			pt.y = log10(pt.y);

		point = Corrdinate(pt);

		// Clip the ploting area if it exceed ranged .
		if (point.x>BoundingRect.right) point.x=BoundingRect.right;
		if (point.x<BoundingRect.left) point.x=BoundingRect.left;
		if (point.y<BoundingRect.top) point.y=BoundingRect.top;
		if (point.y>BoundingRect.bottom) point.y=BoundingRect.bottom;

		if(
			(oldpt.x==BoundingRect.right && point.x==BoundingRect.right) ||
		    (oldpt.x==BoundingRect.left && point.x==BoundingRect.left) ||
			(oldpt.y==BoundingRect.top && point.y==BoundingRect.top) ||
		    (oldpt.y==BoundingRect.bottom && point.y==BoundingRect.bottom)
		   ) 
		{
			pDC->MoveTo(point);
			oldpt = point;
			continue;
		}

		int	symsz = pElement->m_nWidth ;
		if (symsz<0) symsz = 0;
		CRect rect(point.x-symsz,point.y-symsz,point.x+symsz, point.y+symsz);

		if (pElement->m_bSolid==FALSE)
			pDC->SelectObject(pBrushSave);

		if (pElement->m_nSymbol == Dots)	// Draw dots.
			pDC->Ellipse(rect);
		if (pElement->m_nSymbol == Rectangles)	// Draw rectangles.
           	pDC->Rectangle(rect);			
		if (pElement->m_nSymbol == Diamonds) // Draw Diamonds
		    DrawDiamond(pDC, point, symsz);
		if (pElement->m_nSymbol == Asterisk) // Draw Asterisks
		    DrawAsterisk(pDC, point, symsz);
		if (pElement->m_nSymbol == DownTriangles) // Draw Down Triangles
			DrawDownTriangle(pDC, point, symsz);
		if (pElement->m_nSymbol == RightTriangles) // Draw Right Triangles
			DrawRightTriangle(pDC, point, symsz);
		if (pElement->m_nSymbol == UpTriangles) // Draw Up Triangles
			DrawUpTriangle(pDC, point, symsz);
		if (pElement->m_nSymbol == LeftTriangles) // Draw Left Triangles
            DrawLeftTriangle(pDC, point, symsz);

		oldpt = point;

	} 
	
	if (! IsOptimizedDraw())
		pDC->SelectObject(pPenSave);
	if (! IsOptimizedDraw())
		pDC->SelectObject(pBrushSave);

	m_pointPen.DeleteObject();
	m_pointBrush.DeleteObject();
}


void CNTGraphCtrl::OnPlotAreaColorChanged() 
{
    m_brushPlot.DeleteObject();
	InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnControlFrameColorChanged() 
{
    m_brushFrame.DeleteObject();
	InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnFrameStyleChanged() 
{
    m_brushFrame.DeleteObject();
	InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnLabelColorChanged() 
{
    InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnAxisColorChanged() 
{
	m_axisPen.DeleteObject();
    InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnGridColorChanged() 
{
	m_gridPen.DeleteObject();
    InvalidateControl();
	SetModifiedFlag();
}


void CNTGraphCtrl::OnXGridNumberChanged() 
{
    InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnYGridNumberChanged() 
{
    InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnXLabelChanged() 
{
    InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnYLabelChanged() 
{
    InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnShowGridChanged() 
{
    InvalidateControl();
	SetModifiedFlag();
}
void CNTGraphCtrl::OnXLogChanged() 
{
    InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnYLogChanged() 
{
    InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::SetRange(double xmin, double xmax, double ymin, double ymax) 
{

	if (xmin==xmax || ymin==ymax)
		return;
	else if (xmin>xmax || ymin>ymax)
		return;
	else 
	{
		dRangeX[MIN] = xmin ;
		dRangeX[MAX] = xmax ;
		dRangeY[MIN] = ymin ;
		dRangeY[MAX] = ymax ;
	//	m_nExpX = (int)(floor( log10(fabs(dRangeX[MAX])) ) );
	//	m_nExpY = (int)(floor( log10(fabs(dRangeY[MAX])) ) );
    }
	InvalidateControl();
}
void CNTGraphCtrl::AutoRange() 
{
	if (!bIsPlotAvailable) {
		dAutoRangeX[MIN]=dRangeX[MIN];
        dAutoRangeY[MIN]=dRangeY[MIN];
		dAutoRangeX[MAX]=dRangeX[MAX];
		dAutoRangeY[MAX]=dRangeY[MAX];
	}
	SetRange(dAutoRangeX[MIN],dAutoRangeX[MAX],dAutoRangeY[MIN],dAutoRangeY[MAX]);
}

void CNTGraphCtrl::OnTextChanged() 
{
	InvalidateControl();
	SetModifiedFlag();
	COleControl::OnTextChanged();
}

long CNTGraphCtrl::GetTrackMode() 
{
	return m_nTrackMode;
}

void CNTGraphCtrl::SetTrackMode(long nNewValue) 
{
	m_panPoint = Corrdinate(0,0);

	bUnzoom = TRUE;
	dOldRangeX[MIN]=dRangeX[MIN];
	dOldRangeY[MIN]=dRangeY[MIN];
	dOldRangeX[MAX]=dRangeX[MAX];
	dOldRangeY[MAX]=dRangeY[MAX];
    
	m_nTrackMode = nNewValue;
	FireTrackModeChanged(nNewValue);
	SetModifiedFlag();
}

int CNTGraphCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;


	if (m_ttip.Create(this, TTS_ALWAYSTIP) && 
		m_ttip.AddTool(this)
		&& m_ttip.AddTool(this, LPCTSTR("Graph"))
		&&	m_ttip.AddTool(this, TTF_TRACK))
	{
		
	    TOOLINFO ti;
        FillToolInfo(&ti);

        m_ttip.SendMessage(TTM_ADDTOOL, 0, (LPARAM)&ti);
        m_ttip.SetDelayTime(TTDT_AUTOPOP, SHRT_MAX);   // stop the tooltip coming up automatically
        m_ttip.SetDelayTime(TTDT_INITIAL, 0);

	}
	else
	{
		TRACE("Error in creating ToolTip");
		
	}
	
	return 0;
}

void CNTGraphCtrl::RelayEvent(UINT message, WPARAM wParam, LPARAM lParam)
{

	TOOLINFO ti;
	FillToolInfo(&ti);
	m_ttip.SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
	
	
	if (NULL != m_ttip.m_hWnd) {
       MSG msg;

       msg.hwnd= m_hWnd;
       msg.message= message;
       msg.wParam= wParam;
       msg.lParam= lParam;
       msg.time= 0;
       msg.pt.x= LOWORD (lParam);
       msg.pt.y= HIWORD (lParam);

       m_ttip.RelayEvent(&msg);

   }
}

void CNTGraphCtrl::FillToolInfo(TOOLINFO *ti)
{
    memset(ti, 0, sizeof(TOOLINFO));
    ti->cbSize = sizeof(TOOLINFO);
    ti->hwnd = GetParent()->GetSafeHwnd();
    ti->uFlags = TTF_IDISHWND | TTF_ABSOLUTE | TTF_TRACK;
    ti->uId = (UINT)GetSafeHwnd();
}

void CNTGraphCtrl::DoPan(CPoint point, long mode)
{
	double Y1 = PT2DBLY(point.y);
	double Y2 = PT2DBLY(m_panPoint.y);
	double yOffset = Y1 - Y2;
	
	double X1 =  PT2DBLX(point.x);
	double X2 =  PT2DBLX(m_panPoint.x);
	double xOffset = X1 - X2;

	switch (mode){
		case PanXY:
			SetRange(dRangeX[MIN] - xOffset, dRangeX[MAX] - xOffset, 
			 dRangeY[MIN] - yOffset, dRangeY[MAX] - yOffset);
			break;
		case PanX:
			SetRange(dRangeX[MIN] - xOffset, dRangeX[MAX] - xOffset, 
			 dRangeY[MIN], dRangeY[MAX]);
			break;
		case PanY:
			SetRange(dRangeX[MIN], dRangeX[MAX], 
			 dRangeY[MIN] - yOffset, dRangeY[MAX] - yOffset);
			break;
	}
	m_panPoint = point;
}

void CNTGraphCtrl::DoZoom(UINT nFlags, CPoint point) 
{
	if (m_nTrackMode==Zoom) 
	{
		if (bUnzoom) 
		{
			dOldRangeX[MIN]=dRangeX[MIN];
			dOldRangeY[MIN]=dRangeY[MIN];
			dOldRangeX[MAX]=dRangeX[MAX];
			dOldRangeY[MAX]=dRangeY[MAX];
			bUnzoom = FALSE;
		}
         
		CRectTracker tracker;
		double xmin, xmax, ymin, ymax;
        xmin=dRangeX[MIN];
        xmin=dRangeX[MAX];
		ymin=dRangeY[MIN];
		ymax=dRangeY[MAX];

		if (tracker.TrackRubberBand(this, point) )
		{
			CPoint pt1, pt2;

			tracker.m_rect.NormalizeRect ();
			pt1= tracker.m_rect.TopLeft ();
			pt2= tracker.m_rect.BottomRight ();

			xmin = PT2DBLX(pt1.x);
			ymax = PT2DBLY(pt1.y);
			xmax = PT2DBLX(pt2.x);
			ymin = PT2DBLY(pt2.y);
					
		}
        
		SetRange(xmin,xmax,ymin,ymax);	
		
		// Don't ask me why!
		//CClientDC dc(this); 
		//SendMessage(WM_LBUTTONUP, (WPARAM) dc.GetSafeHdc(), 0);
		//COleControl::OnLButtonUp(nFlags, point);
    }
}

void CNTGraphCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_axisRect.PtInRect (point)) 
	{
		
		if (nFlags==MK_LBUTTON) 
		{
    		
			CString str;
			switch (m_nTrackMode)
			{
				case Track:										
					CursorPosition(point);					
					str.Format("x= %g y= %g",m_cursorX,m_cursorY);
					m_ttip.UpdateTipText(LPCTSTR(str), this);
					RelayEvent(WM_LBUTTONDOWN, (WPARAM)nFlags,
					MAKELPARAM(LOWORD(point.x), LOWORD(point.y)));
					break;
				case PanXY:
					DoPan(point,PanXY);
					break;
				case PanX:
					DoPan(point,PanX);
					break;
				case PanY:
					DoPan(point,PanY);
					break;
				default:
					CursorPosition(point);	
					break;
			}

				
		}

	} else 	
	  	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	
	COleControl::OnMouseMove(nFlags, point);
}

void CNTGraphCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{

	if (m_axisRect.PtInRect (point)) 
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));	
        m_panPoint = point;


		CursorPosition(point);

		if (m_nTrackMode == Zoom )
			DoZoom(nFlags, point);
		else if (m_nTrackMode == Track && nFlags==MK_LBUTTON )
        {
			CursorPosition(point);
			CString str;
			str.Format("x= %g y= %g",m_cursorX,m_cursorY);
			m_ttip.UpdateTipText(LPCTSTR(str), this);
			RelayEvent(WM_LBUTTONDOWN, (WPARAM)nFlags,
			MAKELPARAM(LOWORD(point.x), LOWORD(point.y)));
		}
		
    }

	COleControl::OnLButtonDown(nFlags, point);
}

void CNTGraphCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_axisRect.PtInRect (point)) {
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	    TOOLINFO ti;
		FillToolInfo(&ti);
		m_ttip.SendMessage(TTM_TRACKACTIVATE, FALSE, (LPARAM)&ti);
	} else
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	COleControl::OnLButtonUp(nFlags, point);
}

void CNTGraphCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
   if(m_nTrackMode==Zoom)
		SetRange(dOldRangeX[MIN],dOldRangeX[MAX],dOldRangeY[MIN],dOldRangeY[MAX]);
			
	COleControl::OnRButtonDown(nFlags, point);
}
void CNTGraphCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	COleControl::OnProperties(0, NULL, NULL);
	
	COleControl::OnLButtonDblClk(nFlags, point);
}

/////////////////////////////////////////////////////////
// Handlers for the control's picture properties
// m_picPlotArea: 
// m_picControlFrame: 
//
LPPICTUREDISP CNTGraphCtrl::GetPlotAreaPicture() 
{
	return m_picPlotArea.GetPictureDispatch();
}

void CNTGraphCtrl::SetPlotAreaPicture(LPPICTUREDISP newValue) 
{
	m_picPlotArea.SetPictureDispatch(newValue);
	InvalidateControl();
	SetModifiedFlag();
}

LPPICTUREDISP CNTGraphCtrl::GetControlFramePicture() 
{
	return m_picControlFrame.GetPictureDispatch();
}

void CNTGraphCtrl::SetControlFramePicture(LPPICTUREDISP newValue) 
{
	m_picControlFrame.SetPictureDispatch(newValue);
	InvalidateControl();
	SetModifiedFlag();
}
////////////////////////////////////////////////////////
// display the property pages of the control at runtime. 
//
void CNTGraphCtrl::ShowProperties() 
{ 
	COleControl::OnProperties(0, NULL, NULL); 
}

////////////////////////////////////////////////////////
// Copy the control bitmap to the clipboard
// Code Project Article: "Clipboard Copy in a Nutshell"
// by Keith Rule
void CNTGraphCtrl::CopyToClipboard() 
{
    CRect rect;
    CClientDC dc(this);
    GetClientRect(&rect);
	CDC memDC;
    CBitmap bitmap;
    
	// Create memDC
    memDC.CreateCompatibleDC(&dc);
    bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
    
	// Fill in memDC
    memDC.FillSolidRect(rect, dc.GetBkColor());
    OnDraw(&memDC, rect, rect);
    
	// Copy contents of memDC to clipboard
    OpenClipboard();
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, bitmap.GetSafeHandle()); // CF_METAFILEPICT
    CloseClipboard();
    
	// Clean up
    memDC.SelectObject(pOldBitmap);
    bitmap.Detach();	

}
////////////////////////////////////////////
// Print the control
void CNTGraphCtrl::PrintGraph()
{
	char pbuf[100] = "NTGraph ActiveX Control";
	HDC hdcPrn ;

	// Instantiate a CPrintDialog.
	CPrintDialog *printDlg =
	new CPrintDialog(FALSE, PD_ALLPAGES | PD_RETURNDC, NULL);

	// Initialize some of the fields in PRINTDLG structure.
	printDlg->m_pd.nMinPage = printDlg->m_pd.nMaxPage = 1;
	printDlg->m_pd.nFromPage = printDlg->m_pd.nToPage = 1;

	// Display Windows print dialog box.
	// AT 10.07.2003 Added if-Statement
	if(!(printDlg->DoModal() == IDOK) )
		return;
	

	// Obtain a handle to the device context.
	hdcPrn = printDlg->GetPrinterDC();
	if (hdcPrn != NULL)
	{
		CDC *pDC = new CDC;
		pDC->Attach (hdcPrn); // attach a printer DC

		pDC->StartDoc("Graph2D"); // begin a new print job

		pDC->StartPage(); // begin a new page
		pDC->TextOut(100, 10, pbuf);// write the string in pbuf

		CPoint pt(100,100); // position 

		// AT 10.07.2003 Begin
		// compare the resolution of the screen and of the printer


		CDC* hdcScreen = GetDC();

		int nScreenWidth = ::GetDeviceCaps(hdcScreen->m_hDC, HORZRES);
		int nScreenHeight = ::GetDeviceCaps(hdcScreen->m_hDC, VERTRES); 

		int nWidth = ::GetDeviceCaps(hdcPrn, HORZRES);
		int nHeight = ::GetDeviceCaps(hdcPrn, VERTRES); 


		// distinguish between the paper-orientation
		LPDEVMODE lpDevMode = printDlg->GetDevMode();

		if( lpDevMode->dmOrientation == DMORIENT_PORTRAIT )
		{
			// Portrait --> 1/3 of the Paper
			nWidth = static_cast<int>(0.9 * nWidth);
			nHeight = static_cast<int>(0.33 * nHeight);
		}
		else // DMORIENT_LANDSCAPE 
		{ 
			nWidth = static_cast<int>(0.9 * nWidth);
			nHeight = static_cast<int>(0.9 * nHeight);
		}

		CSize sz(nWidth,nHeight); 

		// convert the Textsize:

		CY size;

		CArray <CY, CY> arrFontSize;

		// calculate the conversion factor:
		double dHeightRatio = nHeight/nScreenHeight;
		double dWidthRatio = nWidth/nScreenWidth;
		double dconv = max(dHeightRatio, dWidthRatio);

		// convert each font and keep the actual font-size
		IFont* pf =m_fontTick.m_pFont;
		pf->get_Size(&size);
		arrFontSize.Add(size);
		TRACE("Size: %d\n",size.Lo);
		size.Lo = static_cast<int>(floor(dconv * size.Lo));
		pf->put_Size(size);

		pf =m_fontTitle.m_pFont;
		pf->get_Size(&size);
		arrFontSize.Add(size);
		TRACE("Size: %d\n",size.Lo);
		size.Lo = static_cast<int>(floor(dconv * size.Lo));
		pf->put_Size(size);


		pf =m_fontLabel.m_pFont;
		pf->get_Size(&size);
		arrFontSize.Add(size);
		TRACE("Size: %d\n",size.Lo);
		size.Lo = static_cast<int>(floor(dconv * size.Lo));
		pf->put_Size(size);

		pf =m_fontIdent.m_pFont;
		pf->get_Size(&size);
		arrFontSize.Add(size);
		TRACE("Size: %d\n",size.Lo);
		size.Lo = static_cast<int>(floor(dconv * size.Lo));
		pf->put_Size(size);

		// end convert

		// AT 10.07.2003 End

		// AT 10.07.2003 original: CSize sz(500,300); // width, hieght 
		CRect rect(pt,sz);
		pDC->SetViewportExt(rect.right , rect.bottom);
		pDC->SetViewportOrg(50, 50); // position
		DrawGraph(pDC,rect);

		pDC->EndPage(); // end a page

		pDC->EndDoc(); // end a print job


		// AT 10.07.2003 reconvert the Font

		pf =m_fontTick.m_pFont;
		size = arrFontSize.GetAt(0);
		TRACE("Size: %d\n",size.Lo);
		pf->put_Size(size);

		pf =m_fontTitle.m_pFont;
		size = arrFontSize.GetAt(1);
		TRACE("Size: %d\n",size.Lo);
		pf->put_Size(size);


		pf =m_fontLabel.m_pFont;
		size = arrFontSize.GetAt(2);
		TRACE("Size: %d\n",size.Lo);
		pf->put_Size(size);

		pf =m_fontIdent.m_pFont;
		size = arrFontSize.GetAt(3);
		TRACE("Size: %d\n",size.Lo);
		pf->put_Size(size);

		// AT 10.07.2003 End

		pDC->Detach(); // detach the printer DC
		delete pDC;
	}

	delete printDlg;

	InvalidateControl();

}


/////////////////////////////////////////////////////////////////////////////////
void CNTGraphCtrl::AddElement() 
{
	CGraphElement* pElement = new CGraphElement(m_ElementList.GetCount());
	m_ElementList.AddTail(pElement);
	m_elementCount = m_ElementList.GetCount();
	m_nElementID = m_ElementList.GetCount()-1;
	m_Position = m_ElementList.GetTailPosition();

	SetModifiedFlag();
}

void CNTGraphCtrl::DeleteElement(short ElementID) 
{
	// Gets the position of the element by index.
    POSITION aPosition = m_ElementList.FindIndex(ElementID);

	if (aPosition != NULL) {

		// Now delete the element pointed to this list entry
		delete m_ElementList.GetAt(aPosition);
	    // Finally delete the pointer
		m_ElementList.RemoveAt (aPosition);

		if(m_ElementList.IsEmpty())	
		{
            m_ElementList.RemoveAll();
			AddElement();
			bIsPlotAvailable= FALSE ;
		}

		m_elementCount = m_ElementList.GetCount();
		m_nElementID = m_ElementList.GetCount()-1;
		
		InvalidateControl();
	    SetModifiedFlag();

	} else
		AfxMessageBox("1 Element not found!") ;
	
}
//////////////////////////////////////////////
// Delete all elements and allocate one empty 
void CNTGraphCtrl::ClearGraph() 
{
	// Get the position at the head of the list
	POSITION aPosition = m_ElementList.GetHeadPosition();

	// Now delete the element pointed to by each list entry
	while(aPosition)
		delete m_ElementList.GetNext(aPosition);
 
	// Finally delete all pointers
	m_ElementList.RemoveAll();

	// Allocate a new element
	AddElement();
	dAutoRangeX[MIN]=0;
    dAutoRangeX[MAX]=0;
    dAutoRangeY[MIN]=0;
	dAutoRangeY[MAX]=0;
    m_elementCount = m_ElementList.GetCount();
	bIsPlotAvailable= FALSE ;
	
	InvalidateControl();
    SetModifiedFlag();
		
}
///////////////////////////
// Set element XY data
void CNTGraphCtrl::PlotXY(double X, double Y, short ElementID) 
{
	if(bIsPlotAvailable) 
	{
		if(X<dAutoRangeX[MIN])	dAutoRangeX[MIN]=floor(X);
		if(Y<dAutoRangeY[MIN])	dAutoRangeY[MIN]=floor(Y);
		if(X>dAutoRangeX[MAX])	dAutoRangeX[MAX]=ceil(X);
		if(Y>dAutoRangeY[MAX])  dAutoRangeY[MAX]=ceil(Y);
		
	} else {

        dAutoRangeX[MIN]=floor(X);
        dAutoRangeY[MIN]=floor(Y);
		dAutoRangeX[MAX]=ceil(X);
		dAutoRangeY[MAX]=ceil(Y);

		bIsPlotAvailable= TRUE ;
    }
	

	CElementPoint point(X, Y);

	// Gets the position of the element by index.
    POSITION aPosition = m_ElementList.FindIndex(ElementID);

	if (aPosition != NULL) {
		m_ElementList.GetAt(aPosition)->m_PointList.AddTail(point);
		if(m_ElementList.GetAt(aPosition)->min.x > point.x)
			 m_ElementList.GetAt(aPosition)->min.x=point.x ;
        if(m_ElementList.GetAt(aPosition)->min.y > point.y)
			 m_ElementList.GetAt(aPosition)->min.y=point.y ;
		if(m_ElementList.GetAt(aPosition)->max.x < point.x)
			 m_ElementList.GetAt(aPosition)->max.x=point.x ;
		if(m_ElementList.GetAt(aPosition)->max.y < point.y)
			 m_ElementList.GetAt(aPosition)->max.y=point.y ;
    	m_ElementList.GetAt(aPosition)->bIsPlotAvailable = TRUE ;
	} else
		AfxMessageBox("Element not found!") ;

	InvalidateControl();

}
////////////////////////////
// Set element Y data 
void CNTGraphCtrl::PlotY(double Y, short ElementID) 
{
	
	int X=0;
	
	// Gets the position of the element by index.
    POSITION aPosition = m_ElementList.FindIndex(ElementID);
    if (aPosition != NULL) 
   		X=m_ElementList.GetAt(aPosition)->m_PointList.GetCount();
	else
		return;

	if(bIsPlotAvailable) 
	{
		if(X<dAutoRangeX[MIN])	dAutoRangeX[MIN]=X;
		if(Y<dAutoRangeY[MIN])	dAutoRangeY[MIN]=Y;
		if(X>dAutoRangeX[MAX])	dAutoRangeX[MAX]=X;
		if(Y>dAutoRangeY[MAX])  dAutoRangeY[MAX]=Y;
		
	} 
	else 
	{
		dAutoRangeX[MIN]=floor(static_cast<double>(X));
		dAutoRangeY[MIN]=floor(static_cast<double>(X));
		dAutoRangeX[MAX]=ceil(static_cast<double>(X));
		dAutoRangeY[MAX]=ceil(static_cast<double>(X));
		bIsPlotAvailable= TRUE ;
	}
	
	CElementPoint point((X++), Y);

	m_ElementList.GetAt(aPosition)->m_PointList.AddTail(point);
	if(m_ElementList.GetAt(aPosition)->min.x > point.x)
		 m_ElementList.GetAt(aPosition)->min.x=point.x ;
	if(m_ElementList.GetAt(aPosition)->min.y > point.y)
		 m_ElementList.GetAt(aPosition)->min.y=point.y ;
	if(m_ElementList.GetAt(aPosition)->max.x < point.x)
		 m_ElementList.GetAt(aPosition)->max.x=point.x ;
	if(m_ElementList.GetAt(aPosition)->max.y < point.y)
		 m_ElementList.GetAt(aPosition)->max.y=point.y ;
	m_ElementList.GetAt(aPosition)->bIsPlotAvailable = TRUE ;

	InvalidateControl();
	
}
///////////////////////////////////////////////////
// Get X point value at point index and element id
double CNTGraphCtrl::GetElementXValue(long index, short ElementID) 
{
	// Gets the position of the element by index.
    POSITION aPosition = m_ElementList.FindIndex(ElementID);

	if (aPosition != NULL) {
		POSITION pos = m_ElementList.GetAt(aPosition)->m_PointList.FindIndex(index);
		if(pos != NULL)
		    return m_ElementList.GetAt(aPosition)->m_PointList.GetAt (pos).x;
        else
			AfxMessageBox("Point not found!");
	} else
		AfxMessageBox("Element not found!") ;
   return 0.0;
}
//////////////////////////////////////////////////
// Set X point value at point index and element id
void CNTGraphCtrl::SetElementXValue(long index, short ElementID, double newValue) 
{
	// Gets the position of the element by index.
    POSITION aPosition = m_ElementList.FindIndex(ElementID);

	if (aPosition != NULL) {
		POSITION pos = m_ElementList.GetAt(aPosition)->m_PointList.FindIndex(index);
		if(pos != NULL)
		    m_ElementList.GetAt(aPosition)->m_PointList.GetAt(pos).x=newValue;
        else
			AfxMessageBox("Point not found!");
	} else
		AfxMessageBox("Element not found!") ;

	InvalidateControl();
  	SetModifiedFlag();
}
////////////////////////////////////////////////////
// Get Y point value at point index and element id
double CNTGraphCtrl::GetElementYValue(long index, short ElementID) 
{
	// Gets the position of the element by index.
    POSITION aPosition = m_ElementList.FindIndex(ElementID);

	if (aPosition != NULL) {
		POSITION pos = m_ElementList.GetAt(aPosition)->m_PointList.FindIndex(index);
		if(pos != NULL)
		    return m_ElementList.GetAt(aPosition)->m_PointList.GetAt(pos).y;
        else
			AfxMessageBox("Point not found!");
	} else
		AfxMessageBox("Element not found!") ;

   return 0.0;
}
///////////////////////////////////////////////////////
// Get Y point value at point index and element  index
void CNTGraphCtrl::SetElementYValue(long index, short ElementID, double newValue) 
{
	// Gets the position of the element by index.
    POSITION aPosition = m_ElementList.FindIndex(ElementID);

	if (aPosition != NULL) {
		POSITION pos = m_ElementList.GetAt(aPosition)->m_PointList.FindIndex(index);
		if(pos != NULL)
		    m_ElementList.GetAt(aPosition)->m_PointList.GetAt(pos).y=newValue;
        else
			AfxMessageBox("Point not found!");
	} else
		AfxMessageBox("Element not found!") ;

	InvalidateControl();
  	SetModifiedFlag();
}

/////////////////////////////////
// Set element to be customized
void CNTGraphCtrl::OnElementChanged() 
{
	// Gets the position of the element by index.
    m_Position = m_ElementList.FindIndex(m_nElementID);
	
	if (m_Position == NULL && m_elementCount != 0)
	{
		AfxMessageBox("Element not found!") ;		
		return;
	}
	else
	{
		m_elementLineColor = m_ElementList.GetAt(m_Position)->m_LineColor;
		m_elementPointColor = m_ElementList.GetAt(m_Position)->m_PointColor;

		m_elementWidth = m_ElementList.GetAt(m_Position)->m_nWidth;
		m_elementLinetype = m_ElementList.GetAt(m_Position)->m_nType;

		m_elementPointSymbol = m_ElementList.GetAt(m_Position)->m_nSymbol;
		m_elementSolidPoint = m_ElementList.GetAt(m_Position)->m_bSolid;
		
		m_elementShow = m_ElementList.GetAt(m_Position)->m_bShow;
		m_elementName = m_ElementList.GetAt(m_Position)->m_strName;

		SetModifiedFlag();
	}

	SetModifiedFlag();
}

short CNTGraphCtrl::GetElementCount() 
{
	return m_ElementList.GetCount();
}


void CNTGraphCtrl::OnElementLineColorChanged() 
{
	m_ElementList.GetAt(m_Position)->m_LineColor=m_elementLineColor;

	m_linePen.DeleteObject();
   	m_pointPen.DeleteObject();
	m_lineBrush.DeleteObject();
    m_pointBrush.DeleteObject();

    InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnElementPointColorChanged() 
{
	m_ElementList.GetAt(m_Position)->m_PointColor=m_elementPointColor;

	m_linePen.DeleteObject();
   	m_pointPen.DeleteObject();
	m_lineBrush.DeleteObject();
    m_pointBrush.DeleteObject();

    InvalidateControl();
	SetModifiedFlag();	
}

void CNTGraphCtrl::OnElementLinetypeChanged() 
{
	m_ElementList.GetAt(m_Position)->m_nType=m_elementLinetype;
    
	m_linePen.DeleteObject();
   	m_pointPen.DeleteObject();
	m_lineBrush.DeleteObject();
    m_pointBrush.DeleteObject();
	
	InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnElementWidthChanged() 
{

	if(m_elementWidth>5)
		m_elementWidth=5;
	if(m_elementWidth<0)
		m_elementWidth=0;

	m_ElementList.GetAt(m_Position)->m_nWidth=m_elementWidth;

	m_linePen.DeleteObject();
   	m_pointPen.DeleteObject();
	m_lineBrush.DeleteObject();
    m_pointBrush.DeleteObject();
	
	InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnElementPointSymbolChanged() 
{
	m_ElementList.GetAt(m_Position)->m_nSymbol=m_elementPointSymbol;

	m_linePen.DeleteObject();
   	m_pointPen.DeleteObject();
	m_lineBrush.DeleteObject();
    m_pointBrush.DeleteObject();

    InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnElementSolidPointChanged() 
{

	m_ElementList.GetAt(m_Position)->m_bSolid=m_elementSolidPoint;

	m_linePen.DeleteObject();
   	m_pointPen.DeleteObject();
	m_lineBrush.DeleteObject();
    m_pointBrush.DeleteObject();
	
	InvalidateControl();
	SetModifiedFlag();

}

void CNTGraphCtrl::OnElementShowChanged() 
{
	m_ElementList.GetAt(m_Position)->m_bShow=m_elementShow;
        
	m_linePen.DeleteObject();
   	m_pointPen.DeleteObject();
	m_lineBrush.DeleteObject();
    m_pointBrush.DeleteObject();
	
	InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnElementNameChanged() 
{

	m_ElementList.GetAt(m_Position)->m_strName=m_elementName;
        
	m_linePen.DeleteObject();
   	m_pointPen.DeleteObject();
	m_lineBrush.DeleteObject();
    m_pointBrush.DeleteObject();
	
	InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnElementIdentifyChanged() 
{
	InvalidateControl();
	SetModifiedFlag();
}

////////////////////////////////////////////////////////////
// Draw Diamonds
void DrawDiamond(CDC* pDC, CPoint point, int symsz)
{
	POINT points[5];

	points[0].x = point.x;
	points[0].y = point.y - symsz;
	points[1].x = point.x + symsz;
	points[1].y = point.y;
	points[2].x = point.x;
	points[2].y = point.y + symsz;
	points[3].x = point.x - symsz;
	points[3].y = point.y;
	pDC->Polygon (points, 4);
}
// Draw Asterisk
void DrawAsterisk(CDC* pDC, CPoint point, int symsz)
{
	POINT points[4];

	points[0].x = point.x;
	points[0].y = point.y - symsz;
	points[1].x = point.x + symsz/2;
	points[1].y = point.y + symsz/2;
	points[2].x = point.x - symsz/2;
	points[2].y = point.y + symsz;
	pDC->Polygon (points, 3);

	points[0].x = point.x - symsz;
	points[0].y = point.y - symsz/2;
	points[1].x = point.x + symsz/2 ;
	points[1].y = point.y;
    points[2].x = point.x + symsz;
	points[2].y = point.y + symsz;
	pDC->Polygon (points, 3);

	points[0].x = point.x - symsz/2;
	points[0].y = point.y - symsz/2;
	points[1].x = point.x + symsz ;
	points[1].y = point.y - symsz/2;
    points[2].x = point.x - symsz/2;
	points[2].y = point.y + symsz;
	pDC->Polygon (points, 3);
}
void DrawDownTriangle(CDC* pDC, CPoint point, int symsz)
{
	POINT points[4];
	points[0].x = point.x - symsz;
	points[0].y = point.y - symsz;
	points[1].x = point.x + symsz;
	points[1].y = point.y - symsz;
	points[2].x = point.x;
	points[2].y = point.y + symsz;
	pDC->Polygon (points, 3);
}
void DrawRightTriangle(CDC* pDC, CPoint point, int symsz)
{
	POINT points[4];

	points[0].x = point.x - symsz;
	points[0].y = point.y - symsz;
	points[1].x = point.x + symsz;
	points[1].y = point.y;
	points[2].x = point.x - symsz;
	points[2].y = point.y + symsz;
	pDC->Polygon (points, 3);
}
void DrawUpTriangle(CDC* pDC, CPoint point, int symsz)
{
	POINT points[4];

	points[0].x = point.x ;
	points[0].y = point.y - symsz ;
	points[1].x = point.x + symsz;
	points[1].y = point.y + symsz;
	points[2].x = point.x - symsz;
	points[2].y = point.y + symsz;
	pDC->Polygon (points, 3);
}
void DrawLeftTriangle(CDC* pDC, CPoint point, int symsz)
{
	POINT points[4];

	points[0].x = point.x + symsz - 1;
	points[0].y = point.y - symsz;
	points[1].x = point.x - symsz;
	points[1].y = point.y;
	points[2].x = point.x + symsz - 1;
	points[2].y = point.y + symsz;
	pDC->Polygon (points, 3);
}



///////////////////////////////////////////////////////
//
// Added by Robert Harber 
// Montag, 14. Juli 2003
//
/****************************************/
/* Method to Save the graph as a bitmap*/
/****************************************/
void CNTGraphCtrl::SaveAs(LPCTSTR szFilename) 
{
  PBITMAPINFO pbi;
  CRect rect;
  CClientDC dc(this);
  GetClientRect(&rect);
  CDC memDC;
  CBitmap bitmap;
    
  // Create memDC
  memDC.CreateCompatibleDC(&dc);
  bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
  CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
    
  // Fill in memDC
  memDC.FillSolidRect(rect, dc.GetBkColor());
  OnDraw(&memDC, rect, rect);

  //Create the bitmap info struct
  pbi = CreateBitmapInfoStruct((HBITMAP)bitmap.GetSafeHandle());
  
  //Create the bitmap
  CreateBMPFile((char*)szFilename, pbi, (HBITMAP)bitmap.GetSafeHandle(), memDC);  
       
  // Clean up
  memDC.SelectObject(pOldBitmap);
  bitmap.Detach();
 
}

/*******************************************/
/*Function to create the Bitmap info struct*/
/*******************************************/
PBITMAPINFO CNTGraphCtrl::CreateBitmapInfoStruct(HBITMAP hBmp)
{ 
  BITMAP bmp; 
  PBITMAPINFO pbmi; 
  WORD cClrBits; 

  //Retrieve the bitmap colour format, width, and height. 
  if(!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) 
  {
    MessageBox("Unable to retrieve colour format, width and height of bitmap", "", 
							MB_OK|MB_ICONERROR);
    return NULL;
  }

  //Convert the color format to a count of bits. 
  cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
  if(cClrBits == 1) 
    cClrBits = 1; 
  else if(cClrBits <= 4) 
    cClrBits = 4; 
  else if (cClrBits <= 8) 
    cClrBits = 8; 
  else if (cClrBits <= 16) 
    cClrBits = 16; 
  else if (cClrBits <= 24) 
    cClrBits = 24; 
  else cClrBits = 32; 

  //Allocate memory for the BITMAPINFO structure. (This structure 
  //contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
  //data structures.) 

  if(cClrBits != 24) 
    pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + 
											sizeof(RGBQUAD) * (1<< cClrBits)); 
  else //There is no RGBQUAD array for the 24-bit-per-pixel format.
    pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER)); 

  //Initialize the fields in the BITMAPINFO structure.
  pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
  pbmi->bmiHeader.biWidth = bmp.bmWidth; 
  pbmi->bmiHeader.biHeight = bmp.bmHeight; 
  pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
  pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
  
  if(cClrBits < 24) 
    pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

  //If the bitmap is not compressed, set the BI_RGB flag. 
  pbmi->bmiHeader.biCompression = BI_RGB; 

  //Compute the number of bytes in the array of color 
  //indices and store the result in biSizeImage. 
  //For Windows NT, the width must be DWORD aligned unless 
  //the bitmap is RLE compressed. This example shows this. 
  //For Windows 95/98/Me, the width must be WORD aligned unless the 
  //bitmap is RLE compressed.
  pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
                                  * pbmi->bmiHeader.biHeight; 
  
  //Set biClrImportant to 0, indicating that all of the 
  //device colors are important. 
  pbmi->bmiHeader.biClrImportant = 0; 
  
  return pbmi; 
} 

/*******************************/
/*Function to create the Bitmap*/
/*******************************/
void CNTGraphCtrl::CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC) 
{
  HANDLE hf;                //file handle 
  BITMAPFILEHEADER hdr;     //bitmap file-header 
  PBITMAPINFOHEADER pbih;   //bitmap info-header 
  LPBYTE lpBits;            //memory pointer 
  DWORD dwTotal;            //total count of bytes 
  DWORD cb;                 //incremental count of bytes 
  BYTE *hp;                 //byte pointer 
  DWORD dwTmp; 

  pbih = (PBITMAPINFOHEADER) pbi; 
  lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

  if(!lpBits) 
  {
    MessageBox("Unable to allocate memory for saving bitmap", "", MB_OK|MB_ICONERROR);
    return;
  }

  //Retrieve the color table (RGBQUAD array) and the bits (array of palette indices) from the DIB. 
  if(!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS)) 
  {
    MessageBox("Unable to retrieve colour table when saving bitmap", "", MB_OK|MB_ICONERROR);
    return;
  } 
    
  //Create the .BMP file. 
  hf = CreateFile(pszFile, 
                  GENERIC_READ | GENERIC_WRITE, 
                  (DWORD) 0, 
                  NULL, 
                  CREATE_ALWAYS, 
                  FILE_ATTRIBUTE_NORMAL, 
                  (HANDLE) NULL);
  
  if(hf == INVALID_HANDLE_VALUE) 
  {
    MessageBox("Unable to open the file when saving bitmap", "", MB_OK|MB_ICONERROR);
    return;
  }
  
  hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
  //Compute the size of the entire file. 
  hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
                pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + 
				pbih->biSizeImage); 

  hdr.bfReserved1 = 0; 
  hdr.bfReserved2 = 0; 

  //Compute the offset to the array of color indices. 
  hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
                  pbih->biSize + pbih->biClrUsed * sizeof (RGBQUAD); 

  //Copy the BITMAPFILEHEADER into the .BMP file. 
  if(!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp,  NULL)) 
  {
    MessageBox("Unable to write file header when saving bitmap", "", MB_OK|MB_ICONERROR);
    return;
  }
    
  //Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
  if(!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) 
                + pbih->biClrUsed * sizeof (RGBQUAD), 
                (LPDWORD) &dwTmp, ( NULL)) )
  {
    MessageBox("Unable to write bitmap info", "", MB_OK|MB_ICONERROR);
    return;
  }
  
  //Copy the array of color indices into the .BMP file. 
  dwTotal = cb = pbih->biSizeImage; 
  hp = lpBits; 
  if(!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL)) 
  {
    MessageBox("Unable to write colour indices when saving bitmap", "", MB_OK|MB_ICONERROR);
    return;
  } 

  // Close the .BMP file. 
  if (!CloseHandle(hf)) 
  {
    MessageBox("Unable to close file handle when saving bitmap", "", MB_OK|MB_ICONERROR);
    return;
  } 

  // Free memory. 
  GlobalFree((HGLOBAL)lpBits);
}


////////////////////////////////////////////////////////
// Draw Annotations
void CNTGraphCtrl::Annotate(CDC *pDC)
{
	
	POSITION pos = m_AnnotationList.GetHeadPosition ();
	CGraphAnnotation anno = m_AnnotationList.GetHead ();

	//Start drawing all available labels.
	while(pos)
	{
		anno = m_AnnotationList.GetNext(pos);

		if(!anno.m_bVisible)
			continue;

		pDC->SetTextColor (anno.m_Color);
        pDC->SetTextAlign(TA_LEFT|TA_BOTTOM);
		pDC->SetBkMode(TRANSPARENT);
		
		CFont* pOldFont;

		if(anno.m_bHorizontal)
			pOldFont = SelectFontObject (pDC,m_fontIdent);
		else
			pOldFont = pDC->SelectObject(&m_fontVLabel);

		CRect textRect;
		pDC->DrawText(anno.m_Caption, -1, textRect, DT_SINGLELINE | DT_CALCRECT);

		TRACE("Width %d Height %d \n",textRect.Width(), textRect.Height());

		// calculate the corrdinate of ploting point.
		CPoint point = Corrdinate(anno.place);

		CSize sz;
		if(anno.m_bHorizontal)
			sz = CSize( textRect.Width(), -textRect.Height());
		else 
		{
            sz = CSize(-textRect.Height(), -textRect.Width());
			point.x+=textRect.Height()/2;
		}

		CRect rc(point,sz);

		// Create the new brush as the color of element symbol 
		if (m_annoBrush.m_hObject == NULL)
			m_annoBrush.CreateSolidBrush (anno.m_BkColor);
		
		CBrush* pBrushSave = pDC->SelectObject(&m_annoBrush);

		if (
			m_axisRect.PtInRect(rc.BottomRight()) && 
			m_axisRect.PtInRect(rc.TopLeft()) 
		   )   
		{
			pDC->FillRect(rc, &m_annoBrush);
			pDC->TextOut(point.x,point.y,anno.m_Caption);			
        }
		pDC->SelectObject(pOldFont);
		pDC->SelectObject(pBrushSave);
		m_annoBrush.DeleteObject();
	}

	
}

void CNTGraphCtrl::AddAnnotation() 
{
	CGraphAnnotation anno;
	anno.m_Caption.Format("Annotation-%d", m_AnnotationList.GetCount());
	
	m_AnnotationList.AddTail(anno);
	m_nAnnotation = m_AnnotationList.GetCount()-1;
	TRACE("m_nAnnotation = %d\n",m_nAnnotation);
	m_nAnnoCount = m_AnnotationList.GetCount();

	InvalidateControl();
}

void CNTGraphCtrl::DeleteAnnotation(short annoID) 
{
    POSITION pos = m_AnnotationList.FindIndex(annoID);

	if(pos)
	{
		m_AnnotationList.RemoveAt(pos);
		m_nAnnotation--;
		m_nAnnoCount = m_AnnotationList.GetCount();

		InvalidateControl();
	}
}

void CNTGraphCtrl::OnAnnoLabelCaptionChanged() 
{
    POSITION pos = m_AnnotationList.FindIndex(m_nAnnotation);

	if(pos)
	{
		m_AnnotationList.GetAt(pos).m_Caption = m_annoLabelCaption;

		TRACE("Caption : %s\n", m_annoLabelCaption);

		InvalidateControl();
		SetModifiedFlag();
	}
}

void CNTGraphCtrl::OnAnnoLabelXChanged() 
{
    POSITION pos = m_AnnotationList.FindIndex(m_nAnnotation);
    
	if(pos)
	{
		m_AnnotationList.GetAt(pos).place.x = m_annoLabelX;

		TRACE("LabelX = %g\n", m_annoLabelX);

		InvalidateControl();
		SetModifiedFlag();
	}
}

void CNTGraphCtrl::OnAnnoLabelYChanged() 
{
    POSITION pos = m_AnnotationList.FindIndex(m_nAnnotation);

	if(pos)
	{
		m_AnnotationList.GetAt(pos).place.y = m_annoLabelY;

		TRACE("LabelY = %g\n", m_annoLabelY);

		InvalidateControl();
		SetModifiedFlag();
	}
}

void CNTGraphCtrl::OnAnnoLabelColorChanged() 
{
    POSITION pos = m_AnnotationList.FindIndex(m_nAnnotation);

	if(pos)
	{
		m_AnnotationList.GetAt(pos).m_Color = m_annoLabelColor;

		InvalidateControl();
		SetModifiedFlag();
	}
}
void CNTGraphCtrl::OnAnnoLabelBkColorChanged() 
{
    POSITION pos = m_AnnotationList.FindIndex(m_nAnnotation);

	if(pos)
	{
		m_AnnotationList.GetAt(pos).m_BkColor = m_annoLabelBkColor;

		InvalidateControl();

		SetModifiedFlag();
	}
}

void CNTGraphCtrl::OnAnnoLabelHorizontalChanged() 
{
    POSITION pos = m_AnnotationList.FindIndex(m_nAnnotation);

	if(pos)
	{
		m_AnnotationList.GetAt(pos).m_bHorizontal = m_annoLabelHorizontal;

		InvalidateControl();
		SetModifiedFlag();
	}
}

void CNTGraphCtrl::OnAnnotationChanged() 
{
	if(m_nAnnotation > (m_AnnotationList.GetCount()-1) || m_nAnnotation < 0)
		return;

	POSITION pos = m_AnnotationList.FindIndex(m_nAnnotation);
    
	if(pos)
	{
		m_annoLabelCaption = m_AnnotationList.GetAt(pos).m_Caption;
		m_annoLabelX = m_AnnotationList.GetAt(pos).place.x;
		m_annoLabelY = m_AnnotationList.GetAt(pos).place.y;
		m_annoLabelColor = m_AnnotationList.GetAt(pos).m_Color;
		m_annoLabelBkColor = m_AnnotationList.GetAt(pos).m_BkColor;
		m_annoLabelHorizontal = m_AnnotationList.GetAt(pos).m_bHorizontal;
		m_annoVisible = m_AnnotationList.GetAt(pos).m_bVisible;

		SetModifiedFlag();
	}
}

void CNTGraphCtrl::OnAnnoVisibleChanged() 
{
    POSITION pos = m_AnnotationList.FindIndex(m_nAnnotation);

	if(pos)
	{
		m_AnnotationList.GetAt(pos).m_bVisible = m_annoVisible;

		InvalidateControl();
		SetModifiedFlag();
	}
}


short CNTGraphCtrl::GetAnnoCount() 
{
	return m_nAnnoCount;
}

/////////////////////////////////////////////////////
// Draw Cursors
void CNTGraphCtrl::DrawCursor(CDC *pDC)
{
	
	POSITION pos = m_CursorList.GetHeadPosition ();
	
	CGraphCursor cursor = m_CursorList.GetHead ();
    int index=0;

	//Start drawing all available labels.
	while(pos)
	{
		//m_nCursorID = index;
		cursor = m_CursorList.GetNext(pos);


		if(!cursor.m_bVisible)
			continue;

		if (m_cursorPen.m_hObject == NULL)
			m_cursorPen.CreatePen(PS_SOLID, 0, cursor.m_Color);

		CPen* pPenSave = pDC->SelectObject(&m_cursorPen);

		if(cursor.m_nStyle == XY || cursor.m_nStyle == X)
		{
			pDC->MoveTo(Corrdinate(dRangeX[MIN],cursor.position.y));
			pDC->LineTo(Corrdinate(dRangeX[MAX],cursor.position.y));
		}

		if(cursor.m_nStyle == XY || cursor.m_nStyle == Y)
		{
			pDC->MoveTo(Corrdinate(cursor.position.x,dRangeY[MIN]));
			pDC->LineTo(Corrdinate(cursor.position.x,dRangeY[MAX]));
		}

		if (! IsOptimizedDraw())
			pDC->SelectObject(pPenSave);
		
		m_cursorPen.DeleteObject();
		index++;

	}
}

void CNTGraphCtrl::CursorPosition(CPoint point) 
{
     double rx,ry;

	 rx = PT2DBLX(point.x);
	 ry = PT2DBLY(point.y);

	 m_cursorX = rx;
	 m_cursorY = ry;

	 CElementPoint pt(rx,ry);


	 POSITION pos = m_CursorList.FindIndex(m_nCursorID);
      
	 if (m_axisRect.PtInRect(point) && pos) 
	 {	
		 if(m_CursorList.GetAt(pos).m_nMode > 0)
		 {
			if (m_CursorList.GetAt(pos).m_nMode == Snap && m_elementCount > 0)
			{		
				pt = FindPoint(rx,ry);
				rx = pt.x;
				ry = pt.y;
			}

			m_CursorList.GetAt(pos).position.x = rx;
			m_CursorList.GetAt(pos).position.y = ry;
			InvalidateControl(m_axisRect);
			
		 }
	 }

	//Fire Cursor Position Event
	FireCursorPosition(rx,ry);
	
}


CElementPoint CNTGraphCtrl::FindPoint(double cursor_x, double cursor_y)
{
	CGraphElement* pElement = m_ElementList.GetAt(m_Position);

	int i=0, index = 0;

	POSITION pos = pElement->m_PointList.GetHeadPosition();

    CElementPoint point = pElement->m_PointList.GetHead();

	double dx = fabs(cursor_x - point.x);
	double dy = fabs(cursor_y - point.y);
	double dr = sqrt(dx + dy);

	while(pos)
	{
		point = pElement->m_PointList.GetNext(pos);

		dx = fabs(cursor_x - point.x);
		dy = fabs(cursor_y - point.y);
	
		if (sqrt(dx+dy) < dr )
		{
			dr = sqrt(dx+dy);
            index = i;
		}
		i++;
	}

	pos = pElement->m_PointList.FindIndex(index);

	return pElement->m_PointList.GetAt(pos);
}


void CNTGraphCtrl::OnCursorColorChanged() 
{
    POSITION pos = m_CursorList.FindIndex(m_nCursorID);

	if(pos)
	{
		m_CursorList.GetAt(pos).m_Color = m_cursorColor;

		InvalidateControl();
		SetModifiedFlag();
	}
}

void CNTGraphCtrl::AddCursor() 
{
	CGraphCursor cursor;
	
	m_CursorList.AddTail(cursor);
	m_nCursorID = m_CursorList.GetCount()-1;
	m_nCursorCount = m_CursorList.GetCount();
	
	InvalidateControl();

}

void CNTGraphCtrl::DeleteCursor(short cursorID) 
{
    POSITION pos = m_CursorList.FindIndex(cursorID);
    
	if(pos)
	{
		m_CursorList.RemoveAt(pos);
		m_nCursorID--;
		m_nCursorCount = m_CursorList.GetCount();

		InvalidateControl();
	}

}

void CNTGraphCtrl::OnCursorChanged() 
{
	if(m_nCursorID > (m_CursorList.GetCount()-1) || m_nCursorID < 0)
		return;

	POSITION pos = m_CursorList.FindIndex(m_nCursorID);
    
	if(pos)
	{
		m_cursorX = m_CursorList.GetAt(pos).position.x;
		m_cursorY = m_CursorList.GetAt(pos).position.y;
		m_cursorColor = m_CursorList.GetAt(pos).m_Color;
		m_cursorVisible = m_CursorList.GetAt(pos).m_bVisible;
		m_cursorMode = m_CursorList.GetAt(pos).m_nMode;
		m_cursorStyle = m_CursorList.GetAt(pos).m_nStyle;

		SetModifiedFlag();
	}
}

void CNTGraphCtrl::OnCursorXChanged() 
{
    POSITION pos = m_CursorList.FindIndex(m_nCursorID);
    if(pos)
	{
		m_CursorList.GetAt(pos).position.x = m_cursorX;

		InvalidateControl(m_axisRect);
		SetModifiedFlag();
	}
}

void CNTGraphCtrl::OnCursorYChanged() 
{
    POSITION pos = m_CursorList.FindIndex(m_nCursorID);
    
	if(pos)
	{
		m_CursorList.GetAt(pos).position.y = m_cursorY;

		InvalidateControl(m_axisRect);
		SetModifiedFlag();
	}
}

void CNTGraphCtrl::OnCursorStyleChanged() 
{
    POSITION pos = m_CursorList.FindIndex(m_nCursorID);

	if(pos)
	{
		m_CursorList.GetAt(pos).m_nStyle = (Crosshair)m_cursorStyle;

		InvalidateControl(m_axisRect);
		SetModifiedFlag();
	}
}

void CNTGraphCtrl::OnCursorVisibleChanged() 
{
    POSITION pos = m_CursorList.FindIndex(m_nCursorID);

	if(pos)
	{
		m_CursorList.GetAt(pos).m_bVisible = m_cursorVisible;

		InvalidateControl(m_axisRect);
		SetModifiedFlag();
	}
}

short CNTGraphCtrl::GetCursorCount() 
{
	return m_nCursorCount;
}

void CNTGraphCtrl::OnCursorModeChanged() 
{
    POSITION pos = m_CursorList.FindIndex(m_nCursorID);
    
	if(pos)
	{
		m_CursorList.GetAt(pos).m_nMode = (CursorMode)m_cursorMode;

		if (m_CursorList.GetAt(pos).m_nMode == Snap && m_elementCount > 0)
		{		
			CElementPoint pt;
			pt = FindPoint(m_cursorX,m_cursorY);
			m_cursorX = pt.x;
			OnCursorXChanged();
			m_cursorY = pt.y;
			OnCursorYChanged();
		}

		InvalidateControl(m_axisRect);
		SetModifiedFlag();
	}
}

void CNTGraphCtrl::OnFormatAxisBottomChanged() 
{
	InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnFormatAxisLeftChanged() 
{
	InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnYTimeChanged() 
{
	InvalidateControl();
	SetModifiedFlag();
}

void CNTGraphCtrl::OnXTimeChanged() 
{
	InvalidateControl();
	SetModifiedFlag();
}


CString CNTGraphCtrl::FormatXAxisLabel(double data)
{
  CString result;
  char format[200];
  char buffer[200];  

  if(m_xLog)
		result.Format("1E%d",ROUND(data)); // Log Scale
  else if (m_xTime)
  {
	    COleDateTime t((DATE)data);
		result = t.Format(m_formatAxisBottom);
  }
  else
  {
		sprintf(format,"%s",(LPCTSTR)m_formatAxisBottom);
		sprintf(buffer,format,data);
		result = buffer;
  }

  return result;
}

CString CNTGraphCtrl::FormatYAxisLabel(double data)
{
  CString result;
  char format[200];
  char buffer[200];  

  if(m_yLog)
		result.Format("1E%d",ROUND(data)); // Log Scale
  else if (m_yTime)
  {
	    COleDateTime t((DATE)data);
		result = t.Format(m_formatAxisLeft);
  }
  else
  {
		sprintf(format,"%s",(LPCTSTR)m_formatAxisLeft);
		sprintf(buffer,format,data);
		result = buffer;
  }

  return result;
}