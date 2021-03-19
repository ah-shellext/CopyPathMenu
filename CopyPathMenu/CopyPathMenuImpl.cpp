#include "pch.h"
#include "CopyPathMenuImpl.h"

HRESULT STDMETHODCALLTYPE CCopyPathMenuImpl::Initialize(
    __in_opt PCIDLIST_ABSOLUTE pidlFolder,
    __in_opt IDataObject *pdtobj,
    __in_opt HKEY hkeyProgID
) {
    this->curr_folder = pidlFolder;
    this->sel_object = pdtobj;
    this->sel_count = 0;

    FORMATETC fe = { CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM stm = {};
    if (pdtobj != nullptr && SUCCEEDED(pdtobj->GetData(&fe, &stm))) {
        auto hDrop = static_cast<HDROP>(GlobalLock(stm.hGlobal));
        if (hDrop != nullptr) {
            this->sel_count = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);
        }
    }

    return S_OK;
}

// Naming
// N: No quote; Q: quote
// BCK: back slash; FNT: front slash; DBL: double bask slash; NME: name only
const UINT MENUID_RESTART_EXPLORER = 0;
const UINT MENUID_N_BCK = 1;
const UINT MENUID_Q_BCK = 2;
const UINT MENUID_N_FNT = 3;
const UINT MENUID_Q_FNT = 4;
const UINT MENUID_N_DBL = 5;
const UINT MENUID_Q_DBL = 6;
const UINT MENUID_N_NME = 7;
const UINT MENUID_Q_NME = 8;
const UINT MENUID_N_BCK_COMMA = 9;
const UINT MENUID_Q_BCK_COMMA = 10;
const UINT MENUID_N_FNT_COMMA = 11;
const UINT MENUID_Q_FNT_COMMA = 12;
const UINT MENUID_N_DBL_COMMA = 13;
const UINT MENUID_Q_DBL_COMMA = 14;
const UINT MENUID_N_NME_COMMA = 15;
const UINT MENUID_Q_NME_COMMA = 16;

/**
 * @brief Represents a menu action.
 */
struct MenuAction {
    LPWSTR title;
    UINT id;
    UINT pos;
};

