#include "BaseGameLogic.h"
#include "BaseGameApp.h"
#include "../Actors/ActorFactory.h"
#include "../Actors/Actor.h"
#include "../ResourceCache/XmlResource.h"
#include "../EventManager/Events.h"
#include "../UserInterface/HumanView.h"
#include "boost/lexical_cast.hpp"

BaseGameLogic::BaseGameLogic()
	: m_Actors(),
	m_LastActorId(0),
	m_GameState(BGS_Initializing),
	m_GameViews(),
	m_pActorFactory(nullptr),
	m_HumanPlayersAttached(0),
	m_HumanGamesLoaded(0),
	m_AIPlayersAttached(0),
	m_IsProxy(false),
	m_RemotePlayerId(0),
	m_IsRenderDiagnostics(false),
	m_pPhysics(nullptr)
{
// 	m_pProjectManager = DEBUG_NEW ProjectManager;
// 	DEBUG_ASSERT(m_pProjectManager);
// 	m_pProjectManager->Initialize(g_pApp->GetResCache()->Match("world\\*.xml"));
	
// 	RegisterEngineScriptEvents();
// 	IEventManager::Get()->VAddListener(
// 		boost::bind(&BaseGameLogic::RequestDestroyActorDelegate, this, std::placeholders::_1), EvtData_Request_Destroy_Actor::sk_EventType);
}

BaseGameLogic::~BaseGameLogic()
{
	while (!m_GameViews.empty())
	{
		m_GameViews.pop_front();
	}

	SAFE_DELETE(m_pActorFactory);
// 	SAFE_DELETE(m_pProjectManager);

	for (auto& actor : m_Actors)
	{
		actor.second->Destroy();
	}
	m_Actors.clear();
}

bool BaseGameLogic::Init()
{
	m_pActorFactory = VCreateActorFactory();

	return true;
}

std::string BaseGameLogic::GetActorXml(ActorId id)
{
	StrongActorPtr pActor = MakeStrongPtr(VGetActor(id));
	if (pActor)
		return pActor->ToXml();
	else
		DEBUG_ERROR("Couldn't find actor: " + boost::lexical_cast<std::string>(id));

	return std::string();
}

bool BaseGameLogic::VLoadGame(const std::string& projectFile)
{
	unique_ptr<tinyxml2::XMLDocument> pDoc = unique_ptr<tinyxml2::XMLDocument>(DEBUG_NEW tinyxml2::XMLDocument());
	if (pDoc == nullptr || (pDoc->LoadFile(projectFile.c_str()) != tinyxml2::XML_SUCCESS))
	{
		DEBUG_ERROR("Failed to find level resource file: " + projectFile);
		return false;
	}

	tinyxml2::XMLElement *pRoot = pDoc->RootElement();
	if (pRoot == nullptr)
	{
		return false;
	}

	tinyxml2::XMLElement* pAsset = pRoot->FirstChildElement("AssetFile");
	if (pAsset != nullptr)
	{
		std::string assetFile = Utility::GetDirectory(projectFile) + "\\" + pAsset->GetText();
		LoadAssets(assetFile);
	}

	tinyxml2::XMLElement* pSceneService = pRoot->FirstChildElement("SceneService");
	if (pSceneService != nullptr)
	{
		tinyxml2::XMLElement* pCamera = pSceneService->FirstChildElement("Camera");
		for (auto& gameView : m_GameViews)
		{
			if (gameView->VGetType() == GameView_Human)
			{
				shared_ptr<HumanView> pHumanView = static_pointer_cast<HumanView, IGameView>(gameView);
				pHumanView->LoadGame(pCamera);
			}
		}

		tinyxml2::XMLElement* pGrid = pSceneService->FirstChildElement("Grid");
		VCreateActor(pGrid);
	}

	if (!VLoadGameDelegate())
		return false;

	if (m_IsProxy)
	{
		shared_ptr<EvtData_Remote_Environment_Loaded> pNewGameEvent(DEBUG_NEW EvtData_Remote_Environment_Loaded);
		IEventManager::Get()->VTriggerEvent(pNewGameEvent);
	}
	else
	{
		shared_ptr<EvtData_Environment_Loaded> pNewGameEvent(DEBUG_NEW EvtData_Environment_Loaded);
		IEventManager::Get()->VTriggerEvent(pNewGameEvent);
	}
	return true;
}

