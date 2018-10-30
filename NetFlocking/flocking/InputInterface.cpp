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

#include "InputInterface.h"
#include "SDLInterface.h"

#include "EventManager.h"

#include "SDL.h"

InputInterface* InputInterface::mpInstance = NULL;

InputInterface::InputInterface() {
	mNumKeys = NULL;
}

InputInterface* InputInterface::getInstance() {
	if (mpInstance == NULL)
		mpInstance = new InputInterface();

	return mpInstance;
}

void InputInterface::onKeyDown() {
	mpVKeyState = SDL_GetKeyboardState(&mNumKeys);
}

void InputInterface::onKeyUp() {
	mpVKeyState = SDL_GetKeyboardState(&mNumKeys);
}

bool InputInterface::isKeyDown(SDL_Scancode key) {
	if (mpVKeyState == NULL)
		return false;

	if (mpVKeyState[key] == 1)
		return true;
	else
		return false;
}

bool InputInterface::isKeyUp(SDL_Scancode key) {
	if (mpVKeyState == NULL)
		return false;

	if (mpVKeyState[key] == 0)
		return true;
	else
		return false;
}

void InputInterface::update() {
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent)) {

		switch (sdlEvent.type) {
		case SDL_KEYDOWN:
			switch (sdlEvent.key.keysym.sym) {
			case SDLK_ESCAPE:
				SDLInterface::getInstance()->isExit = true;
				break;
			case SDLK_d:
				
			}
			break;
		case SDL_KEYUP:
			onKeyUp();
			break;
		case SDL_QUIT:
			SDLInterface::getInstance()->isExit = true;
			break;
		}
	}
}