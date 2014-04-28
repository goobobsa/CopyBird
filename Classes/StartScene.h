#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"
#include "Common.h"

class StartScene : public cocos2d::Layer
{
public:
	Sprite* rateButton;
	Sprite* playButton;
	Sprite* rankButton;
	Sprite* clickedButton;
	Point touchBeginPoint;

    static cocos2d::Scene* createScene();

    virtual bool init();  
    
	CREATE_FUNC(StartScene);

	bool checkMenuButton(Sprite* button, string name, Point& point);

	EventListener* listener;
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onExit();

private:
	Layer* createStartBackLayer();
};

#endif // __START_SCENE_H__
