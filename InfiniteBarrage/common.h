#pragma once
#include "resource.h"

#define WIDTH 1920
#define HEIGHT 1080

#define SINGLETION(T) static T* getInstance() { static T Instance; return &Instance; }

#define DISALLOW_COPY_AND_ASSIGN(T)	\
    T(const T&) = delete;			\
    T& operator=(const T&) = delete;

#define SAFE_DELETE(p) if (p) { delete p; p = nullptr;}

#define TITLE_SCENE		0
#define GAME_SCENE_01	1
#define HELP_SCENE		2
#define GAME_OVER_SCENE 3

struct SLabel
{
	const TCHAR* pText = nullptr;
	int x = 0;
	int y = 0;
	int fontSize = 10;
	int fontThickness = 6;
	unsigned int color = 0;
	unsigned int edgeColor = 0;
};