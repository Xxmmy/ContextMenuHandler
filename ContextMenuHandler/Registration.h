#pragma once

#include <olectl.h>
#include <strsafe.h>
#include <vector>
#include <memory>

///////////////////////////////////////////////////////////////////////////////////////////////////

class Registration
{
public:
	Registration(const std::vector<std::wstring>& prodHkcr, const std::wstring& prodClsid, const std::wstring& prodName, const std::wstring& modPath);

	HRESULT Register();
	HRESULT Unregister();

private:
	enum class UNREG_
	{
		DO_NOTHING,
		DELETE_KEY,
		DELETE_VAL
	};

	struct REG_ITEM_
	{
		UNREG_ UnregType;
		HKEY PredefKey;
		std::wstring SubKeyFormat;
		std::wstring SubKeyData1;
		std::wstring SubKeyData2;
		std::wstring ValueName;
		std::wstring ValueData;
	};

	std::vector<REG_ITEM_> regList_{};
	std::unique_ptr<WCHAR[]> subKeyBuffer_;
	size_t subKeySize_;

	size_t GetSubKeySize_();
};
