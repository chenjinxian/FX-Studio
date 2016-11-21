#include "DemoApp.h"

DemoApp g_DemoApp;

DemoApp::DemoApp() : BaseGameApp()
{
}

DemoApp::~DemoApp()
{
}

const wchar_t* DemoApp::VGetWindowTitle()
{
	return _T("TinyEngine - Demo");
}

const wchar_t* DemoApp::VGetWindowClass()
{
	return _T("TinyEngine");
}

HICON DemoApp::VGetIcon()
{
	return LoadIcon(NULL, IDI_APPLICATION);
}

BaseGameLogic * DemoApp::VCreateGameAndView()
{
	m_pGameLogic = DEBUG_NEW DemoLogic();
	m_pGameLogic->Init();

// 	shared_ptr<IGameView> menuView(GCC_NEW MainMenuView());
// 	m_pGameLogic->VAddView(menuView);

	return m_pGameLogic;
}

DemoLogic::DemoLogic()
{
	RegisterAllDelegates();
}

DemoLogic::~DemoLogic()
{
	RemoveAllDelegates();
}

void DemoLogic::VSetProxy()
{
	BaseGameLogic::VSetProxy();
}

void DemoLogic::VMoveActor(ActorId id, const Matrix& mat)
{
	BaseGameLogic::VMoveActor(id, mat);

// 	StrongActorPtr pActor = MakeStrongPtr(VGetActor(id));
// 	if (pActor)
// 	{
// 		shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::m_Name));
// 		if (pTransformComponent && pTransformComponent->GetPosition().y < -25)
// 		{
// 			shared_ptr<EvtData_Destroy_Actor> pDestroyActorEvent(DEBUG_NEW EvtData_Destroy_Actor(id));
// 			IEventManager::Get()->VQueueEvent(pDestroyActorEvent);
// 		}
// 	}
}

void DemoLogic::VChangeState(BaseGameState gameState)
{
	BaseGameLogic::VChangeState(gameState);

	switch (gameState)
	{
	case BGS_WaitingForPlayers:
	{
// 		shared_ptr<IGameView> playersView(DEBUG_NEW SamplesView(g_pApp->m_pRenderer));
// 		VAddView(playersView);

		if (m_IsProxy)
		{
			return;
		}

		break;
	}


	case BGS_SpawningPlayersActors:
	{
		if (m_IsProxy)
		{
			return;
		}

		for (auto it = m_GameViews.begin(); it != m_GameViews.end(); ++it)
		{
			shared_ptr<IGameView> pView = *it;
			if (pView->VGetType() == GameView_Human)
			{
// 				StrongActorPtr pActor = VCreateActor("actors\\player_teapot.xml", NULL, Matrix());
// 				if (pActor)
// 				{
// 					shared_ptr<EvtData_New_Actor> pNewActorEvent(GCC_NEW EvtData_New_Actor(pActor->GetId(), pView->VGetId()));
// 					IEventManager::Get()->VTriggerEvent(pNewActorEvent);
// 				}
			}
			else if (pView->VGetType() == GameView_Remote)
			{
			}
			else if (pView->VGetType() == GameView_AI)
			{
			}
		}

		break;
	}
	}
}

void DemoLogic::VAddView(shared_ptr<IGameView> pView, ActorId actorId /*= INVALID_ACTOR_ID*/)
{
	BaseGameLogic::VAddView(pView, actorId);
// 	if (dynamic_pointer_cast<SamplesView>(pView))
// 	{
// 		m_HumanPlayersAttached++;
// 	}
}

void DemoLogic::RequestStartGameDelegate(IEventDataPtr pEventData)
{
	VChangeState(BGS_WaitingForPlayers);
}

void DemoLogic::RemoteClientDelegate(IEventDataPtr pEventData)
{

}

void DemoLogic::NetworkPlayerActorAssignmentDelegate(IEventDataPtr pEventData)
{

}

void DemoLogic::EnvironmentLoadedDelegate(IEventDataPtr pEventData)
{
	++m_HumanGamesLoaded;
}

void DemoLogic::ThrustDelegate(IEventDataPtr pEventData)
{

}

void DemoLogic::SteerDelegate(IEventDataPtr pEventData)
{

}

void DemoLogic::StartThrustDelegate(IEventDataPtr pEventData)
{

}

void DemoLogic::EndThrustDelegate(IEventDataPtr pEventData)
{

}

