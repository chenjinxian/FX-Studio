#include "Scene.h"
#include "SceneNode.h"

Scene::Scene(shared_ptr<IRenderer> pRenderer) : m_pRenderer(pRenderer), m_pRootNode(nullptr), m_pCamera(nullptr)
{
}

Scene::~Scene()
{
}

void Scene::OnUpdate(uint32_t deltaMs)
{
	if (m_pRootNode != nullptr)
	{
		static uint32_t lastTime = GetTickCount();
		uint32_t elapsedTime = 0;
		uint32_t currentTime = GetTickCount();
		elapsedTime = currentTime - lastTime;
		lastTime = currentTime;

		m_pRootNode->VOnUpdate(this, elapsedTime);
	}
}

void Scene::OnRender()
{

}

HRESULT Scene::OnRestore()
{
	return S_OK;
}

shared_ptr<ISceneNode> Scene::FindActor(ActorId actorId)
{
	auto actor = m_ActorMap.find(actorId);
	if (actor != m_ActorMap.end())
	{
		return actor->second;
	}
	else
	{
		return shared_ptr<ISceneNode>();
	}
}

void Scene::AddChild(ActorId actorId, shared_ptr<ISceneNode> child)
{
	if (actorId != INVALID_ACTOR_ID)
	{
		m_ActorMap[actorId] = child;
	}

	m_pRootNode->VAddChild(child);
}

void Scene::RemoveChild(ActorId actorId)
{
	if (actorId != INVALID_ACTOR_ID)
	{
		m_ActorMap.erase(actorId);
		m_pRootNode->VRemoveChild(actorId);
	}
}
