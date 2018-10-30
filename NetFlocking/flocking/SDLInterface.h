// Certificate of Authenticity
//
// EGP-405-01 Networking for Online Games
// Project 2 & Lab 4
// 10-30-2018
//
// Vedant Chaudhari, 1532077
// John Malvey
//
// We certify that this work is entirely our own.The assessor of this project may reproduce this project 
// and provide copies to other academic staff, and/or communicate a copy of this project to a plagiarism 
// - checking service, which may retain a copy of the project on its database.

#pragma once

#include <iostream>
#include <math.h>
#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "Define.h"

#include "EventManager.h"
#include "Event.h"

#include "ChangeDirectionEvent.h"

class SDLInterface {
private:
	static SDLInterface* mpInstance;

	SDL_Window* mpWindow;
	SDL_Renderer* mpRenderer;

	int mWindowWidth;
	int mWindowHeight;

public:
	bool isExit = false;

	SDLInterface();
	~SDLInterface();

	void init(int width, int height, const std::string& name);
	void render();
	void exit();

	void background();

	// Draw Functions
	void setColor(int r, int g, int b, int a);
	void drawTriangle(float x, float y, float width, float height, float angle);
	void drawLine(int x1, int y1, int x2, int y2);
	void drawCircle(int x, int y, int radius);

	void addEvent(EVENT_TYPE eType);

	// Getters
	static SDLInterface* getInstance();

	inline SDL_Window* getWindow() { return mpWindow; };
	inline SDL_Renderer* getRenderer() { return mpRenderer; };

	inline int getWidth() { return mWindowWidth; };
	inline int getHeight() { return mWindowHeight; };
};