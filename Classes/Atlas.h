#pragma once

#include <map>
using namespace std;

struct Atlas
{
	float width;
	float height;
	float x;
	float y;

	Atlas() : width(0.0f), height(0.0f), x(0.0f), y(0.0f) {};
	Atlas(float w, float h, float x, float y)
	{
		width = w;
		height = h;
		this->x = x;
		this->y = y;
	}
};

extern map<string, Atlas> g_Atlas;
void importAtlas();
