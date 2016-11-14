#include "EventManager.h"

static IEventManager* g_pEventManager = nullptr;
GenericObjectFactory<IEventData, EventType> g_eventFactory;

IEventManager* IEventManager::Get()
{
	return g_pEventManager;
}

IEventManager::IEventManager(const char* name, bool setAsGlobal)
{
	if (setAsGlobal)
	{
		if (g_pEventManager != nullptr)
		{
			delete g_pEventManager;
		}

		g_pEventManager = this;
	}
}

IEventManager::~IEventManager()
{
	if (g_pEventManager == this)
	{
		g_pEventManager = nullptr;
	}
}