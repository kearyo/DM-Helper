// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#define WINVER 0x0600

#define NO_WARN_MBCS_MFC_DEPRECATION TRUE

#if !defined(AFX_STDAFX_H__4FB782FA_3217_4CF2_9FC1_BAAB14031BE1__INCLUDED_)
#define AFX_STDAFX_H__4FB782FA_3217_4CF2_9FC1_BAAB14031BE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers


#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>
#include <afxmt.h>

#include <afxsock.h>		// MFC socket extensions

#include <Unknwn.h> 
#include <windows.h>
#include <objidl.h>
#define ULONG_PTR ULONG
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#include <gdiplusbitmap.h>
#include "ImageEx.h"


#include <vector>

#include "DM Helper Common.h"
#include "ColoredListCtrl.h"
#include "TransparentListBox.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4FB782FA_3217_4CF2_9FC1_BAAB14031BE1__INCLUDED_)
#include <afxcontrolbars.h>
#include <afxdisp.h>
#include <afxdisp.h>
#include <afxdisp.h>
//#include <afxcontrolbars.h>
// #include <afxcontrolbars.h>
