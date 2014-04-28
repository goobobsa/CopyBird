#include "StartScene.h"
#include "Tools.h"
#include "GameScene.h"

USING_NS_CC;

Scene* StartScene::createScene()
{
    auto scene = Scene::create();
    
	auto layer = StartScene::create();

    scene->addChild(layer);

    return scene;
}

bool StartScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	::init();
	auto backLayer = createStartBackLayer();
	this->addChild(backLayer);

	clickedButton = nullptr;

	listener = EventListenerTouchOneByOne::create();
	((EventListenerTouchOneByOne*)listener)->setSwallowTouches(true);
	((EventListenerTouchOneByOne*)listener)->onTouchBegan = CC_CALLBACK_2(StartScene::onTouchBegan, this);
	((EventListenerTouchOneByOne*)listener)->onTouchEnded = CC_CALLBACK_2(StartScene::onTouchEnded, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);	
    
    return true;
}

Layer* StartScene::createStartBackLayer()
{
	Layer* backLayer = createCommonBackLayer();

	// logo
	auto spriteLogo = createAtlasSprite("title");
	spriteLogo->setPosition(Point(visibleSize.width / 2, visibleSize.height * 2 / 3));
	backLayer->addChild(spriteLogo);

	// flappy bird
	auto spriteBird = createBird();
	spriteBird->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 25));
	spriteBird->runAction(createFlyAction(Point(spriteBird->getPosition())));
	backLayer->addChild(spriteBird);

	// rate button
	rateButton = createAtlasSprite("button_rate");
	rateButton->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 35));
	backLayer->addChild(rateButton);

	// play button
	playButton = createAtlasSprite("button_play");
	playButton->setPosition(Point(visibleSize.width / 4, visibleSize.height / 2 - 120));
	backLayer->addChild(playButton, 1000);
	
	// rank button
	rankButton = createAtlasSprite("button_score");
	rankButton->setPosition(Point(visibleSize.width * 3 / 4, visibleSize.height / 2 - 120));
	backLayer->addChild(rankButton, 1000);

	// copyright
	auto spriteCopyright = createAtlasSprite("brand_copyright");
	spriteCopyright->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 170));
	backLayer->addChild(spriteCopyright, 1000);
	
	return backLayer;
}

bool StartScene::checkMenuButton(Sprite* button, string name, Point& point)
{
	Size buttonSize = ::getSpriteSize(name);
	float buttonX = button->getPositionX();
	float buttonY = button->getPositionY();

	if (abs(point.x - buttonX) < buttonSize.width / 2 &&
		abs(point.y - buttonY) < buttonSize.height / 2)
	{
		clickedButton = button;
		return true;
	}

	return false;
}

bool StartScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	 Point location = touch->getLocation();
	 touchBeginPoint.x = location.x;
	 touchBeginPoint.y = location.y;

	 if (rateButton)
		 checkMenuButton(rateButton, "button_rate", touchBeginPoint);

	 if (playButton)
		 checkMenuButton(playButton, "button_play", touchBeginPoint);

	 if (rankButton)
		 checkMenuButton(rankButton, "button_score", touchBeginPoint);

	 if (clickedButton)
		 clickedButton->setPosition(Point(clickedButton->getPositionX(), clickedButton->getPositionY()));

	return true;
}

void StartScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	Point location = touch->getLocation();
	touchBeginPoint.setPoint(0.0f, 0.0f);

	if (clickedButton)
	{
		clickedButton->setPosition(Point(clickedButton->getPositionX(), clickedButton->getPositionY()));

		if (clickedButton == rateButton)
		{
		}
		else if (clickedButton == playButton)
		{
			Scene* gameScene = nullptr;
			if (::g_initFlag == false)
			{
				getEventDispatcher()->removeEventListener(listener);
				gameScene = GameScene::createScene();				
			}

			auto trans = TransitionFade::create(0.5, gameScene, Color3B(0, 0, 0));
			Director::getInstance()->replaceScene(trans);
			SimpleAudioEngine::getInstance()->playEffect(uiPath.c_str());
		}
		else if (clickedButton == rankButton)
		{
		}

		clickedButton = nullptr;
	}
}

void StartScene::onExit()
{
	getEventDispatcher()->removeEventListener(listener);

	Layer::onExit();
}
