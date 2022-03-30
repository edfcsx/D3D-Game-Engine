#ifndef _DEMO_SHANK_GAME_H_
#define _DEMO_SHANK_GAME_H_

#include "Game.h"
#include "Sprite.h"
#include "Shank.h"

class ShankGame : public Game {
private:
	Image* logoImg = nullptr;
	
	Sprite* backg = nullptr;
	Sprite* logo1 = nullptr;
	Sprite* logo2 = nullptr;

	Shank* shank = nullptr;
public:
	void Init();
	void Update();
	void Draw();
	void Finalize();
};

#endif // !_DEMO_SHANK_GAME_H_