bool BaseGameLogic::VCreateNewProject(const std::string& project)
{
	std::string asset = project;
	asset.replace(asset.find_last_of('.'), asset.length(), ".asset");

	return CreateDefaultProject(project, Utility::GetFileName(asset)) &&
		CreateDefaultAsset(asset);
}

void BaseGameLogic::VSetProxy()
{
	m_IsProxy = true;

	IEventManager::Get()->VAddListener(
		boost::bind(&BaseGameLogic::RequestNewActorDelegate, this, _1), EvtData_Request_New_Actor::sk_EventType);

// 	m_pPhysics.reset(CreateNullPhysics());
}

void BaseGameLogic::VAddView(shared_ptr<IGameView> pView, ActorId actorId)
{
	uint64_t viewId = m_GameViews.size();
	m_GameViews.push_back(pView);
	pView->VOnAttach(viewId, actorId);
	pView->VOnInitGameViews();
}

void BaseGameLogic::VRemoveView(shared_ptr<IGameView> pView)
{
	m_GameViews.remove(pView);
}

StrongActorPtr BaseGameLogic::VCreateActor(
	tinyxml2::XMLElement *pActorRoot, const Matrix& initialTransform, ActorId serversActorId)
{
	DEBUG_ASSERT(m_pActorFactory != nullptr);
	DEBUG_ASSERT(pActorRoot != nullptr);

	if (!m_IsProxy && serversActorId != INVALID_ACTOR_ID)
		return StrongActorPtr();

	if (m_IsProxy && serversActorId == INVALID_ACTOR_ID)
		return StrongActorPtr();

	StrongActorPtr pActor = m_pActorFactory->CreateActor(pActorRoot, initialTransform, serversActorId);
	if (pActor)
	{
		m_Actors.insert(std::make_pair(pActor->GetActorId(), pActor));
		if (!m_IsProxy && (m_GameState == BGS_SpawningPlayersActors || m_GameState == BGS_Running))
		{
// 			shared_ptr<EvtData_Request_New_Actor> pNewActor(DEBUG_NEW EvtData_Request_New_Actor(actorResource, initialTransform, pActor->GetId()));
// 			IEventManager::Get()->VTriggerEvent(pNewActor);
		}
		shared_ptr<EvtData_New_Actor> pNewActorEvent(DEBUG_NEW EvtData_New_Actor(pActor->GetActorId()));
		IEventManager::Get()->VQueueEvent(pNewActorEvent);
		return pActor;
	}
	else
	{
		return StrongActorPtr();
	}
}

void BaseGameLogic::VDestroyActor(ActorId actorId)
{
	shared_ptr<EvtData_Destroy_Actor> pEvent(DEBUG_NEW EvtData_Destroy_Actor(actorId));
	IEventManager::Get()->VTriggerEvent(pEvent);

	auto findIt = m_Actors.find(actorId);
	if (findIt != m_Actors.end())
	{
		findIt->second->Destroy();
		m_Actors.erase(findIt);
	}
}

WeakActorPtr BaseGameLogic::VGetActor(ActorId actorId)
{
	auto findIt = m_Actors.find(actorId);
	if (findIt != m_Actors.end())
		return findIt->second;
	return WeakActorPtr();
}

void BaseGameLogic::VMoveActor(ActorId actorId, const Matrix& mat)
{
	 
}

