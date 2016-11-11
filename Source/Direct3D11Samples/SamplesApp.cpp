#include "SamplesApp.h"
#include "SamplesView.h"

SamplesApp g_GameApp;

SamplesApp::SamplesApp()
{
}

SamplesApp::~SamplesApp()
{
}

HICON SamplesApp::VGetIcon()
{
	return LoadIcon(GetInstance(), MAKEINTRESOURCE(IDI_APPLICATION));
}

BaseGameLogic* SamplesApp::VCreateGameAndView()
{
	m_pGameLogic = GCC_NEW SamplesLogic();
	m_pGameLogic->Init();

	shared_ptr<IGameView> menuView(GCC_NEW MainMenuView());
	m_pGameLogic->VAddView(menuView);

	return m_pGameLogic;
}

void SamplesApp::VRegisterGameEvents(void)
{

}

void SamplesApp::VCreateNetworkEventForwarder(void)
{

}

void SamplesApp::VDestroyNetworkEventForwarder(void)
{

}

SamplesLogic::SamplesLogic()
{
	RegisterAllDelegates();
}

SamplesLogic::~SamplesLogic()
{
	RemoveAllDelegates();
}

void SamplesLogic::VSetProxy()
{
	BaseGameLogic::VSetProxy();
}

void SamplesLogic::VMoveActor(ActorId id, const Matrix& mat)
{
	BaseGameLogic::VMoveActor(id, mat);

	StrongActorPtr pActor = MakeStrongPtr(VGetActor(id));
	if (pActor)
	{
		shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::m_Name));
		if (pTransformComponent && pTransformComponent->GetPosition().y < -25)
		{
			shared_ptr<EvtData_Destroy_Actor> pDestroyActorEvent(GCC_NEW EvtData_Destroy_Actor(id));
			IEventManager::Get()->VQueueEvent(pDestroyActorEvent);
		}
	}
}

void SamplesLogic::VChangeState(BaseGameState gameState)
{
	BaseGameLogic::VChangeState(gameState);

	switch (gameState)
	{
	case BGS_WaitingForPlayers:
	{
		DEBUG_ERROR(m_ExpectedPlayers == 1);
		for (int i = 0; i < m_ExpectedPlayers; ++i)
		{
			shared_ptr<IGameView> playersView(GCC_NEW SamplesView(g_pApp->m_pRenderer));
			VAddView(playersView);

			if (m_IsProxy)
			{
				return;
			}
		}

		for (int i = 0; i < m_ExpectedRemotePlayers; ++i)
		{
// 			shared_ptr<IGameView> remoteGameView(GCC_NEW NetworkGameView);
// 			VAddView(remoteGameView);
		}

		for (int i = 0; i < m_ExpectedAI; ++i)
		{
// 			shared_ptr<IGameView> aiView(GCC_NEW AITeapotView(m_pPathingGraph));
// 			VAddView(aiView);
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
// 				shared_ptr<NetworkGameView> pNetworkGameView = static_pointer_cast<NetworkGameView, IGameView>(pView);
// 				StrongActorPtr pActor = VCreateActor("actors\\remote_teapot.xml", NULL);
// 				if (pActor)
// 				{
// 					shared_ptr<EvtData_New_Actor> pNewActorEvent(GCC_NEW EvtData_New_Actor(pActor->GetId(), pNetworkGameView->VGetId()));
// 					IEventManager::Get()->VQueueEvent(pNewActorEvent);
// 				}
			}
			else if (pView->VGetType() == GameView_AI)
			{
// 				shared_ptr<AITeapotView> pAiView = static_pointer_cast<AITeapotView, IGameView>(pView);
// 				StrongActorPtr pActor = VCreateActor("actors\\ai_teapot.xml", NULL);
// 				if (pActor)
// 				{
// 					shared_ptr<EvtData_New_Actor> pNewActorEvent(GCC_NEW EvtData_New_Actor(pActor->GetId(), pAiView->VGetId()));
// 					IEventManager::Get()->VQueueEvent(pNewActorEvent);
// 				}
			}
		}

		break;
	}
	}
}

void SamplesLogic::VAddView(shared_ptr<IGameView> pView, ActorId actorId /*= INVALID_ACTOR_ID*/)
{
	BaseGameLogic::VAddView(pView, actorId);
	if (dynamic_pointer_cast<SamplesView>(pView))
	{
		m_HumanPlayersAttached++;
	}
// 	else if (dynamic_pointer_cast<AITeapotView>(pView))
// 	{
// 		m_AIPlayersAttached++;
// 	}
}

void SamplesLogic::RequestStartGameDelegate(IEventDataPtr pEventData)
{
	VChangeState(BGS_WaitingForPlayers);
}

void SamplesLogic::RemoteClientDelegate(IEventDataPtr pEventData)
{

}

void SamplesLogic::NetworkPlayerActorAssignmentDelegate(IEventDataPtr pEventData)
{

}

