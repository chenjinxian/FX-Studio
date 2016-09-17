#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"

class SceneNode : public ISceneNode
{
	friend class Scene;

public:
	SceneNode();
	virtual ~SceneNode();

	virtual void VOnUpdate(Scene* pScene, uint32_t elapsedMs) override;
	virtual void VOnRestore(Scene* pScene) override;

	virtual void VPreRender(Scene* pScene) override;
	virtual void VRender(Scene* pScene) override;
	virtual void VRenderChildren(Scene* pScene) override;
	virtual void VPostRender(Scene* pScene) override;
	virtual bool VIsVisible(Scene* pScene) const override;

	virtual void VAddChild(shared_ptr<ISceneNode> child) override;
	virtual void VRemoveChild(ActorId actorId) override;
};

class RootNode : public SceneNode
{
public:
	RootNode();
	~RootNode();

	virtual void VRenderChildren(Scene* pScene);
	virtual void VAddChild(shared_ptr<ISceneNode> child);
	virtual void VRemoveChild(ActorId actorId);
	virtual bool VIsVisible(Scene* pScene) const { return true; }
};

class CameraNode : public SceneNode
{
public:
	CameraNode();
	~CameraNode();

	virtual void VRender(Scene* pScene);
	virtual void VOnRestore(Scene* pScene);
	virtual bool VIsVisible(Scene* pScene) const { return true; }
};