HRESULT STDMETHODCALLTYPE CCopyPathMenuImpl::QueryContextMenu(
    __in HMENU hmenu,
    __in UINT indexMenu,
    __in UINT idCmdFirst,
    __in UINT idCmdLast,
    __in UINT uFlags
) {
    if (uFlags & CMF_VERBSONLY) {
        return S_OK;
    }
    if (uFlags & CMF_DEFAULTONLY) {
        return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
    }
    if (this->curr_folder == nullptr && this->sel_object == nullptr) {
        return S_FALSE;
    }

    // menu actions
    std::vector<MenuAction> actions;
    if (this->sel_count <= 1) { // directory background or single file
        actions = {
            {L"引用符なし バックスラッシュ (\\)", MENUID_N_BCK}, // |\|
            {L"引用符付き バックスラッシュ (\"\\\")", MENUID_Q_BCK}, // |"\"|
            {L"引用符なし フロントスラッシュ (/)", MENUID_N_FNT}, // |/|
            {L"引用符付き フロントスラッシュ (\"/\")", MENUID_Q_FNT}, // |"/"|
            {L"引用符なし ダブルバックスラッシュ (\\\\)", MENUID_N_DBL}, // |\\|
            {L"引用符付き ダブルバックスラッシュ (\"\\\\\")", MENUID_Q_DBL}, // |"\\"|
            {L"", 0}, // splitter
            {L"引用符なし 名前のみ (x)", MENUID_N_NME}, // |x|
            {L"引用符付き 名前のみ (\"x\")", MENUID_Q_NME}, // |"x"|
        };
    } else { // multiple files
        actions = {
            {L"引用符なし バックスラッシュ セミコロン (\\;\\)", MENUID_N_BCK}, // |\|
            {L"引用符付き バックスラッシュ セミコロン (\"\\\";\"\\\")", MENUID_Q_BCK}, // |"\"|
            {L"引用符なし フロントスラッシュ セミコロン (/;/)", MENUID_N_FNT}, // |/|
            {L"引用符付き フロントスラッシュ セミコロン (\"/\";\"/\")", MENUID_Q_FNT}, // |"/"|
            {L"引用符なし ダブルバックスラッシュ セミコロン (\\\\;\\\\)", MENUID_N_DBL}, // |\\|
            {L"引用符付き ダブルバックスラッシュ セミコロン (\"\\\\\";\"\\\\\")", MENUID_Q_DBL}, // |"\\"|
            {L"", 0}, // splitter
            {L"引用符なし 名前のみ セミコロン (;)", MENUID_N_NME}, // |x|
            {L"引用符付き 名前のみ セミコロン (;)", MENUID_Q_NME}, // |"x"|
            {L"", 0}, // splitter
            {L"引用符なし バックスラッシュ コンマ (\\,\\)", MENUID_N_BCK_COMMA},  // |\|
            {L"引用符付き バックスラッシュ コンマ (\"\\\",\"\\\")", MENUID_Q_BCK_COMMA}, // |"\"|
            {L"引用符なし フロントスラッシュ コンマ (/,/)", MENUID_N_FNT_COMMA}, // |/|
            {L"引用符付き フロントスラッシュ コンマ (\"/\",\"/\")", MENUID_Q_FNT_COMMA}, // |"/"|
            {L"引用符なし ダブルバックスラッシュ コンマ (\\\\,\\\\)", MENUID_N_DBL_COMMA}, // |\\|
            {L"引用符付き ダブルバックスラッシュ コンマ (\"\\\\\",\"\\\\\")", MENUID_Q_DBL_COMMA}, // |"\\"|
            {L"", 0}, // splitter
            {L"引用符なし 名前のみ コンマ (,)", MENUID_N_NME_COMMA,17}, // |x|
            {L"引用符付き 名前のみ コンマ (,)", MENUID_Q_NME_COMMA, 18}, // |"x"|
        };
    }

    // copy sub menu
    HMENU copy_menu = CreateMenu();

    // temp mii
    MENUITEMINFO mii;
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STRING | MIIM_ID;

    // splitter mii
    MENUITEMINFO mii_splitter;
    mii_splitter.cbSize = sizeof(MENUITEMINFO);
    mii_splitter.fMask = MIIM_FTYPE;
    mii_splitter.fType = MFT_SEPARATOR;

    // command count
    int cmd_count = 0;

    // insert to sub menu
    for (int i = 0; i < actions.size(); i++) {
        auto action = actions.at(i);
        if (action.id == 0) {
            InsertMenuItem(copy_menu, i, true, &mii_splitter);
        } else {
            mii.dwTypeData = action.title;
            mii.wID = idCmdFirst + action.id;
            InsertMenuItem(copy_menu, i, true, &mii);
            cmd_count++;
        }
    }

    // path copy menu
    MENUITEMINFO mii_copy;
    mii_copy.cbSize = sizeof(MENUITEMINFO);
    mii_copy.fMask = MIIM_STRING | MIIM_SUBMENU;
    mii_copy.dwTypeData = L"パスのコピー(&P)";
    mii_copy.hSubMenu = copy_menu;

    // restart explorer
    MENUITEMINFO mii_re;
    mii_re.cbSize = sizeof(MENUITEMINFO);
    mii_re.fMask = MIIM_STRING | MIIM_ID;
    mii_re.dwTypeData = L"エクスプローラーを再起動(&R)";
    mii_re.wID = idCmdFirst + MENUID_RESTART_EXPLORER;

    // insert to hmenu
    if (this->sel_count == 0) { // folder background
        cmd_count++;
        InsertMenuItem(hmenu, indexMenu + 0, true, &mii_splitter);
        InsertMenuItem(hmenu, indexMenu + 1, true, &mii_copy);
        InsertMenuItem(hmenu, indexMenu + 2, true, &mii_re);
        InsertMenuItem(hmenu, indexMenu + 3, true, &mii_splitter);
    } else {
        InsertMenuItem(hmenu, indexMenu + 0, true, &mii_splitter);
        InsertMenuItem(hmenu, indexMenu + 1, true, &mii_copy);
        InsertMenuItem(hmenu, indexMenu + 2, true, &mii_splitter);
    }

    // make hResult
    return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, (USHORT) cmd_count);
}

