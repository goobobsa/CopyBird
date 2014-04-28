#include "Tools.h"
#include "Atlas.h"

Texture2D* textureAtlas = nullptr;
Size visibleSize;
float flySpeed = 2.5;
float flyOffset = 5;
int g_flyTag = 1000;
bool g_initFlag = false;

string wingPath;
string hitPath;
string scorePath;
string fallPath;
string uiPath;

void init()
{
	::importAtlas();

	FileUtils::getInstance()->addSearchPath("res/");
	textureAtlas = Director::getInstance()->getTextureCache()->addImage("atlas.png");

	wingPath = FileUtils::getInstance()->fullPathForFilename("sfx_wing.wav");
	hitPath = FileUtils::getInstance()->fullPathForFilename("sfx_hit.wav");
	scorePath = FileUtils::getInstance()->fullPathForFilename("sfx_point.wav");
	fallPath = FileUtils::getInstance()->fullPathForFilename("sfx_die.wav");
	uiPath = FileUtils::getInstance()->fullPathForFilename("sfx_swooshing.wav");

	SimpleAudioEngine::getInstance()->preloadEffect(wingPath.c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(hitPath.c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(scorePath.c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(fallPath.c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(uiPath.c_str());

	visibleSize = Director::getInstance()->getVisibleSize();
}

Sprite* createAtlasSprite(string name)
{
	Atlas tmpTable = g_Atlas[name];
	int rectX = tmpTable.x;
	int rectY = tmpTable.y;
	int rectWidth = tmpTable.width;
	int rectHeight = tmpTable.height;

	if (name == "land")
		rectX = rectX + 1;

	Rect rect = Rect(rectX, rectY, rectWidth, rectHeight);
	SpriteFrame* frame = SpriteFrame::createWithTexture(textureAtlas, rect);
	Sprite* atlasSprite = Sprite::createWithSpriteFrame(frame);
	return atlasSprite;
}

Size getSpriteSize(string name)
{
	return Size(g_Atlas[name].width, g_Atlas[name].height);
}

Layer* createCommonBackLayer()
{
	Layer* layerBg = Layer::create();

	int randomBgIndex = rand() % 2 + 1;
	string bgName = "bg_day";
	if (randomBgIndex == 2)
		bgName = "bg_night";

	Sprite* bg = createAtlasSprite(bgName);
	bg->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	layerBg->addChild(bg);

	float landHeight = getSpriteSize("land").height;

	Sprite* land_1 = createAtlasSprite("land");
	land_1->setPosition(visibleSize.width / 2, landHeight / 2);
	layerBg->addChild(land_1, 100, 123);

	MoveTo* move1 = MoveTo::create(2, Point(-visibleSize.width / 2, landHeight / 2));
	Place* reset1 = Place::create(Point(visibleSize.width / 2, landHeight / 2));
	land_1->runAction(RepeatForever::create(Sequence::create(move1, reset1, nullptr)));

	Sprite* land_2 = createAtlasSprite("land");
	land_2->setPosition(visibleSize.width * 3 / 2, landHeight / 2);
	layerBg->addChild(land_2, 100, 124);

	MoveTo* move2 = MoveTo::create(2, Point(visibleSize.width / 2, landHeight / 2));
	Place* reset2 = Place::create(Point(visibleSize.width * 3 / 2, landHeight / 2));
	land_2->runAction(RepeatForever::create(Sequence::create(move2, reset2, nullptr)));

	return layerBg;
}

Sprite* createBird()
{
	int randomIndex = rand() % 2;

	Vector<SpriteFrame*> birdFrames;
	for (int i = 0; i < 3; ++i)
	{
		char buff[128] = { 0, };
		sprintf(buff, "bird%d_%d", randomIndex, i);
		string frameName = buff;

		Atlas tmpTable = g_Atlas[frameName];
		Rect rect = Rect(tmpTable.x, tmpTable.y, tmpTable.width, tmpTable.height);
		SpriteFrame* frame = SpriteFrame::createWithTexture(textureAtlas, rect);
		birdFrames.pushBack(frame);
	}

	Sprite* spriteBird = Sprite::createWithSpriteFrame(birdFrames.at(0));
	Animation* animation = Animation::createWithSpriteFrames(birdFrames, 0.1f);
	Animate* animate = Animate::create(animation);
	spriteBird->runAction(RepeatForever::create(animate));

	return spriteBird;
}

Action* createFlyAction(Point position)
{
	auto moveUp = MoveTo::create(1.0 / flySpeed, Point(position.x, position.y + flyOffset));
	auto moveDown = MoveTo::create(1.0 / flySpeed, Point(position.x, position.y - flyOffset));

	auto flyAction = RepeatForever::create(Sequence::create(moveUp, moveDown, nullptr));
	flyAction->setTag(g_flyTag);

	return flyAction;
}
