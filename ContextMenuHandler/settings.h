#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Macros.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#define SW_(sa) L##sa
#define SW(sa) SW_(sa)

#define VN(major,minor,build,revision) major,minor,build,revision

#define VA_(v) #v
#define VA(major,minor,build,revision) VA_(major.minor.build.revision)

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Product and file version (resource file).
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#define VERSION_MAJOR				1
#define VERSION_MINOR				2
#define VERSION_BUILD				3
#define VERSION_REVISION			4

#define PRODUCT_VERSION_N			VN(VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD,VERSION_REVISION)
#define PRODUCT_VERSION_A			VA(VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD,VERSION_REVISION)

#define FILEVERSION_N				VN(VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD,VERSION_REVISION)

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Product name (resource file).
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#define PRODUCT_NAME_A				"Context Menu Handler"
#define PRODUCT_NAME_W				SW(PRODUCT_NAME_A)

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Product description (resource file).
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#define PRODUCT_DESCRIPTION_A		"Context Menu Handler Shell Extension"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Product copyright (resource file).
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#define PRODUCT_COPYRIGHT_A			"Copyright (c) 2019 Your Name"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Product icon (resource file). This will add an icon to menu item(s). These values can be
// commented if an icon is not needed. This shell extension doesn't handle the case where
// you may need different icons for different menu items.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#define PRODUCT_ICON_A				"icon.ico"
#define PRODUCT_ICON_N				65535

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// true  : Show this shell extension on multiple selection in Windows File Explorer.
// false : Show this shell extension on single selection only.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#define PRODUCT_MULTISELECT_B		false

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Where the shell extension will be registered (comma separated).
// Below are some examples but there are more possibilities (ShellEx\ContextMenuHandlers).
//
// *                    : All files
// AllFileSystemObjects : All files and file folders
// Directory            : File folders
// Drive                : All drives
// Folder               : All folders
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#define PRODUCT_HKCR_W				L"*", L"Directory", L"Drive"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Class ID of the shell extension. Make sure to create a new one for each extension.
// Visual Studio Menu -> Tools -> Create GUID -> Option 4.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#define PRODUCT_CLSID_W				L"{AAAAAAAA-BBBB-CCCC-DDDD-EEEEEEEEEEEE}"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Menu item added to the context menu -> L"Menu Item"
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#define PRODUCT_MENU_W				PRODUCT_NAME_W

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Submenu item(s) (comma separated). This value can be commented if there is no need for
// submenus or you want to add your own custom values at runtime. In this case you will
// need to edit the ContextMenuHandler::QueryContextMenu_ method in settings.cpp.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#define PRODUCT_SUBMENU_W			L"SubMenu 1", L"SubMenu 2", L"SubMenu 3", L"SubMenu 4"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Mutex name of an external program. This value can be commented if you don't need this
// feature. This can be used to know if a program is already running (see MutexExists in
// helpers.h). If the mutex exists, you can customize the menu display by editing the
// ContextMenuHandler::QueryContextMenu_ method in settings.cpp.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#define PRODUCT_MUTEX_W				L"Local\\{BBBBBBBB-CCCC-DDDD-EEEE-FFFFFFFFFFFF}"
