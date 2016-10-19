#pragma once

class Actor;
class ActorComponent;

typedef uint32_t ActorId;
typedef uint32_t ComponentId;

const ActorId INVALID_ACTOR_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

typedef shared_ptr<Actor> StrongActorPtr;
typedef weak_ptr<Actor> WeakActorPtr;
typedef shared_ptr<ActorComponent> StrongActorComponentPtr;
typedef weak_ptr<ActorComponent> WeakActorComponentPtr;

template<class T>
struct SortBy_SharedPtr_Content
{
	bool operator()(const shared_ptr<T> &lhs, const shared_ptr<T> &rhs) const
	{
		return *lhs < *rhs;
	}
};

struct AppMsg
{
	HWND m_hWnd;
	UINT m_uMsg;
	WPARAM m_wParam;
	LPARAM m_lParam;
};

class IScreenElement
{
public:
	IScreenElement() {}
	~IScreenElement() {}

	virtual HRESULT VOnRestore() = 0;
	virtual HRESULT VOnLostDevice() = 0;
	virtual HRESULT VOnDestoryDevice() = 0;
	virtual void VOnUpdate(double fTime, float fElapsedTime) = 0;
	virtual HRESULT VOnRender(double fTime, float fElapsedTime) = 0;

	virtual int VGetZOrder() const = 0;
	virtual void VSetZOrder(int zOrder) = 0;
	virtual bool VIsVisible() const = 0;
	virtual void VSetVisible(bool visible) = 0;

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) = 0;
	virtual bool const operator<(const IScreenElement& other) { return VGetZOrder() < other.VGetZOrder(); }
};

class IGamePhysics;

class IGameLogic
{
public:
	IGameLogic() {}
	~IGameLogic() {}

	virtual WeakActorPtr VGetActor(ActorId actorId) = 0;
	virtual StrongActorPtr VCreateActor(
		const std::string& actorResource, TiXmlElement *overrides,
		const Matrix& initialTransform, ActorId serversActorId = INVALID_ACTOR_ID) = 0;
	virtual void VDestroyActor(ActorId actorId) = 0;
	virtual void VMoveActor(ActorId actorId, const Matrix& mat) = 0;

	virtual void VOnUpdate(double fTime, float fElapsedTime) = 0;
	virtual bool VLoadGame(const std::string& projectXml) = 0;
	virtual void VSetProxy() = 0;
	virtual void VChangeState(enum BaseGameState newState) = 0;
	virtual shared_ptr<IGamePhysics> VGetGamePhysics(void) = 0;
};

enum GameViewType
{
	GameView_Human,
	GameView_Remote,
	GameView_AI,
	GameView_Recorder,
	GameView_Other
};

typedef uint32_t GameViewId;
const GameViewId INVALID_GAME_VIEW_ID = 0xffffffff;

class IGameView
{
public:
	IGameView() {}
	~IGameView() {}

	virtual HRESULT VOnRestore() = 0;
	virtual HRESULT VOnLostDevice() = 0;
	virtual HRESULT VOnDestoryDevice() = 0;
	virtual void VOnUpdate(double fTime, float fElapsedTime) = 0;
	virtual void VOnRender(double fTime, float fElapsedTime) = 0;
	virtual void VOnAttach(GameViewId viewId, ActorId actorId) = 0;
	virtual GameViewType VGetType() = 0;
	virtual GameViewId VGetId() const = 0;
	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) = 0;
};

typedef std::list<shared_ptr<IScreenElement> > ScreenElementList;
typedef std::list<shared_ptr<IGameView> > GameViewList;

class IKeyboardHandler
{
public:
	virtual bool VOnKeyDown(uint8_t c) = 0;
	virtual bool VOnKeyUp(uint8_t c) = 0;
};

class IPointerHandler
{
public:
	virtual bool VOnPointerMove(const Vector2 &pos, int radius) = 0;
	virtual bool VOnPointerButtonDown(const Vector2 &pos, int radius, const std::string &buttonName) = 0;
	virtual bool VOnPointerButtonUp(const Vector2 &pos, int radius, const std::string &buttonName) = 0;
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
	virtual uint32_t VGetLoadedResourceSize(char *rawBuffer, uint32_t rawSize) = 0;
	virtual bool VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle) = 0;
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
class SceneNodeProperties;

class IRenderState
{
public:
	virtual std::string VToString() = 0;
};

class IRenderer
{
public:
	IRenderer() {}
	~IRenderer() {}

	virtual void VSetBackgroundColor(const Color& color) = 0;
	virtual HRESULT VOnRestore() = 0;
	virtual bool VPreRender() = 0;
	virtual bool VPostRender() = 0;
	virtual void VShutdown() = 0;
};

class ISceneNode
{
public:
	ISceneNode() {}
	~ISceneNode() {}

	virtual const SceneNodeProperties* VGet() const = 0;

	virtual HRESULT VOnUpdate(Scene* pScene, double fTime, float fElapsedTime) = 0;
	virtual HRESULT VOnRestore(Scene* pScene) = 0;
	virtual HRESULT VOnLostDevice(Scene* pScene) = 0;
	virtual HRESULT VOnDestoryDevice(Scene* pScene) = 0;
	virtual LRESULT CALLBACK VOnMsgProc(Scene *pScene, AppMsg msg) = 0;

	virtual HRESULT VPreRender(Scene* pScene) = 0;
	virtual HRESULT VRender(Scene* pScene, double fTime, float fElapsedTime) = 0;
	virtual HRESULT VRenderChildren(Scene* pScene, double fTime, float fElapsedTime) = 0;
	virtual HRESULT VPostRender(Scene* pScene) = 0;
	virtual bool VIsVisible(Scene* pScene) const = 0;

	virtual bool VAddChild(shared_ptr<ISceneNode> child) = 0;
	virtual bool VRemoveChild(ActorId actorId) = 0;
};

class IGamePhysics
{
public:
	IGamePhysics() {}
	~IGamePhysics() {}

	virtual bool VInitialize() = 0;
	virtual void VSyncVisibleScene() = 0;
	virtual void VOnUpdate(double fTime, float fElapsedTime) = 0;
};