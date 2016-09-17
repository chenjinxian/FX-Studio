#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "ActorComponent.h"

class RenderComponentInterface : public ActorComponent
{
public:
	RenderComponentInterface() {}
	virtual ~RenderComponentInterface() {}

	virtual shared_ptr<SceneNode> VGetSceneNode() = 0;
};

