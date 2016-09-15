#pragma once

#include "RenderEngineBase.h"

struct AppMsg
{
	HWND m_hWnd;
	UINT m_uMsg;
	WPARAM m_wParam;
	LPARAM m_lParam;
};

class Actor;
class ActorComponent;

typedef unsigned int ActorId;
typedef unsigned int ComponentId;

const ActorId INVALID_ACTOR_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

typedef shared_ptr<Actor> StrongActorPtr;
typedef weak_ptr<Actor> WeakActorPtr;
typedef shared_ptr<ActorComponent> StrongActorComponentPtr;
typedef weak_ptr<ActorComponent> WeakActorComponentPtr;

class IScreenElement
{
public:
	virtual HRESULT VOnRestore() = 0;
	virtual HRESULT VOnLostDevice() = 0;
	virtual HRESULT VOnRender(double fTime, float fElapsedTime) = 0;
	virtual void VOnUpdate(int deltaMilliseconds) = 0;

	virtual bool VIsVisible() const = 0;
	virtual void VSetVisible(bool visible) = 0;

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) = 0;

	virtual ~IScreenElement() { };
};

class IGameLogic
{
public:
	virtual WeakActorPtr VGetActor(const ActorId id) = 0;
	virtual StrongActorPtr VCreateActor(const std::string &actorResource, TiXmlElement *overrides, const Matrix *initialTransform = NULL, const ActorId serversActorId = INVALID_ACTOR_ID) = 0;
	virtual void VDestroyActor(const ActorId actorId) = 0;
	virtual bool VLoadGame(const char* levelResource) = 0;
	virtual void VOnUpdate(float time, float elapsedTime) = 0;
};

typedef unsigned int GameViewId;
extern const GameViewId gc_InvalidGameViewId;

class IGameView
{
public:
	virtual HRESULT VOnRestore() = 0;
	virtual void VOnRender(double fTime, float fElapsedTime) = 0;
	virtual HRESULT VOnLostDevice() = 0;
	virtual GameViewId VGetId() const = 0;
	virtual void VOnAttach(GameViewId vid, ActorId aid) = 0;

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) = 0;
	virtual void VOnUpdate(const int deltaMilliseconds) = 0;

	virtual ~IGameView() { };
};



typedef std::list<shared_ptr<IScreenElement> > ScreenElementList;
typedef std::list<shared_ptr<IGameView> > GameViewList;

class IKeyboardHandler
{
public:
	virtual bool VOnKeyDown(const BYTE c) = 0;
	virtual bool VOnKeyUp(const BYTE c) = 0;
};

class IPointerHandler
{
public:
	virtual bool VOnPointerMove(const Vector2 &pos, const int radius) = 0;
	virtual bool VOnPointerButtonDown(const Vector2 &pos, const int radius, const std::string &buttonName) = 0;
	virtual bool VOnPointerButtonUp(const Vector2 &pos, const int radius, const std::string &buttonName) = 0;
};

class Resource;
class IResourceFile;
class ResHandle;

class IResourceLoader
{
public:
	virtual std::string VGetPattern() = 0;
	virtual bool VUseRawFile() = 0;
	virtual bool VDiscardRawBufferAfterLoad() = 0;
	virtual bool VAddNullZero() { return false; }
	virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) = 0;
	virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle) = 0;
};

class IResourceFile
{
public:
	virtual bool VOpen() = 0;
	virtual int VGetRawResourceSize(const Resource &r) = 0;
	virtual int VGetRawResource(const Resource &r, char *buffer) = 0;
	virtual int VGetNumResources() const = 0;
	virtual std::string VGetResourceName(int num) const = 0;
	virtual bool VIsUsingDevelopmentDirectories(void) const = 0;
	virtual ~IResourceFile() { }
};

enum RenderPass
{
	RenderPass_0,
	RenderPass_Static = RenderPass_0,
	RenderPass_Actor,
	RenderPass_Sky,
	RenderPass_NotRendered,
	RenderPass_Last
};




class Scene;
class SceneNodeProperty;
class RayCast;
class LightNode;

typedef std::list<shared_ptr<LightNode> > Lights;


class IRenderState
{
public:
	virtual std::string VToString() = 0;
};

class IRenderer
{
public:
	virtual void VSetBackgroundColor(Color color) = 0;
	virtual HRESULT VOnRestore() = 0;
	virtual void VShutdown() = 0;
	virtual bool VPreRender() = 0;
	virtual bool VPostRender() = 0;
	virtual void VSetWorldTransform(const Matrix *m) = 0;
	virtual void VSetViewTransform(const Matrix *m) = 0;
	virtual void VSetProjectionTransform(const Matrix *m) = 0;
};

class ISceneNode
{
public:
	virtual const SceneNodeProperty* const VGet() const = 0;

	virtual void VSetTransform(const Matrix& toWorld, const Matrix& fromWorld) = 0;

	virtual HRESULT VOnUpdate(Scene* pScene, DWORD const elapsedMs) = 0;
	virtual HRESULT VOnRestore(Scene* pScene) = 0;

	virtual HRESULT VPreRender(Scene* pScene) = 0;
	virtual bool VIsVisible(Scene* pScene) const = 0;
	virtual HRESULT VRender(Scene* pScene) = 0;
	virtual HRESULT VRenderChildren(Scene* pScene) = 0;
	virtual HRESULT VPostRender(Scene* pScene) = 0;

	virtual bool VAddChild(shared_ptr<ISceneNode> kid) = 0;
	virtual bool VRemoveChild(ActorId id) = 0;
	virtual HRESULT VOnLostDevice(Scene* pScene) = 0;
	virtual HRESULT VPick(Scene* pScene, RayCast *pRayCast) = 0;

	virtual ~ISceneNode() { };
};

#include "RenderEngineApp/RenderEngineApp.h"
#include "RenderEngineApp/BaseGameLogic.h"
#include "UserInterface/HumanView.h"
#include "Graphics3D/MovementController.h"
#include "Graphics3D/SceneNode.h"
