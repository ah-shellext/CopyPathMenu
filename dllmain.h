// dllmain.h : 模块类的声明。

class CCopyShellModule : 
    public ATL::CAtlDllModuleT<CCopyShellModule> {
public:
    DECLARE_LIBID(LIBID_CopyShellLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_COPYSHELL, "{22DDD61F-8A9B-45E0-95BB-64A810308877}")
};

extern class CCopyShellModule _AtlModule;
