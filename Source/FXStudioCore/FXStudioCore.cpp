// FXStudioCore.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FXStudioCore.h"
#include "FXStudioView.h"
#include "ModelImporter.h"

FXSTUDIOCORE_API bool FX_APIENTRY CreateInstance(
	int* instancePtrAddress,
	int* hPrevInstancePtrAddress,
	int* hWndMain,
	int nCmdShow,
	int screenWidth, int screenHeight)
{
	HINSTANCE hInstance = (HINSTANCE)instancePtrAddress;
	HINSTANCE hPrevInstance = (HINSTANCE)hPrevInstancePtrAddress;
	HWND hMainWnd = (HWND)hWndMain;

	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	_CrtSetDbgFlag(tmpDbgFlag);

	SetCurrentDirectory(Utility::GetExecutableDirectory().c_str());
	Logger::Init("logging.xml");

	if (g_pApp == nullptr)
		return false;

	g_pApp->GetGameConfig().InitConfig("EditorOptions.xml", nullptr);
	if (!g_pApp->InitEnvironment())
		return false;

	g_pApp->GetGameConfig().m_ScreenWidth = screenWidth;
	g_pApp->GetGameConfig().m_ScreenHeight = screenHeight;
	g_pApp->SetupWindow(hInstance, hMainWnd);
	if (!g_pApp->InitRenderer())
		return false;
	g_pApp->OnStartRender();

	return true;
}

FXSTUDIOCORE_API int FX_APIENTRY DestroyInstance()
{
	g_pApp->OnClose();
	Logger::Destroy();
	return 0;
}

FXSTUDIOCORE_API void FX_APIENTRY ResizeWnd(int screenWidth, int screenHeight)
{
	g_pApp->OnResize(screenWidth, screenHeight);
}

FXSTUDIOCORE_API void FX_APIENTRY WndProc(int *hWndPtrAddress, int uMsg, int* wParam, int* lParam)
{
	HWND hWnd = (HWND)hWndPtrAddress;

	switch (uMsg)
	{
	case WM_CLOSE:
		g_pApp->OnClose();
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	{
		if (g_pApp->GetGameLogic() != nullptr)
		{
			AppMsg msg;
			msg.m_hWnd = hWnd;
			msg.m_uMsg = uMsg;
			msg.m_wParam = WPARAM(wParam);
			msg.m_lParam = LPARAM(lParam);

			g_pApp->OnDispatchMsg(msg);
		}
		break;
	}
	}
}

FXSTUDIOCORE_API void FX_APIENTRY RenderFrame()
{
	g_pApp->OnRenderFrame();
}

FXSTUDIOCORE_API bool FX_APIENTRY IsGameRunning()
{
	return true;
}

FXSTUDIOCORE_API void FX_APIENTRY OpenProject(BSTR lFileName)
{
	std::string project = Utility::WS2S(std::wstring(lFileName, SysStringLen(lFileName)));
	FXStudioLogic* pEditorLogic = dynamic_cast<FXStudioLogic*>(g_pApp->GetGameLogic());
	if (pEditorLogic != nullptr)
	{
		g_pApp->GetGameConfig().m_Project = project;
		pEditorLogic->VChangeState(BGS_LoadingGameEnvironment);
	}
}

FXSTUDIOCORE_API void FX_APIENTRY SetCameraType(int type)
{
	FXStudioLogic* pEditorLogic = dynamic_cast<FXStudioLogic*>(g_pApp->GetGameLogic());
	if (pEditorLogic != nullptr)
	{
		shared_ptr<FXStudioView> pView = pEditorLogic->GetHumanView();
		if (pView != nullptr)
		{
			pView->SetCameraType(type);
		}
	}
}

FXSTUDIOCORE_API void FX_APIENTRY SetTransformType(int type)
{
	FXStudioLogic* pEditorLogic = dynamic_cast<FXStudioLogic*>(g_pApp->GetGameLogic());
	if (pEditorLogic != nullptr)
	{
		shared_ptr<FXStudioView> pView = pEditorLogic->GetHumanView();
		if (pView != nullptr)
		{
			pView->GetGizmosNode()->SetTransformType(type);
		}
	}
}

FXSTUDIOCORE_API unsigned int FX_APIENTRY GetPickedActor(int cursorX, int cursorY, int* mesh)
{
	FXStudioLogic* pEditorLogic = dynamic_cast<FXStudioLogic*>(g_pApp->GetGameLogic());
	if (pEditorLogic == nullptr)
	{
		return INVALID_ACTOR_ID;
	}

	shared_ptr<FXStudioView> pView = pEditorLogic->GetHumanView();
	if (pView == nullptr)
	{
		return INVALID_ACTOR_ID;
	}

	if (pView->GetGizmosNode()->IsPicked() || GetKeyState(VK_MENU) < 0)
	{
		*mesh = 0;
		return pView->GetScene()->GetPickedActor();
	}
	else
	{
		return pView->GetScene()->PickActor(cursorX, cursorY, mesh);
	}
}

FXSTUDIOCORE_API void FX_APIENTRY SetPickedActor(unsigned int actorId)
{
	FXStudioLogic* pEditorLogic = dynamic_cast<FXStudioLogic*>(g_pApp->GetGameLogic());
	if (pEditorLogic == nullptr)
	{
		return;
	}

	shared_ptr<FXStudioView> pView = pEditorLogic->GetHumanView();
	if (pView == nullptr)
	{
		return;
	}

	pView->GetScene()->SetPickedActor(actorId, 0);
}

