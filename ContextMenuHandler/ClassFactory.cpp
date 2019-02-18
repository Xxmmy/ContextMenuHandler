#include "Global.h"
#include "ClassFactory.h"
#include "ContextMenuHandler.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

ClassFactory::ClassFactory(Global& global) : global_{ global }
{
	++global_.ModRefCount;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ClassFactory::~ClassFactory()
{
	--global_.ModRefCount;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP ClassFactory::QueryInterface(REFIID riid, LPVOID* ppvObject)
{
	if (ppvObject == nullptr)
	{
		return E_POINTER;
	}
	else if (IsEqualIID(riid, IID_IClassFactory))
	{
		*ppvObject = static_cast<LPCLASSFACTORY>(this);
		this->AddRef();
		return S_OK;
	}
	else
	{
		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP_(ULONG) ClassFactory::AddRef()
{
	return ++refCount_;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP_(ULONG) ClassFactory::Release()
{
	ULONG rc{ --refCount_ };
	if (rc == 0)
	{
		delete this;
	}
	return rc;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP ClassFactory::CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, LPVOID* ppvObject)
{
	if (pUnkOuter != nullptr)
	{
		*ppvObject = nullptr;
		return CLASS_E_NOAGGREGATION;
	}

	ContextMenuHandler* cmh{ new (std::nothrow) ContextMenuHandler(global_) };
	if (cmh == nullptr)
	{
		*ppvObject = nullptr;
		return E_OUTOFMEMORY;
	}

	HRESULT hr{ cmh->QueryInterface(riid, ppvObject) };
	cmh->Release();

	return hr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP ClassFactory::LockServer(BOOL fLock)
{
	UNREFERENCED_PARAMETER(fLock);

	return E_NOTIMPL;
}
