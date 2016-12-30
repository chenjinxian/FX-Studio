#pragma once

class Actor;
class ActorComponent;

typedef std::string ActorType;
typedef uint64_t ActorId;
typedef uint64_t ComponentId;

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

class GameTime;

class IScreenElement
{
public:
	IScreenElement() {}
	~IScreenElement() {}

	virtual HRESULT VOnInitScreenElements() = 0;
	virtual HRESULT VOnDeleteScreenElements() = 0;
	virtual void VOnUpdate(const GameTime& gameTime) = 0;
	virtual HRESULT VOnRender(const GameTime& gameTime) = 0;

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
		std::string actorXml, const Matrix& initialTransform = Matrix::Identity, ActorId serversActorId = INVALID_ACTOR_ID) = 0;
	virtual StrongActorPtr VCreateActor(
		tinyxml2::XMLElement *pActorRoot, const Matrix& initialTransform = Matrix::Identity, ActorId serversActorId = INVALID_ACTOR_ID) = 0;
	virtual void VDestroyActor(ActorId actorId) = 0;
	virtual void VMoveActor(ActorId actorId, const Matrix& mat) = 0;

	virtual void VOnUpdate(const GameTime& gameTime) = 0;
	virtual bool VLoadGame(const std::string& projectXml) = 0;
	virtual bool VCreateNewProject(const std::string& project) = 0;
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

typedef uint64_t GameViewId;
const GameViewId INVALID_GAME_VIEW_ID = (uint64_t)-1;

class IGameView
{
public:
	IGameView() {}
	~IGameView() {}

	virtual HRESULT VOnInitGameViews() = 0;
	virtual HRESULT VOnDeleteGameViews() = 0;
	virtual void VOnUpdate(const GameTime& gameTime) = 0;
	virtual void VOnRender(const GameTime& gameTime) = 0;
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
	virtual bool VOnPointerWheel(int16_t delta) = 0;
	virtual bool VOnPointerButtonDown(const Vector2 &pos, int radius, const std::string &buttonName) = 0;
	virtual bool VOnPointerButtonUp(const Vector2 &pos, int radius, const std::string &buttonName) = 0;
};

class Resource;
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
	virtual ~IResourceFile() { }
	virtual bool VOpen() = 0;
	virtual int VGetRawResourceSize(const Resource &r) = 0;
	virtual int VGetRawResource(const Resource &r, char *buffer) = 0;
	virtual int VGetNumResources() const = 0;
	virtual std::string VGetResourceName(int num) const = 0;
	virtual bool VIsUsingDevelopmentDirectories(void) const = 0;
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
class RayCast;
class LightNode;

typedef std::list<shared_ptr<LightNode> > Lights;

class IRenderState
{
public:
	virtual std::string VToString() = 0;
};

class IResourceExtraData;
class IRenderer
{
public:
	IRenderer() {}
	~IRenderer() {}

	enum IndexFormat
	{
		Format_unknow = 0,
		Format_uint32 = 42,
		Format_uint16 = 57
	};

	virtual bool VInitRenderer(HWND hWnd) = 0;
	virtual void VDeleteRenderer() = 0;
	virtual void VResizeSwapChain() = 0;
	virtual bool VPreRender(const GameTime& gameTime) = 0;
	virtual bool VPostRender() = 0;
	virtual void VSetBackgroundColor(const Color& color) = 0;

	virtual void VInputSetup(D3D_PRIMITIVE_TOPOLOGY topology, ID3D11InputLayout* pInputLayout) = 0;
	virtual void VSetVertexBuffers(ID3D11Buffer* pVertexBuffer, uint32_t* stride, uint32_t* offset) = 0;
	virtual void VSetIndexBuffer(ID3D11Buffer* pIndexBuffer, IndexFormat format, uint32_t offset) = 0;
	virtual void VDrawMesh(uint32_t indexCount, uint32_t startIndex, int32_t baseVertex, ID3DX11EffectPass* pD3DX11EffectPass) = 0;

	virtual bool VCompileShaderFromMemory(const void* pBuffer, uint32_t lenght, shared_ptr<IResourceExtraData> pExtraData) = 0;
	virtual bool VCreateShaderFromMemory(const void* pBuffer, uint32_t lenght, shared_ptr<IResourceExtraData> pExtraData) = 0;
	virtual bool VCreateDDSTextureResoure(char *rawBuffer, uint32_t rawSize, shared_ptr<IResourceExtraData> pExtraData) = 0;
	virtual bool VCreateWICTextureResoure(char *rawBuffer, uint32_t rawSize, shared_ptr<IResourceExtraData> pExtraData) = 0;
	virtual const std::string& VGetDeviceName() = 0;
};

class ISceneNode
{
public:
	ISceneNode() {}
	~ISceneNode() {}

	virtual const SceneNodeProperties* VGet() const = 0;
	virtual void VSetTransform(const Matrix& worldMatrix) = 0;

	virtual HRESULT VOnUpdate(Scene* pScene, const GameTime& gameTime) = 0;
	virtual HRESULT VOnInitSceneNode(Scene* pScene) = 0;
	virtual HRESULT VOnDeleteSceneNode(Scene* pScene) = 0;

	virtual HRESULT VPreRender(Scene* pScene) = 0;
	virtual HRESULT VRender(Scene* pScene, const GameTime& gameTime) = 0;
	virtual HRESULT VRenderChildren(Scene* pScene, const GameTime& gameTime) = 0;
	virtual HRESULT VPostRender(Scene* pScene) = 0;
	virtual bool VIsVisible(Scene* pScene) const = 0;

	virtual bool VAddChild(shared_ptr<ISceneNode> child) = 0;
	virtual bool VRemoveChild(ActorId actorId) = 0;
	virtual ActorId VPick(Scene* pScene, int cursorX, int cursorY) = 0;
};

class IGamePhysics
{
public:
	IGamePhysics() {}
	~IGamePhysics() {}

	virtual bool VInitialize() = 0;
	virtual void VSyncVisibleScene() = 0;
	virtual void VOnUpdate(const GameTime& gameTime) = 0;
};
