#include "MovementController.h"
#include "SceneNode.h"

MovementController::MovementController(shared_ptr<SceneNode> ojbect, float initialYaw, float initialPitch)
{
}


MovementController::~MovementController()
{
}

void MovementController::OnUpdate(unsigned long elapsedMs)
{

}

bool MovementController::VOnPointerMove(const Vector2 &pos, const int radius)
{
	return true;
}

bool MovementController::VOnPointerButtonDown(const Vector2 &pos, const int radius, const std::string &buttonName)
{
	return true;
}

bool MovementController::VOnPointerButtonUp(const Vector2 &pos, const int radius, const std::string &buttonName)
{
	return true;
}

bool MovementController::VOnKeyDown(const BYTE c)
{
	return true;
}

bool MovementController::VOnKeyUp(const BYTE c)
{
	return true;
}
