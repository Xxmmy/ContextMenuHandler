#include "Registration.h"
#include "helpers.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

static LPCWSTR EMPTY{ L"" };
static LPCWSTR REG_FMT_CLSID{ L"CLSID\\%s" };
static LPCWSTR REG_FMT_INPROC32{ L"CLSID\\%s\\InprocServer32" };
static LPCWSTR REG_FMT_APPROVED{ L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved" };
static LPCWSTR REG_FMT_CLASSROOT{ L"%s\\ShellEx\\ContextMenuHandlers\\%s" };
static LPCWSTR REG_THREADINGMODEL{ L"ThreadingModel" };
static LPCWSTR REG_APARTMENT{ L"Apartment" };

///////////////////////////////////////////////////////////////////////////////////////////////////

Registration::Registration(const std::vector<std::wstring>& prodHkcr, const std::wstring& prodClsid, const std::wstring& prodName, const std::wstring& modPath)
{
	regList_.push_back({ UNREG_::DELETE_KEY, HKEY_CLASSES_ROOT, REG_FMT_CLSID, prodClsid, EMPTY, EMPTY, prodName });
	regList_.push_back({ UNREG_::DO_NOTHING, HKEY_CLASSES_ROOT, REG_FMT_INPROC32, prodClsid, EMPTY, EMPTY, modPath });
	regList_.push_back({ UNREG_::DO_NOTHING, HKEY_CLASSES_ROOT, REG_FMT_INPROC32, prodClsid, EMPTY, REG_THREADINGMODEL, REG_APARTMENT });
	regList_.push_back({ UNREG_::DELETE_VAL, HKEY_LOCAL_MACHINE, REG_FMT_APPROVED, EMPTY, EMPTY, prodClsid, prodName });

	for (const auto& hkcr : prodHkcr)
	{
		regList_.push_back({ UNREG_::DELETE_KEY, HKEY_CLASSES_ROOT, REG_FMT_CLASSROOT, hkcr, prodName, EMPTY, prodClsid });
	}

	subKeySize_ = GetSubKeySize_();
	subKeyBuffer_ = std::make_unique<WCHAR[]>(subKeySize_);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

HRESULT Registration::Register()
{
	HRESULT hr{ S_OK };

	for (const auto& regItem : regList_)
	{
		if (StringCchPrintfW(subKeyBuffer_.get(), subKeySize_, regItem.SubKeyFormat.c_str(), regItem.SubKeyData1.c_str(), regItem.SubKeyData2.c_str()) != S_OK)
		{
			Unregister();
			hr = SELFREG_E_CLASS;
			break;
		}
		else if (RegSetData(regItem.PredefKey, subKeyBuffer_.get(), regItem.ValueName.c_str(), regItem.ValueData.c_str()) == false)
		{
			Unregister();
			hr = SELFREG_E_CLASS;
			break;
		}
	}

	return hr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

HRESULT Registration::Unregister()
{
	HRESULT hr{ S_OK };

	for (const auto& regItem : regList_)
	{
		if (regItem.UnregType != UNREG_::DO_NOTHING)
		{
			if (StringCchPrintfW(subKeyBuffer_.get(), subKeySize_, regItem.SubKeyFormat.c_str(), regItem.SubKeyData1.c_str(), regItem.SubKeyData2.c_str()) != S_OK)
			{
				hr = SELFREG_E_CLASS;
			}
			else if (regItem.UnregType == UNREG_::DELETE_KEY)
			{
				RegDeleteTreeW(regItem.PredefKey, subKeyBuffer_.get());
				if (RegKeyExists(regItem.PredefKey, subKeyBuffer_.get()))
				{
					hr = SELFREG_E_CLASS;
				}
			}
			else if (regItem.UnregType == UNREG_::DELETE_VAL)
			{
				RegDeleteKeyValueW(regItem.PredefKey, subKeyBuffer_.get(), regItem.ValueName.c_str());
				if (RegValueExists(regItem.PredefKey, subKeyBuffer_.get(), regItem.ValueName.c_str()))
				{
					hr = SELFREG_E_CLASS;
				}
			}
		}
	}

	return hr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

size_t Registration::GetSubKeySize_()
{
	size_t size{ 0 };

	for (const auto& regItem : regList_)
	{
		auto sz{ regItem.SubKeyFormat.size() + 1 };
		sz += regItem.SubKeyData1.size();
		sz += regItem.SubKeyData2.size();
		if (sz > size)
		{
			size = sz;
		}
	}

	return size;
}
