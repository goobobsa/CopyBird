#pragma once

#include "Common.h"

USING_NS_CC;
using namespace std;

extern Texture2D* textureAtlas;
extern Size visibleSize;
extern float flySpeed;
extern float flyOffset;
extern int g_flyTag;
extern bool g_initFlag;

extern string wingPath;
extern string hitPath;
extern string scorePath;
extern string fallPath;
extern string uiPath;

void init();
Size getSpriteSize(string name);
Sprite* createAtlasSprite(string name);
Layer* createCommonBackLayer();
Sprite* createBird();
Action* createFlyAction(Point position);