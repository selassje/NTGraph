/*
MIT License

Copyright(c) 2003- 2020 Przemyslaw Koziol,  Nikolai Teofilov  nteofilov@yahoo.de

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "NTGraphCtl.hpp"
#include "AnnoPpg.hpp"
#include "CursorPpg.hpp"
#include "ElementPpg.hpp"
#include "FormatPpg.hpp"
#include "NTGraphPpg.hpp"
#include "msstkppg.h"

void DrawDiamond(CDC* pDC, CPoint point, int symsz);
void DrawAsterisk(CDC* pDC, CPoint point, int symsz);
void DrawDownTriangle(CDC* pDC, CPoint point, int symsz);
void DrawRightTriangle(CDC* pDC, CPoint point, int symsz);
void DrawUpTriangle(CDC* pDC, CPoint point, int symsz);
void DrawLeftTriangle(CDC* pDC, CPoint point, int symsz);

#define MAX 1
#define MIN 0
#define PT2DBLX(x) (double)(((x)- static_cast<double>(m_axisRect.left)) * dResX) + dRangeX[MIN]
#define PT2DBLY(y) (double)((  static_cast<double>(m_axisRect.bottom) - (y)) * dResY) + dRangeY[MIN]
#define ROUND(x) ((int)((x) + (((x) < 0.0) ? -0.5 : 0.5)))

#define IDH_WHATISTHISTHING 18001

IMPLEMENT_DYNCREATE(CNTGraphCtrl, COleControl)

BEGIN_MESSAGE_MAP(CNTGraphCtrl, COleControl)

ON_WM_LBUTTONDBLCLK()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_RBUTTONDOWN()
ON_WM_CREATE()

ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CNTGraphCtrl, COleControl)

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

DISP_FUNCTION_ID(CNTGraphCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

BEGIN_EVENT_MAP(CNTGraphCtrl, COleControl)

EVENT_CUSTOM("CursorPosition", FireCursorPosition, VTS_R8 VTS_R8)
EVENT_CUSTOM("TrackModeChanged", FireTrackModeChanged, VTS_I4)
EVENT_STOCK_CLICK()

END_EVENT_MAP()

BEGIN_PROPPAGEIDS(CNTGraphCtrl, 8)
PROPPAGEID(CNTGraphPropPage::guid)
PROPPAGEID(CElementPropPage::guid)
PROPPAGEID(CAnnoPropPage::guid)
PROPPAGEID(CCursorPropPage::guid)
PROPPAGEID(CFormatPropPage::guid)
PROPPAGEID(CLSID_StockFontPage)
PROPPAGEID(CLSID_StockColorPage)
PROPPAGEID(CLSID_StockPicturePage)
END_PROPPAGEIDS(CNTGraphCtrl)

IMPLEMENT_OLECREATE_EX(CNTGraphCtrl, "NTGRAPH.NTGraphCtrl.1",
    0xc9fe01c2, 0x2746, 0x479b, 0x96, 0xab, 0xe0, 0xbe, 0x99, 0x31, 0xb0, 0x18)

IMPLEMENT_OLETYPELIB(CNTGraphCtrl, _tlid, _wVerMajor, _wVerMinor)

const IID BASED_CODE IID_DNTGraph = { 0xac90a107, 0x78e8, 0x4ed8, { 0x99, 0x5a, 0x3a, 0xe8, 0xbb, 0x30, 0x44, 0xa7 } };
const IID BASED_CODE IID_DNTGraphEvents = { 0xd59adfe5, 0x1b52, 0x45d5, { 0xae, 0x1d, 0x8f, 0xa1, 0x46, 0x15, 0x21, 0x54 } };

static const DWORD BASED_CODE _dwNTGraphOleMisc = OLEMISC_ACTIVATEWHENVISIBLE | OLEMISC_SETCLIENTSITEFIRST | OLEMISC_INSIDEOUT | OLEMISC_CANTLINKINSIDE | OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CNTGraphCtrl, IDS_NTGRAPH, _dwNTGraphOleMisc)

BOOL CNTGraphCtrl::CNTGraphCtrlFactory::UpdateRegistry(BOOL bRegister)
{
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

static const FONTDESC _fontdescIdent = { sizeof(FONTDESC), OLESTR("Arial"), FONTSIZE(8), FW_THIN,
    ANSI_CHARSET, FALSE, FALSE, FALSE };

static const FONTDESC _fontdescTick = { sizeof(FONTDESC), OLESTR("Arial"), FONTSIZE(8), FW_NORMAL,
    ANSI_CHARSET, FALSE, FALSE, FALSE };

static const FONTDESC _fontdescLabel = { sizeof(FONTDESC), OLESTR("Arial"), FONTSIZE(10), FW_NORMAL,
    ANSI_CHARSET, FALSE, FALSE, FALSE };

static const FONTDESC _fontdescTitle = { sizeof(FONTDESC), OLESTR("Times New Roman"), FONTSIZE(12), FW_BOLD,
    ANSI_CHARSET, FALSE, FALSE, FALSE };

CNTGraphCtrl::CNTGraphCtrl()
    : m_fontTick(&m_xFontNotification)
    , m_fontTitle(&m_xFontNotification)
    , m_fontLabel(&m_xFontNotification)
    , m_fontIdent(&m_xFontNotification)
{
    InitializeIIDs(&IID_DNTGraph, &IID_DNTGraphEvents);

    COleControl::SetInitialSize(250, 150);

    m_nGridX = 5;
    m_nGridY = 5;

    bIsPlotAvailable = FALSE;
    bUnzoom = FALSE;

    SetRange(0.0, 1.0, 0.0, 1.0);

    m_nTrackMode = None;
    m_frameStyle = Bitmap;

    m_panPoint = Corrdinate(0, 0);

    AddElement();
    m_elementIdentify = FALSE;

    m_picFrame.CreateFromBitmap(IDB_FRAME);

    m_formatAxisBottom = "%g";
    m_formatAxisLeft = "%g";
}

CNTGraphCtrl::~CNTGraphCtrl()
{
    POSITION aPosition = m_ElementList.GetHeadPosition();

    while (aPosition)
        delete m_ElementList.GetNext(aPosition);
    m_ElementList.RemoveAll();
}

void CNTGraphCtrl::OnDraw(
    CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
    DrawGraphOffScreen(pdc, rcBounds);
}
void CNTGraphCtrl::OnDrawMetafile(CDC* pDC, const CRect& rcBounds)
{
    DrawGraph(pDC, rcBounds);
}

void CNTGraphCtrl::DoPropExchange(CPropExchange* pPX)
{
    ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
    COleControl::DoPropExchange(pPX);

    PX_Bool(pPX, _T("XLog"), m_xLog, FALSE);
    PX_Bool(pPX, _T("YLog"), m_yLog, FALSE);
    PX_Bool(pPX, _T("ShowGrid"), m_showGrid, FALSE);
    PX_Long(pPX, _T("TrackMode"), m_nTrackMode, (TrackModeState)None);
    PX_Short(pPX, _T("FrameStyle"), m_frameStyle, (FrameStyle)Bitmap);

    PX_Color(pPX, _T("PlotAreaColor"), m_plotAreaColor, RGB(0x00, 0x00, 0x00));
    PX_Color(pPX, _T("ControlFrameColor"), m_controlFrameColor, RGB(0xFF, 0xFF, 0xFF));
    PX_Color(pPX, _T("AxisColor"), m_axisColor, RGB(0x00, 0x00, 0x00));
    PX_Color(pPX, _T("LabelColor"), m_labelColor, RGB(0x00, 0x00, 0x00));
    PX_Color(pPX, _T("GridColor"), m_gridColor, RGB(0x80, 0x80, 0x80));

    PX_Short(pPX, _T("Cursor"), m_nCursorID, 0);
    PX_Short(pPX, _T("CursorStyle"), m_cursorStyle, 0);
    PX_Short(pPX, _T("CursorMode"), m_cursorMode, 0);
    PX_Double(pPX, _T("CursorX"), m_cursorX, 0.0);
    PX_Double(pPX, _T("CursorY"), m_cursorY, 0.0);
    PX_Color(pPX, _T("CursorColor"), m_cursorColor, RGB(0xFF, 0xFF, 0xFF));

    PX_String(pPX, _T("FormatAxisLeft"), m_formatAxisLeft, _T("%g"));
    PX_String(pPX, _T("FormatAxisBottom"), m_formatAxisBottom, _T("%g"));
    PX_Bool(pPX, _T("XTime"), m_xTime, FALSE);
    PX_Bool(pPX, _T("YTime"), m_yTime, FALSE);

    PX_Short(pPX, _T("Element"), m_nElementID, 0);
    PX_Short(pPX, _T("ElementCount"), m_elementCount, 0);
    PX_Bool(pPX, _T("ElementShow"), m_elementShow, TRUE);
    PX_Bool(pPX, _T("ElementSolidPoint"), m_elementSolidPoint, TRUE);
    PX_Color(pPX, _T("ElementLineColor"), m_elementLineColor, RGB(0xFF, 0xFF, 0xFF));
    PX_Color(pPX, _T("ElementPointColor"), m_elementPointColor, RGB(0xFF, 0xFF, 0xFF));
    PX_Long(pPX, _T("ElementWidth"), m_elementWidth, 0);
    PX_Long(pPX, _T("ElementLinetype"), m_elementLinetype, 0);
    PX_Long(pPX, _T("ElementPointSymbol"), m_elementPointSymbol, 0);
    PX_String(pPX, _T("ElementName"), m_elementName, _T(""));

    PX_Short(pPX, _T("Annotation"), m_nAnnotation, 0);
    PX_Short(pPX, _T("AnnoCount"), m_nAnnoCount, 0);
    PX_String(pPX, _T("AnnoLabelCaption"), m_annoLabelCaption, _T(""));
    PX_Color(pPX, _T("AnnoLabelColor"), m_annoLabelColor, RGB(0xFF, 0xFF, 0xFF));
    PX_Double(pPX, _T("AnnoLabelX"), m_annoLabelX, 0);
    PX_Double(pPX, _T("AnnoLabelY"), m_annoLabelY, 0);
    PX_Bool(pPX, _T("AnnoLabelHorizontal"), m_annoLabelHorizontal, TRUE);
    PX_Bool(pPX, _T("AnnoVisible"), m_annoVisible, TRUE);

    PX_Font(pPX, _T("TickFont"), m_fontTick, &_fontdescTick);
    PX_Font(pPX, _T("TitleFont"), m_fontTitle, &_fontdescTitle);
    PX_Font(pPX, _T("LabelFont"), m_fontLabel, &_fontdescLabel);
    PX_Font(pPX, _T("IdentFont"), m_fontIdent, &_fontdescIdent);

    PX_Picture(pPX, "PlotAreaPicture", m_picPlotArea);
    PX_Picture(pPX, "ControlFramePicture", m_picControlFrame);
}

DWORD CNTGraphCtrl::GetControlFlags()
{
    DWORD dwFlags = COleControl::GetControlFlags();
    dwFlags |= noFlickerActivate;
    return dwFlags;
}

void CNTGraphCtrl::OnResetState()
{
    COleControl::OnResetState();
    m_picPlotArea.CreateEmpty();
    m_picControlFrame.CreateEmpty();
}

void CNTGraphCtrl::AboutBox()
{
    CDialog dlgAbout(IDD_ABOUTBOX_NTGRAPH);
    dlgAbout.DoModal();
}

void CNTGraphCtrl::CreateFont(CDC* pDC)
{
    if (m_fontY.m_hObject == NULL) {
        LOGFONT lf;
        CFont *pOldFont, *pLogFont;

        pOldFont = SelectFontObject(pDC, m_fontLabel);
        pLogFont = pDC->SelectObject(pOldFont);

        if (pLogFont)
            pLogFont->GetLogFont(&lf);

        SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
        lf.lfOrientation = 900;
        lf.lfEscapement = 900;

        m_fontY.CreateFontIndirect(&lf);

        pOldFont = SelectFontObject(pDC, m_fontIdent);
        pLogFont = pDC->SelectObject(pOldFont);

        if (pLogFont)
            pLogFont->GetLogFont(&lf);

        lf.lfOrientation = 900;
        lf.lfEscapement = 900;

        m_fontVLabel.CreateFontIndirect(&lf);
    }
}

LPFONTDISP CNTGraphCtrl::GetTickFont() { return m_fontTick.GetFontDispatch(); }

void CNTGraphCtrl::SetTickFont(LPFONTDISP newValue)
{
    m_fontTick.InitializeFont(&_fontdescTick, newValue);
    OnFontChanged(); //notify any changes
    SetModifiedFlag();
}

LPFONTDISP CNTGraphCtrl::GetTitleFont() { return m_fontTitle.GetFontDispatch(); }

void CNTGraphCtrl::SetTitleFont(LPFONTDISP newValue)
{
    m_fontTitle.InitializeFont(&_fontdescTitle, newValue);
    OnFontChanged();
    SetModifiedFlag();
}

LPFONTDISP CNTGraphCtrl::GetLabelFont() { return m_fontLabel.GetFontDispatch(); }

void CNTGraphCtrl::SetLabelFont(LPFONTDISP newValue)
{
    m_fontLabel.InitializeFont(&_fontdescLabel, newValue);
    OnFontChanged();
    SetModifiedFlag();
}

LPFONTDISP CNTGraphCtrl::GetIdentFont() { return m_fontIdent.GetFontDispatch(); }

void CNTGraphCtrl::SetIdentFont(LPFONTDISP newValue)
{
    m_fontIdent.InitializeFont(&_fontdescIdent, newValue);
    OnFontChanged();
    SetModifiedFlag();
}

void CNTGraphCtrl::OnFontChanged()
{
    m_fontY.DeleteObject();
    m_fontVLabel.DeleteObject();
    InvalidateControl();
    SetModifiedFlag();
    COleControl::OnFontChanged();
}

void CNTGraphCtrl::PrepareForDrawing(CDC* pDC, const CRect& rect)
{

    pDC->SetMapMode(MM_TEXT);
    pDC->SetWindowOrg(0, 0);
    pDC->SetWindowExt(rect.right, rect.bottom);
    pDC->SetViewportOrg(0, 0);
    pDC->SetViewportExt(rect.right, rect.bottom);

    m_ctlRect = rect;
    pDC->DPtoLP(&m_ctlRect);

    CreateFont(pDC);
    CalcRect(pDC);
    ResCalc();
}

void CNTGraphCtrl::CalcRect(CDC* pDC)
{
    int offset = 15;

    CSize txtXLabelSize, txtYLabelSize(0, 0), txtTitleSize;
    CString str;
    CFont *pOldFont, *pSaveFont;

    pOldFont = SelectFontObject(pDC, m_fontTick);

    double res = (dRangeY[MAX] - dRangeY[MIN]) / m_nGridY;
    CSize sz(0, 0);

    for (int i = 0; i <= m_nGridY; i++) {
        double y = dRangeY[MIN] + (res * (double)i);

        str = FormatYAxisLabel(y);

        sz = pDC->GetTextExtent(str);

        if (txtYLabelSize.cx < sz.cx)
            txtYLabelSize = sz;
    }

    str = FormatXAxisLabel(dRangeX[MAX]);

    txtXLabelSize = pDC->GetTextExtent(str);

    pDC->SelectObject(pOldFont);

    pSaveFont = SelectFontObject(pDC, m_fontTitle);

    const CString& strCaption = InternalGetText();

    txtTitleSize = pDC->GetTextExtent(strCaption);

    pDC->SelectObject(pOldFont);

    m_axisRect.left = m_ctlRect.left + txtTitleSize.cy + txtYLabelSize.cx + offset;
    m_axisRect.right = m_ctlRect.right - (txtXLabelSize.cx / 2) - offset;
    m_axisRect.top = m_ctlRect.top + txtTitleSize.cy + offset;
    m_axisRect.bottom = m_ctlRect.bottom - (txtTitleSize.cy + txtXLabelSize.cy) - offset;
}

void CNTGraphCtrl::ResCalc()
{
    double dpixelx, dpixely;

    dpixelx = (double)m_axisRect.Width();
    dpixely = (double)m_axisRect.Height();
    dResY = (dRangeY[MAX] - dRangeY[MIN]) / dpixely;
    dResX = (dRangeX[MAX] - dRangeX[MIN]) / dpixelx;
}

CPoint CNTGraphCtrl::Corrdinate(double x, double y)
{
    double rx, ry;
    int xPixel, yPixel;
    CPoint retPt;

    rx = x - dRangeX[MIN];
    ry = y - dRangeY[MIN];

    xPixel = (int)(rx / dResX);
    yPixel = (int)(ry / dResY);

    retPt.x = xPixel + m_axisRect.left;
    retPt.y = m_axisRect.bottom - yPixel;
    return retPt;
}

CPoint CNTGraphCtrl::Corrdinate(const CElementPoint& pt)
{
    double rx, ry;
    int xPixel, yPixel;
    CPoint retPt;

    rx = pt.x - dRangeX[MIN];
    ry = pt.y - dRangeY[MIN];

    xPixel = (int)(rx / dResX);
    yPixel = (int)(ry / dResY);

    retPt.x = xPixel + m_axisRect.left;
    retPt.y = m_axisRect.bottom - yPixel;
    return retPt;
}

double CNTGraphCtrl::RoundUp(double val, int ex)
{
    int exponent;
    int i;
    exponent = (int)(floor(log10(fabs(val))));

    if (ex == 0) {
        if (exponent < 0)
            for (i = exponent; i < 0; i++) {
                val *= 10.0;
            }
        else
            for (i = 0; i < exponent; i++) {
                val /= 10.0;
            }
    } else {

        if (ex < 0)
            if (exponent < 0)
                for (i = ex; i < 0; i++) {
                    val *= 10.0;
                }
            else
                return val;
        else if (exponent < 0)
            return val;
        else
            for (i = 0; i < ex; i++) {
                val /= 10.0;
            }
    }

    return val;
}

void CNTGraphCtrl::DrawGraphOffScreen(CDC* pdc, const CRect& rcBounds)
{
    CDC dcMem;

    CBitmap bitOff;
    CRect rcBoundsDP(rcBounds);
    pdc->LPtoDP(&rcBoundsDP);

    CRect rcBitmapBounds(0, 0, rcBoundsDP.Width(), rcBoundsDP.Height());

    dcMem.CreateCompatibleDC(pdc);

    bitOff.CreateCompatibleBitmap(pdc, rcBitmapBounds.Width(), rcBitmapBounds.Height());

    CBitmap* pOldBitmap = dcMem.SelectObject(&bitOff);

    int iSavedDC = dcMem.SaveDC();

    DrawGraph(&dcMem, rcBitmapBounds);

    dcMem.RestoreDC(iSavedDC);

    pdc->SetMapMode(MM_TEXT);
    pdc->SetWindowOrg(0, 0);
    pdc->SetViewportOrg(0, 0);

    pdc->BitBlt(rcBoundsDP.left,
        rcBoundsDP.top,
        rcBoundsDP.Width(),
        rcBoundsDP.Height(),
        &dcMem,
        0,
        0,
        SRCCOPY);

    dcMem.SelectObject(pOldBitmap);
}

void CNTGraphCtrl::DrawGraph(CDC* pDC, const CRect& rcBounds)
{
    PrepareForDrawing(pDC, rcBounds);

    DrawControlFrame(pDC);

    DrawTitle(pDC);

    if (m_showGrid)
        if (m_xLog || m_yLog) {
            if (m_xLog)
                DrawXLogGrid(pDC);
            if (m_yLog)
                DrawYLogGrid(pDC);

            DrawGrid(pDC);
        } else
            DrawGrid(pDC);
    else if (m_xLog || m_yLog) {
        if (m_xLog)
            DrawXLogGrid(pDC);
        if (m_yLog)
            DrawYLogGrid(pDC);

        DrawTicks(pDC);
    } else
        DrawTicks(pDC);

    if (m_frameStyle == Flat)
        DrawAxis(pDC);

    DrawAxisLabel(pDC);

    if (bIsPlotAvailable)
        PlotElement(pDC);

    if (m_CursorList.GetCount())
        DrawCursor(pDC);

    if (m_AnnotationList.GetCount())
        Annotate(pDC);
}

void CNTGraphCtrl::DrawControlFrame(CDC* pDC)
{

    if (m_brushFrame.m_hObject == NULL)
        m_brushFrame.CreateSolidBrush(m_controlFrameColor);

    if (m_brushPlot.m_hObject == NULL)
        m_brushPlot.CreateSolidBrush(m_plotAreaColor);

    CBrush* pBrushSave = pDC->SelectObject(&m_brushFrame);
    pDC->Rectangle(m_ctlRect);

    if (m_frameStyle == Scope)
        ::DrawEdge(pDC->GetSafeHdc(), m_ctlRect, EDGE_RAISED, BF_RECT);

    if (m_frameStyle == Bitmap)
        m_picFrame.Render(pDC, m_ctlRect, m_ctlRect);

    m_picControlFrame.Render(pDC, m_ctlRect, m_ctlRect);

    pDC->SelectObject(&m_brushPlot);
    pDC->Rectangle(m_axisRect);

    if (m_frameStyle != Flat)
        ::DrawEdge(pDC->GetSafeHdc(), m_axisRect, EDGE_SUNKEN, BF_RECT);

    m_picPlotArea.Render(pDC, m_axisRect, m_axisRect);

    if (!IsOptimizedDraw())
        pDC->SelectObject(pBrushSave);
}
void CNTGraphCtrl::DrawAxis(CDC* pDC)
{
    if (m_axisPen.m_hObject == NULL)
        m_axisPen.CreatePen(PS_SOLID, 2, m_axisColor);

    CPen* pPenSave = pDC->SelectObject(&m_axisPen);

    pDC->MoveTo(Corrdinate(dRangeX[MIN], dRangeY[MIN]));
    pDC->LineTo(Corrdinate(dRangeX[MAX], dRangeY[MIN]));
    pDC->MoveTo(Corrdinate(dRangeX[MIN], dRangeY[MIN]));
    pDC->LineTo(Corrdinate(dRangeX[MIN], dRangeY[MAX]));
    pDC->MoveTo(Corrdinate(dRangeX[MAX], dRangeY[MIN]));
    pDC->LineTo(Corrdinate(dRangeX[MAX], dRangeY[MAX]));
    pDC->MoveTo(Corrdinate(dRangeX[MIN], dRangeY[MAX]));
    pDC->LineTo(Corrdinate(dRangeX[MAX], dRangeY[MAX]));

    if (!IsOptimizedDraw())
        pDC->SelectObject(pPenSave);
}

void CNTGraphCtrl::DrawYLogGrid(CDC* pDC)
{
    const int FINE_SCALE = 5;
    CPoint m_start, m_stop;
    int i;
    int j;
    double y;
    double step;
    double Ymax, Ymin;

    Ymax = dRangeY[MAX];
    Ymin = dRangeY[MIN];

    dRangeY[MIN] = 0;
    dRangeY[MAX] = m_nGridY;
    ResCalc();

    if (m_gridPen.m_hObject == NULL)
        m_gridPen.CreatePen(PS_SOLID, 0, m_gridColor);

    CPen* pPenSave = pDC->SelectObject(&m_gridPen);

    for (j = (int)dRangeY[MIN]; j < (int)dRangeY[MAX]; j++) {
        y = (double)j;
        step = (pow(10, y + 1) - pow(10, y)) / (double)FINE_SCALE;
        for (i = 0; i < FINE_SCALE; i++) {

            y = log10(pow(10, y) + step);
            m_start = Corrdinate(dRangeX[MIN], y);
            m_stop = Corrdinate(dRangeX[MAX], y);

            if (!m_showGrid) {
                m_stop = m_start;

                if (m_frameStyle == Flat)
                    m_stop.x += 5;
                else
                    m_stop.x -= 5;
            }

            pDC->MoveTo(m_start);
            pDC->LineTo(m_stop);
        }
    }
    if (m_frameStyle == Flat) {
        for (j = (int)dRangeY[MIN]; j < (int)dRangeY[MAX]; j++) {
            y = (double)j;
            step = (pow(10, y + 1) - pow(10, y)) / (double)FINE_SCALE;
            for (i = 0; i < FINE_SCALE; i++) {

                y = log10(pow(10, y) + step);
                m_start = Corrdinate(dRangeX[MIN], y);
                m_stop = Corrdinate(dRangeX[MAX], y);

                if (!m_showGrid) {
                    m_start = m_stop;
                    m_stop.x -= 5;
                    pDC->MoveTo(m_start);
                    pDC->LineTo(m_stop);
                }
            }
        }
    }

    dRangeY[MIN] = Ymin;
    dRangeY[MAX] = Ymax;
    ResCalc();

    if (!IsOptimizedDraw())
        pDC->SelectObject(pPenSave);
}

void CNTGraphCtrl::DrawXLogGrid(CDC* pDC)
{
    const int FINE_SCALE = 5;
    CPoint m_start, m_stop;
    int i;
    int j;
    double x;
    double step;
    double Xmax, Xmin;

    Xmax = dRangeX[MAX];
    Xmin = dRangeX[MIN];

    dRangeX[MIN] = 0;
    dRangeX[MAX] = m_nGridX;
    ResCalc();

    if (m_gridPen.m_hObject == NULL)
        m_gridPen.CreatePen(PS_SOLID, 0, m_gridColor);

    CPen* pPenSave = pDC->SelectObject(&m_gridPen);

    for (j = (int)dRangeX[MIN]; j < (int)dRangeX[MAX]; j++) {
        x = (double)j;
        step = (pow(10, x + 1) - pow(10, x)) / (double)FINE_SCALE;
        for (i = 0; i < FINE_SCALE; i++) {

            x = log10(pow(10, x) + step);
            m_start = Corrdinate(x, dRangeY[MIN]);
            m_stop = Corrdinate(x, dRangeY[MAX]);

            if (!m_showGrid) {
                m_stop = m_start;

                if (m_frameStyle == Flat)
                    m_stop.y -= 5;
                else
                    m_stop.y += 5;
            }

            pDC->MoveTo(m_start);
            pDC->LineTo(m_stop);
        }
    }
    if (m_frameStyle == Flat) {
        for (j = (int)dRangeX[MIN]; j < (int)dRangeX[MAX]; j++) {
            x = (double)j;
            step = (pow(10, x + 1) - pow(10, x)) / (double)FINE_SCALE;
            for (i = 0; i < FINE_SCALE; i++) {

                x = log10(pow(10, x) + step);
                m_start = Corrdinate(x, dRangeY[MIN]);
                m_stop = Corrdinate(x, dRangeY[MAX]);

                if (!m_showGrid) {
                    m_start = m_stop;
                    m_stop.y += 5;
                    pDC->MoveTo(m_start);
                    pDC->LineTo(m_stop);
                }
            }
        }
    }
    dRangeX[MIN] = Xmin;
    dRangeX[MAX] = Xmax;
    ResCalc();

    if (!IsOptimizedDraw())
        pDC->SelectObject(pPenSave);
}

void CNTGraphCtrl::DrawTicks(CDC* pDC)
{

    CPoint m_start, m_stop;
    int i;
    double x, y;
    double step;

    if (m_gridPen.m_hObject == NULL)
        m_gridPen.CreatePen(PS_SOLID, 0, m_gridColor);

    CPen* pPenSave = pDC->SelectObject(&m_gridPen);

    if (!m_xLog) {
        step = (dRangeX[MAX] - dRangeX[MIN]) / (double)m_nGridX;

        for (i = 1; i < m_nGridX; i++) {
            x = dRangeX[MIN] + (step * (double)i);
            m_start = Corrdinate(x, dRangeY[MIN]);
            m_stop = m_start;

            if (m_frameStyle == Flat)
                m_stop.y -= 6;
            else
                m_stop.y += 6;

            pDC->MoveTo(m_start);
            pDC->LineTo(m_stop);
        }

        for (i = 1; i < m_nGridX * 3; i++) {
            x = dRangeX[MIN] + (step * (double)i) / 3;
            m_start = Corrdinate(x, dRangeY[MIN]);
            m_stop = m_start;

            if (m_frameStyle == Flat)
                m_stop.y -= 4;
            else
                m_stop.y += 4;

            pDC->MoveTo(m_start);
            pDC->LineTo(m_stop);
        }

        if (m_frameStyle == Flat) {
            for (i = 1; i < m_nGridX; i++) {
                x = dRangeX[MIN] + (step * (double)i);
                m_start = Corrdinate(x, dRangeY[MAX]);
                m_stop = m_start;
                m_stop.y += 6;
                pDC->MoveTo(m_start);
                pDC->LineTo(m_stop);
            }

            for (i = 1; i < m_nGridX * 3; i++) {
                x = dRangeX[MIN] + (step * (double)i) / 3;
                m_start = Corrdinate(x, dRangeY[MAX]);
                m_stop = m_start;
                m_stop.y += 4;
                pDC->MoveTo(m_start);
                pDC->LineTo(m_stop);
            }
        }
    }

    if (!m_yLog) {
        step = (dRangeY[MAX] - dRangeY[MIN]) / (double)m_nGridY;

        for (i = 1; i < m_nGridY; i++) {
            y = dRangeY[MIN] + (step * (double)i);
            m_start = Corrdinate(dRangeX[MIN], y);
            m_stop = m_start;

            if (m_frameStyle == Flat)
                m_stop.x += 6;
            else
                m_stop.x -= 6;

            pDC->MoveTo(m_start);
            pDC->LineTo(m_stop);
        }

        for (i = 1; i < m_nGridY * 3; i++) {
            y = dRangeY[MIN] + (step * (double)i) / 3;
            m_start = Corrdinate(dRangeX[MIN], y);
            m_stop = m_start;

            if (m_frameStyle == Flat)
                m_stop.x += 4;
            else
                m_stop.x -= 4;

            pDC->MoveTo(m_start);
            pDC->LineTo(m_stop);
        }

        if (m_frameStyle == Flat) {
            for (i = 1; i < m_nGridY; i++) {
                y = dRangeY[MIN] + (step * (double)i);
                m_start = Corrdinate(dRangeX[MAX], y);
                m_stop = m_start;
                m_stop.x -= 6;
                pDC->MoveTo(m_start);
                pDC->LineTo(m_stop);
            }
            for (i = 1; i < m_nGridY * 3; i++) {
                y = dRangeY[MIN] + (step * (double)i) / 3;
                m_start = Corrdinate(dRangeX[MAX], y);
                m_stop = m_start;
                m_stop.x -= 4;
                pDC->MoveTo(m_start);
                pDC->LineTo(m_stop);
            }
        }
    }

    if (!IsOptimizedDraw())
        pDC->SelectObject(pPenSave);
}

void CNTGraphCtrl::DrawGrid(CDC* pDC)
{
    CPoint m_start, m_stop;
    int i;
    double x, y;
    double step;

    if (m_gridPen.m_hObject == NULL)
        m_gridPen.CreatePen(PS_SOLID, 0, m_gridColor);

    CPen* pPenSave = pDC->SelectObject(&m_gridPen);

    if (!m_xLog) {
        step = (dRangeX[MAX] - dRangeX[MIN]) / (double)m_nGridX;
        for (i = 1; i < m_nGridX; i++) {
            x = dRangeX[MIN] + (step * (double)i);
            m_start = Corrdinate(x, dRangeY[MIN]);
            m_stop = Corrdinate(x, dRangeY[MAX]);
            pDC->MoveTo(m_start);
            pDC->LineTo(m_stop);
        }
    }

    if (!m_yLog) {
        step = (dRangeY[MAX] - dRangeY[MIN]) / (double)m_nGridY;
        for (i = 1; i < m_nGridY; i++) {
            y = dRangeY[MIN] + (step * (double)i);
            m_start = Corrdinate(dRangeX[MIN], y);
            m_stop = Corrdinate(dRangeX[MAX], y);
            pDC->MoveTo(m_start);
            pDC->LineTo(m_stop);
        }
    }

    if (!IsOptimizedDraw())
        pDC->SelectObject(pPenSave);
}

void CNTGraphCtrl::DrawTitle(CDC* pDC)
{

    CFont* pOldFont;
    TEXTMETRIC tm;
    const CString& strCaption = InternalGetText();

    pOldFont = SelectFontObject(pDC, m_fontTitle);
    pDC->SetTextColor(m_labelColor);
    pDC->GetTextMetrics(&tm);

    pDC->SetTextAlign(TA_TOP | TA_CENTER);
    pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut((m_ctlRect.left + m_ctlRect.right) / 2,
        (m_ctlRect.top + m_axisRect.top - tm.tmHeight) / 2,
        strCaption, strCaption.GetLength());

    pDC->SelectObject(pOldFont);
}

void CNTGraphCtrl::DrawAxisLabel(CDC* pDC)
{
    CFont* pOldFont;

    int i;
    double res, x, y;
    CPoint point;
    CSize txtSize;
    CString str;

    pDC->SetTextColor(m_labelColor);
    pDC->SetTextAlign(TA_RIGHT);
    pDC->SetBkMode(TRANSPARENT);

    pOldFont = SelectFontObject(pDC, m_fontTick);

    res = (dRangeY[MAX] - dRangeY[MIN]) / m_nGridY;
    CSize sz(0, 0);

    for (i = 0; i <= m_nGridY; i++) {
        y = dRangeY[MIN] + (res * (double)i);
        point = Corrdinate(dRangeX[MIN], y);

        str = FormatYAxisLabel(y);

        txtSize = pDC->GetTextExtent(str);

        if (sz.cx < txtSize.cx)
            sz = txtSize;

        point.x -= 5;
        point.y -= txtSize.cy / 2;

        pDC->TextOut(point.x, point.y, str);
    }

    res = (dRangeX[MAX] - dRangeX[MIN]) / m_nGridX;

    for (i = 0; i <= m_nGridX; i++) {
        x = dRangeX[MIN] + (res * (double)i);
        point = Corrdinate(x, dRangeY[MIN]);

        str = FormatXAxisLabel(x);

        txtSize = pDC->GetTextExtent(str);

        point.x += txtSize.cx / 2;
        point.y += 5;

        pDC->TextOut(point.x, point.y, str);
    }

    SelectFontObject(pDC, m_fontLabel);

    txtSize = pDC->GetTextExtent(m_xLabel);
    point.x = m_axisRect.CenterPoint().x + (txtSize.cx / 2);
    point.y = (m_ctlRect.bottom + m_axisRect.bottom) / 2;
    point.y -= (txtSize.cy - sz.cy) / 2;

    pDC->TextOut(point.x, point.y, m_xLabel);

    pDC->SelectObject(&m_fontY);

    txtSize = pDC->GetTextExtent(m_yLabel);
    point.x = (m_ctlRect.left + m_axisRect.left) / 2;
    point.x -= (txtSize.cy + sz.cx) / 2;
    point.y = m_axisRect.CenterPoint().y - (txtSize.cx / 2);
    pDC->TextOut(point.x, point.y, m_yLabel);

    pDC->SelectObject(pOldFont);
}

void CNTGraphCtrl::PlotElement(CDC* pDC)
{
    POSITION element = m_ElementList.GetHeadPosition();
    ASSERT(element);

    int index = 1;

    while (element) {

        CGraphElement* pElement = m_ElementList.GetNext(element);

        if (!pElement->bIsPlotAvailable)
            continue;

        if (!pElement->m_bShow)
            continue;

        if (pElement->m_nType != Null)
            PlotElementLines(pDC, pElement);

        if (pElement->m_nSymbol != Nosym)
            PlotElementPoints(pDC, pElement);

        if (m_elementIdentify)
            DrawElementLabel(pDC, pElement, index);
        index++;
    }
}

void CNTGraphCtrl::DrawElementLabel(CDC* pDC, CGraphElement* pElement, int index)
{

    CFont* pOldFont;
    TEXTMETRIC tm;
    CString strCaption = pElement->m_strName;

    pOldFont = SelectFontObject(pDC, m_fontIdent);
    pDC->SetTextColor(pElement->m_LineColor);
    pDC->GetTextMetrics(&tm);

    pDC->SetTextAlign(TA_TOP | TA_CENTER);
    pDC->SetBkMode(TRANSPARENT);

    CRect rect(m_axisRect);

    rect.left += tm.tmAveCharWidth * strCaption.GetLength();
    rect.top += tm.tmHeight * index;

    pDC->TextOut(rect.left, rect.top, strCaption, strCaption.GetLength());

    pDC->SelectObject(pOldFont);
}

void CNTGraphCtrl::PlotElementLines(CDC* pDC, CGraphElement* pElement)
{

    if (m_linePen.m_hObject == NULL)
        m_linePen.CreatePen(pElement->m_nType,
            pElement->m_nWidth,
            pElement->m_LineColor);

    CPen* pPenSave = pDC->SelectObject(&m_linePen);

    if (m_pointPen.m_hObject == NULL)
        m_pointPen.CreatePen(0, 0, pElement->m_PointColor);

    if (m_lineBrush.m_hObject == NULL)
        m_lineBrush.CreateSolidBrush(pElement->m_PointColor);

    CBrush* pBrushSave = pDC->SelectObject(&m_lineBrush);

    CRect BoundingRect;
    BoundingRect = m_axisRect;
    BoundingRect.DeflateRect(pElement->m_nWidth, pElement->m_nWidth);

    POSITION pos = pElement->m_PointList.GetHeadPosition();
    CElementPoint pt = pElement->m_PointList.GetHead();

    if (m_xLog)
        pt.x = log10(pt.x);
    if (m_yLog)
        pt.y = log10(pt.y);

    CPoint point = Corrdinate(pt);

    if (point.x < BoundingRect.left)
        point.x = BoundingRect.left;
    if (point.y < BoundingRect.top)
        point.y = BoundingRect.top;
    if (point.y > BoundingRect.bottom)
        point.y = BoundingRect.bottom;

    pDC->MoveTo(point);
    CPoint oldpt(point);

    while (pos) {
        pt = pElement->m_PointList.GetNext(pos);

        if (m_xLog)
            pt.x = log10(pt.x);
        if (m_yLog)
            pt.y = log10(pt.y);

        point = Corrdinate(pt);

        CPoint p;
        CRect rect;

        if (point.x > BoundingRect.right)
            point.x = BoundingRect.right;
        if (point.x < BoundingRect.left)
            point.x = BoundingRect.left;
        if (point.y < BoundingRect.top)
            point.y = BoundingRect.top;
        if (point.y > BoundingRect.bottom)
            point.y = BoundingRect.bottom;

        if (
            (oldpt.x == BoundingRect.right && point.x == BoundingRect.right) || (oldpt.x == BoundingRect.left && point.x == BoundingRect.left) || (oldpt.y == BoundingRect.top && point.y == BoundingRect.top) || (oldpt.y == BoundingRect.bottom && point.y == BoundingRect.bottom)) {

            switch (pElement->m_nType) {
            case Bars:
                rect = CRect(oldpt, point);
                pDC->FillSolidRect(rect, pElement->m_LineColor);
                pDC->MoveTo(point);
                break;
            case Stick:
                pDC->MoveTo(point);
                p = Corrdinate(pt.x, pElement->min.y);
                p.x = point.x;
                if (p.y > BoundingRect.bottom)
                    p.y = BoundingRect.bottom;
                if (p.x > BoundingRect.right)
                    p.y = BoundingRect.right;
                pDC->LineTo(p);
                pDC->MoveTo(point);
                break;
            default:
                pDC->MoveTo(point);
                oldpt = point;
                continue;
            }
        }

        switch (pElement->m_nType) {

        case XYStep:
            p.x = point.x;
            p.y = oldpt.y;
            pDC->LineTo(p);
            pDC->LineTo(point);
            break;

        case YXStep:
            p.y = point.y;
            p.x = oldpt.x;
            pDC->LineTo(p);
            pDC->LineTo(point);
            break;

        case Bars:
            p = Corrdinate(pt.x, pElement->min.y);
            p.x = point.x;
            if (p.y < BoundingRect.top)
                p.y = BoundingRect.top;
            if (p.y > BoundingRect.bottom)
                p.y = BoundingRect.bottom;
            rect = CRect(oldpt, p);
            pDC->FillSolidRect(rect, pElement->m_LineColor);
            pDC->MoveTo(point);
            break;

        case Stick:
            pDC->LineTo(point);
            p = Corrdinate(pt.x, pElement->min.y);
            p.x = point.x;
            if (p.y > BoundingRect.bottom)
                p.y = BoundingRect.bottom;
            if (p.x > BoundingRect.right)
                p.y = BoundingRect.right;
            pDC->LineTo(p);
            pDC->MoveTo(point);
            break;

        default:
            pDC->LineTo(point);
            break;
        }

        oldpt = point;
    }

    if (!IsOptimizedDraw())
        pDC->SelectObject(pPenSave);

    if (!IsOptimizedDraw())
        pDC->SelectObject(pBrushSave);

    m_linePen.DeleteObject();
    m_lineBrush.DeleteObject();
}

void CNTGraphCtrl::PlotElementPoints(CDC* pDC, CGraphElement* pElement)
{
    if (m_pointPen.m_hObject == NULL)
        m_pointPen.CreatePen(0, 0, pElement->m_PointColor);

    CPen* pPenSave = pDC->SelectObject(&m_pointPen);

    if (m_pointBrush.m_hObject == NULL)
        m_pointBrush.CreateSolidBrush(pElement->m_PointColor);

    CBrush* pBrushSave = pDC->SelectObject(&m_pointBrush);

    CRect BoundingRect;
    BoundingRect = m_axisRect;
    BoundingRect.DeflateRect(pElement->m_nWidth, pElement->m_nWidth);

    POSITION pos = pElement->m_PointList.GetHeadPosition();
    CElementPoint pt = pElement->m_PointList.GetHead();

    if (m_xLog)
        pt.x = log10(pt.x);
    if (m_yLog)
        pt.y = log10(pt.y);

    CPoint point = Corrdinate(pt);

    if (point.x < BoundingRect.left)
        point.x = BoundingRect.left;
    if (point.y < BoundingRect.top)
        point.y = BoundingRect.top;
    if (point.y > BoundingRect.bottom)
        point.y = BoundingRect.bottom;

    pDC->MoveTo(point);
    CPoint oldpt(point);

    while (pos) {
        pt = pElement->m_PointList.GetNext(pos);

        if (m_xLog)
            pt.x = log10(pt.x);
        if (m_yLog)
            pt.y = log10(pt.y);

        point = Corrdinate(pt);
        if (point.x > BoundingRect.right)
            point.x = BoundingRect.right;
        if (point.x < BoundingRect.left)
            point.x = BoundingRect.left;
        if (point.y < BoundingRect.top)
            point.y = BoundingRect.top;
        if (point.y > BoundingRect.bottom)
            point.y = BoundingRect.bottom;

        if (
            (oldpt.x == BoundingRect.right && point.x == BoundingRect.right) || (oldpt.x == BoundingRect.left && point.x == BoundingRect.left) || (oldpt.y == BoundingRect.top && point.y == BoundingRect.top) || (oldpt.y == BoundingRect.bottom && point.y == BoundingRect.bottom)) {
            pDC->MoveTo(point);
            oldpt = point;
            continue;
        }

        int symsz = pElement->m_nWidth;
        if (symsz < 0)
            symsz = 0;
        CRect rect(point.x - symsz, point.y - symsz, point.x + symsz, point.y + symsz);

        if (pElement->m_bSolid == FALSE)
            pDC->SelectObject(pBrushSave);

        if (pElement->m_nSymbol == Dots)
            pDC->Ellipse(rect);
        if (pElement->m_nSymbol == Rectangles)
            pDC->Rectangle(rect);
        if (pElement->m_nSymbol == Diamonds)
            DrawDiamond(pDC, point, symsz);
        if (pElement->m_nSymbol == Asterisk)
            DrawAsterisk(pDC, point, symsz);
        if (pElement->m_nSymbol == DownTriangles)
            DrawDownTriangle(pDC, point, symsz);
        if (pElement->m_nSymbol == RightTriangles)
            DrawRightTriangle(pDC, point, symsz);
        if (pElement->m_nSymbol == UpTriangles)
            DrawUpTriangle(pDC, point, symsz);
        if (pElement->m_nSymbol == LeftTriangles)
            DrawLeftTriangle(pDC, point, symsz);

        oldpt = point;
    }

    if (!IsOptimizedDraw())
        pDC->SelectObject(pPenSave);
    if (!IsOptimizedDraw())
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
    OnControlFrameColorChanged();
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
    const double epsilon = 0.1;
    if (fabs(xmin - xmax) > epsilon && fabs(ymin - ymax) > epsilon && xmin <= xmax && ymin <= ymax) {
        dRangeX[MIN] = xmin;
        dRangeX[MAX] = xmax;
        dRangeY[MIN] = ymin;
        dRangeY[MAX] = ymax;
    }
    InvalidateControl();
}
void CNTGraphCtrl::AutoRange()
{
    if (!bIsPlotAvailable) {
        dAutoRangeX[MIN] = dRangeX[MIN];
        dAutoRangeY[MIN] = dRangeY[MIN];
        dAutoRangeX[MAX] = dRangeX[MAX];
        dAutoRangeY[MAX] = dRangeY[MAX];
    }
    SetRange(dAutoRangeX[MIN], dAutoRangeX[MAX], dAutoRangeY[MIN], dAutoRangeY[MAX]);
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
    m_panPoint = Corrdinate(0, 0);

    bUnzoom = TRUE;
    dOldRangeX[MIN] = dRangeX[MIN];
    dOldRangeY[MIN] = dRangeY[MIN];
    dOldRangeX[MAX] = dRangeX[MAX];
    dOldRangeY[MAX] = dRangeY[MAX];

    m_nTrackMode = nNewValue;
    FireTrackModeChanged(nNewValue);
    SetModifiedFlag();
}

int CNTGraphCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (COleControl::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (m_ttip.Create(this, TTS_ALWAYSTIP) && m_ttip.AddTool(this)
        && m_ttip.AddTool(this, LPCTSTR("Graph"))
        && m_ttip.AddTool(this, TTF_TRACK)) {

        TOOLINFO ti;
        FillToolInfo(&ti);

        m_ttip.SendMessage(TTM_ADDTOOL, 0, (LPARAM)&ti);
        m_ttip.SetDelayTime(TTDT_AUTOPOP, SHRT_MAX); // stop the tooltip coming up automatically
        m_ttip.SetDelayTime(TTDT_INITIAL, 0);

    } else {
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

        msg.hwnd = m_hWnd;
        msg.message = message;
        msg.wParam = wParam;
        msg.lParam = lParam;
        msg.time = 0;
        msg.pt.x = LOWORD(lParam);
        msg.pt.y = HIWORD(lParam);

        m_ttip.RelayEvent(&msg);
    }
}

void CNTGraphCtrl::FillToolInfo(TOOLINFO* ti)
{
    memset(ti, 0, sizeof(TOOLINFO));
    ti->cbSize = sizeof(TOOLINFO);
    ti->hwnd = GetParent()->GetSafeHwnd();
    ti->uFlags = TTF_IDISHWND | TTF_ABSOLUTE | TTF_TRACK;
    ti->uId = reinterpret_cast<UINT_PTR>(GetSafeHwnd());
}

void CNTGraphCtrl::DoPan(CPoint point, long mode)
{
    double Y1 = PT2DBLY(point.y);
    double Y2 = PT2DBLY(m_panPoint.y);
    double yOffset = Y1 - Y2;

    double X1 = PT2DBLX(point.x);
    double X2 = PT2DBLX(m_panPoint.x);
    double xOffset = X1 - X2;

    switch (mode) {
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
    if (m_nTrackMode == Zoom) {
        if (bUnzoom) {
            dOldRangeX[MIN] = dRangeX[MIN];
            dOldRangeY[MIN] = dRangeY[MIN];
            dOldRangeX[MAX] = dRangeX[MAX];
            dOldRangeY[MAX] = dRangeY[MAX];
            bUnzoom = FALSE;
        }

        CRectTracker tracker;
        double xmin, xmax, ymin, ymax;
        xmin = dRangeX[MIN];
        xmax = dRangeX[MAX];
        ymin = dRangeY[MIN];
        ymax = dRangeY[MAX];

        if (tracker.TrackRubberBand(this, point)) {
            CPoint pt1, pt2;

            tracker.m_rect.NormalizeRect();
            pt1 = tracker.m_rect.TopLeft();
            pt2 = tracker.m_rect.BottomRight();

            xmin = PT2DBLX(pt1.x);
            ymax = PT2DBLY(pt1.y);
            xmax = PT2DBLX(static_cast<unsigned long long>((pt2.x)));
            ymin = PT2DBLY(pt2.y);
        }

        SetRange(xmin, xmax, ymin, ymax);
    }
}

void CNTGraphCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_axisRect.PtInRect(point)) {

        if (nFlags == MK_LBUTTON) {

            CString str;
            switch (m_nTrackMode) {
            case Track:
                CursorPosition(point);
                str.Format("x= %g y= %g", m_cursorX, m_cursorY);
                m_ttip.UpdateTipText(LPCTSTR(str), this);
                RelayEvent(WM_LBUTTONDOWN, (WPARAM)nFlags,
                    MAKELPARAM(LOWORD(point.x), LOWORD(point.y)));
                break;
            case PanXY:
                DoPan(point, PanXY);
                break;
            case PanX:
                DoPan(point, PanX);
                break;
            case PanY:
                DoPan(point, PanY);
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

    if (m_axisRect.PtInRect(point)) {
        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        m_panPoint = point;

        CursorPosition(point);

        if (m_nTrackMode == Zoom)
            DoZoom(nFlags, point);
        else if (m_nTrackMode == Track && nFlags == MK_LBUTTON) {
            CursorPosition(point);
            CString str;
            str.Format("x= %g y= %g", m_cursorX, m_cursorY);
            m_ttip.UpdateTipText(LPCTSTR(str), this);
            RelayEvent(WM_LBUTTONDOWN, (WPARAM)nFlags,
                MAKELPARAM(LOWORD(point.x), LOWORD(point.y)));
        }
    }

    COleControl::OnLButtonDown(nFlags, point);
}

void CNTGraphCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_axisRect.PtInRect(point)) {
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
    if (m_nTrackMode == Zoom)
        SetRange(dOldRangeX[MIN], dOldRangeX[MAX], dOldRangeY[MIN], dOldRangeY[MAX]);

    COleControl::OnRButtonDown(nFlags, point);
}
void CNTGraphCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    COleControl::OnProperties(0, NULL, NULL);

    COleControl::OnLButtonDblClk(nFlags, point);
}

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

void CNTGraphCtrl::ShowProperties()
{
    COleControl::OnProperties(0, NULL, NULL);
}

void CNTGraphCtrl::CopyToClipboard()
{
    CRect rect;
    CClientDC dc(this);
    GetClientRect(&rect);
    CDC memDC;
    CBitmap bitmap;

    memDC.CreateCompatibleDC(&dc);
    bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    memDC.FillSolidRect(rect, dc.GetBkColor());
    OnDraw(&memDC, rect, rect);

    OpenClipboard();
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, bitmap.GetSafeHandle());
    CloseClipboard();

    memDC.SelectObject(pOldBitmap);
    bitmap.Detach();
}

void CNTGraphCtrl::PrintGraph()
{
    HDC hdcPrn;

    CPrintDialog* printDlg = new CPrintDialog(FALSE, PD_ALLPAGES | PD_RETURNDC, NULL);

    printDlg->m_pd.nMinPage = printDlg->m_pd.nMaxPage = 1;
    printDlg->m_pd.nFromPage = printDlg->m_pd.nToPage = 1;

    if (!(printDlg->DoModal() == IDOK))
        return;

    hdcPrn = printDlg->GetPrinterDC();
    if (hdcPrn != NULL) {
        char pbuf[100] = "NTGraph ActiveX Control";
        CDC* pDC = new CDC;
        pDC->Attach(hdcPrn);

        pDC->StartDoc("Graph2D");

        pDC->StartPage();
        pDC->TextOut(100, 10, pbuf);

        CPoint pt(100, 100);

        CDC* hdcScreen = GetDC();

        int nScreenWidth = ::GetDeviceCaps(hdcScreen->m_hDC, HORZRES);
        int nScreenHeight = ::GetDeviceCaps(hdcScreen->m_hDC, VERTRES);

        int nWidth = ::GetDeviceCaps(hdcPrn, HORZRES);
        int nHeight = ::GetDeviceCaps(hdcPrn, VERTRES);

        LPDEVMODE lpDevMode = printDlg->GetDevMode();

        if (lpDevMode->dmOrientation == DMORIENT_PORTRAIT) {
            nWidth = static_cast<int>(0.9 * nWidth);
            nHeight = static_cast<int>(0.33 * nHeight);
        } else {
            nWidth = static_cast<int>(0.9 * nWidth);
            nHeight = static_cast<int>(0.9 * nHeight);
        }

        CSize sz(nWidth, nHeight);

        CY size;

        CArray<CY, CY> arrFontSize;

        double dHeightRatio = static_cast<double>(nHeight) / nScreenHeight;
        double dWidthRatio = static_cast<double>(nWidth) / nScreenWidth;
        double dconv = max(dHeightRatio, dWidthRatio);

        IFont* pf = m_fontTick.m_pFont;
        pf->get_Size(&size);
        arrFontSize.Add(size);
        TRACE("Size: %d\n", size.Lo);
        size.Lo = static_cast<int>(floor(dconv * size.Lo));
        pf->put_Size(size);

        pf = m_fontTitle.m_pFont;
        pf->get_Size(&size);
        arrFontSize.Add(size);
        TRACE("Size: %d\n", size.Lo);
        size.Lo = static_cast<int>(floor(dconv * size.Lo));
        pf->put_Size(size);

        pf = m_fontLabel.m_pFont;
        pf->get_Size(&size);
        arrFontSize.Add(size);
        TRACE("Size: %d\n", size.Lo);
        size.Lo = static_cast<int>(floor(dconv * size.Lo));
        pf->put_Size(size);

        pf = m_fontIdent.m_pFont;
        pf->get_Size(&size);
        arrFontSize.Add(size);
        TRACE("Size: %d\n", size.Lo);
        size.Lo = static_cast<int>(floor(dconv * size.Lo));
        pf->put_Size(size);

        CRect rect(pt, sz);
        pDC->SetViewportExt(rect.right, rect.bottom);
        pDC->SetViewportOrg(50, 50);
        DrawGraph(pDC, rect);

        pDC->EndPage();

        pDC->EndDoc();

        pf = m_fontTick.m_pFont;
        size = arrFontSize.GetAt(0);
        TRACE("Size: %d\n", size.Lo);
        pf->put_Size(size);

        pf = m_fontTitle.m_pFont;
        size = arrFontSize.GetAt(1);
        TRACE("Size: %d\n", size.Lo);
        pf->put_Size(size);

        pf = m_fontLabel.m_pFont;
        size = arrFontSize.GetAt(2);
        TRACE("Size: %d\n", size.Lo);
        pf->put_Size(size);

        pf = m_fontIdent.m_pFont;
        size = arrFontSize.GetAt(3);
        TRACE("Size: %d\n", size.Lo);
        pf->put_Size(size);

        pDC->Detach();
        delete pDC;
    }

    delete printDlg;
    InvalidateControl();
}

void CNTGraphCtrl::AddElement()
{
    CGraphElement* pElement = new CGraphElement(GetElementCount());
    m_ElementList.AddTail(pElement);
    m_elementCount = GetElementCount();
    m_nElementID = GetElementCount() - 1;
    m_Position = m_ElementList.GetTailPosition();
    SetModifiedFlag();
}

void CNTGraphCtrl::DeleteElement(short ElementID)
{
    POSITION aPosition = m_ElementList.FindIndex(ElementID);

    if (aPosition != NULL) {
        delete m_ElementList.GetAt(aPosition);

        m_ElementList.RemoveAt(aPosition);

        if (m_ElementList.IsEmpty()) {
            m_ElementList.RemoveAll();
            AddElement();
            bIsPlotAvailable = FALSE;
        }

        m_elementCount = GetElementCount();
        m_nElementID = m_elementCount - 1;

        InvalidateControl();
        SetModifiedFlag();

    } else
        AfxMessageBox("1 Element not found!");
}

void CNTGraphCtrl::ClearGraph()
{
    POSITION aPosition = m_ElementList.GetHeadPosition();

    while (aPosition)
        delete m_ElementList.GetNext(aPosition);

    m_ElementList.RemoveAll();

    AddElement();
    dAutoRangeX[MIN] = 0;
    dAutoRangeX[MAX] = 0;
    dAutoRangeY[MIN] = 0;
    dAutoRangeY[MAX] = 0;
    m_elementCount = GetElementCount();
    bIsPlotAvailable = FALSE;

    InvalidateControl();
    SetModifiedFlag();
}

void CNTGraphCtrl::PlotXY(double X, double Y, short ElementID)
{
    if (bIsPlotAvailable) {
        if (X < dAutoRangeX[MIN])
            dAutoRangeX[MIN] = floor(X);
        if (Y < dAutoRangeY[MIN])
            dAutoRangeY[MIN] = floor(Y);
        if (X > dAutoRangeX[MAX])
            dAutoRangeX[MAX] = ceil(X);
        if (Y > dAutoRangeY[MAX])
            dAutoRangeY[MAX] = ceil(Y);

    } else {

        dAutoRangeX[MIN] = floor(X);
        dAutoRangeY[MIN] = floor(Y);
        dAutoRangeX[MAX] = ceil(X);
        dAutoRangeY[MAX] = ceil(Y);

        bIsPlotAvailable = TRUE;
    }

    POSITION aPosition = m_ElementList.FindIndex(ElementID);

    if (aPosition != NULL) {
        CElementPoint point(X, Y);
        const auto graphElement = m_ElementList.GetAt(aPosition);
        graphElement->m_PointList.AddTail(point);
        if (graphElement->min.x > point.x)
            graphElement->min.x = point.x;
        if (graphElement->min.y > point.y)
            graphElement->min.y = point.y;
        if (graphElement->max.x < point.x)
            graphElement->max.x = point.x;
        if (graphElement->max.y < point.y)
            graphElement->max.y = point.y;
        graphElement->bIsPlotAvailable = TRUE;
    } else
        AfxMessageBox("Element not found!");

    InvalidateControl();
}

void CNTGraphCtrl::PlotY(double Y, short ElementID)
{

    int X = 0;

    // Gets the position of the element by index.
    POSITION aPosition = m_ElementList.FindIndex(ElementID);
    if (aPosition != NULL)
        X = static_cast<int>(m_ElementList.GetAt(aPosition)->m_PointList.GetCount());
    else
        return;

    if (bIsPlotAvailable) {
        if (X < dAutoRangeX[MIN])
            dAutoRangeX[MIN] = X;
        if (Y < dAutoRangeY[MIN])
            dAutoRangeY[MIN] = Y;
        if (X > dAutoRangeX[MAX])
            dAutoRangeX[MAX] = X;
        if (Y > dAutoRangeY[MAX])
            dAutoRangeY[MAX] = Y;

    } else {
        double dX = static_cast<double>(X);
        dAutoRangeX[MIN] = floor(dX);
        dAutoRangeY[MIN] = floor(dX);
        dAutoRangeX[MAX] = ceil(dX);
        dAutoRangeY[MAX] = ceil(dX);
        bIsPlotAvailable = TRUE;
    }

    CElementPoint point((X++), Y);
    auto graphElement = m_ElementList.GetAt(aPosition);

    graphElement->m_PointList.AddTail(point);
    if (graphElement->min.x > point.x)
        graphElement->min.x = point.x;
    if (graphElement->min.y > point.y)
        graphElement->min.y = point.y;
    if (graphElement->max.x < point.x)
        graphElement->max.x = point.x;
    if (graphElement->max.y < point.y)
        graphElement->max.y = point.y;
    graphElement->bIsPlotAvailable = TRUE;

    InvalidateControl();
}

double CNTGraphCtrl::GetElementXValue(long index, short ElementID)
{
    POSITION aPosition = m_ElementList.FindIndex(ElementID);

    if (aPosition != NULL) {
        POSITION pos = m_ElementList.GetAt(aPosition)->m_PointList.FindIndex(static_cast<size_t>(index));
        if (pos != NULL)
            return m_ElementList.GetAt(aPosition)->m_PointList.GetAt(pos).x;
        else
            AfxMessageBox("Point not found!");
    } else
        AfxMessageBox("Element not found!");
    return 0.0;
}

void CNTGraphCtrl::SetElementXValue(long index, short ElementID, double newValue)
{
    POSITION aPosition = m_ElementList.FindIndex(ElementID);

    if (aPosition != NULL) {
        POSITION pos = m_ElementList.GetAt(aPosition)->m_PointList.FindIndex(static_cast<size_t>(index));
        if (pos != NULL)
            m_ElementList.GetAt(aPosition)->m_PointList.GetAt(pos).x = newValue;
        else
            AfxMessageBox("Point not found!");
    } else
        AfxMessageBox("Element not found!");

    InvalidateControl();
    SetModifiedFlag();
}

double CNTGraphCtrl::GetElementYValue(long index, short ElementID)
{
    POSITION aPosition = m_ElementList.FindIndex(ElementID);

    if (aPosition != NULL) {
        POSITION pos = m_ElementList.GetAt(aPosition)->m_PointList.FindIndex(static_cast<size_t>(index));
        if (pos != NULL)
            return m_ElementList.GetAt(aPosition)->m_PointList.GetAt(pos).y;
        else
            AfxMessageBox("Point not found!");
    } else
        AfxMessageBox("Element not found!");

    return 0.0;
}

void CNTGraphCtrl::SetElementYValue(long index, short ElementID, double newValue)
{
    POSITION aPosition = m_ElementList.FindIndex(ElementID);

    if (aPosition != NULL) {
        POSITION pos = m_ElementList.GetAt(aPosition)->m_PointList.FindIndex(static_cast<size_t>(index));
        if (pos != NULL)
            m_ElementList.GetAt(aPosition)->m_PointList.GetAt(pos).y = newValue;
        else
            AfxMessageBox("Point not found!");
    } else
        AfxMessageBox("Element not found!");

    InvalidateControl();
    SetModifiedFlag();
}

void CNTGraphCtrl::OnElementChanged()
{
    m_Position = m_ElementList.FindIndex(m_nElementID);

    if (m_Position == NULL && m_elementCount != 0) {
        AfxMessageBox("Element not found!");
        return;
    } else {
        const auto graphElement = m_ElementList.GetAt(m_Position);

        m_elementLineColor = graphElement->m_LineColor;
        m_elementPointColor = graphElement->m_PointColor;

        m_elementWidth = graphElement->m_nWidth;
        m_elementLinetype = graphElement->m_nType;

        m_elementPointSymbol = graphElement->m_nSymbol;
        m_elementSolidPoint = graphElement->m_bSolid;

        m_elementShow = graphElement->m_bShow;
        m_elementName = graphElement->m_strName;

        SetModifiedFlag();
    }

    SetModifiedFlag();
}

short CNTGraphCtrl::GetElementCount()
{
    return static_cast<short>(m_ElementList.GetCount());
}

void CNTGraphCtrl::OnElementLineColorChanged()
{
    m_ElementList.GetAt(m_Position)->m_LineColor = m_elementLineColor;

    m_linePen.DeleteObject();
    m_pointPen.DeleteObject();
    m_lineBrush.DeleteObject();
    m_pointBrush.DeleteObject();

    InvalidateControl();
    SetModifiedFlag();
}

void CNTGraphCtrl::OnElementPointColorChanged()
{
    m_ElementList.GetAt(m_Position)->m_PointColor = m_elementPointColor;

    m_linePen.DeleteObject();
    m_pointPen.DeleteObject();
    m_lineBrush.DeleteObject();
    m_pointBrush.DeleteObject();

    InvalidateControl();
    SetModifiedFlag();
}

void CNTGraphCtrl::OnElementLinetypeChanged()
{
    m_ElementList.GetAt(m_Position)->m_nType = m_elementLinetype;

    m_linePen.DeleteObject();
    m_pointPen.DeleteObject();
    m_lineBrush.DeleteObject();
    m_pointBrush.DeleteObject();

    InvalidateControl();
    SetModifiedFlag();
}

void CNTGraphCtrl::OnElementWidthChanged()
{

    if (m_elementWidth > 5)
        m_elementWidth = 5;
    if (m_elementWidth < 0)
        m_elementWidth = 0;

    m_ElementList.GetAt(m_Position)->m_nWidth = m_elementWidth;

    m_linePen.DeleteObject();
    m_pointPen.DeleteObject();
    m_lineBrush.DeleteObject();
    m_pointBrush.DeleteObject();

    InvalidateControl();
    SetModifiedFlag();
}

void CNTGraphCtrl::OnElementPointSymbolChanged()
{
    m_ElementList.GetAt(m_Position)->m_nSymbol = m_elementPointSymbol;

    m_linePen.DeleteObject();
    m_pointPen.DeleteObject();
    m_lineBrush.DeleteObject();
    m_pointBrush.DeleteObject();

    InvalidateControl();
    SetModifiedFlag();
}

void CNTGraphCtrl::OnElementSolidPointChanged()
{

    m_ElementList.GetAt(m_Position)->m_bSolid = m_elementSolidPoint;

    m_linePen.DeleteObject();
    m_pointPen.DeleteObject();
    m_lineBrush.DeleteObject();
    m_pointBrush.DeleteObject();

    InvalidateControl();
    SetModifiedFlag();
}

void CNTGraphCtrl::OnElementShowChanged()
{
    m_ElementList.GetAt(m_Position)->m_bShow = m_elementShow;

    m_linePen.DeleteObject();
    m_pointPen.DeleteObject();
    m_lineBrush.DeleteObject();
    m_pointBrush.DeleteObject();

    InvalidateControl();
    SetModifiedFlag();
}

void CNTGraphCtrl::OnElementNameChanged()
{

    m_ElementList.GetAt(m_Position)->m_strName = m_elementName;

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
    pDC->Polygon(points, 4);
}

void DrawAsterisk(CDC* pDC, CPoint point, int symsz)
{
    POINT points[4];

    points[0].x = point.x;
    points[0].y = point.y - symsz;
    points[1].x = point.x + symsz / 2;
    points[1].y = point.y + symsz / 2;
    points[2].x = point.x - symsz / 2;
    points[2].y = point.y + symsz;
    pDC->Polygon(points, 3);

    points[0].x = point.x - symsz;
    points[0].y = point.y - symsz / 2;
    points[1].x = point.x + symsz / 2;
    points[1].y = point.y;
    points[2].x = point.x + symsz;
    points[2].y = point.y + symsz;
    pDC->Polygon(points, 3);

    points[0].x = point.x - symsz / 2;
    points[0].y = point.y - symsz / 2;
    points[1].x = point.x + symsz;
    points[1].y = point.y - symsz / 2;
    points[2].x = point.x - symsz / 2;
    points[2].y = point.y + symsz;
    pDC->Polygon(points, 3);
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
    pDC->Polygon(points, 3);
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
    pDC->Polygon(points, 3);
}
void DrawUpTriangle(CDC* pDC, CPoint point, int symsz)
{
    POINT points[4];

    points[0].x = point.x;
    points[0].y = point.y - symsz;
    points[1].x = point.x + symsz;
    points[1].y = point.y + symsz;
    points[2].x = point.x - symsz;
    points[2].y = point.y + symsz;
    pDC->Polygon(points, 3);
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
    pDC->Polygon(points, 3);
}

void CNTGraphCtrl::SaveAs(LPCTSTR szFilename)
{
    PBITMAPINFO pbi;
    CRect rect;
    CClientDC dc(this);
    GetClientRect(&rect);
    CDC memDC;
    CBitmap bitmap;

    memDC.CreateCompatibleDC(&dc);
    bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    memDC.FillSolidRect(rect, dc.GetBkColor());
    OnDraw(&memDC, rect, rect);

    pbi = CreateBitmapInfoStruct((HBITMAP)bitmap.GetSafeHandle());

    CreateBMPFile((char*)szFilename, pbi, (HBITMAP)bitmap.GetSafeHandle(), memDC);

    memDC.SelectObject(pOldBitmap);
    bitmap.Detach();
}

PBITMAPINFO CNTGraphCtrl::CreateBitmapInfoStruct(HBITMAP hBmp)
{
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD cClrBits;

    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) {
        MessageBox("Unable to retrieve colour format, width and height of bitmap", "",
            MB_OK | MB_ICONERROR);
        return NULL;
    }

    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if (cClrBits <= 1)
        cClrBits = 1;
    else if (cClrBits <= 4)
        cClrBits = 4;
    else if (cClrBits <= 8)
        cClrBits = 8;
    else if (cClrBits <= 16)
        cClrBits = 16;
    else if (cClrBits <= 24)
        cClrBits = 24;
    else
        cClrBits = 32;

    if (cClrBits != 24)
        pbmi = (PBITMAPINFO)LocalAlloc(LPTR, offsetof(BITMAPINFO, bmiHeader) + sizeof(RGBQUAD) * (1ull << cClrBits));
    else
        pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;

    if (cClrBits < 24)
        pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

    pbmi->bmiHeader.biCompression = BI_RGB;

    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
        * pbmi->bmiHeader.biHeight;

    pbmi->bmiHeader.biClrImportant = 0;

    return pbmi;
}

void CNTGraphCtrl::CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC)
{
    HANDLE hf;
    BITMAPFILEHEADER hdr;
    PBITMAPINFOHEADER pbih;
    LPBYTE lpBits;
    DWORD dwTotal;
    DWORD cb;
    BYTE* hp;
    DWORD dwTmp;

    pbih = (PBITMAPINFOHEADER)pbi;
    lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, static_cast<size_t>(pbih->biSizeImage));

    if (!lpBits) {
        MessageBox("Unable to allocate memory for saving bitmap", "", MB_OK | MB_ICONERROR);
        return;
    }

    if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS)) {
        MessageBox("Unable to retrieve colour table when saving bitmap", "", MB_OK | MB_ICONERROR);
        return;
    }

    hf = CreateFile(pszFile,
        GENERIC_READ | GENERIC_WRITE,
        (DWORD)0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        (HANDLE)NULL);

    if (hf == INVALID_HANDLE_VALUE) {
        MessageBox("Unable to open the file when saving bitmap", "", MB_OK | MB_ICONERROR);
        return;
    }

    hdr.bfType = 0x4d42;

    hdr.bfSize = (DWORD)(offsetof(BITMAPINFO, bmiHeader) + static_cast<size_t>(pbih->biSize) + static_cast<size_t>(pbih->biClrUsed) * sizeof(RGBQUAD) + static_cast<size_t>(pbih->biSizeImage));

    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

    hdr.bfOffBits = static_cast<DWORD> (offsetof(BITMAPINFO, bmiHeader) + static_cast<size_t>(pbih->biSize) + static_cast<size_t>(pbih->biClrUsed) * sizeof(RGBQUAD));

    if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), (LPDWORD)&dwTmp, NULL)) {
        MessageBox("Unable to write file header when saving bitmap", "", MB_OK | MB_ICONERROR);
        return;
    }

    if (!WriteFile(hf, (LPVOID)pbih, static_cast<DWORD>(offsetof(BITMAPINFO, bmiHeader) + static_cast<size_t>(pbih->biClrUsed) * sizeof(RGBQUAD)),
            (LPDWORD)&dwTmp, (NULL))) {
        MessageBox("Unable to write bitmap info", "", MB_OK | MB_ICONERROR);
        return;
    }

    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;
    if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL)) {
        MessageBox("Unable to write colour indices when saving bitmap", "", MB_OK | MB_ICONERROR);
        return;
    }

    if (!CloseHandle(hf)) {
        MessageBox("Unable to close file handle when saving bitmap", "", MB_OK | MB_ICONERROR);
        return;
    }

    GlobalFree((HGLOBAL)lpBits);
}

void CNTGraphCtrl::Annotate(CDC* pDC)
{

    POSITION pos = m_AnnotationList.GetHeadPosition();
    CGraphAnnotation anno = m_AnnotationList.GetHead();

    while (pos) {
        anno = m_AnnotationList.GetNext(pos);

        if (!anno.m_bVisible)
            continue;

        pDC->SetTextColor(anno.m_Color);
        pDC->SetTextAlign(TA_LEFT | TA_BOTTOM);
        pDC->SetBkMode(TRANSPARENT);

        CFont* pOldFont;

        if (anno.m_bHorizontal)
            pOldFont = SelectFontObject(pDC, m_fontIdent);
        else
            pOldFont = pDC->SelectObject(&m_fontVLabel);

        CRect textRect;
        pDC->DrawText(anno.m_Caption, -1, textRect, DT_SINGLELINE | DT_CALCRECT);

        TRACE("Width %d Height %d \n", textRect.Width(), textRect.Height());

        CPoint point = Corrdinate(anno.place);

        CSize sz;
        if (anno.m_bHorizontal)
            sz = CSize(textRect.Width(), -textRect.Height());
        else {
            sz = CSize(-textRect.Height(), -textRect.Width());
            point.x += textRect.Height() / 2;
        }

        CRect rc(point, sz);

        if (m_annoBrush.m_hObject == NULL)
            m_annoBrush.CreateSolidBrush(anno.m_BkColor);

        CBrush* pBrushSave = pDC->SelectObject(&m_annoBrush);

        if (
            m_axisRect.PtInRect(rc.BottomRight()) && m_axisRect.PtInRect(rc.TopLeft())) {
            pDC->FillRect(rc, &m_annoBrush);
            pDC->TextOut(point.x, point.y, anno.m_Caption);
        }
        pDC->SelectObject(pOldFont);
        pDC->SelectObject(pBrushSave);
        m_annoBrush.DeleteObject();
    }
}

void CNTGraphCtrl::AddAnnotation()
{
    CGraphAnnotation anno;
    anno.m_Caption.Format("Annotation-%lld", m_AnnotationList.GetCount());

    m_AnnotationList.AddTail(anno);
    m_nAnnotation = static_cast<short>(m_AnnotationList.GetCount()) - 1;
    TRACE("m_nAnnotation = %d\n", m_nAnnotation);
    m_nAnnoCount = static_cast<short>(m_AnnotationList.GetCount());

    InvalidateControl();
}

void CNTGraphCtrl::DeleteAnnotation(short annoID)
{
    POSITION pos = m_AnnotationList.FindIndex(annoID);

    if (pos) {
        m_AnnotationList.RemoveAt(pos);
        m_nAnnotation--;
        m_nAnnoCount = static_cast<short>(m_AnnotationList.GetCount());

        InvalidateControl();
    }
}

void CNTGraphCtrl::OnAnnoLabelCaptionChanged()
{
    POSITION pos = m_AnnotationList.FindIndex(m_nAnnotation);

    if (pos) {
        m_AnnotationList.GetAt(pos).m_Caption = m_annoLabelCaption;

        TRACE("Caption : %s\n", m_annoLabelCaption);

        InvalidateControl();
        SetModifiedFlag();
    }
}

void CNTGraphCtrl::OnAnnoLabelXChanged()
{
    POSITION pos = m_AnnotationList.FindIndex(m_nAnnotation);

    if (pos) {
        m_AnnotationList.GetAt(pos).place.x = m_annoLabelX;

        TRACE("LabelX = %g\n", m_annoLabelX);

        InvalidateControl();
        SetModifiedFlag();
    }
}

void CNTGraphCtrl::OnAnnoLabelYChanged()
{
    POSITION pos = m_AnnotationList.FindIndex(m_nAnnotation);

    if (pos) {
        m_AnnotationList.GetAt(pos).place.y = m_annoLabelY;

        TRACE("LabelY = %g\n", m_annoLabelY);

        InvalidateControl();
        SetModifiedFlag();
    }
}

void CNTGraphCtrl::OnAnnoLabelColorChanged()
{
    POSITION pos = m_AnnotationList.FindIndex(m_nAnnotation);

    if (pos) {
        m_AnnotationList.GetAt(pos).m_Color = m_annoLabelColor;

        InvalidateControl();
        SetModifiedFlag();
    }
}
void CNTGraphCtrl::OnAnnoLabelBkColorChanged()
{
    POSITION pos = m_AnnotationList.FindIndex(m_nAnnotation);

    if (pos) {
        m_AnnotationList.GetAt(pos).m_BkColor = m_annoLabelBkColor;

        InvalidateControl();

        SetModifiedFlag();
    }
}

void CNTGraphCtrl::OnAnnoLabelHorizontalChanged()
{
    POSITION pos = m_AnnotationList.FindIndex(m_nAnnotation);

    if (pos) {
        m_AnnotationList.GetAt(pos).m_bHorizontal = m_annoLabelHorizontal;

        InvalidateControl();
        SetModifiedFlag();
    }
}

void CNTGraphCtrl::OnAnnotationChanged()
{
    if (m_nAnnotation > (m_AnnotationList.GetCount() - 1) || m_nAnnotation < 0)
        return;

    POSITION pos = m_AnnotationList.FindIndex(m_nAnnotation);

    if (pos) {
        const auto graphAnnotation = m_AnnotationList.GetAt(pos);
        m_annoLabelCaption = graphAnnotation.m_Caption;

        m_annoLabelX = graphAnnotation.place.x;
        m_annoLabelY = graphAnnotation.place.y;
        m_annoLabelColor = graphAnnotation.m_Color;
        m_annoLabelBkColor = graphAnnotation.m_BkColor;
        m_annoLabelHorizontal = graphAnnotation.m_bHorizontal;
        m_annoVisible = graphAnnotation.m_bVisible;

        SetModifiedFlag();
    }
}

void CNTGraphCtrl::OnAnnoVisibleChanged()
{
    POSITION pos = m_AnnotationList.FindIndex(m_nAnnotation);

    if (pos) {
        m_AnnotationList.GetAt(pos).m_bVisible = m_annoVisible;

        InvalidateControl();
        SetModifiedFlag();
    }
}

short CNTGraphCtrl::GetAnnoCount()
{
    return m_nAnnoCount;
}

void CNTGraphCtrl::DrawCursor(CDC* pDC)
{
    POSITION pos = m_CursorList.GetHeadPosition();

    CGraphCursor cursor = m_CursorList.GetHead();
    int index = 0;

    while (pos) {
        cursor = m_CursorList.GetNext(pos);

        if (!cursor.m_bVisible)
            continue;

        if (m_cursorPen.m_hObject == NULL)
            m_cursorPen.CreatePen(PS_SOLID, 0, cursor.m_Color);

        CPen* pPenSave = pDC->SelectObject(&m_cursorPen);

        if (cursor.m_nStyle == Crosshair::XY || cursor.m_nStyle == Crosshair::X) {
            pDC->MoveTo(Corrdinate(dRangeX[MIN], cursor.position.y));
            pDC->LineTo(Corrdinate(dRangeX[MAX], cursor.position.y));
        }

        if (cursor.m_nStyle == Crosshair::XY || cursor.m_nStyle == Crosshair::Y) {
            pDC->MoveTo(Corrdinate(cursor.position.x, dRangeY[MIN]));
            pDC->LineTo(Corrdinate(cursor.position.x, dRangeY[MAX]));
        }

        if (!IsOptimizedDraw())
            pDC->SelectObject(pPenSave);

        m_cursorPen.DeleteObject();
        index++;
    }
}

void CNTGraphCtrl::CursorPosition(CPoint point)
{
    double rx, ry;

    rx = PT2DBLX(point.x);
    ry = PT2DBLY(point.y);

    m_cursorX = rx;
    m_cursorY = ry;

    POSITION pos = m_CursorList.FindIndex(m_nCursorID);

    if (m_axisRect.PtInRect(point) && pos) {
        auto graphCursor = m_CursorList.GetAt(pos);
        if (static_cast<int>(graphCursor.m_nMode) > 0) {
            if (graphCursor.m_nMode == CursorMode::Snap && m_elementCount > 0) {
                auto pt = FindPoint(rx, ry);
                rx = pt.x;
                ry = pt.y;
            }
            graphCursor.position.x = rx;
            graphCursor.position.y = ry;
            InvalidateControl(m_axisRect);
        }
    }
    FireCursorPosition(rx, ry);
}

CElementPoint CNTGraphCtrl::FindPoint(double cursor_x, double cursor_y)
{
    CGraphElement* pElement = m_ElementList.GetAt(m_Position);

    int i = 0, index = 0;

    POSITION pos = pElement->m_PointList.GetHeadPosition();

    CElementPoint point = pElement->m_PointList.GetHead();

    double dx = fabs(cursor_x - point.x);
    double dy = fabs(cursor_y - point.y);
    double dr = sqrt(dx + dy);

    while (pos) {
        point = pElement->m_PointList.GetNext(pos);

        dx = fabs(cursor_x - point.x);
        dy = fabs(cursor_y - point.y);

        if (sqrt(dx + dy) < dr) {
            dr = sqrt(dx + dy);
            index = i;
        }
        i++;
    }

    pos = pElement->m_PointList.FindIndex(static_cast<size_t>(index));

    return pElement->m_PointList.GetAt(pos);
}

void CNTGraphCtrl::OnCursorColorChanged()
{
    POSITION pos = m_CursorList.FindIndex(m_nCursorID);

    if (pos) {
        m_CursorList.GetAt(pos).m_Color = m_cursorColor;

        InvalidateControl();
        SetModifiedFlag();
    }
}

void CNTGraphCtrl::AddCursor()
{
    CGraphCursor cursor;

    m_CursorList.AddTail(cursor);
    m_nCursorID = static_cast<short>(m_CursorList.GetCount() - 1);
    m_nCursorCount = static_cast<short>(m_CursorList.GetCount());

    InvalidateControl();
}

void CNTGraphCtrl::DeleteCursor(short cursorID)
{
    POSITION pos = m_CursorList.FindIndex(cursorID);

    if (pos) {
        m_CursorList.RemoveAt(pos);
        m_nCursorID--;
        m_nCursorCount = static_cast<short>(m_CursorList.GetCount());

        InvalidateControl();
    }
}

void CNTGraphCtrl::OnCursorChanged()
{
    if (m_nCursorID > (m_CursorList.GetCount() - 1) || m_nCursorID < 0)
        return;

    POSITION pos = m_CursorList.FindIndex(m_nCursorID);

    if (pos) {
        const auto graphCursor = m_CursorList.GetAt(pos);
        m_cursorX = graphCursor.position.x;
        m_cursorY = graphCursor.position.y;
        m_cursorColor = graphCursor.m_Color;
        m_cursorVisible = graphCursor.m_bVisible;
        m_cursorMode = static_cast<short>(graphCursor.m_nMode);
        m_cursorStyle = static_cast<short>(graphCursor.m_nStyle);
        SetModifiedFlag();
    }
}

void CNTGraphCtrl::OnCursorXChanged()
{
    POSITION pos = m_CursorList.FindIndex(m_nCursorID);
    if (pos) {
        m_CursorList.GetAt(pos).position.x = m_cursorX;

        InvalidateControl(m_axisRect);
        SetModifiedFlag();
    }
}

void CNTGraphCtrl::OnCursorYChanged()
{
    POSITION pos = m_CursorList.FindIndex(m_nCursorID);

    if (pos) {
        m_CursorList.GetAt(pos).position.y = m_cursorY;

        InvalidateControl(m_axisRect);
        SetModifiedFlag();
    }
}

void CNTGraphCtrl::OnCursorStyleChanged()
{
    POSITION pos = m_CursorList.FindIndex(m_nCursorID);

    if (pos) {
        m_CursorList.GetAt(pos).m_nStyle = (Crosshair)m_cursorStyle;

        InvalidateControl(m_axisRect);
        SetModifiedFlag();
    }
}

void CNTGraphCtrl::OnCursorVisibleChanged()
{
    POSITION pos = m_CursorList.FindIndex(m_nCursorID);

    if (pos) {
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

    if (pos) {
        m_CursorList.GetAt(pos).m_nMode = (CursorMode)m_cursorMode;

        if (m_CursorList.GetAt(pos).m_nMode == CursorMode::Snap && m_elementCount > 0) {
            CElementPoint pt;
            pt = FindPoint(m_cursorX, m_cursorY);
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

    if (m_xLog)
        result.Format("1E%d", ROUND(data));
    else if (m_xTime) {
        COleDateTime t((DATE)data);
        result = t.Format(m_formatAxisBottom);
    } else {
        sprintf(format, "%s", (LPCTSTR)m_formatAxisBottom);
        sprintf(buffer, format, data);
        result = buffer;
    }

    return result;
}

CString CNTGraphCtrl::FormatYAxisLabel(double data)
{
    CString result;
    char format[200];
    char buffer[200];

    if (m_yLog)
        result.Format("1E%d", ROUND(data));
    else if (m_yTime) {
        COleDateTime t((DATE)data);
        result = t.Format(m_formatAxisLeft);
    } else {
        sprintf(format, "%s", (LPCTSTR)m_formatAxisLeft);
        sprintf(buffer, format, data);
        result = buffer;
    }

    return result;
}
