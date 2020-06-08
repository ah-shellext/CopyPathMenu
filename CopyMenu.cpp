#include "stdafx.h"
#include "CopyMenu.h"
#include "Utils.hpp"

#include <atlconv.h>
#include <string>

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

const UINT MAX_PATH_LENGTH = 2048;

HRESULT STDMETHODCALLTYPE CCopyMenu::Initialize( 
    /* [annotation][unique][in] */ 
    __in_opt  PCIDLIST_ABSOLUTE pidlFolder,
    /* [annotation][unique][in] */ 
    __in_opt  IDataObject *pdtobj,
    /* [annotation][unique][in] */ 
    __in_opt  HKEY hkeyProgID
) {
    this->pdtobj = pdtobj;
    return S_OK;
}

const UINT MENU_ID_PREFIX = 0;

const UINT MENU_ID_CPY          = MENU_ID_PREFIX + 0;
const UINT MENU_ID_CPY_QTE      = MENU_ID_PREFIX + 1;
const UINT MENU_ID_CPY_FNT      = MENU_ID_PREFIX + 2;
const UINT MENU_ID_CPY_FNT_QTE  = MENU_ID_PREFIX + 3;
const UINT MENU_ID_CPY_DBL      = MENU_ID_PREFIX + 4;
const UINT MENU_ID_CPY_DBL_QTE  = MENU_ID_PREFIX + 5;
const UINT MENU_ID_CPY_NAME     = MENU_ID_PREFIX + 6;

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
    __in  UINT uFlags
) {

    if (uFlags & CMF_VERBSONLY)
        return S_OK;

    if (uFlags & CMF_DEFAULTONLY)
        return MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_NULL, 0);

    HMENU subMenu = CreateMenu();

    MENUITEMINFO outMenu;

    outMenu.cbSize = sizeof(MENUITEMINFO);
    outMenu.fMask = MIIM_STRING | MIIM_SUBMENU;

    outMenu.dwTypeData = _T("パスのコピー");
    outMenu.hSubMenu = subMenu;

    MENUITEMINFO cpy_menu_split;
    MENUITEMINFO cpy_menuItem;              // no quotation             F:\Projects\CopyShell\.gitignore
    MENUITEMINFO cpy_qte_menuItem;          // quotation                "F:\Projects\CopyShell\.gitignore"
    MENUITEMINFO cpy_fnt_menuItem;          // front + no quotation     F:/Projects/CopyShell/.gitignore
    MENUITEMINFO cpy_fnt_qte_menuItem;      // front + quotation        "F:/Projects/CopyShell/.gitignore"
    MENUITEMINFO cpy_dbl_menuItem;          // double + no quotation    F:\\Projects\\CopyShell\\.gitignore
    MENUITEMINFO cpy_dbl_qte_menuItem;      // double + quotation       "F:\\Projects\\CopyShell\\.gitignore"
    MENUITEMINFO cpy_name_menuItem;         // filename                 .gitignore

    cpy_menu_split.cbSize = sizeof(MENUITEMINFO);
    cpy_menu_split.fMask = MIIM_FTYPE;
    cpy_menu_split.fType = MFT_SEPARATOR;

    cpy_menuItem.cbSize = sizeof(MENUITEMINFO);
    cpy_menuItem.fMask = MIIM_STRING | MIIM_ID;
    cpy_menuItem.dwTypeData = _T("引用記号抜き (バックスラッシュ \\)"); // \ 
    cpy_menuItem.wID = idCmdFirst + MENU_ID_CPY;

    cpy_qte_menuItem.cbSize = sizeof(MENUITEMINFO);
    cpy_qte_menuItem.fMask = MIIM_STRING | MIIM_ID;
    cpy_qte_menuItem.dwTypeData = _T("引用記号付き (バックスラッシュ \"\\\")"); // "\"
    cpy_qte_menuItem.wID = idCmdFirst + MENU_ID_CPY_QTE;

    cpy_fnt_menuItem.cbSize = sizeof(MENUITEMINFO);
    cpy_fnt_menuItem.fMask = MIIM_STRING | MIIM_ID;
    cpy_fnt_menuItem.dwTypeData = _T("引用記号抜き (フロントラッシュ /)"); // /
    cpy_fnt_menuItem.wID = idCmdFirst + MENU_ID_CPY_FNT;

    cpy_fnt_qte_menuItem.cbSize = sizeof(MENUITEMINFO);
    cpy_fnt_qte_menuItem.fMask = MIIM_STRING | MIIM_ID;
    cpy_fnt_qte_menuItem.dwTypeData = _T("引用記号付き (フロントラッシュ \"/\")"); // "/"
    cpy_fnt_qte_menuItem.wID = idCmdFirst + MENU_ID_CPY_FNT_QTE;

    cpy_dbl_menuItem.cbSize = sizeof(MENUITEMINFO);
    cpy_dbl_menuItem.fMask = MIIM_STRING | MIIM_ID;
    cpy_dbl_menuItem.dwTypeData = _T("引用記号抜き (ダブルバックスラッシュ \\\\)"); // \\ 
    cpy_dbl_menuItem.wID = idCmdFirst + MENU_ID_CPY_DBL;

    cpy_dbl_qte_menuItem.cbSize = sizeof(MENUITEMINFO);
    cpy_dbl_qte_menuItem.fMask = MIIM_STRING | MIIM_ID;
    cpy_dbl_qte_menuItem.dwTypeData = _T("引用記号付き (ダブルバックスラッシュ \"\\\\\")"); // "\\"
    cpy_dbl_qte_menuItem.wID = idCmdFirst + MENU_ID_CPY_DBL_QTE;

    cpy_name_menuItem.cbSize = sizeof(MENUITEMINFO);
    cpy_name_menuItem.fMask = MIIM_STRING | MIIM_ID;
    cpy_name_menuItem.dwTypeData = _T("名前のみ");
    cpy_name_menuItem.wID = idCmdFirst + MENU_ID_CPY_NAME;

    InsertMenu(hmenu, indexMenu, MF_SEPARATOR | MF_BYPOSITION, 0, nullptr);
    InsertMenuItem(hmenu, indexMenu + 1, true, &outMenu);
    InsertMenu(hmenu, indexMenu + 2, MF_SEPARATOR | MF_BYPOSITION, 0, nullptr);

    InsertMenuItem(subMenu, 0, true, &cpy_menuItem);
    InsertMenuItem(subMenu, 1, true, &cpy_qte_menuItem);
    InsertMenuItem(subMenu, 2, true, &cpy_fnt_menuItem);
    InsertMenuItem(subMenu, 3, true, &cpy_fnt_qte_menuItem);
    InsertMenuItem(subMenu, 4, true, &cpy_dbl_menuItem);
    InsertMenuItem(subMenu, 5, true, &cpy_dbl_qte_menuItem);

    InsertMenuItem(subMenu, 6, true, &cpy_menu_split);
    InsertMenuItem(subMenu, 7, true, &cpy_name_menuItem);

    return MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_NULL, 8);
}

