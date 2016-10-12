#include "ThirdPersonShoot.h"

ShootGameApp g_GameApp;

ShootGameApp::ShootGameApp()
{
}

ShootGameApp::~ShootGameApp()
{
}

HICON ShootGameApp::VGetIcon()
{

}

BaseGameLogic* ShootGameApp::VCreateGameAndView()
{
	m_pGameLogic = GCC_NEW ShootGameLogic();
	m_pGameLogic->Init();

	shared_ptr<IGameView> menuView(GCC_NEW MainMenuView());
	m_pGameLogic->VAddView(menuView);

	return m_pGameLogic;
}

void ShootGameApp::VRegisterGameEvents(void)
{

}

void ShootGameApp::VCreateNetworkEventForwarder(void)
{

}

void ShootGameApp::VDestroyNetworkEventForwarder(void)
{

}

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	return GameMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

ShootGameLogic::ShootGameLogic()
{

}

ShootGameLogic::~ShootGameLogic()
{

}

void ShootGameLogic::VSetProxy()
{

}

void ShootGameLogic::VMoveActor(ActorId id, const Matrix& mat)
{

}

void ShootGameLogic::VChangeState(BaseGameState gameState)
{

}

void ShootGameLogic::VAddView(shared_ptr<IGameView> pView, ActorId actorId /*= INVALID_ACTOR_ID*/)
{

}

void ShootGameLogic::RequestStartGameDelegate(IEventDataPtr pEventData)
{

}

void ShootGameLogic::RemoteClientDelegate(IEventDataPtr pEventData)
{

}

void ShootGameLogic::EnvironmentLoadedDelegate(IEventDataPtr pEventData)
{

}

void ShootGameLogic::ThrustDelegate(IEventDataPtr pEventData)
{

}

void ShootGameLogic::SteerDelegate(IEventDataPtr pEventData)
{

}

void ShootGameLogic::StartThrustDelegate(IEventDataPtr pEventData)
{

}

void ShootGameLogic::EndThrustDelegate(IEventDataPtr pEventData)
{

}

void ShootGameLogic::StartSteerDelegate(IEventDataPtr pEventData)
{

}

void ShootGameLogic::EndSteerDelegate(IEventDataPtr pEventData)
{

}

bool ShootGameLogic::VLoadGameDelegate(TiXmlElement* pLevelData)
{

}

void ShootGameLogic::RegisterAllDelegates(void)
{

}

void ShootGameLogic::RemoveAllDelegates(void)
{

}
