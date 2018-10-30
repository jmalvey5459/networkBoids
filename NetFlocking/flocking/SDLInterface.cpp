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

#include "SDLInterface.h"

#include "SDL.h"
#include "SDL_image.h"

SDLInterface* SDLInterface::mpInstance = NULL;

SDLInterface::SDLInterface() {
	this->mpWindow = nullptr;
	this->mpRenderer = nullptr;
}

SDLInterface::~SDLInterface() {}

SDLInterface* SDLInterface::getInstance() {
	if (mpInstance == NULL)
		mpInstance = new SDLInterface();

	return mpInstance;
}

void SDLInterface::init(int width, int height, const std::string& title) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return;

	mpWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		width, height, SDL_WINDOW_OPENGL);
	if (mpWindow == nullptr)
		return;

	// Initialize PNG images
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
		return;

	mpRenderer = SDL_CreateRenderer(mpWindow, -1, SDL_RENDERER_ACCELERATED);
	if (mpRenderer == nullptr)
		return;

	this->mWindowWidth = width;
	this->mWindowHeight = height;
}

void SDLInterface::render() {
	SDL_RenderPresent(mpRenderer);
}

void SDLInterface::exit() {
	SDL_DestroyRenderer(mpRenderer);
	SDL_DestroyWindow(mpWindow);
	SDL_Quit();
}

void SDLInterface::background() {
	setColor(0, 0, 0, 255);
	SDL_RenderClear(mpRenderer);
}

void SDLInterface::setColor(int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(mpRenderer, r, g, b, a);
}

void SDLInterface::drawTriangle(float x, float y, float width, float height, float angle) {
	// Right Corner
	float x1 = width / 2;
	float y1 = 0;

	// Left Top
	float x2 = -width / 2;
	float y2 = -height / 2;

	// Left Bottom
	float x3 = -width / 2;
	float y3 = height / 2;

	float s = sin(-angle);
	float c = cos(-angle);

	float x1r = (x1 * c) - (y1 * s) + x;
	float y1r = (x1 * s) + (y1 * c) + y;

	float x2r = (x2 * c) - (y2 * s) + x;
	float y2r = (x2 * s) + (y2 * c) + y;

	float x3r = (x3 * c) - (y3 * s) + x;
	float y3r = (x3 * s) + (y3 * c) + y;

	SDL_RenderDrawLine(mpRenderer, x1r, y1r, x2r, y2r);
	SDL_RenderDrawLine(mpRenderer, x2r, y2r, x3r, y3r);
	SDL_RenderDrawLine(mpRenderer, x3r, y3r, x1r, y1r);
}

void SDLInterface::drawLine(int x1, int y1, int x2, int y2) {
	SDL_RenderDrawLine(mpRenderer, x1, y1, x2, y2);
}

void SDLInterface::drawCircle(int x, int y, int radius) {
	int x1, y1, x2, y2;
	double theta = 0;

	while (theta < 2 * PI) {
		x1 = x + cos(theta) * radius;
		y1 = y - sin(theta) * radius;

		theta += (2 * PI / 32);

		x2 = x + cos(theta) * radius;
		y2 = y - sin(theta) * radius;

		drawLine(x1, y1, x2, y2);
	}
}

void SDLInterface::addEvent(EVENT_TYPE eType) {
	switch (eType) {
	case CHANGE_DIRECTION_EVENT:
		ChangeDirectionEvent* mEvent = new ChangeDirectionEvent(100, 100);
		EventManager::getInstance()->add(mEvent->getType(), mEvent);
		break;
	default:
		std::cout << "Unknown Event Type" << std::endl;
		break;
	}
}