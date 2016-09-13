#pragma once
#include "../RenderEngineInterface.h"

class SceneNodeProperty
{
	friend class SceneNode;
	
public:
	SceneNodeProperty() : m_ActorId(INVALID_ACTOR_ID), m_Radius(0.0f), m_RenderPass(RenderPass_0) {}
	~SceneNodeProperty() {}

	const ActorId& GetActorId() const { return m_ActorId; }
	const std::string& Name() const { return m_ActorName; }
	const Matrix& ToWorld() const { return m_ToWorld; }
	const Matrix& FromWorld() const { return m_FromWorld; }
	void Transform(Matrix& toWorld, Matrix& fromWorld) const
	{
		toWorld = m_ToWorld; fromWorld = m_FromWorld;
	}
	RenderPass GetRenderPass() const { return m_RenderPass; }
	float Radius() const { return m_Radius; }

private:
	ActorId m_ActorId;
	std::string m_ActorName;
	Matrix m_ToWorld;
	Matrix m_FromWorld;
	float m_Radius;
	RenderPass m_RenderPass;
};

typedef std::vector<shared_ptr<ISceneNode> > SceneNodeList;

class SceneNode : public ISceneNode
{
	friend class Scene;

public:
	SceneNode();
	virtual ~SceneNode();

	virtual const SceneNodeProperty* const VGet() const override { return &m_Property; }

	virtual void VSetTransform(const Matrix& toWorld, const Matrix& fromWorld) override;
	virtual HRESULT VOnUpdate(Scene* pScene, DWORD const elapsedMs) override;
	virtual HRESULT VOnRestore(Scene* pScene) override;
	virtual HRESULT VOnLostDevice(Scene* pScene) override;

	virtual HRESULT VPreRender(Scene* pScene) override;
	virtual HRESULT VRender(Scene* pScene) override;
	virtual HRESULT VRenderChildren(Scene* pScene) override;
	virtual HRESULT VPostRender(Scene* pScene) override;

	virtual bool VAddChild(shared_ptr<ISceneNode> kid) override;
	virtual bool VRemoveChild(ActorId id) override;
	virtual bool VIsVisible(Scene* pScene) const override;
	virtual HRESULT VPick(Scene* pScene, RayCast *pRayCast) override;

protected:
	SceneNodeList m_Children;
	SceneNode* m_pParent;
	SceneNodeProperty m_Property;
};

class RootNode : public SceneNode
{
public:
	RootNode();

	virtual bool VAddChild(shared_ptr<ISceneNode> child);
	virtual HRESULT VRenderChildren(Scene* pScene);
	virtual bool VRemoveChild(ActorId id);
	virtual bool VIsVisible(Scene* pScene) const { return true; }
};

class CameraNode : public SceneNode
{
public:
	CameraNode();

	virtual HRESULT VRender(Scene* pScene);
	virtual HRESULT VOnRestore(Scene* pScene);
	virtual bool VIsVisible(Scene* pScene) const { return m_IsActive; }

protected:
	bool m_IsActive;
};