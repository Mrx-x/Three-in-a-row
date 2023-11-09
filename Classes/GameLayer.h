#pragma once 

#include "cocos2d.h"
#include "ui\UITextField.h"
#include <string>

USING_NS_CC;

class GameLayer : public LayerColor
{
public:
    static cocos2d::LayerColor* createLayer(const std::string& name, const Color4B& color);

    virtual bool init();

    CREATE_FUNC(GameLayer);
};