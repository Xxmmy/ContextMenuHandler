#pragma once

#include "targetver.h"
#include <wtypesbase.h>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////////

class Global
{
public:
	Global();
	ULONG ModRefCount;
	const HMODULE ModHandle;
	const std::wstring ModPath;
	const std::wstring ProdName;
	const std::wstring ProdClassId;
	const std::wstring ProdMenuItem;
	const std::vector<std::wstring> ProdClassRoots;
	const std::vector<std::wstring> ProdSubMenuItems;
	const std::wstring ProdMutexName;
	const WORD ProdIconId;
	const bool ProdMultiSelect;
};
