#pragma once
#include "../TinyEngine/TinyEngine.h"

class DemoApp : public TinyEngineApp
{
public:
	DemoApp();
	~DemoApp();

protected:
	virtual const std::wstring& VGetWindowTitle() override;
	virtual const std::wstring& VGetWindowClass() override;
	virtual HICON VGetIcon() override;
};

