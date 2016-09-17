#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"

class SceneNode;
class CameraNode;
typedef std::map<ActorId, shared_ptr<ISceneNode> > SceneActorMap;

class Scene
{
public:
	Scene(shared_ptr<IRenderer> pRenderer);
	virtual ~Scene();

	void OnUpdate(uint32_t deltaMs);
	void OnRender();
	HRESULT OnRestore();
	shared_ptr<ISceneNode> FindActor(ActorId actorId);
	void AddChild(ActorId actorId, shared_ptr<ISceneNode> child);
	void RemoveChild(ActorId actorId);

	void SetCamera(shared_ptr<CameraNode> pCamera) { m_pCamera = pCamera; }
	const shared_ptr<CameraNode> GetCamera() const { return m_pCamera; }

	shared_ptr<IRenderer> GetRenderder() { return m_pRenderer; }

private:
	shared_ptr<IRenderer> m_pRenderer;
	shared_ptr<SceneNode> m_pRootNode;
	shared_ptr<CameraNode> m_pCamera;
	SceneActorMap m_ActorMap;
};

