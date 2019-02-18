#pragma once

#include <shlobj.h>

///////////////////////////////////////////////////////////////////////////////////////////////////

class ContextMenuHandler : public IShellExtInit, public IContextMenu
{
public:
	ContextMenuHandler(Global& global);
	~ContextMenuHandler();

	IFACEMETHODIMP QueryInterface(REFIID riid, LPVOID* ppvObject);
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();

	IFACEMETHODIMP Initialize(PCIDLIST_ABSOLUTE pidlFolder, LPDATAOBJECT pdtobj, HKEY hkeyProgID);

	IFACEMETHODIMP QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
	IFACEMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO pici);
	IFACEMETHODIMP GetCommandString(UINT_PTR idCmd, UINT uType, UINT* pReserved, LPSTR pszName, UINT cchMax);

private:
	Global& global_;
	ULONG refCount_{ 1 };
	HBITMAP hBitmapMenu_{ nullptr };
	LPDATAOBJECT pDataObject_{ nullptr };

	bool Initialize_(PCIDLIST_ABSOLUTE pidlFolder, LPDATAOBJECT pdtobj, HKEY hkeyProgID);
	std::pair<bool, ULONG> QueryContextMenu_(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast);
	HRESULT InvokeCommand_(LPCMINVOKECOMMANDINFO pici);
};
