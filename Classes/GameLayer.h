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

    static int removeBlocks(LayerColor* layer, std::vector<std::vector<Sprite*>>& blocks, std::vector<std::vector<bool>>& visited, int numRows, int numCols, int countNeighbor);

    CREATE_FUNC(GameLayer);
};