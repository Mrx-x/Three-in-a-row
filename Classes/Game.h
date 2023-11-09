#pragma once

#include "cocos2d.h"
#include "ui\UITextField.h"

#include <vector>

USING_NS_CC;

class Game : public cocos2d::Scene
{
private:
    int numRows = 10;
    int numCols = 16;
    int numColors = 3;
    int blockWidth = 72;
    int blockHeight = 72;
    int offsetWidth = 64;
    int score = 0;
    std::vector<std::vector<Sprite*>> blocks;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void replaceScene(cocos2d::Ref* pSender);
    void fillBoard(cocos2d::LayerColor* layer);
    cocos2d::Vec2 getPositionForBlock(int row, int col, int offsetHeight, int padding = 0);
    cocos2d::Color3B getRandomColor();

    void DFS(int row, int col, int& countNeighbor, Color3B targetColor, std::vector<std::vector<bool>>& visited);

    CREATE_FUNC(Game);
};