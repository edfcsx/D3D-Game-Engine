/**********************************************************************************
// Shank
//
// Criação:     19 Mai 2007
// Atualização: 04 Ago 2021
// Compilador:  Visual C++ 2019
//
// Descrição:   Um jogo para Windows é uma classe derivada de Game.
//              Crie uma classe derivada e sobrescreva os métodos Init,
//              Update, Draw e Finalize.
//
**********************************************************************************/

#include "Shank.h"

void Shank::Init() {
	backg = new Sprite("Resources/Background.jpg");
	shank = new Sprite("Resources/Shank.png");
	
	logoImg = new Image("Resources/Logo.png");
	logo1 = new Sprite(logoImg);
	logo2 = new Sprite(logoImg);

	x = 80.0f;
	y = 90.0f;
}

void Shank::Update() {
	if (window->KeyDown(VK_ESCAPE)) {
		window->Close();
	}

	// desloca o personagem
	// desloca personagem
	if (window->KeyDown(VK_LEFT))
		x -= 50.0f * gameTime;
	if (window->KeyDown(VK_RIGHT))
		x += 50.0f * gameTime;
	if (window->KeyDown(VK_UP))
		y -= 50.0f * gameTime;
	if (window->KeyDown(VK_DOWN))
		y += 50.0f * gameTime;
}

void Shank::Draw() {
	backg->Draw(0.0f, 0.0f, Layer::BACK);
	shank->Draw(x, y);
	logo1->Draw(40.0f, 60.0f, Layer::UPPER);
	logo2->Draw(400.0f, 450.0f, Layer::LOWER);
}

void Shank::Finalize() {
	delete backg;
	delete shank;
	delete logo1;
	delete logo2;

	delete logoImg;
}
