#include "Game.h"
#include "GameLayer.h"
#include "ui\UIButton.h"

Scene* Game::createScene()
{
	auto scene = Game::create();
	return scene;
}


bool Game::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	blocks = std::vector<std::vector<Sprite*>>(numRows, std::vector<Sprite*>(numCols, nullptr));

	Size winSize = Director::getInstance()->getWinSize();

	const float topMenuHeight = 30.0f;

	auto topMenuLayer = GameLayer::createLayer("topMenuLayer", Color4B(153, 165, 188, 255));
	topMenuLayer->setContentSize(Size(winSize.width, topMenuHeight));
	topMenuLayer->setPosition(0, winSize.height - topMenuHeight);

	const float fontSize = 24.0f;

	auto labelHeight = Label::createWithSystemFont("Height ", "Arial", fontSize);
	auto labelColor = Label::createWithSystemFont("Color", "Arial", fontSize);
	auto labelWidth = Label::createWithSystemFont("Width", "Arial", fontSize);
	auto textFieldHeight = ui::TextField::create(std::to_string(numRows), "Arial", 24);
	auto textFieldWidth = ui::TextField::create(std::to_string(numCols), "Arial", 24);
	auto textFieldColor = ui::TextField::create(std::to_string(numColors), "Arial", 24);


	labelHeight->setColor(Color3B::BLACK);
	labelWidth->setColor(Color3B::BLACK);
	labelColor->setColor(Color3B::BLACK);

	labelHeight->setAnchorPoint(Vec2(0, 0));
	labelWidth->setAnchorPoint(Vec2(0, 0));
	labelColor->setAnchorPoint(Vec2(0, 0));
	textFieldHeight->setAnchorPoint(Vec2(0, 0));
	textFieldColor->setAnchorPoint(Vec2(0, 0));
	textFieldWidth->setAnchorPoint(Vec2(0, 0));

	textFieldHeight->setMaxLengthEnabled(true);
	textFieldColor->setMaxLengthEnabled(true);
	textFieldWidth->setMaxLengthEnabled(true);
	textFieldHeight->setMaxLength(2);
	textFieldColor->setMaxLength(2);
	textFieldWidth->setMaxLength(2);
	textFieldHeight->setColor(Color3B::WHITE);

	const int offsetX = 10;
	labelHeight->setPosition(Vec2(0, 0));
	textFieldHeight->setPosition(Vec2(labelHeight->getPosition().x + labelHeight->getContentSize().width + offsetX, 0));
	labelWidth->setPosition(Vec2(textFieldHeight->getPosition().x + textFieldHeight->getContentSize().width + 2 * offsetX, 0));
	textFieldWidth->setPosition(Vec2(labelWidth->getPosition().x + labelWidth->getContentSize().width + offsetX, 0));
	labelColor->setPosition(Vec2(textFieldWidth->getPosition().x + textFieldWidth->getContentSize().width + 2 * offsetX, 0));
	textFieldColor->setPosition(Vec2(labelColor->getPosition().x + labelColor->getContentSize().width + offsetX, 0));


	auto startLabel = Label::createWithSystemFont("Start", "Arial", 24);
	auto startItem = MenuItemLabel::create(startLabel, CC_CALLBACK_1(Game::replaceScene, this));
	startItem->setAnchorPoint(Vec2(0, 0));
	auto menu = Menu::create(startItem, nullptr);
	menu->setContentSize({ startLabel->getContentSize().width, startLabel->getContentSize().height });

	Size sizeMenuLayer = topMenuLayer->getContentSize();
	menu->setPosition(Vec2(sizeMenuLayer.width - menu->getContentSize().width - 2 * offsetX, 0));
	log("pos x %f = = %f", sizeMenuLayer.height, sizeMenuLayer.width);

	topMenuLayer->addChild(labelHeight);
	topMenuLayer->addChild(textFieldHeight);
	topMenuLayer->addChild(labelWidth);
	topMenuLayer->addChild(textFieldWidth);
	topMenuLayer->addChild(labelColor);
	topMenuLayer->addChild(textFieldColor);
	topMenuLayer->addChild(menu);


	auto gameLayer = GameLayer::createLayer("gameLayer", Color4B(49, 77, 121, 255));
	gameLayer->setContentSize(Size(winSize.width, winSize.height - topMenuHeight));
	gameLayer->setPosition(Vec2(0, 0));

	fillBoard(gameLayer);

	addChild(topMenuLayer);
	addChild(gameLayer);
	return true;
}


