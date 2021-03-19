// dllmain.h : Declaration of module class.

class CCopyPathMenuModule : public ATL::CAtlDllModuleT< CCopyPathMenuModule >
{
public :
	DECLARE_LIBID(LIBID_CopyPathMenuLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_COPYPATHMENU, "{bc4b0323-3734-441f-8a5a-aeb4d4dd51d8}")
};

extern class CCopyPathMenuModule _AtlModule;
