// CopyMenu.cpp : CCopyMenu 的实现

#include "stdafx.h"
#include "CopyMenu.h"


// CCopyMenu

HRESULT STDMETHODCALLTYPE CCopyMenu::Initialize( 
	/* [annotation][unique][in] */ 
	__in_opt  PCIDLIST_ABSOLUTE pidlFolder,
	/* [annotation][unique][in] */ 
	__in_opt  IDataObject *pdtobj,
	/* [annotation][unique][in] */ 
	__in_opt  HKEY hkeyProgID)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CCopyMenu::QueryContextMenu( 
	/* [annotation][in] */ 
	__in  HMENU hmenu,
	/* [annotation][in] */ 
	__in  UINT indexMenu,
	/* [annotation][in] */ 
	__in  UINT idCmdFirst,
	/* [annotation][in] */ 
	__in  UINT idCmdLast,
	/* [annotation][in] */ 
	__in  UINT uFlags)
{
		return S_OK;
}

HRESULT STDMETHODCALLTYPE CCopyMenu::InvokeCommand( 
	/* [annotation][in] */ 
	__in  CMINVOKECOMMANDINFO *pici)
{
		return S_OK;
}

HRESULT STDMETHODCALLTYPE CCopyMenu::GetCommandString( 
	/* [annotation][in] */ 
	__in  UINT_PTR idCmd,
	/* [annotation][in] */ 
	__in  UINT uType,
	/* [annotation][in] */ 
	__reserved  UINT *pReserved,
	/* [annotation][out] */ 
	__out_awcount(!(uType & GCS_UNICODE), cchMax)  LPSTR pszName,
	/* [annotation][in] */ 
	__in  UINT cchMax)
{
		return S_OK;
}