void Game::replaceScene(cocos2d::Ref* pSender)
{
	auto newScene = Game::create();
	if (newScene)
	{
		Director::getInstance()->replaceScene(newScene);
	}
}

cocos2d::Color3B Game::getRandomColor()
{
	int color = std::rand() % 3;
	switch (color)
	{
	case 0:
	{
		return Color3B::RED;
	}
	case 1:
	{
		return Color3B::GREEN;
	}
	case 2:
		return Color3B::BLUE;
	}
}

cocos2d::Vec2 Game::getPositionForBlock(int row, int col, int offsetHeight, int padding)
{
	float x = col * (blockWidth + padding) + offsetWidth;
	float y = row * (blockHeight + padding) + offsetHeight;
	return cocos2d::Vec2(x, y);
}

void Game::fillBoard(LayerColor* layer)
{
	//calc the height offset
	const int offsetHeight = (layer->getContentSize().height - blockHeight * numRows) / 2;
	const int padding = 0;
	auto physicsWorld = getPhysicsWorld();
	physicsWorld->setSpeed(2.0f);

	Size sizeLayer = layer->getContentSize();
	auto scoreLabel = Label::createWithSystemFont("Score", "Arial", 30);
	scoreLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
	scoreLabel->setPosition(Vec2(sizeLayer.width / 2 - scoreLabel->getContentSize().width, sizeLayer.height - scoreLabel->getContentSize().height));
	scoreLabel->setColor(Color3B::GREEN);

	auto pointsLabel = Label::createWithSystemFont("0", "Arial", 30);
	pointsLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
	pointsLabel->setPosition(Vec2(sizeLayer.width / 2 + 10, sizeLayer.height - pointsLabel->getContentSize().height));
	pointsLabel->setColor(Color3B::GREEN);
	pointsLabel->setName("pointsLabel");

	layer->addChild(scoreLabel);
	layer->addChild(pointsLabel);

	auto bottomEdge = Node::create();
	auto bottomBody = PhysicsBody::createEdgeSegment(Vec2(offsetWidth, offsetHeight), Vec2(numCols * (blockWidth)+offsetWidth, offsetHeight), PhysicsMaterial(0.0f, 0.0f, 1.0f));
	bottomBody->setDynamic(false);
	bottomBody->setGravityEnable(false);
	bottomEdge->setPhysicsBody(bottomBody);

	addChild(bottomEdge);

	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			auto block = Sprite::create("block.png");
			if (!block) log("Sprite for [%d][%d] not was loaded", row, col);

			blocks[row][col] = block;
			block->setAnchorPoint(Vec2(0, 0));
			block->setColor(getRandomColor());
			block->setPosition(getPositionForBlock(row, col, offsetHeight, padding));

			auto blockBody = PhysicsBody::createBox(Size(blockWidth, blockHeight), PhysicsMaterial(0.0f, 0.0f, 1.0f));
			blockBody->setDynamic(true);
			blockBody->setGravityEnable(true);
			blockBody->setCategoryBitmask(0x01);
			blockBody->setCollisionBitmask(0x01);
			blockBody->setRotationEnable(false);
		
			block->setPhysicsBody(blockBody);

			layer->addChild(block);

			auto listener = EventListenerMouse::create();
			listener->onMouseDown = [this, listener, physicsWorld, row, col, offsetHeight, padding, layer](EventMouse* event)
			{
				if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
				{
					Vec2 pos = { event->getCursorX() - layer->getPositionX(), event->getCursorY() - layer->getPositionY() };
					int rowFromPos = static_cast<int>((pos.y - offsetHeight) / (blockHeight + padding));
					int colFromPos = static_cast<int>((pos.x - offsetWidth) / (blockWidth + padding));
					if (!(pos.x < offsetWidth || pos.x > numCols * blockWidth + offsetWidth || pos.y < offsetHeight || pos.y > numRows * blockHeight + offsetHeight))
					{
						if (blocks[rowFromPos][colFromPos] != nullptr && blocks[rowFromPos][colFromPos]->getBoundingBox().containsPoint(pos))
						{
							log("SPRITE WAS CLIKED [%d][%d]", rowFromPos, colFromPos);
							Color3B targetColor = blocks[rowFromPos][colFromPos]->getColor();
							std::vector<std::vector<bool>> visited(numRows, std::vector<bool>(numCols, false));

							int countNeighbor = 0;

							DFS(rowFromPos, colFromPos, countNeighbor, targetColor, visited);

							int resultDeletion = GameLayer::removeBlocks(layer, blocks, visited, numRows, numCols, countNeighbor);
							score += resultDeletion;

							Label* pScore = dynamic_cast<Label*>(layer->getChildByName("pointsLabel"));
							pScore->setString(std::to_string(score));

							if (!this->checkGameOver())
							{
								this->gameOverScene();
							}

							if (resultDeletion) { _eventDispatcher->removeEventListener(listener); }
							event->stopPropagation();
						}
					}
				}
			};
			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, blocks[row][col]);
		}
	}
}


