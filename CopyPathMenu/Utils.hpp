#pragma once

__pragma(warning(push)) __pragma(warning(disable:4996))

#include <shlobj.h>
#include <shobjidl.h>
#include <atlconv.h>

#include <vector>
#include <string>

class Utils {
public:

    /**
     * @breif Represent the max path length, that is 2048B.
     */
    static const UINT MAX_PATH_LENGTH = 2048;

    /**
     * @brief Get folder name from given folder, that is LPCITEMIDLIST.
     */
    static bool GetFolderNameFromItemIDList(LPCITEMIDLIST pidlFolder, std::wstring *out) {
        auto current_path = new WCHAR[MAX_PATH_LENGTH];
        bool ok = SHGetPathFromIDListW(pidlFolder, current_path);
        if (ok) {
            *out = current_path;
        }
        return ok;
    }

    /**
     * @brief Get filenames from given selected files, that is IDataObject.
     */
    static bool GetFilenamesFromDataObject(IDataObject *pdtobj, std::vector<std::wstring> *out) {
        FORMATETC fe = { CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
        STGMEDIUM stm = {};
        if (pdtobj == nullptr || !SUCCEEDED(pdtobj->GetData(&fe, &stm))) {
            return false;
        }
        auto hDrop = static_cast<HDROP>(GlobalLock(stm.hGlobal));
        if (hDrop == nullptr) {
            return false;
        }

        out->clear();
        auto str = new WCHAR[MAX_PATH_LENGTH];
        UINT cnt = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);
        for (UINT i = 0; i < cnt; i++) {
            memset(str, 0, MAX_PATH_LENGTH);
            DragQueryFile(hDrop, i, str, MAX_PATH_LENGTH);
            std::wstring name = str;
            if (name.empty()) {
                continue;
            }
            out->push_back(name);
        }
        delete[] str;
        GlobalUnlock(stm.hGlobal);

        return true;
    }

    /**
     * @brief Copy given wstring to clipboard.
     */
    static bool CopyToClipboard(const std::wstring &str) {
        if (!OpenClipboard(nullptr)) {
            return false;
        }
        if (!EmptyClipboard()) {
            return false;
        }

        HGLOBAL buf = GlobalAlloc(GHND | GMEM_SHARE, (str.length() + 1) * sizeof(WCHAR));
        if (buf == 0) {
            return false;
        }
        auto str_mem = (LPWSTR) GlobalLock(buf);
        if (str_mem == 0) {
            return false;
        }
        wcscpy(str_mem, str.data());
        SetClipboardData(CF_UNICODETEXT, buf);

        GlobalUnlock(buf);
        CloseClipboard();
        return true;
    }

    /**
     * @brief Replace given src all to dst from wstring.
     */
    static void ReplaceWstring(std::wstring *str, const std::wstring &src, const std::wstring &dst) {
        int pos = 0;
        size_t srclen = src.size();
        size_t dstlen = dst.size();

        while ((pos = str->find(src, pos)) != std::string::npos) {
            str->replace(pos, srclen, dst);
            pos += dstlen;
        }
    }

    /**
     * @brief Split given wstring using delim to vector of wstring.
     */
    static std::vector<std::wstring> SplitWstring(const std::wstring &str, const std::wstring &delim) {
        std::vector<std::wstring> result;
        size_t len = wcslen(str.data());

        if (str.data() && len) {
            auto src = new WCHAR[len + 1];
            wcscpy(src, str.data());
            src[len] = '\0';
            auto tokenptr = wcstok(src, delim.data());
            while (tokenptr != nullptr) {
                std::wstring tk = tokenptr;
                result.emplace_back(tk);
                tokenptr = wcstok(nullptr, delim.data());
            }
            delete[] src;
        }
        return result;
    }

    /**
     * @brief Join given filenames and options to result wstring.
     */
    static std::wstring JoinFilenames(std::vector<std::wstring> filenames, bool use_quote, bool only_name, bool use_comma) {
        auto func = [only_name, use_quote](std::wstring name) -> std::wstring {
            std::wstring res = L"";
            if (only_name) {
                auto sp = Utils::SplitWstring(name, L"\\");
                if (!sp.empty()) {
                    res = sp.at(sp.size() - 1);
                }
            } else {
                res = name;
            }
            if (use_quote) {
                res = L"\"" + res + L"\"";
            }
            return res;
        };

        std::wstring join = L";";
        if (use_comma) {
            join = L",";
        }

        std::wstring out = func(filenames.at(0));
        for (auto iter = std::next(filenames.begin()); iter != filenames.end(); iter++) {
            out += join + func(*iter);
        }
        return out;
    }
};

__pragma(warning(pop))
