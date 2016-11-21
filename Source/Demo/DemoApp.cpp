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

}

DemoLogic::~DemoLogic()
{

}

void DemoLogic::VSetProxy()
{

}

void DemoLogic::VMoveActor(ActorId id, const Matrix& mat)
{

}

void DemoLogic::VChangeState(BaseGameState gameState)
{

}

void DemoLogic::VAddView(shared_ptr<IGameView> pView, ActorId actorId /*= INVALID_ACTOR_ID*/)
{

}

void DemoLogic::RequestStartGameDelegate(IEventDataPtr pEventData)
{

}

void DemoLogic::RemoteClientDelegate(IEventDataPtr pEventData)
{

}

void DemoLogic::NetworkPlayerActorAssignmentDelegate(IEventDataPtr pEventData)
{

}

void DemoLogic::EnvironmentLoadedDelegate(IEventDataPtr pEventData)
{

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

}

void DemoLogic::RegisterAllDelegates(void)
{

}

void DemoLogic::RemoveAllDelegates(void)
{

}