FXSTUDIOCORE_API unsigned int FX_APIENTRY AddActor(BSTR actorResource)
{
	std::string actorXml = Utility::WS2S(std::wstring(actorResource, SysStringLen(actorResource)));
	StrongActorPtr pActor = g_pApp->GetGameLogic()->VCreateActor(actorXml);
	if (!pActor)
		return INVALID_ACTOR_ID;

	FXStudioLogic* pEditorLogic = dynamic_cast<FXStudioLogic*>(g_pApp->GetGameLogic());
	if (pEditorLogic != nullptr)
	{
		shared_ptr<FXStudioView> pView = pEditorLogic->GetHumanView();
		if (pView != nullptr)
		{
			pView->GetScene()->SetPickedActor(pActor->GetActorId(), 0);
			pView->GetScene()->ResetActorTransform(pActor->GetActorId());
		}
	}

	return pActor->GetActorId();
}

FXSTUDIOCORE_API bool FX_APIENTRY ModifyActor(BSTR modificationXml)
{
	std::string actorModificationXML = Utility::WS2S(std::wstring(modificationXml, SysStringLen(modificationXml)));
	tinyxml2::XMLDocument doc;
	if (doc.Parse(actorModificationXML.c_str(), actorModificationXML.length()) != tinyxml2::XML_SUCCESS)
		return false;

	tinyxml2::XMLElement* pRoot = doc.RootElement();
	if (pRoot == nullptr)
		return false;

	ActorId actorId = pRoot->IntAttribute("id");
	if (actorId != INVALID_ACTOR_ID)
	{
		g_pApp->GetGameLogic()->VModifyActor(pRoot->IntAttribute("id"), pRoot);
	}
// 	else
// 	{
// 		FXStudioLogic* pEditorLogic = dynamic_cast<FXStudioLogic*>(g_pApp->GetGameLogic());
// 		if (pEditorLogic == nullptr)
// 			return false;
// 		
// 		shared_ptr<FXStudioView> pView = pEditorLogic->GetHumanView();
// 		if (pView == nullptr)
// 			return false;
// 
// 		tinyxml2::XMLElement* pCameraNode = pRoot->FirstChildElement("EditorCamera");
// 		pView->MoveEditorCamera(pCameraNode);
// 	}

	return true;
}

FXSTUDIOCORE_API bool FX_APIENTRY RemoveActor(unsigned int actorId)
{
	g_pApp->GetGameLogic()->VDestroyActor(actorId);
	return true;
}

FXSTUDIOCORE_API int FX_APIENTRY ImportModel(BSTR modelImportPath, BSTR modelExportPath, ProgressCallback progressCallback)
{
	std::string importPath = Utility::WS2S(std::wstring(modelImportPath, SysStringLen(modelImportPath)));
	std::string exportPath = Utility::WS2S(std::wstring(modelExportPath, SysStringLen(modelExportPath)));
	ModelImporter::GetImporter().LoadModel(importPath, exportPath, progressCallback);

	return 0;
}

FXSTUDIOCORE_API unsigned int FX_APIENTRY AddEffect(BSTR effectObjectPath, BSTR effectName)
{
	std::string objectPath = Utility::WS2S(std::wstring(effectObjectPath, SysStringLen(effectObjectPath)));
	std::string effect = Utility::WS2S(std::wstring(effectName, SysStringLen(effectName)));

	return g_pApp->AddEffect(objectPath, effect);
}

FXSTUDIOCORE_API unsigned int FX_APIENTRY ModifyEffect(BSTR effectObjectPath, BSTR effectName)
{
	std::string objectPath = Utility::WS2S(std::wstring(effectObjectPath, SysStringLen(effectObjectPath)));
	std::string effect = Utility::WS2S(std::wstring(effectName, SysStringLen(effectName)));

	return g_pApp->ModifyEffect(objectPath, effect);
}

FXSTUDIOCORE_API void FX_APIENTRY GetMaterialXml(BSTR effectObjectPath, char* effectXmlPtr, unsigned int size)
{
	std::string objectPath = Utility::WS2S(std::wstring(effectObjectPath, SysStringLen(effectObjectPath)));

	const std::string& effectXml = g_pApp->GetEffectXml(objectPath);
	strncpy_s(effectXmlPtr, size, effectXml.c_str(), size);
}

FXSTUDIOCORE_API void FX_APIENTRY ModifyMaterial(BSTR materialPath, bool withEffect)
{
	std::string material = Utility::WS2S(std::wstring(materialPath, SysStringLen(materialPath)));

	return g_pApp->ModifyMaterial(material, withEffect);
}

FXSTUDIOCORE_API void FX_APIENTRY AddMaterial(BSTR materialName, BSTR bmpPath)
{
	std::string material = Utility::WS2S(std::wstring(materialName, SysStringLen(materialName)));
	std::string bmp = Utility::WS2S(std::wstring(bmpPath, SysStringLen(bmpPath)));

	FXStudioLogic* pEditorLogic = dynamic_cast<FXStudioLogic*>(g_pApp->GetGameLogic());
	if (pEditorLogic != nullptr)
	{
		shared_ptr<FXStudioView> pView = pEditorLogic->GetHumanView();
		if (pView != nullptr)
		{
			pView->AddMaterial(material, bmp);
		}
	}
}

FXSTUDIOCORE_API void FX_APIENTRY SetMoveDelegate(MoveDelegate delegate)
{
	FXStudioLogic* pEditorLogic = dynamic_cast<FXStudioLogic*>(g_pApp->GetGameLogic());
	if (pEditorLogic != nullptr)
	{
		pEditorLogic->SetMoveDelegate(delegate);
	}
}

