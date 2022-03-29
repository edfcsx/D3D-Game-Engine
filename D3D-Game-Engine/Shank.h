#ifndef _PROGJOGOS_DEMO_SHANK_H_
#define _PROGJOGOS_DEMO_SHANK_H_

#include "Game.h"
#include "Sprite.h"

class Shank : public Game {
private:
	Sprite* backg = nullptr;
	Sprite* shank = nullptr;

	Image* logoImg = nullptr;
	Sprite* logo1 = nullptr;
	Sprite* logo2 = nullptr;

	float x = 0, y = 0;
public:
	void Init();
	void Update();
	void Draw();
	void Finalize();
};

#endif