void SamplesLogic::EnvironmentLoadedDelegate(IEventDataPtr pEventData)
{
	++m_HumanGamesLoaded;
}

void SamplesLogic::ThrustDelegate(IEventDataPtr pEventData)
{

}

void SamplesLogic::SteerDelegate(IEventDataPtr pEventData)
{

}

void SamplesLogic::StartThrustDelegate(IEventDataPtr pEventData)
{

}

void SamplesLogic::EndThrustDelegate(IEventDataPtr pEventData)
{

}

void SamplesLogic::StartSteerDelegate(IEventDataPtr pEventData)
{

}

void SamplesLogic::EndSteerDelegate(IEventDataPtr pEventData)
{

}

bool SamplesLogic::VLoadGameDelegate(TiXmlElement* pLevelData)
{
	return true;
}

void SamplesLogic::RegisterAllDelegates(void)
{
	IEventManager* pGlobalEventManager = IEventManager::Get();
	pGlobalEventManager->VAddListener(
		boost::bind(&SamplesLogic::RemoteClientDelegate, this, _1), EvtData_Remote_Client::sk_EventType);
	pGlobalEventManager->VAddListener(
		boost::bind(&SamplesLogic::MoveActorDelegate, this, _1), EvtData_Move_Actor::sk_EventType);
	pGlobalEventManager->VAddListener(
		boost::bind(&SamplesLogic::RequestStartGameDelegate, this, _1), EvtData_Request_Start_Game::sk_EventType);
	pGlobalEventManager->VAddListener(
		boost::bind(&SamplesLogic::NetworkPlayerActorAssignmentDelegate, this, _1), EvtData_Network_Player_Actor_Assignment::sk_EventType);
	pGlobalEventManager->VAddListener(
		boost::bind(&SamplesLogic::EnvironmentLoadedDelegate, this, _1), EvtData_Environment_Loaded::sk_EventType);
	pGlobalEventManager->VAddListener(
		boost::bind(&SamplesLogic::EnvironmentLoadedDelegate, this, _1), EvtData_Remote_Environment_Loaded::sk_EventType);

// 	pGlobalEventManager->VAddListener(boost::bind(&SamplesLogic::StartThrustDelegate, this, _1), EvtData_StartThrust::sk_EventType);
// 	pGlobalEventManager->VAddListener(boost::bind(&SamplesLogic::EndThrustDelegate, this, _1), EvtData_EndThrust::sk_EventType);
// 	pGlobalEventManager->VAddListener(boost::bind(&SamplesLogic::StartSteerDelegate, this, _1), EvtData_StartSteer::sk_EventType);
// 	pGlobalEventManager->VAddListener(boost::bind(&SamplesLogic::EndSteerDelegate, this, _1), EvtData_EndSteer::sk_EventType);
}

void SamplesLogic::RemoveAllDelegates(void)
{
	IEventManager* pGlobalEventManager = IEventManager::Get();
	pGlobalEventManager->VRemoveListener(
		boost::bind(&SamplesLogic::RemoteClientDelegate, this, _1), EvtData_Remote_Client::sk_EventType);
	pGlobalEventManager->VRemoveListener(
		boost::bind(&SamplesLogic::MoveActorDelegate, this, _1), EvtData_Move_Actor::sk_EventType);
	pGlobalEventManager->VRemoveListener(
		boost::bind(&SamplesLogic::RequestStartGameDelegate, this, _1), EvtData_Request_Start_Game::sk_EventType);
	pGlobalEventManager->VRemoveListener(
		boost::bind(&SamplesLogic::NetworkPlayerActorAssignmentDelegate, this, _1), EvtData_Network_Player_Actor_Assignment::sk_EventType);
	pGlobalEventManager->VRemoveListener(
		boost::bind(&SamplesLogic::EnvironmentLoadedDelegate, this, _1), EvtData_Environment_Loaded::sk_EventType);
	pGlobalEventManager->VRemoveListener(
		boost::bind(&SamplesLogic::EnvironmentLoadedDelegate, this, _1), EvtData_Remote_Environment_Loaded::sk_EventType);
	if (m_IsProxy)
	{
		pGlobalEventManager->VRemoveListener(
			boost::bind(&SamplesLogic::RequestNewActorDelegate, this, _1), EvtData_Request_New_Actor::sk_EventType);
	}

// 	pGlobalEventManager->VRemoveListener(boost::bind(&SamplesLogic::StartThrustDelegate, _1), EvtData_StartThrust::sk_EventType);
// 	pGlobalEventManager->VRemoveListener(boost::bind(&SamplesLogic::EndThrustDelegate, _1), EvtData_EndThrust::sk_EventType);
// 	pGlobalEventManager->VRemoveListener(boost::bind(&SamplesLogic::StartSteerDelegate, _1), EvtData_StartSteer::sk_EventType);
// 	pGlobalEventManager->VRemoveListener(boost::bind(&SamplesLogic::EndSteerDelegate, _1), EvtData_EndSteer::sk_EventType);
}

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	return GameMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
