#include "stdafx.h"
#include "FXStudioApp.h"
#include "FXStudioView.h"
#include <direct.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTKd.lib")
#pragma comment(lib, "Effects11d.lib")
#pragma comment(lib, "tinyxml2d.lib")
#pragma comment(lib, "zlibstatic.lib")
#else
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "Effects11.lib")
#pragma comment(lib, "tinyxml2.lib")
#pragma comment(lib, "zlibstatic.lib")
#endif

// #ifdef _DEBUG
// #pragma comment(lib, "RenderEngined.lib")
// #else
// #pragma comment(lib, "RenderEngine.lib")
// #endif

FXStudioApp globalApp;

HICON FXStudioApp::VGetIcon()
{
	return LoadIcon(NULL, IDI_APPLICATION);
}

BaseGameLogic* FXStudioApp::VCreateGameAndView()
{
	BaseGameLogic* pGameLogic = DEBUG_NEW FXStudioLogic();
	pGameLogic->Init();

	shared_ptr<IGameView> pGameView(DEBUG_NEW FXStudioView(m_pRenderer));
	pGameLogic->VAddView(pGameView);

	return pGameLogic;
}

FXStudioLogic::FXStudioLogic()
	: BaseGameLogic(),
	m_MoveDelegate(nullptr)
{
	m_ProjectDirectory = _getcwd(NULL, 0);
	int slashGamePos = m_ProjectDirectory.rfind("\\Bin");
	m_ProjectDirectory = m_ProjectDirectory.substr(0, slashGamePos);

	IEventManager* pEventMgr = IEventManager::Get();
	pEventMgr->VAddListener(
		boost::bind(&FXStudioLogic::MoveActorDelegate, this, _1), EvtData_Move_Actor::sk_EventType);
}

FXStudioLogic::~FXStudioLogic()
{
	IEventManager* pEventMgr = IEventManager::Get();
	pEventMgr->VRemoveListener(
		boost::bind(&FXStudioLogic::MoveActorDelegate, this, _1), EvtData_Move_Actor::sk_EventType);
}

void FXStudioLogic::VMoveActor(ActorId actorId)
{
	if (m_MoveDelegate != nullptr)
	{
		tinyxml2::XMLDocument outDoc;

		tinyxml2::XMLElement* pRoot = outDoc.NewElement("Actor");
		outDoc.InsertEndChild(pRoot);
		pRoot->SetAttribute("id", actorId);

		StrongActorPtr pActor = MakeStrongPtr(VGetActor(actorId));
		if (pActor != nullptr)
		{
			shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::m_Name));
			if (pTransformComponent != nullptr)
			{
				pRoot->InsertEndChild(pTransformComponent->VGenerateXml(&outDoc));

				tinyxml2::XMLPrinter printer;
				outDoc.Accept(&printer);
				m_MoveDelegate(printer.CStr());
			}
		}
	}
}

bool FXStudioLogic::VLoadGame(const std::string& projectXml)
{
	while (m_Actors.size() > 0)
	{
		ActorId id = m_Actors.begin()->first;
		VDestroyActor(id);
	}

	if (!BaseGameLogic::VLoadGame(projectXml))
	{
		return false;
	}

	VChangeState(BGS_Running);
	return true;
}

shared_ptr<FXStudioView> FXStudioLogic::GetHumanView()
{
	DEBUG_ASSERT(m_GameViews.size() == 1);
	shared_ptr<IGameView> pGameView = *m_GameViews.begin();
	shared_ptr<FXStudioView> editorHumanView = static_pointer_cast<FXStudioView>(pGameView);
	return editorHumanView;
}
