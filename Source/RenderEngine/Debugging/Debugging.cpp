#include <unknwn.h>
int Refs(IUnknown *pUnk)
{
	pUnk->AddRef();
	return pUnk->Release();
}

