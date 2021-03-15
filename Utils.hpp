#pragma once

#include <atlconv.h>
#include <string>
#include <vector>

class Utils {
public:
    // static std::wstring s2ws(const std::string &s) {
    //     int slength = (int) s.length() + 1;
    //     int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, nullptr, 0);
    //     auto buf = new wchar_t[len];
    //     MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    //     std::wstring r(buf);
    //     delete[] buf;
    //     return r;
    // }
    //
    // static bool wstring_endsWith(const std::wstring &str, const std::wstring &sub) {
    //     return str.rfind(sub) == (str.length() - sub.length());
    // }

    // Replace wstring.
    static void wstring_replace(std::wstring &str, const std::wstring &src, const std::wstring &dst) {
        int pos = 0;
        size_t srclen = src.size();
        size_t dstlen = dst.size();

        while ((pos = str.find(src, pos)) != std::string::npos) {
            str.replace(pos, srclen, dst);
            pos += dstlen;
        }
    }

    // Split wstring.
    static std::vector<std::wstring> wstring_split(const std::wstring &str, const std::wstring &delim) {
        std::vector<std::wstring> result;
        size_t len = wcslen(str.data());
        if (str.data() && len) {
            auto *src = new WCHAR[len + 1];
            wcscpy(src, str.data());
            src[len] = '\0';
            WCHAR *tokenptr = wcstok(src, delim.data());
            while (tokenptr != nullptr) {
                std::wstring tk = tokenptr;
                result.emplace_back(tk);
                tokenptr = wcstok(nullptr, delim.data());
            }
            delete[] src;
        }
        return result;
    }

    // Join filenames.
    static std::wstring join_filenames(std::vector<std::wstring> vec, bool quote, bool only_name, bool use_comma) {
        auto func = [&](std::wstring name) -> std::wstring {
            if (only_name) {
                auto sp = Utils::wstring_split(name, _T("\\"));
                if (!sp.empty()) {
                    return sp.at(sp.size() - 1);
                }
            }
            return name;
        };
        std::wstring out = func(vec.at(0));
        std::wstring join = _T(";");
        if (use_comma) {
            join = _T(",");
        }
        if (quote) {
            out = _T("\"") + out + _T("\"");
        }
        for (auto iter = vec.begin() + 1; iter != vec.end(); iter++) {
            if (quote) {
                out += join + _T("\"") + func(*iter) + _T("\"");
            } else {
                out += join + func(*iter);
            }
        }
        return out;
    }

    static const UINT MAX_PATH_LENGTH = 2048;

    // Get filenames from IDataObject *.
    static std::vector<std::wstring> get_filenames_from_data_object(IDataObject *pdtobj, bool *ok) {
        std::vector<std::wstring> sel_files;
        *ok = true;

        FORMATETC fe = { CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
        STGMEDIUM stm = {};
        if (SUCCEEDED(pdtobj->GetData(&fe, &stm))) {
            auto hDrop = static_cast<HDROP>(GlobalLock(stm.hGlobal));
            if (hDrop == nullptr) {
                *ok = false;
                ReleaseStgMedium(&stm);
                return sel_files;
            }

            UINT cnt = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);
            auto str = new WCHAR[MAX_PATH_LENGTH];
            try {
                for (UINT i = 0; i < cnt; i++) {
                    memset(str, 0, MAX_PATH_LENGTH);
                    int size = DragQueryFile(hDrop, i, str, MAX_PATH_LENGTH);
                    if (size == 0) {
                        continue;
                    }
                    std::wstring name = str; // WCHAR * -> wstr
                    if (name.empty()) {
                        continue;
                    }
                    sel_files.push_back(name);
                }
            } catch (...) {
                *ok = false;
            }
            delete[] str;
            GlobalUnlock(stm.hGlobal);
        }

        ReleaseStgMedium(&stm);
        return sel_files;
    }
};