void BaseGameLogic::VModifyActor(const ActorId actorId, tinyxml2::XMLElement* overrides)
{
	DEBUG_ASSERT(m_pActorFactory);
	if (!m_pActorFactory)
		return;

	auto findIt = m_Actors.find(actorId);
	if (findIt != m_Actors.end())
	{
		m_pActorFactory->ModifyActor(findIt->second, overrides);
	}
}

ActorFactory* BaseGameLogic::VCreateActorFactory()
{
	return DEBUG_NEW ActorFactory();
}

void BaseGameLogic::VOnUpdate(const GameTime& gameTime)
{
	switch (m_GameState)
	{
	case BGS_Initializing:
		VChangeState(BGS_MainMenu);
		break;

	case BGS_MainMenu:
		break;

	case BGS_LoadingGameEnvironment:
		break;

	case BGS_WaitingForPlayersToLoadEnvironment:
		VChangeState(BGS_SpawningPlayersActors);
		break;

	case BGS_SpawningPlayersActors:
		VChangeState(BGS_Running);
		break;

	case BGS_WaitingForPlayers:
		if (!g_pApp->GetGameConfig().m_Project.empty())
		{
			VChangeState(BGS_LoadingGameEnvironment);
		}
		break;

	case BGS_Running:
// 		m_pProcessManager->UpdateProcesses(gameTime);

		if (m_pPhysics && !m_IsProxy)
		{
			m_pPhysics->VOnUpdate(gameTime);
			m_pPhysics->VSyncVisibleScene();
		}

		break;

	default:
		DEBUG_ERROR("Unrecognized state.");
	}

	for (auto& gameView : m_GameViews)
	{
		gameView->VOnUpdate(gameTime);
	}

	for (auto& actor : m_Actors)
	{
		actor.second->Update(gameTime);
	}
}

void BaseGameLogic::VChangeState(BaseGameState gameState)
{
	if (gameState == BGS_WaitingForPlayers)
	{
		m_GameViews.pop_front();

// 		VSetProxy();
// 
// 		if (!g_pApp->AttachAsClient())
// 		{
// 			VChangeState(BGS_MainMenu);
// 			return;
// 		}
	}
	else if (gameState == BGS_LoadingGameEnvironment)
	{
		m_GameState = gameState;
		if (!g_pApp->VLoadGame())
		{
			DEBUG_ERROR("The game failed to load.");
			g_pApp->SetQuitting(true, false);
		}
		else
		{
			VChangeState(BGS_WaitingForPlayersToLoadEnvironment);
			return;
		}
	}

	m_GameState = gameState;
}

void BaseGameLogic::VRenderDiagnostics()
{
	if (m_IsRenderDiagnostics)
	{
// 		m_pPhysics->VRenderDiagnostics();
	}
}

void BaseGameLogic::RequestDestroyActorDelegate(IEventDataPtr pEventData)
{
// 	shared_ptr<EvtData_Request_Destroy_Actor> pCastEventData = static_pointer_cast<EvtData_Request_Destroy_Actor>(pEventData);
// 	VDestroyActor(pCastEventData->GetActorId());
}

void BaseGameLogic::MoveActorDelegate(IEventDataPtr pEventData)
{
	shared_ptr<EvtData_Move_Actor> pCastEventData = static_pointer_cast<EvtData_Move_Actor>(pEventData);
	VMoveActor(pCastEventData->GetActorId(), pCastEventData->GetMatrix());
}

void BaseGameLogic::RequestNewActorDelegate(IEventDataPtr pEventData)
{
	DEBUG_ASSERT(m_IsProxy);
	if (!m_IsProxy)
		return;

// 	shared_ptr<EvtData_Request_New_Actor> pCastEventData = static_pointer_cast<EvtData_Request_New_Actor>(pEventData);
// 
// 	StrongActorPtr pActor = VCreateActor(pCastEventData->GetActorResource(), NULL, pCastEventData->GetInitialTransform(), pCastEventData->GetServerActorId());
// 	if (pActor)
// 	{
// 		shared_ptr<EvtData_New_Actor> pNewActorEvent(DEBUG_NEW EvtData_New_Actor(pActor->GetId(), pCastEventData->GetViewId()));
// 		IEventManager::Get()->VQueueEvent(pNewActorEvent);
// 	}
}

