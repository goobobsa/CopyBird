#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "Common.h"
#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
public :
	int totalScore;
	bool ready;
	bool gameOver;

	Sprite* spriteBird;
	Sprite* readyTextSprite;
	Sprite* readyImage;

	EventListenerTouchOneByOne* touchListener;
	EventListenerKeyboard* keyboardListener;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();  
    
    CREATE_FUNC(GameScene);

	void createGameScene();
	void resetGameSceneValue();
	void createLayerBg();
	void initReady();
	void hideReady();
	void createPipes();
	void initPipe();
	void GameOver();
	void createPipe(float dt);
	void checkHit(float dt);	
	void rotateBird(float dt);
	
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onKeyPressed(EventKeyboard::KeyCode key_code, Event* unused_event);
	virtual void onExit();
};

#endif // __GAME_SCENE_H__
