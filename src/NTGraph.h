#if !defined(AFX_NTGRAPH_H__49D9E193_23FE_44DF_B0D0_9B05A7580DCB__INCLUDED_)
#define AFX_NTGRAPH_H__49D9E193_23FE_44DF_B0D0_9B05A7580DCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// NTGraph.h : main header file for NTGRAPH.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CNTGraphApp : See NTGraph.cpp for implementation.

class CNTGraphApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NTGRAPH_H__49D9E193_23FE_44DF_B0D0_9B05A7580DCB__INCLUDED)
