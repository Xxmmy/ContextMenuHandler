#pragma once

#include <unknwn.h>

///////////////////////////////////////////////////////////////////////////////////////////////////

class ClassFactory : public IClassFactory
{
public:
	ClassFactory(Global& global);
	~ClassFactory();

	IFACEMETHODIMP QueryInterface(REFIID riid, LPVOID* ppvObject);
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();

	IFACEMETHODIMP CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, LPVOID* ppvObject);
	IFACEMETHODIMP LockServer(BOOL fLock);

private:
	Global& global_;
	ULONG refCount_{ 1 };
};
