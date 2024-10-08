#pragma once

// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CPdfviewerctrl1 wrapper class

class CPdfviewerctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CPdfviewerctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xD4F80D00, 0x5F46, 0x4902, { 0x8A, 0xFC, 0xD0, 0x2, 0x69, 0x32, 0x3, 0x39 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// Attributes
public:


// Operations
public:

// _DPDFViewer

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	short LoadPDFFile(LPCTSTR strPDFFile)
	{
		short result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I2, (void*)&result, parms, strPDFFile);
		return result;
	}
	void GoToPage(short iPage)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, iPage);
	}
	void GoToNextPage()
	{
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	short SearchPrevText()
	{
		short result;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	short SearchNextText()
	{
		short result;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	void ShowOriginalSize()
	{
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	short Search(LPCTSTR strSearchText)
	{
		short result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_I2, (void*)&result, parms, strSearchText);
		return result;
	}
	short SaveBitmaps(LPCTSTR strSaveFileName)
	{
		short result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I2, (void*)&result, parms, strSaveFileName);
		return result;
	}
	void PDFZoomIn()
	{
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void PDFZoomOut()
	{
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void PDFPrint()
	{
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void GoToPrevPage()
	{
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void PDFClose()
	{
		InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//

short GetTotalPage()
{
	short result;
	GetProperty(0xc, VT_I2, (void*)&result);
	return result;
}
void SetTotalPage(short propVal)
{
	SetProperty(0xc, VT_I2, propVal);
}
unsigned long GetBackgroudColor()
{
	unsigned long result;
	GetProperty(0x10, VT_UI4, (void*)&result);
	return result;
}
void SetBackgroudColor(unsigned long propVal)
{
	SetProperty(0x10, VT_UI4, propVal);
}
CString GetLicenseKey()
{
	CString result;
	GetProperty(0x13, VT_BSTR, (void*)&result);
	return result;
}
void SetLicenseKey(CString propVal)
{
	SetProperty(0x13, VT_BSTR, propVal);
}
short GetPDFPageNo()
{
	short result;
	GetProperty(0x15, VT_I2, (void*)&result);

	return result;
}
void SetPDFPageNo(short propVal)
{
	SetProperty(0x15, VT_I2, propVal);
}
float GetRotateAngle()
{
	float result;
	GetProperty(0x16, VT_R4, (void*)&result);
	return result;
}
void SetRotateAngle(float propVal)
{
	SetProperty(0x16, VT_R4, propVal);
}
CString GetPassword()
{
	CString result;
	GetProperty(0x17, VT_BSTR, (void*)&result);
	return result;
}
void SetPassword(CString propVal)
{
	SetProperty(0x17, VT_BSTR, propVal);
}


};
