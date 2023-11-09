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

int GameLayer::removeBlocks(LayerColor* layer, std::vector<std::vector<Sprite*>>& blocks, std::vector<std::vector<bool>>& visited, int numRows, int numCols, int countNeighbor)
{
	int countDeleteBlocks = 0;
	if (countNeighbor >= 3)
	{
		for (int row = 0; row < numRows; ++row)
		{
			for (int col = 0; col < numCols; col++)
			{
				if (visited[row][col] == true)
				{
					auto blockBody = blocks[row][col]->getPhysicsBody();
					if (blockBody)
					{
						//blockBody->setDynamic(false);
						blockBody->removeFromWorld();
						// physicsWorld->removeBody(blockBody);
					}

					layer->removeChild(blocks[row][col]);
					//blocks[row][col]->removeFromParent();
					blocks[row][col] = nullptr;
					++countDeleteBlocks;
				}
			}
		}

		for (int col = 0; col < numCols; ++col)
		{
			int count = 0;
			for (int row = 0; row < numRows; ++row)
			{
				if (blocks[row][col] == nullptr && row != numRows - 1)
				{
					++count;
				}
				else if (count)
				{
					for (int current = row; current < numRows; ++current)
					{
						blocks[current - count][col] = blocks[current][col];
						blocks[current][col] = nullptr;
					}
					count = 0;
					--row;
				}
			}
		}
		return countDeleteBlocks;
	}
	else return countDeleteBlocks;
}