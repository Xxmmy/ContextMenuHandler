#include "Global.h"
#include "ContextMenuHandler.h"
#include "helpers.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

ContextMenuHandler::ContextMenuHandler(Global& global) : global_{ global }
{
	++global_.ModRefCount;

	if (global_.ProdIconId != 0)
	{
		hBitmapMenu_ = BitmapMenuFromIcon(global_.ModHandle, global_.ProdIconId);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ContextMenuHandler::~ContextMenuHandler()
{
	if (pDataObject_ != nullptr)
	{
		pDataObject_->Release();
	}

	if (hBitmapMenu_ != nullptr)
	{
		DeleteObject(hBitmapMenu_);
	}

	--global_.ModRefCount;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP ContextMenuHandler::QueryInterface(REFIID riid, LPVOID* ppvObject)
{
	if (ppvObject == nullptr)
	{
		return E_POINTER;
	}
	else if (IsEqualIID(riid, IID_IContextMenu))
	{
		*ppvObject = static_cast<LPCONTEXTMENU>(this);
	}
	else if (IsEqualIID(riid, IID_IShellExtInit))
	{
		*ppvObject = static_cast<LPSHELLEXTINIT>(this);
	}
	else
	{
		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}

	this->AddRef();

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP_(ULONG) ContextMenuHandler::AddRef()
{
	return ++refCount_;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP_(ULONG) ContextMenuHandler::Release()
{
	ULONG rc{ --refCount_ };
	if (rc == 0)
	{
		delete this;
	}
	return rc;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP ContextMenuHandler::Initialize(PCIDLIST_ABSOLUTE pidlFolder, LPDATAOBJECT pdtobj, HKEY hkeyProgID)
{
	if (pdtobj == nullptr)
	{
		return E_INVALIDARG;
	}

	if (pDataObject_ != nullptr)
	{
		pDataObject_->Release();
		pDataObject_ = nullptr;
	}

	UINT dataCount{ GetDataCount(pdtobj) };
	if (!global_.ProdMultiSelect && dataCount == 1 || global_.ProdMultiSelect && dataCount != 0)
	{
		if (Initialize_(pidlFolder, pdtobj, hkeyProgID))
		{
			pDataObject_ = pdtobj;
			pDataObject_->AddRef();

			return S_OK;
		}
	}

	return E_FAIL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP ContextMenuHandler::QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
	if ((uFlags & CMF_DEFAULTONLY) != 0)
	{
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
	}

	auto ret{ QueryContextMenu_(hmenu, indexMenu, idCmdFirst, idCmdLast) };
	if (ret.first == true)
	{
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ret.second);
	}

	UINT idCmd{ idCmdFirst };
	HMENU subMenuHandle{ nullptr };
	size_t subMenuCount{ global_.ProdSubMenuItems.size() };

	if (subMenuCount != 0)
	{
		if ((subMenuHandle = CreatePopupMenu()) == nullptr)
		{
			return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
		}

		for (UINT i = 0; i < subMenuCount; ++i)
		{
			if (idCmd == idCmdLast || MenuInsertItem(subMenuHandle, i, global_.ProdSubMenuItems[i].c_str(), idCmd++, MFS_ENABLED, nullptr, hBitmapMenu_) == false)
			{
				DestroyMenu(subMenuHandle);
				return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
			}
		}
	}

	if (MenuInsertItem(hmenu, indexMenu, global_.ProdMenuItem.c_str(), idCmd++, MFS_ENABLED, subMenuHandle, hBitmapMenu_) == false)
	{
		if (subMenuHandle != nullptr)
		{
			DestroyMenu(subMenuHandle);
		}
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
	}

	return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, idCmd - idCmdFirst);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP ContextMenuHandler::InvokeCommand(LPCMINVOKECOMMANDINFO pici)
{
	if (HIWORD(pici->lpVerb) != 0)
	{
		return E_FAIL;
	}

	return InvokeCommand_(pici);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP ContextMenuHandler::GetCommandString(UINT_PTR idCmd, UINT uType, UINT* pReserved, LPSTR pszName, UINT cchMax)
{
	UNREFERENCED_PARAMETER(idCmd);
	UNREFERENCED_PARAMETER(uType);
	UNREFERENCED_PARAMETER(pReserved);
	UNREFERENCED_PARAMETER(pszName);
	UNREFERENCED_PARAMETER(cchMax);

	return E_NOTIMPL;
}
