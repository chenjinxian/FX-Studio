#pragma once
#include "../TinyEngine/TinyEngine.h"

class DemoApp : public BaseGameApp
{
public:
	DemoApp();
	~DemoApp();

protected:
	virtual const wchar_t* VGetWindowTitle() override;
	virtual const wchar_t* VGetWindowClass() override;
	virtual HICON VGetIcon() override;
};