HRESULT STDMETHODCALLTYPE CCopyPathMenuImpl::InvokeCommand(
    __in CMINVOKECOMMANDINFO *pici
) {
    UINT idCmd = LOWORD(pici->lpVerb);
    if (HIWORD(pici->lpVerb) != 0) {
        return E_INVALIDARG;
    }
    if (this->curr_folder == nullptr && this->sel_object == nullptr) {
        return S_FALSE;
    }

    // ================
    // restart explorer
    // ================

    if (idCmd == MENUID_RESTART_EXPLORER) {
        system("taskkill /f /im explorer.exe && start explorer.exe");
        return S_OK;
    }

    // ==============
    // other commands
    // ==============

    // get selected filename
    std::vector<std::wstring> sel_filenames;
    if (this->sel_count == 0) { // directory background
        std::wstring curr_path;
        bool ok = Utils::GetFolderNameFromItemIDList(this->curr_folder, &curr_path);
        if (!ok) {
            MessageBox(nullptr, L"フォルダーパスの取得にエラーが発生しました。", L"エラー", MB_OK);
            return S_FALSE;
        }
        sel_filenames.emplace_back(curr_path);
    } else { // files
        bool ok = Utils::GetFilenamesFromDataObject(this->sel_object, &sel_filenames);
        if (!ok) {
            MessageBox(nullptr, L"ファイルパスの取得にエラーが発生しました。", L"エラー", MB_OK);
            return S_FALSE;
        }
    }

    // get result string
    std::wstring res;
    switch (idCmd) {
    case MENUID_N_BCK:
        res = Utils::JoinFilenames(sel_filenames, false, false, false);
        break;
    case MENUID_Q_BCK:
        res = Utils::JoinFilenames(sel_filenames, true, false, false);
        break;
    case MENUID_N_FNT:
        res = Utils::JoinFilenames(sel_filenames, false, false, false);
        Utils::ReplaceWstring(&res, L"\\", L"/");
        break;
    case MENUID_Q_FNT:
        res = Utils::JoinFilenames(sel_filenames, true, false, false);
        Utils::ReplaceWstring(&res, L"\\", L"/");
        break;
    case MENUID_N_DBL:
        res = Utils::JoinFilenames(sel_filenames, false, false, false);
        Utils::ReplaceWstring(&res, L"\\", L"\\\\");
        break;
    case MENUID_Q_DBL:
        res = Utils::JoinFilenames(sel_filenames, true, false, false);
        Utils::ReplaceWstring(&res, L"\\", L"\\\\");
        break;
    case MENUID_N_NME:
        res = Utils::JoinFilenames(sel_filenames, false, true, false);
        break;
    case MENUID_Q_NME:
        res = Utils::JoinFilenames(sel_filenames, true, true, false);
        break;
    case MENUID_N_BCK_COMMA:
        res = Utils::JoinFilenames(sel_filenames, false, false, true);
        break;
    case MENUID_Q_BCK_COMMA:
        res = Utils::JoinFilenames(sel_filenames, true, false, true);
        break;
    case MENUID_N_FNT_COMMA:
        res = Utils::JoinFilenames(sel_filenames, false, false, true);
        Utils::ReplaceWstring(&res, L"\\", L"/");
        break;
    case MENUID_Q_FNT_COMMA:
        res = Utils::JoinFilenames(sel_filenames, true, false, true);
        Utils::ReplaceWstring(&res, L"\\", L"/");
        break;
    case MENUID_N_DBL_COMMA:
        res = Utils::JoinFilenames(sel_filenames, false, false, true);
        Utils::ReplaceWstring(&res, L"\\", L"\\\\");
        break;
    case MENUID_Q_DBL_COMMA:
        res = Utils::JoinFilenames(sel_filenames, true, false, true);
        Utils::ReplaceWstring(&res, L"\\", L"\\\\");
        break;
    case MENUID_N_NME_COMMA:
        res = Utils::JoinFilenames(sel_filenames, false, true, true);
        break;
    case MENUID_Q_NME_COMMA:
        res = Utils::JoinFilenames(sel_filenames, true, true, true);
        break;
    default:
        return S_FALSE;
    }

    // copy to clipboard
    auto ok = Utils::CopyToClipboard(res);
    if (!ok) {
        return S_FALSE;
    }

    return S_OK;
}
