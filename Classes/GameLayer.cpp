#include "GameLayer.h"
#include <iostream>
cocos2d::LayerColor* GameLayer::createLayer(const std::string& name, const Color4B& color)
{
	auto layer = LayerColor::create(color);
	layer->setName(name);

	return layer;
}

bool GameLayer::init()
{
	if (!LayerColor::init())
	{
		return false;
	}

	return true;
}