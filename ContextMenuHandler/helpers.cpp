#include "helpers.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

static const WCHAR BACKSLASH{ '\\' };
static const WCHAR QUOTE{ '\"' };

///////////////////////////////////////////////////////////////////////////////////////////////////

bool IsEqualClassId(REFCLSID clsid1, LPCWSTR clsid2)
{
	CLSID clsid;

	if (CLSIDFromString(clsid2, &clsid) == NOERROR)
	{
		return !memcmp(&clsid1, &clsid, sizeof(CLSID));
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool RegKeyExists(HKEY idKey, LPCWSTR pSubKey)
{
	HKEY hKey;

	if (RegOpenKeyExW(idKey, pSubKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool RegValueExists(HKEY idKey, LPCWSTR pSubKey, LPCWSTR pValName)
{
	return RegGetValueW(idKey, pSubKey, pValName, RRF_RT_ANY, nullptr, nullptr, nullptr) == ERROR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool RegSetData(HKEY idKey, LPCWSTR pSubKey, LPCWSTR pValName, LPCWSTR pValData)
{
	size_t cbData{ (wcslen(pValData) + 1) * sizeof(WCHAR) };
	return RegSetKeyValueW(idKey, pSubKey, pValName, REG_SZ, pValData, static_cast<DWORD>(cbData)) == ERROR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

typedef DWORD ARGB;

HBITMAP BitmapFromIcon(HICON hIcon)
{
	HBITMAP hBitmap{ nullptr };

	IWICImagingFactory* pFactory;
	if (SUCCEEDED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory))))
	{
		IWICBitmap* pBitmap;
		if (SUCCEEDED(pFactory->CreateBitmapFromHICON(hIcon, &pBitmap)))
		{
			IWICFormatConverter* pConverter;
			if (SUCCEEDED(pFactory->CreateFormatConverter(&pConverter)))
			{
				if (SUCCEEDED(pConverter->Initialize(pBitmap, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom)))
				{
					UINT cx, cy;
					if (SUCCEEDED(pConverter->GetSize(&cx, &cy)))
					{
						HDC hDC{ GetDC(nullptr) };
						if (hDC != nullptr)
						{
							LPBYTE pBuffer;
							BITMAPINFO bmi{};
							bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
							bmi.bmiHeader.biPlanes = 1;
							bmi.bmiHeader.biCompression = BI_RGB;
							bmi.bmiHeader.biWidth = static_cast<LONG>(cx);
							bmi.bmiHeader.biHeight = -static_cast<LONG>(cy);
							bmi.bmiHeader.biBitCount = 32;
							hBitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, reinterpret_cast<LPVOID*>(&pBuffer), nullptr, 0);
							if (hBitmap != nullptr)
							{
								UINT cbStride{ cx * sizeof(ARGB) };
								UINT cbBuffer{ cy * cbStride };
								if (FAILED(pConverter->CopyPixels(nullptr, cbStride, cbBuffer, pBuffer)))
								{
									hBitmap = nullptr;
								}
							}
							ReleaseDC(nullptr, hDC);
						}
					}
				}
				pConverter->Release();
			}
			pBitmap->Release();
		}
		pFactory->Release();
	}

	return hBitmap;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

HBITMAP BitmapMenuFromIcon(HMODULE hModule, WORD idIcon)
{
	HICON hIcon{ static_cast<HICON>(LoadImageW(hModule, reinterpret_cast<LPCWSTR>(idIcon), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR)) };

	if (hIcon != nullptr)
	{
		HBITMAP hBitmap{ BitmapFromIcon(hIcon) };
		DestroyIcon(hIcon);
		return hBitmap;
	}

	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool MenuInsertItem(HMENU hMenu, UINT nItemPos, LPCWSTR pMenuText, UINT idCmd, UINT fState, HMENU hSubMenu, HBITMAP hBmpItem)
{
	MENUITEMINFOW mii{ sizeof(MENUITEMINFOW) };

	if (pMenuText != nullptr)
	{
		mii.fMask = MIIM_STRING | MIIM_STATE | MIIM_ID;
		mii.dwTypeData = const_cast<LPWSTR>(pMenuText);
		mii.fState = fState;
		mii.wID = idCmd;

		if (hSubMenu != nullptr)
		{
			mii.fMask |= MIIM_SUBMENU;
			mii.hSubMenu = hSubMenu;
		}

		if (hBmpItem != nullptr)
		{
			mii.fMask |= MIIM_BITMAP;
			mii.hbmpItem = hBmpItem;
		}
	}
	else
	{
		mii.fMask = MIIM_FTYPE;
		mii.fType = MFT_SEPARATOR;
	}

	return InsertMenuItemW(hMenu, nItemPos, TRUE, &mii) != FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool MutexExists(LPCWSTR name)
{
	HANDLE mutex{ OpenMutexW(SYNCHRONIZE, FALSE, name) };

	if (mutex != nullptr)
	{
		CloseHandle(mutex);
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::wstring GetRenamedFilePath(const std::wstring& filePath, const std::wstring& newFileName)
{
	size_t pos{ filePath.find_last_of(BACKSLASH) + 1 };
	return filePath.substr(0, pos) + newFileName;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::wstring GetEscapedArgument(std::wstring argument)
{
	if (!argument.empty())
	{
		if (argument.find_first_of(L" \"\t") == argument.npos)
		{
			return argument;
		}

		argument.reserve(argument.length() * 2);
		size_t index{ argument.length() - 1 };
		bool quote{ true };

		while (index != argument.npos)
		{
			if (quote && argument[index] == BACKSLASH)
			{
				argument.insert(index, 1, BACKSLASH);
			}
			else if (argument[index] == QUOTE)
			{
				argument.insert(index, 1, BACKSLASH);
				quote = true;
			}
			else if (quote == true)
			{
				quote = false;
			}

			--index;
		}
	}

	return QUOTE + argument + QUOTE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::wstring GetAppDataFilePath(const std::wstring& folderName, const std::wstring& fileName)
{
	std::wstring filePath{};
	LPWSTR appDataPath;

	if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DEFAULT, nullptr, &appDataPath) == S_OK)
	{
		filePath = appDataPath;
		filePath += BACKSLASH + folderName + BACKSLASH + fileName;
		CoTaskMemFree(appDataPath);
	}

	return filePath;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UINT GetDataCount(LPDATAOBJECT pDataObject)
{
	UINT count{ 0 };
	FORMATETC fetc{ CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stgm;

	if (pDataObject->GetData(&fetc, &stgm) == S_OK)
	{
		count = DragQueryFileW(static_cast<HDROP>(stgm.hGlobal), 0xFFFFFFFF, nullptr, 0);
		ReleaseStgMedium(&stgm);
	}

	return count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::wstring GetDataItem(LPDATAOBJECT pDataObject)
{
	WCHAR item[MAX_PATH]{};
	FORMATETC fetc{ CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stgm;

	if (pDataObject->GetData(&fetc, &stgm) == S_OK)
	{
		DragQueryFileW(static_cast<HDROP>(stgm.hGlobal), 0, item, _countof(item));
		ReleaseStgMedium(&stgm);
	}

	return item;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::wstring> GetDataItems(LPDATAOBJECT pDataObject)
{
	std::vector<std::wstring> items{};
	FORMATETC fetc{ CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stgm;

	if (pDataObject->GetData(&fetc, &stgm) == S_OK)
	{
		UINT index{ 0 };
		WCHAR item[MAX_PATH];
		HDROP hdrop{ static_cast<HDROP>(stgm.hGlobal) };

		while (DragQueryFileW(hdrop, index++, item, _countof(item)) != 0)
		{
			items.push_back(item);
		}

		ReleaseStgMedium(&stgm);
	}

	return items;
}