bool BaseGameLogic::CreateDefaultProject(const std::string& project, const std::string& defautAsset)
{
	tinyxml2::XMLDocument outDoc;
	outDoc.InsertFirstChild(outDoc.NewDeclaration());

	tinyxml2::XMLElement* pRoot = outDoc.NewElement("Project");
	outDoc.InsertEndChild(pRoot);

	tinyxml2::XMLElement* pAsset = outDoc.NewElement("AssetFile");
	pAsset->InsertFirstChild(outDoc.NewText(defautAsset.c_str()));
	pRoot->InsertFirstChild(pAsset);

	tinyxml2::XMLElement* pScene = outDoc.NewElement("SceneService");
	pRoot->InsertEndChild(pScene);

	tinyxml2::XMLElement* pCamera = outDoc.NewElement("Camera");
	tinyxml2::XMLElement* pGrid = outDoc.NewElement("Grid");
	pScene->InsertFirstChild(pCamera);
	pScene->InsertEndChild(pGrid);

	pCamera->SetAttribute("type", "Perspective");
	tinyxml2::XMLElement* pTranslation = outDoc.NewElement("Translation");
	pTranslation->SetAttribute("x", 0.0f);
	pTranslation->SetAttribute("y", 0.0f);
	pTranslation->SetAttribute("z", 50.0f);
	tinyxml2::XMLElement* pRotation = outDoc.NewElement("Rotation");
	pRotation->SetAttribute("x", 45.0f);
	pRotation->SetAttribute("y", 45.0f);
	pRotation->SetAttribute("z", 0.0f);
	pCamera->InsertFirstChild(pTranslation);
	pCamera->InsertEndChild(pRotation);

	pGrid->SetAttribute("type", "Grid");
	tinyxml2::XMLElement* pComponent = outDoc.NewElement("GridRenderComponent");
	pGrid->InsertFirstChild(pComponent);

	tinyxml2::XMLElement* pMajorTicks = outDoc.NewElement("MajorTicksColor");
	pMajorTicks->SetAttribute("r", 0.2f);
	pMajorTicks->SetAttribute("g", 0.2f);
	pMajorTicks->SetAttribute("b", 0.2f);
	pMajorTicks->SetAttribute("a", 1.0f);
	tinyxml2::XMLElement* pTicks = outDoc.NewElement("TicksColor");
	pTicks->SetAttribute("r", 0.961f);
	pTicks->SetAttribute("g", 0.871f);
	pTicks->SetAttribute("b", 0.702f);
	pTicks->SetAttribute("a", 1.0f);
	pComponent->InsertFirstChild(pMajorTicks);
	pComponent->InsertEndChild(pTicks);

	tinyxml2::XMLPrinter printer;
	outDoc.Accept(&printer);

	return Utility::WriteFileData(project, printer.CStr(), printer.CStrSize() - 1);
}

