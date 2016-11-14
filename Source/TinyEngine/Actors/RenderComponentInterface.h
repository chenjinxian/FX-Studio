#pragma once
#include "ActorComponent.h"

class RenderComponentInterface : public ActorComponent
{
public:
	virtual shared_ptr<SceneNode> VGetSceneNode() = 0;
};

