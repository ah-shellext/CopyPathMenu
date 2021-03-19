#include "pch.h"
#include "CopyPathMenuImpl.h"

HRESULT STDMETHODCALLTYPE CCopyPathMenuImpl::Initialize(
    __in_opt PCIDLIST_ABSOLUTE pidlFolder,
    __in_opt IDataObject *pdtobj,
    __in_opt HKEY hkeyProgID
) {
    this->curr_folder = pidlFolder;
    this->sel_object = pdtobj;

    return S_OK;
}
HRESULT STDMETHODCALLTYPE CCopyPathMenuImpl::QueryContextMenu(
    __in HMENU hmenu,
    __in UINT indexMenu,
    __in UINT idCmdFirst,
    __in UINT idCmdLast,
    __in UINT uFlags
) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CCopyPathMenuImpl::InvokeCommand(
    __in CMINVOKECOMMANDINFO *pici
) {
    return S_OK;
}
