#pragma once
#include "../RenderEngine/RenderEngine.h"

class FXStudioApp : public RenderEngineApp
{
public:
	FXStudioApp(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd = NULL, int screenWidth = SCREEN_WIDTH, int screenHeight = SCREEN_HEIGHT);
	virtual ~FXStudioApp();

protected:
	BaseGameLogic* VCreateGameAndView() override;
};

class FXStudioLogic : public BaseGameLogic
{
public:
	FXStudioLogic();
	~FXStudioLogic();
};