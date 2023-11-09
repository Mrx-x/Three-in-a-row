#include "Game.h"
#include "GameLayer.h"

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