#include "Scene.h"

Scene::Scene(shared_ptr<IRenderer> pRenderer) : m_pRenderer(pRenderer)
{
}

Scene::~Scene()
{
}

void Scene::OnUpdate(uint32_t deltaMilliseconds)
{

}

void Scene::OnRender()
{

}

HRESULT Scene::OnRestore()
{
	return S_OK;
}