void DemoLogic::StartSteerDelegate(IEventDataPtr pEventData)
{

}

void DemoLogic::EndSteerDelegate(IEventDataPtr pEventData)
{

}

bool DemoLogic::VLoadGameDelegate(tinyxml2::XMLElement* pLevelData)
{
	return true;
}

void DemoLogic::RegisterAllDelegates(void)
{
	IEventManager* pGlobalEventManager = IEventManager::Get();
	pGlobalEventManager->VAddListener(
		boost::bind(&DemoLogic::RemoteClientDelegate, this, _1), EvtData_Remote_Client::sk_EventType);
	pGlobalEventManager->VAddListener(
		boost::bind(&DemoLogic::MoveActorDelegate, this, _1), EvtData_Move_Actor::sk_EventType);
	pGlobalEventManager->VAddListener(
		boost::bind(&DemoLogic::RequestStartGameDelegate, this, _1), EvtData_Request_Start_Game::sk_EventType);
	pGlobalEventManager->VAddListener(
		boost::bind(&DemoLogic::NetworkPlayerActorAssignmentDelegate, this, _1), EvtData_Network_Player_Actor_Assignment::sk_EventType);
	pGlobalEventManager->VAddListener(
		boost::bind(&DemoLogic::EnvironmentLoadedDelegate, this, _1), EvtData_Environment_Loaded::sk_EventType);
	pGlobalEventManager->VAddListener(
		boost::bind(&DemoLogic::EnvironmentLoadedDelegate, this, _1), EvtData_Remote_Environment_Loaded::sk_EventType);

	// 	pGlobalEventManager->VAddListener(boost::bind(&DemoLogic::StartThrustDelegate, this, _1), EvtData_StartThrust::sk_EventType);
	// 	pGlobalEventManager->VAddListener(boost::bind(&DemoLogic::EndThrustDelegate, this, _1), EvtData_EndThrust::sk_EventType);
	// 	pGlobalEventManager->VAddListener(boost::bind(&DemoLogic::StartSteerDelegate, this, _1), EvtData_StartSteer::sk_EventType);
	// 	pGlobalEventManager->VAddListener(boost::bind(&DemoLogic::EndSteerDelegate, this, _1), EvtData_EndSteer::sk_EventType);
}

void DemoLogic::RemoveAllDelegates(void)
{
	IEventManager* pGlobalEventManager = IEventManager::Get();
	pGlobalEventManager->VRemoveListener(
		boost::bind(&DemoLogic::RemoteClientDelegate, this, _1), EvtData_Remote_Client::sk_EventType);
	pGlobalEventManager->VRemoveListener(
		boost::bind(&DemoLogic::MoveActorDelegate, this, _1), EvtData_Move_Actor::sk_EventType);
	pGlobalEventManager->VRemoveListener(
		boost::bind(&DemoLogic::RequestStartGameDelegate, this, _1), EvtData_Request_Start_Game::sk_EventType);
	pGlobalEventManager->VRemoveListener(
		boost::bind(&DemoLogic::NetworkPlayerActorAssignmentDelegate, this, _1), EvtData_Network_Player_Actor_Assignment::sk_EventType);
	pGlobalEventManager->VRemoveListener(
		boost::bind(&DemoLogic::EnvironmentLoadedDelegate, this, _1), EvtData_Environment_Loaded::sk_EventType);
	pGlobalEventManager->VRemoveListener(
		boost::bind(&DemoLogic::EnvironmentLoadedDelegate, this, _1), EvtData_Remote_Environment_Loaded::sk_EventType);
	if (m_IsProxy)
	{
		pGlobalEventManager->VRemoveListener(
			boost::bind(&DemoLogic::RequestNewActorDelegate, this, _1), EvtData_Request_New_Actor::sk_EventType);
	}

	// 	pGlobalEventManager->VRemoveListener(boost::bind(&DemoLogic::StartThrustDelegate, _1), EvtData_StartThrust::sk_EventType);
	// 	pGlobalEventManager->VRemoveListener(boost::bind(&DemoLogic::EndThrustDelegate, _1), EvtData_EndThrust::sk_EventType);
	// 	pGlobalEventManager->VRemoveListener(boost::bind(&DemoLogic::StartSteerDelegate, _1), EvtData_StartSteer::sk_EventType);
	// 	pGlobalEventManager->VRemoveListener(boost::bind(&DemoLogic::EndSteerDelegate, _1), EvtData_EndSteer::sk_EventType);
}