bool BaseGameLogic::CreateDefaultAsset(const std::string& asset)
{
	tinyxml2::XMLDocument outDoc;
	outDoc.InsertFirstChild(outDoc.NewDeclaration());

	tinyxml2::XMLElement* pRoot = outDoc.NewElement("Assets");
	outDoc.InsertEndChild(pRoot);

	tinyxml2::XMLElement* pCameras = outDoc.NewElement("Cameras");
	tinyxml2::XMLElement* pEffects = outDoc.NewElement("Effects");
	tinyxml2::XMLElement* pGeometries = outDoc.NewElement("Geometries");
	tinyxml2::XMLElement* pLights = outDoc.NewElement("Lights");
	tinyxml2::XMLElement* pMaterials = outDoc.NewElement("Materials");
	tinyxml2::XMLElement* pModels = outDoc.NewElement("Models");
	tinyxml2::XMLElement* pScenes = outDoc.NewElement("Scenes");
	tinyxml2::XMLElement* pSkybox = outDoc.NewElement("Skybox");
	tinyxml2::XMLElement* pTextures = outDoc.NewElement("Textures");
	pRoot->InsertFirstChild(pCameras);
	pRoot->InsertEndChild(pEffects);
	pRoot->InsertEndChild(pGeometries);
	pRoot->InsertEndChild(pLights);
	pRoot->InsertEndChild(pMaterials);
	pRoot->InsertEndChild(pModels);
	pRoot->InsertEndChild(pScenes);
	pRoot->InsertEndChild(pSkybox);
	pRoot->InsertEndChild(pTextures);

	tinyxml2::XMLElement* pEffect = outDoc.NewElement("Effect");
	pEffect->SetAttribute("name", "DefaultEffect");
	pEffect->InsertFirstChild(outDoc.NewText("Effects\\DefaultEffect.fx"));
	pEffects->InsertFirstChild(pEffect);

	pMaterials->SetAttribute("name", "DefaultMaterial");
	tinyxml2::XMLElement* pTechnique = outDoc.NewElement("Technique");
	pTechnique->SetAttribute("name", "main11");
	tinyxml2::XMLElement* pPass = outDoc.NewElement("Pass");
	pPass->InsertFirstChild(outDoc.NewText("p0"));
	pTechnique->InsertFirstChild(pPass);
	pMaterials->InsertFirstChild(pTechnique);

	tinyxml2::XMLElement* pImage = outDoc.NewElement("Image");
	pImage->SetAttribute("name", "DefaultColor");
	pImage->InsertFirstChild(outDoc.NewText("Textures\\DefaultColor.dds"));
	pTextures->InsertFirstChild(pImage);

	pSkybox->SetAttribute("type", "Skybox");
	tinyxml2::XMLElement* pComponent = outDoc.NewElement("SkyboxRenderComponent");
	pSkybox->InsertFirstChild(pComponent);

	tinyxml2::XMLElement* pColor = outDoc.NewElement("Color");
	pColor->SetAttribute("r", 1.0f);
	pColor->SetAttribute("g", 1.0f);
	pColor->SetAttribute("b", 1.0f);
	pColor->SetAttribute("a", 1.0f);
	tinyxml2::XMLElement* pTexture = outDoc.NewElement("Texture");
	pTexture->SetText("Textures\\Skybox.dds");
	pComponent->InsertFirstChild(pColor);
	pComponent->InsertEndChild(pTexture);

	tinyxml2::XMLPrinter printer;
	outDoc.Accept(&printer);

	return Utility::WriteFileData(asset, printer.CStr(), printer.CStrSize() - 1);
}

bool BaseGameLogic::LoadAssets(const std::string& asset)
{
	unique_ptr<tinyxml2::XMLDocument> pDoc = unique_ptr<tinyxml2::XMLDocument>(DEBUG_NEW tinyxml2::XMLDocument());
	if (pDoc == nullptr || (pDoc->LoadFile(asset.c_str()) != tinyxml2::XML_SUCCESS))
	{
		DEBUG_ERROR("Failed to find level resource file: " + asset);
		return false;
	}

	tinyxml2::XMLElement *pRoot = pDoc->RootElement();
	if (pRoot == nullptr)
	{
		return false;
	}

	tinyxml2::XMLElement* pSkybox = pRoot->FirstChildElement("Skybox");
	if (pSkybox != nullptr)
	{
		VCreateActor(pSkybox);
	}

	tinyxml2::XMLElement* pModels = pRoot->FirstChildElement("Models");
	if (pModels)
	{
		for (tinyxml2::XMLElement* pNode = pModels->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
		{
			VCreateActor(pNode);
		}
	}

	return true;
}
