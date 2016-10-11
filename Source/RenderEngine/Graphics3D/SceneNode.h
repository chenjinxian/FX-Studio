#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"

class SceneNode;
class Scene;
class RayCast;
class MovementController;
class IResourceExtraData;
class ActorComponent;
class BaseRenderComponent;

typedef BaseRenderComponent* WeakBaseRenderComponentPtr;

class SceneNodeProperties
{
	friend class SceneNode;

public:
	SceneNodeProperties() : m_ActorId(INVALID_ACTOR_ID) {}
	~SceneNodeProperties() {}

	ActorId GetActorId() const { return m_ActorId; }
	const std::string& GetActorName() const { return m_ActorName; }

protected:
	ActorId m_ActorId;
	std::string m_ActorName;
};

typedef std::vector<shared_ptr<ISceneNode> > SceneNodeList;

class SceneNode : public ISceneNode
{
	friend class Scene;

public:
	SceneNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass);
	virtual ~SceneNode();

	virtual const SceneNodeProperties* VGet() const override { return &m_Properties; }

	virtual HRESULT VOnUpdate(Scene* pScene, uint32_t elapsedMs) override;
	virtual HRESULT VOnRestore(Scene* pScene) override;
	virtual HRESULT VOnLostDevice(Scene *pScene);

	virtual HRESULT VPreRender(Scene* pScene) override;
	virtual HRESULT VRender(Scene* pScene) override;
	virtual HRESULT VRenderChildren(Scene* pScene) override;
	virtual HRESULT VPostRender(Scene* pScene) override;
	virtual bool VIsVisible(Scene* pScene) const override;

	virtual bool VAddChild(shared_ptr<ISceneNode> child) override;
	virtual bool VRemoveChild(ActorId actorId) override;

protected:
	SceneNodeList m_Children;
	SceneNode* m_pParent;
	SceneNodeProperties m_Properties;
	WeakBaseRenderComponentPtr m_pRenderComponent;
};

class RootNode : public SceneNode
{
public:
	RootNode();
	~RootNode();

	virtual HRESULT VRenderChildren(Scene* pScene);
	virtual bool VAddChild(shared_ptr<ISceneNode> child);
	virtual bool VRemoveChild(ActorId actorId);
	virtual bool VIsVisible(Scene* pScene) const { return true; }
};

class CameraNode : public SceneNode
{
public:
	CameraNode();
	~CameraNode();

	virtual HRESULT VRender(Scene* pScene);
	virtual HRESULT VOnRestore(Scene* pScene);
	virtual bool VIsVisible(Scene* pScene) const { return true; }
};