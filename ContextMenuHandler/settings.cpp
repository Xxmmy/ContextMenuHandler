#include "Global.h"
#include "ContextMenuHandler.h"
#include "helpers.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// This is where you make sure the selection(s) is what you want in case the class root
// alone is not enough. Return true if the selection(s) is what you want or false otherwise.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool ContextMenuHandler::Initialize_(PCIDLIST_ABSOLUTE pidlFolder, LPDATAOBJECT pdtobj, HKEY hkeyProgID)
{
	UNREFERENCED_PARAMETER(pidlFolder);
	UNREFERENCED_PARAMETER(pdtobj);
	UNREFERENCED_PARAMETER(hkeyProgID);

	//std::wstring filePath{ GetDataItem(pdtobj) };
	//// if fileExtension == ".dat" then return true else return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// This is where you can customize the menu in case the mutex was found or submenu items are
// set at runtime instead of using a list in settings.h. You can use GetAppDataFilePath or
// GetRenamedFilePath from helpers functions to get a file path in %APPDATA% or in the same
// directory as this shell extension. Return a std::pair<bool, ULONG> where the first part
// (boolean) specify that the second part (ULONG) should be returned and skip the automatic
// context menu creation. The ULONG is the number of item(s) added to the context menu. To
// make sure the automatic context menu is created, return std::make_pair(false, 0).
//
///////////////////////////////////////////////////////////////////////////////////////////////////

std::pair<bool, ULONG> ContextMenuHandler::QueryContextMenu_(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast)
{
	UNREFERENCED_PARAMETER(hmenu);
	UNREFERENCED_PARAMETER(indexMenu);
	UNREFERENCED_PARAMETER(idCmdFirst);
	UNREFERENCED_PARAMETER(idCmdLast);

	//if (MutexExists(global_.ProdMutexName.c_str()))
	//{
	//	std::wstring menuText{ global_.ProdMenuItem + L" (already running)" };
	//	ULONG itemCount{ MenuInsertItem(hmenu, indexMenu, menuText.c_str(), idCmdFirst, MFS_DISABLED, nullptr, hBitmapMenu_) ? 1UL : 0UL };
	//	return std::make_pair(true, itemCount);
	//}

	//std::vector<std::wstring>& items{ const_cast<std::vector<std::wstring>&>(global_.ProdSubMenuItems) };
	//std::wstring subMenuPath{ GetAppDataFilePath(L"FolderName", L"FileName.xml") };
	//std::wstring subMenuPath{ GetRenamedFilePath(global_.ModPath, L"FileName.xml") };
	//items.push_back(subMenuItem);

	return std::make_pair(false, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// This is where you do something with the selection(s). Return the HRESULT S_OK for success
// or E_FAIL otherwise. That being said, at this point, the return value have no consequence
// on this shell extension.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

HRESULT ContextMenuHandler::InvokeCommand_(LPCMINVOKECOMMANDINFO pici)
{
	UNREFERENCED_PARAMETER(pici);

	//UINT indexMenu{ HIWORD(pici->lpVerb) };
	//std::wstring menuItem{ global_.ProdSubMenuItems[indexMenu] };

	//std::wstring shellItem{ GetDataItem(pDataObject_) };

	//std::vector<std::wstring> shellItems{ GetDataItems(pDataObject_) };

	//// https://docs.microsoft.com/en-us/windows/desktop/shell/datascenarios
	//if (SUCCEEDED(OleSetClipboard(pDataObject_)))
	//{
	//	std::wstring arguments{ L"-s:" + GetEscapedArgument(menuItem) };
	//	std::wstring exePath{ GetRenamedFilePath(global_.ModPath, L"Program.exe") };
	//	ShellExecuteW(nullptr, nullptr, exePath.c_str(), arguments.c_str(), nullptr, SW_SHOW);
	//}

	return S_OK;
}
