#pragma once

#include <atlconv.h>
#include <string>
#include <vector>

class Utils {
public:
    static std::wstring s2ws(const std::string &s) {
        int slength = (int) s.length() + 1;
        int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, nullptr, 0);
        auto buf = new wchar_t[len];
        MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
        std::wstring r(buf);
        delete[] buf;
        return r;
    }

    static void wstring_replace(std::wstring &str, const std::wstring &src, const std::wstring &dst) {
        int pos = 0;
        size_t srclen = src.size();
        size_t dstlen = dst.size();

        while ((pos = str.find(src, pos)) != std::string::npos) {
            str.replace(pos, srclen, dst);
            pos += dstlen;
        }
    }

    static bool wstring_endsWith(const std::wstring &str, const std::wstring &sub) {
        return str.rfind(sub) == (str.length() - sub.length());
    }

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
};
