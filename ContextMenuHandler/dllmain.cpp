#include "Global.h"
#include "ClassFactory.h"
#include "Registration.h"
#include "helpers.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

static Global global{};

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOL APIENTRY DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
	UNREFERENCED_PARAMETER(lpvReserved);

	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		WCHAR path[MAX_PATH];
		DWORD size{ GetModuleFileNameW(hinstDLL, path, _countof(path)) };
		if (size == 0 || size == _countof(path))
		{
			return FALSE;
		}

		const_cast<std::wstring&>(global.ModPath) = path;
		const_cast<HMODULE&>(global.ModHandle) = hinstDLL;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

__control_entrypoint(DllExport)
STDAPI DllCanUnloadNow()
{
	return global.ModRefCount == 0 ? S_OK : S_FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

_Check_return_
STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID FAR* ppv)
{
	if (ppv == nullptr)
	{
		return E_POINTER;
	}
	else if (!IsEqualClassId(rclsid, global.ProdClassId.c_str()))
	{
		*ppv = nullptr;
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	ClassFactory* cf{ new (std::nothrow) ClassFactory(global) };
	if (cf == nullptr)
	{
		*ppv = nullptr;
		return E_OUTOFMEMORY;
	}

	HRESULT hr{ cf->QueryInterface(riid, ppv) };
	cf->Release();

	return hr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

__control_entrypoint(DllExport)
STDAPI DllRegisterServer()
{
	Registration reg{ global.ProdClassRoots, global.ProdClassId, global.ProdName, global.ModPath };
	return reg.Register();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

__control_entrypoint(DllExport)
STDAPI DllUnregisterServer()
{
	Registration reg{ global.ProdClassRoots, global.ProdClassId, global.ProdName, global.ModPath };
	return reg.Unregister();
}
