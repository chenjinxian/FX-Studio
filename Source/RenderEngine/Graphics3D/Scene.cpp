#include "Scene.h"

Scene::Scene(shared_ptr<IRenderer> pRenderer)
{
}

Scene::~Scene()
{
}

HRESULT Scene::OnRender()
{
	return S_OK;
}

HRESULT Scene::OnRestore()
{
	return S_OK;
}

HRESULT Scene::OnLostDevice()
{
	return S_OK;
}

HRESULT Scene::OnUpdate(const int deltaMilliseconds)
{
	return S_OK;
}

shared_ptr<ISceneNode> Scene::FindActor(ActorId id)
{
	return shared_ptr<ISceneNode>();
}

bool Scene::AddChild(ActorId id, shared_ptr<ISceneNode> kid)
{
	return true;
}

bool Scene::RemoveChild(ActorId id)
{
	return true;
}
