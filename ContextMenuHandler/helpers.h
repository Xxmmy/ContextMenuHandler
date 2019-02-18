#pragma once

#include <wincodec.h>
#include <shlobj_core.h>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////////

// clsid1 : CLSID 1.
// clsid2 : CLSID 2 as string including brackets {AAAAAAAA-BBBB-CCCC-DDDD-EEEEEEEEEEEE}.
// Return : True if the both CLSID are equal.
bool IsEqualClassId(REFCLSID clsid1, LPCWSTR clsid2);

// idKey : Registry predefined key like HKEY_CLASSES_ROOT or HKEY_LOCAL_MACHINE.
// pSubKey : Registry subkey relative to the predefined key (idKey).
// Return : True if the key exist.
bool RegKeyExists(HKEY idKey, LPCWSTR pSubKey);

// idKey : Registry predefined key like HKEY_CLASSES_ROOT or HKEY_LOCAL_MACHINE.
// pSubKey : Registry subkey relative to the predefined key (idKey).
// pValName : Name of the value.
// Return : True if the value exit.
bool RegValueExists(HKEY idKey, LPCWSTR pSubKey, LPCWSTR pValName);

// idKey : Registry predefined key like HKEY_CLASSES_ROOT or HKEY_LOCAL_MACHINE.
// pSubKey : Registry subkey relative to the predefined key (idKey).
// pValName : Name of the value to set.
// pValData : Value data to be set.
// Return : True if successful.
bool RegSetData(HKEY idKey, LPCWSTR pSubKey, LPCWSTR pValName, LPCWSTR pValData);

// http://msdn.microsoft.com/en-us/library/bb757020.aspx
// hIcon : Handle of the icon.
// Return : Handle of the newly created bitmap if successful or nullptr otherwise (call DeleteObject on this handle).
HBITMAP BitmapFromIcon(HICON hIcon);

// hModule : Handle of the module that contains the icon to be loaded.
// idIcon : Resource id of the icon to be loaded.
// Return : Handle of the newly created bitmap (size 16x16) if successful or nullptr otherwise (call DeleteObject on this handle).
HBITMAP BitmapMenuFromIcon(HMODULE hModule, WORD idIcon);

// hMenu : Handle of the menu in which the new item is inserted.
// nItemPos : Position of the menu item before which to insert the new item.
// pMenuText : Menu item text or nullptr to insert a separator (subsequent parameters are useless).
// idCmd : Application-defined value that identifies the menu item.
// fState : Menu item state.
// hSubMenu : Handle of the submenu associated with the menu item.
// hBmpItem : Handle of the bitmap to be displayed.
// Return : True if successful.
bool MenuInsertItem(HMENU hMenu, UINT nItemPos, LPCWSTR pMenuText, UINT idCmd, UINT fState, HMENU hSubMenu, HBITMAP hBmpItem);

// name : Name of the mutex.
// Returns : True if the mutex exist.
bool MutexExists(LPCWSTR name);

// filePath : Path of the file to be renamed.
// newFileName : New file name.
// Return : String containing the renamed file path.
std::wstring GetRenamedFilePath(const std::wstring& filePath, const std::wstring& newFileName);

// https://docs.microsoft.com/en-us/cpp/cpp/parsing-cpp-command-line-arguments
// argument : Argument to be processed.
// Return : String containing the escaped argument.
std::wstring GetEscapedArgument(std::wstring argument);

// folderName : Folder name in %APPDATA% (does not need to exist).
// fileName : File name to get a path in %APPDATA%\folderName (does not need to exist).
// Return : File path if successful or an empty string otherwise.
std::wstring GetAppDataFilePath(const std::wstring& folderName, const std::wstring& fileName);

// pDataObject : Pointer to a data object interface.
// Return : Count of items in the data object if successful or zero otherwise.
UINT GetDataCount(LPDATAOBJECT pDataObject);

// pDataObject : Pointer to a data object interface.
// Return : First item in the data object if successful or an empty string otherwise.
std::wstring GetDataItem(LPDATAOBJECT pDataObject);

// pDataObject : Pointer to a data object interface.
// Return : All the items in the data object if successful or an empty container otherwise.
std::vector<std::wstring> GetDataItems(LPDATAOBJECT pDataObject);
