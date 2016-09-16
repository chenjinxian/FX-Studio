#pragma once
#include <stdint.h>

class ISceenElement
{
public:
	ISceenElement() {}
	~ISceenElement() {}

	virtual void VOnUpdate(uint32_t deltaMilliseconds) = 0;
	virtual void VOnRender(float totalTime, float elapsedTime) = 0;
};

class IGameLogic
{
public:
	IGameLogic() {}
	~IGameLogic() {}

	virtual void VOnUpdate(float totalTime, float elapsedTime) = 0;
	virtual void VLoadGame(const std::string& projectXml) = 0;
};

class IGameView
{
public:
	IGameView() {}
	~IGameView() {}

	virtual void VOnUpdate(uint32_t deltaMilliseconds) = 0;
	virtual void VOnRender(float totalTime, float elapsedTime) = 0;
};

typedef std::list<shared_ptr<ISceenElement> > SceenElementList;
typedef std::list<shared_ptr<IGameView> > GameViewList;

class IRenderer
{
public:
	IRenderer() {}
	~IRenderer() {}

	virtual void VSetBackgroundColor(const Color& color) = 0;
	virtual bool VPreRender() = 0;
	virtual bool VPostRender() = 0;
	virtual void VShutdown() = 0;
};