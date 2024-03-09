#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <SDL_ttf.h>
class Button {
public:
	SDL_Texture* tex;
	SDL_Rect srect, drect;
	bool isSelected = false;
	Button();
	virtual ~Button();

	void update();

	void draw();



};














#endif