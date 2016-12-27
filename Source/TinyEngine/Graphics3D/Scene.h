#pragma once
#include "../EventManager/EventManager.h"

class SceneNode;
class CameraNode;
typedef std::map<ActorId, shared_ptr<ISceneNode> > SceneActorMap;

class Scene
{
public:
	Scene(shared_ptr<IRenderer> pRenderer);
	virtual ~Scene();

	HRESULT OnUpdate(const GameTime& gameTime);
	HRESULT OnRender(const GameTime& gameTime);
	HRESULT OnInitScene();
	HRESULT OnDeleteScene();
	shared_ptr<ISceneNode> FindActor(ActorId actorId);
	bool AddChild(ActorId actorId, shared_ptr<ISceneNode> pChild);
	bool RemoveChild(ActorId actorId);

	void NewRenderComponentDelegate(IEventDataPtr pEventData);
	void ModifiedRenderComponentDelegate(IEventDataPtr pEventData);
	void DestroyActorDelegate(IEventDataPtr pEventData);
	void MoveActorDelegate(IEventDataPtr pEventData);

	void SetCamera(shared_ptr<CameraNode> pCamera) { m_pCamera = pCamera; }
	const shared_ptr<CameraNode> GetCamera() const { return m_pCamera; }

// 	bool Pick(const Vector3& rayDirection) { return m_pRootNode->VPick(this, rayDirection); }
	shared_ptr<IRenderer> GetRenderder() { return m_pRenderer; }

	void PushAndSetMatrix(const Matrix& worldMatrix)
	{
		if (m_MatrixStack.empty())
		{
			m_MatrixStack.push(worldMatrix);
		}
		else
		{
			m_MatrixStack.push(worldMatrix * m_MatrixStack.top());
		}
	}

	void PopMatrix()
	{
		if (!m_MatrixStack.empty())
			m_MatrixStack.pop();
	}

	Matrix GetTopMatrix()
	{
		if (m_MatrixStack.empty())
		{
			DEBUG_ERROR("push matrix error");
		}
		return m_MatrixStack.top();
	}

private:
	void RenderAlphaPass() {}

	shared_ptr<IRenderer> m_pRenderer;
	shared_ptr<SceneNode> m_pRootNode;
	shared_ptr<CameraNode> m_pCamera;

	std::stack<Matrix> m_MatrixStack;
	SceneActorMap m_ActorMap;
};

