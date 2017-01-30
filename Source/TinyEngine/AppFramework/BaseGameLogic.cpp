#include "BaseGameLogic.h"
#include "BaseGameApp.h"
#include "../Actors/ActorFactory.h"
#include "../Actors/Actor.h"
#include "../ResourceCache/XmlResource.h"
#include "../ResourceCache/ShaderResource.h"
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

	tinyxml2::XMLElement* pEditorCamera = pRoot->FirstChildElement("EditorCamera");
	if (pEditorCamera != nullptr)
	{
		for (auto& gameView : m_GameViews)
		{
			if (gameView->VGetType() == GameView_Human)
			{
				shared_ptr<HumanView> pHumanView = static_pointer_cast<HumanView, IGameView>(gameView);
				pHumanView->LoadGame(pEditorCamera);
			}
		}
	}

	for (tinyxml2::XMLElement* pSceneNode = pEditorCamera->NextSiblingElement(); pSceneNode != nullptr; pSceneNode = pSceneNode->NextSiblingElement())
	{
		tinyxml2::XMLElement* pSkybox = pSceneNode->FirstChildElement("Skybox");
		VCreateActor(pSkybox);

		tinyxml2::XMLElement* pGrid = pSceneNode->FirstChildElement("Grid");
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

StrongActorPtr BaseGameLogic::VCreateActor(std::string actorXml, const Matrix& initialTransform /*= Matrix::Identity*/, ActorId serversActorId /*= INVALID_ACTOR_ID*/)
{
	unique_ptr<tinyxml2::XMLDocument> pDoc = unique_ptr<tinyxml2::XMLDocument>(DEBUG_NEW tinyxml2::XMLDocument());
	if (pDoc == nullptr || (pDoc->Parse(actorXml.c_str(), actorXml.length()) != tinyxml2::XML_SUCCESS))
	{
		return false;
	}

	tinyxml2::XMLElement *pRoot = pDoc->RootElement();
	if (pRoot == nullptr)
	{
		return false;
	}

	return VCreateActor(pRoot, initialTransform, serversActorId);
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
	pRoot->SetAttribute("type", "Project");

	tinyxml2::XMLElement* pAsset = outDoc.NewElement("AssetFile");
	pAsset->SetText(defautAsset.c_str());
	pRoot->InsertFirstChild(pAsset);

	tinyxml2::XMLElement* pEditorCamera = outDoc.NewElement("EditorCamera");
	pRoot->InsertEndChild(pEditorCamera);
	{
		pEditorCamera->SetAttribute("type", "EditorCamera");
		tinyxml2::XMLElement* pTranslation = outDoc.NewElement("Translation");
		pTranslation->SetAttribute("x", 0.0f);
		pTranslation->SetAttribute("y", 0.0f);
		pTranslation->SetAttribute("z", 5.0f);
		tinyxml2::XMLElement* pRotation = outDoc.NewElement("Rotation");
		pRotation->SetAttribute("x", 45.0f);
		pRotation->SetAttribute("y", 45.0f);
		pRotation->SetAttribute("z", 0.0f);
		pEditorCamera->InsertFirstChild(pTranslation);
		pEditorCamera->InsertEndChild(pRotation);
	}

	tinyxml2::XMLElement* pScene = outDoc.NewElement("DefaultScene");
	pRoot->InsertEndChild(pScene);
	pScene->SetAttribute("type", "Scene");

	{
		tinyxml2::XMLElement* pSkybox = outDoc.NewElement("Skybox");
		tinyxml2::XMLElement* pGrid = outDoc.NewElement("Grid");
		pScene->InsertEndChild(pSkybox);
		pScene->InsertEndChild(pGrid);

		pSkybox->SetAttribute("type", "Skybox");
		tinyxml2::XMLElement* pSkyboxRenderComponent = outDoc.NewElement("SkyboxRenderComponent");
		pSkybox->InsertFirstChild(pSkyboxRenderComponent);

		tinyxml2::XMLElement* pColor = outDoc.NewElement("Color");
		pColor->SetAttribute("r", 1.0f);
		pColor->SetAttribute("g", 1.0f);
		pColor->SetAttribute("b", 1.0f);
		pColor->SetAttribute("a", 1.0f);
		tinyxml2::XMLElement* pTexture = outDoc.NewElement("Texture");
		pTexture->SetText("Textures\\Skybox.dds");
		pSkyboxRenderComponent->InsertFirstChild(pColor);
		pSkyboxRenderComponent->InsertEndChild(pTexture);

		pGrid->SetAttribute("type", "Grid");
		tinyxml2::XMLElement* pGridRenderComponent = outDoc.NewElement("GridRenderComponent");
		pGrid->InsertFirstChild(pGridRenderComponent);

		tinyxml2::XMLElement* pGridColor = outDoc.NewElement("Color");
		pGridColor->SetAttribute("r", 1.0f);
		pGridColor->SetAttribute("g", 1.0f);
		pGridColor->SetAttribute("b", 1.0f);
		pGridColor->SetAttribute("a", 1.0f);
		tinyxml2::XMLElement* pGridTexture = outDoc.NewElement("Texture");
		pGridTexture->SetText("Textures\\Grid.dds");
		tinyxml2::XMLElement* pGridSize = outDoc.NewElement("GridSize");
		pGridSize->SetAttribute("x", 10.0f);
		pGridSize->SetAttribute("y", 10.0f);
		tinyxml2::XMLElement* pTicksInterval = outDoc.NewElement("TicksInterval");
		pTicksInterval->SetText("1.0");
		pGridRenderComponent->InsertFirstChild(pGridColor);
		pGridRenderComponent->InsertEndChild(pGridTexture);
		pGridRenderComponent->InsertEndChild(pGridSize);
		pGridRenderComponent->InsertEndChild(pTicksInterval);
	}

	tinyxml2::XMLPrinter printer;
	outDoc.Accept(&printer);

	return Utility::WriteFileData(project, printer.CStr(), printer.CStrSize());
}

void BaseGameLogic::AddVariableElement(
	tinyxml2::XMLDocument& outDoc,
	tinyxml2::XMLElement* pVariables,
	const char* name, const char* value, std::vector<tinyxml2::XMLElement*> pAnnotations)
{
	tinyxml2::XMLElement* pChildVariable = outDoc.NewElement("Variable");
	pChildVariable->SetAttribute("name", name);
	pChildVariable->SetAttribute("value", value);
	for (auto annotation : pAnnotations)
	{
		pChildVariable->InsertEndChild(annotation);
	}
	pVariables->InsertEndChild(pChildVariable);
}

bool BaseGameLogic::CreateDefaultAsset(const std::string& asset)
{
	tinyxml2::XMLDocument outDoc;

	tinyxml2::XMLElement* pRoot = outDoc.NewElement("Assets");
	outDoc.InsertEndChild(pRoot);

	tinyxml2::XMLElement* pEffects = outDoc.NewElement("Effects");
	tinyxml2::XMLElement* pModels = outDoc.NewElement("Models");
	tinyxml2::XMLElement* pTextures = outDoc.NewElement("Textures");
	pRoot->InsertEndChild(pEffects);
	pRoot->InsertEndChild(pModels);
	pRoot->InsertEndChild(pTextures);

	tinyxml2::XMLElement* pChildEffect = outDoc.NewElement("Effect");
	pChildEffect->SetAttribute("name", "DefaultEffect");
	pChildEffect->SetAttribute("object", "Effects\\DefaultEffect.fx");
	pChildEffect->SetAttribute("source", "Effects\\DefaultEffect.fx");
	pEffects->InsertEndChild(pChildEffect);

	tinyxml2::XMLElement* pTechniques = outDoc.NewElement("Techniques");
	tinyxml2::XMLElement* pVariables = outDoc.NewElement("Variables");
	pChildEffect->InsertEndChild(pTechniques);
	pChildEffect->InsertEndChild(pVariables);

	tinyxml2::XMLElement* pChildTechnique = outDoc.NewElement("Technique");
	pChildTechnique->SetAttribute("name", "main11");
	pChildTechnique->SetAttribute("checked", true);
	tinyxml2::XMLElement* pChildPass = outDoc.NewElement("Pass");
	pChildPass->InsertEndChild(outDoc.NewText("p0"));
	pChildTechnique->InsertEndChild(pChildPass);
	pTechniques->InsertEndChild(pChildTechnique);

	{
		tinyxml2::XMLElement* pAnnotation = outDoc.NewElement("UIWidget");
		pAnnotation->SetText("None");
		std::vector<tinyxml2::XMLElement*> annotations;
		annotations.push_back(pAnnotation);
		AddVariableElement(outDoc, pVariables, "WorldITXf", "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0", annotations);
	}

	{
		tinyxml2::XMLElement* pAnnotation = outDoc.NewElement("UIWidget");
		pAnnotation->SetText("None");
		std::vector<tinyxml2::XMLElement*> annotations;
		annotations.push_back(pAnnotation);
		AddVariableElement(outDoc, pVariables, "WvpXf", "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0", annotations);
	}

	{
		tinyxml2::XMLElement* pAnnotation = outDoc.NewElement("UIWidget");
		pAnnotation->SetText("None");
		std::vector<tinyxml2::XMLElement*> annotations;
		annotations.push_back(pAnnotation);
		AddVariableElement(outDoc, pVariables, "WorldXf", "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0", annotations);
	}

	{
		tinyxml2::XMLElement* pAnnotation = outDoc.NewElement("UIWidget");
		pAnnotation->SetText("None");
		std::vector<tinyxml2::XMLElement*> annotations;
		annotations.push_back(pAnnotation);
		AddVariableElement(outDoc, pVariables, "ViewIXf", "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0", annotations);
	}

	{
		std::vector<tinyxml2::XMLElement*> annotations;
		tinyxml2::XMLElement* pAnnotation1 = outDoc.NewElement("Object");
		pAnnotation1->SetText("PointLight0");
		tinyxml2::XMLElement* pAnnotation2 = outDoc.NewElement("Space");
		pAnnotation2->SetText("World");
		annotations.push_back(pAnnotation1);
		annotations.push_back(pAnnotation2);
		AddVariableElement(outDoc, pVariables, "LightPos", "10 10 -10", annotations);
	}

	{
		std::vector<tinyxml2::XMLElement*> annotations;
		tinyxml2::XMLElement* pAnnotation1 = outDoc.NewElement("UIName");
		pAnnotation1->SetText("Ambient Lighting");
		tinyxml2::XMLElement* pAnnotation2 = outDoc.NewElement("UIWidget");
		pAnnotation2->SetText("Color");
		annotations.push_back(pAnnotation1);
		annotations.push_back(pAnnotation2);
		AddVariableElement(outDoc, pVariables, "AmbiColor", "0.1 0.1 0.1", annotations);
	}

	{
		std::vector<tinyxml2::XMLElement*> annotations;
		tinyxml2::XMLElement* pAnnotation1 = outDoc.NewElement("UIName");
		pAnnotation1->SetText("Surface Color");
		tinyxml2::XMLElement* pAnnotation2 = outDoc.NewElement("UIWidget");
		pAnnotation2->SetText("Color");
		annotations.push_back(pAnnotation1);
		annotations.push_back(pAnnotation2);
		AddVariableElement(outDoc, pVariables, "SurfColor", "0.8 0.8 1", annotations);
	}

	{
		std::vector<tinyxml2::XMLElement*> annotations;
		tinyxml2::XMLElement* pAnnotation1 = outDoc.NewElement("UIName");
		pAnnotation1->SetText("Specular Intensity");
		tinyxml2::XMLElement* pAnnotation2 = outDoc.NewElement("UIWidget");
		pAnnotation2->SetText("slider");
		tinyxml2::XMLElement* pAnnotation3 = outDoc.NewElement("UIMin");
		pAnnotation3->SetText("0");
		tinyxml2::XMLElement* pAnnotation4 = outDoc.NewElement("UIMax");
		pAnnotation4->SetText("1");
		tinyxml2::XMLElement* pAnnotation5 = outDoc.NewElement("UIStep");
		pAnnotation5->SetText("0.01");
		annotations.push_back(pAnnotation1);
		annotations.push_back(pAnnotation2);
		annotations.push_back(pAnnotation3);
		annotations.push_back(pAnnotation4);
		annotations.push_back(pAnnotation5);
		AddVariableElement(outDoc, pVariables, "Ks", "0.5", annotations);
	}

	{
		std::vector<tinyxml2::XMLElement*> annotations;
		tinyxml2::XMLElement* pAnnotation1 = outDoc.NewElement("UIName");
		pAnnotation1->SetText("Specular Power");
		tinyxml2::XMLElement* pAnnotation2 = outDoc.NewElement("UIWidget");
		pAnnotation2->SetText("slider");
		tinyxml2::XMLElement* pAnnotation3 = outDoc.NewElement("UIMin");
		pAnnotation3->SetText("1");
		tinyxml2::XMLElement* pAnnotation4 = outDoc.NewElement("UIMax");
		pAnnotation4->SetText("128");
		tinyxml2::XMLElement* pAnnotation5 = outDoc.NewElement("UIStep");
		pAnnotation5->SetText("1");
		annotations.push_back(pAnnotation1);
		annotations.push_back(pAnnotation2);
		annotations.push_back(pAnnotation3);
		annotations.push_back(pAnnotation4);
		annotations.push_back(pAnnotation5);
		AddVariableElement(outDoc, pVariables, "SpecExpon", "30", annotations);
	}

	tinyxml2::XMLElement* pChildTexture = outDoc.NewElement("Texture");
	pChildTexture->SetAttribute("name", "DefaultColor");
	pChildTexture->InsertEndChild(outDoc.NewText("Textures\\DefaultColor.dds"));
	pTextures->InsertEndChild(pChildTexture);

	tinyxml2::XMLPrinter printer;
	outDoc.Accept(&printer);

	return Utility::WriteFileData(asset, printer.CStr(), printer.CStrSize());
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

	tinyxml2::XMLElement* pEffects = pRoot->FirstChildElement("Effects");
	if (pEffects != nullptr)
	{
		for (tinyxml2::XMLElement* pNode = pEffects->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
		{
			Resource effectRes(pNode->Attribute("object"));
			shared_ptr<ResHandle> pEffectResHandle = g_pApp->GetResCache()->GetHandle(&effectRes);
			if (pEffectResHandle != nullptr)
			{
				shared_ptr<HlslResourceExtraData> extra = static_pointer_cast<HlslResourceExtraData>(pEffectResHandle->GetExtraData());
				if (extra != nullptr)
				{
					Effect* pEffect = extra->GetEffect();
					pEffect->GenerateXml(pNode->Attribute("object"), pNode->Attribute("source"), pNode->Attribute("name"));
				}
			}
		}
	}

// 	tinyxml2::XMLElement* pModels = pRoot->FirstChildElement("Models");
// 	if (pModels)
// 	{
// 		for (tinyxml2::XMLElement* pNode = pModels->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
// 		{
// 		}
// 	}

	return true;
}
