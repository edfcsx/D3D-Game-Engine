#include "ShankGame.h"

void ShankGame::Init() {
	logoImg = new Image("Resources/Logo.png");
	
	backg = new Sprite("Resources/Background.jpg");
	logo1 = new Sprite(logoImg);
	logo2 = new Sprite(logoImg);

	shank = new Shank();
}

void ShankGame::Update() {
	if (window->KeyDown(VK_ESCAPE)) {
		window->Close();
	}

	shank->Update();
}

void ShankGame::Draw() {
	backg->Draw(0.0f, 0.0f, Layer::BACK);
	logo1->Draw(40.0f, 60.0f, Layer::UPPER);
	logo2->Draw(400.0f, 450.0f, Layer::LOWER);

	shank->Draw();
}

void ShankGame::Finalize() {
	delete backg;
	delete logo1;
	delete logo2;
	delete shank;
	delete logoImg;
}
