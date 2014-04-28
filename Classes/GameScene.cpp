#include "GameScene.h"
#include "Tools.h"

USING_NS_CC;

float systemGravity = -900.0f;
float landHeight = 112.0f;
float birdX = 90.0f;
float birdSize = 15.0f;
const int pipeCount = 5;
float pipeHeight = 320.0f;
float pipeDistance = 100.0f;
Node* pipes[pipeCount] = { 0, };
int pipeState[pipeCount] = { 0, };
const int PIPE_NEW = 0;
const int PIPE_PASS = 1;
float tabV = 260.0f;

Scene* GameScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Point(0, systemGravity));
    
    auto layer = GameScene::create();

    scene->addChild(layer);

    return scene;
}

bool GameScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

	touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	
	keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	createGameScene();

    return true;
}

void GameScene::resetGameSceneValue()
{
	totalScore = 0;
	gameOver = false;
	ready = false;
}

void GameScene::createGameScene()
{
	resetGameSceneValue();
	
	createLayerBg();
}

void GameScene::createLayerBg()
{
	Layer* layerBg = ::createCommonBackLayer();
	this->addChild(layerBg, 0, 121);

	auto groundNode = Node::create();
	groundNode->setPhysicsBody(PhysicsBody::createEdgeSegment(Point(0, landHeight), Point(288, landHeight)));
	this->addChild(groundNode);

	// add bird
	spriteBird = ::createBird();
	spriteBird->setPosition(birdX, visibleSize.height / 2 + 10);
	spriteBird->setPhysicsBody(PhysicsBody::createCircle(birdSize));
	spriteBird->getPhysicsBody()->setEnable(false);

	spriteBird->runAction(::createFlyAction(Point(spriteBird->getPosition())));
	this->addChild(spriteBird, 1000);

	// add blink white
	auto whiteBlinkSprite = ::createAtlasSprite("white");
	whiteBlinkSprite->setScale(100);
	whiteBlinkSprite->setOpacity(0);
	this->addChild(whiteBlinkSprite, 2000);

	// init ready info
	initReady();
}

void GameScene::initReady()
{
	readyTextSprite = ::createAtlasSprite("text_ready");
	readyTextSprite->setPosition(Point(0, 100));

	readyImage = ::createAtlasSprite("tutorial");

	auto readyNode = Node::create();
	readyNode->addChild(readyTextSprite);
	readyNode->addChild(readyImage);

	readyNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));

	this->addChild(readyNode);
}

void GameScene::hideReady()
{
	spriteBird->stopActionByTag(g_flyTag);
	readyTextSprite->runAction(FadeOut::create(0.2f));
	readyImage->runAction(FadeOut::create(0.2f));
}

void GameScene::createPipes()
{
	initPipe();
	this->schedule(schedule_selector(GameScene::createPipe), 1.3f);
}

void GameScene::initPipe()
{
	for (int i = 0; i < pipeCount; ++i)
	{
		auto downPipe = ::createAtlasSprite("pipe_down");
		auto upPipe = ::createAtlasSprite("pipe_up");

		downPipe->setPosition(Point(0, pipeHeight + pipeDistance));

		auto singlePipe = Node::create();
		singlePipe->addChild(downPipe);
		singlePipe->addChild(upPipe);

		singlePipe->setPosition(Point(-visibleSize.width * 2, 0));
		this->addChild(singlePipe, 10);

		pipes[i] = singlePipe;
		pipeState[i] = PIPE_PASS;
	}
}

void GameScene::createPipe(float dt)
{
	for (int i = 0; i < pipeCount; ++i)
	{
		if (pipes[i]->getPositionX() < -100)
		{
			auto pipeNode = pipes[i];
			pipeState[i] = PIPE_NEW;

			pipeNode->stopAllActions();

			int randomHeight = rand() % 3;

			pipeNode->setPosition(Point(visibleSize.width * 2, 50 * randomHeight));
			pipeNode->setTag(randomHeight);

			auto move = MoveTo::create(8, Point(- visibleSize.width * 2, 50 * randomHeight));
			pipeNode->runAction(move);
			break;
		}
	}
}

void GameScene::rotateBird(float dt)
{
	auto v = spriteBird->getPhysicsBody()->getVelocity();
	spriteBird->setRotation(MIN(MAX(-90.0f, v.y * 0.2f + 60.0f), 30));
}

void GameScene::checkHit(float dt)
{
	if (spriteBird->getPositionY() < landHeight + birdSize)
	{
		GameOver();
		SimpleAudioEngine::getInstance()->playEffect(hitPath.c_str());
		return;
	}

	for (int i = 0; i < pipeCount; ++i)
	{
		// check down
		if (abs(pipes[i]->getPositionX() - birdX) < (birdSize + 26))
		{
			if (spriteBird->getPositionY() < pipes[i]->getPositionY() + pipeHeight / 2 + birdSize)
			{
				//SimpleAudioEngine::getInstance()->playEffect(fallPath.c_str());
				GameOver();
				return;
			}

			if (spriteBird->getPositionY() < pipes[i]->getPositionY() + pipeHeight / 2 + pipeDistance - birdSize)
			{
				//SimpleAudioEngine::getInstance()->playEffect(fallPath.c_str());
				GameOver();
				return;
			}
		}
	}
}

void GameScene::GameOver()
{
	gameOver = true;

	SimpleAudioEngine::getInstance()->playEffect(hitPath.c_str());
	for (int i = 0; i < pipeCount; ++i)
		pipes[i]->stopAllActions();

	spriteBird->getPhysicsBody()->setVelocity(Point(0, 0));
	this->unschedule(schedule_selector(GameScene::createPipe));
	this->unschedule(schedule_selector(GameScene::checkHit));
	this->unschedule(schedule_selector(GameScene::rotateBird));

	auto layerBg = this->getChildByTag(121);
	auto land_1 = layerBg->getChildByTag(123);
	land_1->stopAllActions();
	auto land_2 = layerBg->getChildByTag(124);
	land_2->stopAllActions();
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	Point location = touch->getLocation();
	log("onTouchBegan: %0.2f, %0.2f", location.x, location.y);
	if (ready == false)
	{
		ready = true;
		hideReady();
		createPipes();
		spriteBird->getPhysicsBody()->setEnable(true);
		spriteBird->getPhysicsBody()->setVelocity(Point(0, tabV));

		this->schedule(schedule_selector(GameScene::checkHit), 0);
		this->schedule(schedule_selector(GameScene::rotateBird), 0);
	}
	else
	{
		spriteBird->getPhysicsBody()->setVelocity(Point(0, tabV));
	}

	SimpleAudioEngine::getInstance()->playEffect(wingPath.c_str());

	return true;
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode key_code, Event* unused_event)
{
	if (key_code == EventKeyboard::KeyCode::KEY_SPACE)
	{
		if (ready == false)
		{
			ready = true;
			hideReady();
			createPipes();
			spriteBird->getPhysicsBody()->setEnable(true);
			spriteBird->getPhysicsBody()->setVelocity(Point(0, tabV));

			this->schedule(schedule_selector(GameScene::checkHit), 0);
			this->schedule(schedule_selector(GameScene::rotateBird), 0);
		}
		else
		{
			spriteBird->getPhysicsBody()->setVelocity(Point(0, tabV));
		}

		SimpleAudioEngine::getInstance()->playEffect(wingPath.c_str());
	}
}

void GameScene::onExit()
{
	getEventDispatcher()->removeEventListener(touchListener);
	getEventDispatcher()->removeEventListener(keyboardListener);

	Layer::onExit();
}