void Game::DFS(int row, int col, int& countNeighbor, Color3B targetColor, std::vector<std::vector<bool>>& visited) {
	if (row < 0 || row >= numRows || col < 0 || col >= numCols || visited[row][col] == true || blocks[row][col] == nullptr) {
		return;
	}

	if (blocks[row][col]->getColor() != targetColor) {
		return;
	}

	visited[row][col] = true;
	++countNeighbor;
	DFS(row - 1, col, countNeighbor, targetColor, visited);
	DFS(row + 1, col, countNeighbor, targetColor, visited);
	DFS(row, col - 1, countNeighbor, targetColor, visited);
	DFS(row, col + 1, countNeighbor, targetColor, visited);
}

bool Game::checkGameOver()
{
	int countNeighbor = 0;
	std::vector<std::vector<bool>> visited(numRows, std::vector<bool>(numCols, false));
	for (int row = 0; row < numRows; ++row)
	{
		for (int col = 0; col < numCols; ++col)
		{
			if (blocks[row][col] == nullptr) continue;
			else
			{
				Game::DFS(row, col, countNeighbor, blocks[row][col]->getColor(), visited);
				if (countNeighbor >= 3) return true;
				else countNeighbor = 0;
			}
		}
	}
	return false;
}

void Game::gameOverScene()
{
	Size winSize = Director::getInstance()->getWinSize();

	auto gameOverScene = Scene::create();
	auto layer = LayerColor::create(Color4B(166, 3, 3, 255));

	auto goLabel = Label::createWithSystemFont("GAME OVER", "Arial", 60);
	goLabel->setPosition(winSize.width / 2, winSize.height / 2 + goLabel->getContentSize().height);

	auto scoreLabel = Label::createWithSystemFont(std::string("Your score: ") + std::to_string(score), "Arial", 40);
	scoreLabel->setPosition(winSize.width / 2, winSize.height / 2);

	auto button = ui::Button::create("button.png");
	button->setTitleText("Restart");
	button->setTitleFontSize(40);
	button->setPosition({ winSize.width / 2, winSize.height / 2 - scoreLabel->getContentSize().height * 2 });
	button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				break;
			case ui::Widget::TouchEventType::ENDED:
			{
				replaceScene(sender);
				break;
			}
			default:
				break;
			}
		});

	layer->addChild(goLabel);
	layer->addChild(scoreLabel);
	layer->addChild(button);

	gameOverScene->addChild(layer);
	Director::getInstance()->replaceScene(TransitionFade::create(1.5, gameOverScene, Color3B(163, 72, 110)));
}