HRESULT STDMETHODCALLTYPE CCopyMenu::InvokeCommand( 
    /* [annotation][in] */ 
    __in  CMINVOKECOMMANDINFO *pici
) {
    UINT idCmd = LOWORD(pici->lpVerb);
    if (HIWORD(pici->lpVerb) != 0)
        return E_INVALIDARG;

    // Get Filenames
    std::vector<std::wstring> sel_files;
    FORMATETC fe = { CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM stm = {};
    bool ok = false;

    if (SUCCEEDED(pdtobj->GetData(&fe, &stm))) {
        HDROP hDrop = static_cast<HDROP>(GlobalLock(stm.hGlobal));
        if (hDrop != nullptr) {
            UINT cnt = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);
            if (cnt != 0) {

                LPWSTR str = new WCHAR[2048];

                for (UINT i = 0; i < cnt; i++) {
                    try {
                        memset(str, 0, MAX_PATH_LENGTH);
                        int size = DragQueryFile(hDrop, i, str, MAX_PATH_LENGTH);
                        if (size == 0) continue;

                        std::wstring name = str;
                        // std::string name = ATL::CW2A(str);
                        if (name.empty()) continue;
                        // MessageBox(NULL, name.c_str(), _T("MyShell"), MB_OK);
                        sel_files.push_back(name);
                        ok = true;
                    }
                    catch (...) { }
                }
                delete []str;
                GlobalUnlock(stm.hGlobal);
            }
            ReleaseStgMedium(&stm);
        }
    }
    
    if (!ok) {
        MessageBox(nullptr, _T("ファイルの名前のロードはエラーが発生しました。"), _T("エラー"), MB_OK);
        return S_OK;
    }

    // Cat Filenames
    std::wstring name = sel_files.at(0);

    for (auto iter = sel_files.begin() + 1; iter != sel_files.end(); iter++)
        name += _T(";") + *iter;

    switch (idCmd) {

    case MENU_ID_CPY: // 引用記号抜き \ 
        // MessageBox(NULL, s2ws(std::to_string((long long) sel_files.size())).c_str(), _T("MyShell"), MB_OK);
        break;

    case MENU_ID_CPY_QTE: // 引用記号付き "\"
        name = _T("\"") + name + _T("\"");
        break;

    case MENU_ID_CPY_FNT: // 引用記号抜き /
        Utils::wstring_replace(name, _T("\\"), _T("/"));
        break;

    case MENU_ID_CPY_FNT_QTE: // 引用記号付き "/"
        name = _T("\"") + name + _T("\"");
        Utils::wstring_replace(name, _T("\\"), _T("/"));
        break;

    case MENU_ID_CPY_DBL: // 引用記号抜き \ 
        Utils::wstring_replace(name, _T("\\"), _T("\\\\"));
        break;

    case MENU_ID_CPY_DBL_QTE: // 引用記号付き "\\"
        name = _T("\"") + name + _T("\"");
        Utils::wstring_replace(name, _T("\\"), _T("\\\\"));
        break;

    case MENU_ID_CPY_NAME: // 名前のみ
        std::vector<std::wstring> sp = Utils::wstring_split(name, _T(";"));
        std::vector<std::wstring> namesp = Utils::wstring_split(sp.at(0), _T("\\"));
        std::wstring ret = namesp.at(namesp.size() - 1);
        for (auto iter = sp.begin() + 1; iter != sp.end(); iter++) {
            namesp = Utils::wstring_split(*iter, _T("\\"));
            ret.append(_T(";") + namesp.at(namesp.size() - 1));
        }
        name = ret;
        break;
    }
     
    // Copy Filenames
    if (OpenClipboard(nullptr)) {
        EmptyClipboard();
        HGLOBAL buf = GlobalAlloc(GHND | GMEM_SHARE, (name.length() + 1) * sizeof(WCHAR));
        auto strMem = (LPWSTR) GlobalLock(buf);
        wcscpy(strMem, name.data());
        GlobalUnlock(buf);

        SetClipboardData(CF_UNICODETEXT, buf);
        CloseClipboard();
    }

    return E_INVALIDARG;
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
    __in  UINT cchMax
) {
    return S_OK;
}
