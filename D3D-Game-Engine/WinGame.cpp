/**********************************************************************************
// WinGame
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

#include <sstream>
#include "Resources.h"
#include "Engine.h"
#include "Game.h"
#include "Timer.h"
#include "Sprite.h"

// ------------------------------------------------------------------------------

class WinGame : public Game {
	private:
		Sprite* backg = nullptr;            // sprite do fundo de tela
		Sprite* shank = nullptr;            // sprite do personagem 

		Image* logoImg = nullptr;           // imagem do logotipo
		Sprite* logo1 = nullptr;            // sprite 1 do logotipo
		Sprite* logo2 = nullptr;            // sprite 2 do logotipo

		float x = 0, y = 0;                 // posição x,y do shank
	public:
		void Init();
		void Update();
		void Draw();
		void Finalize();
};

// ------------------------------------------------------------------------------

void WinGame::Init() {
	backg = new Sprite("Resources/Background.jpg");
	shank = new Sprite("Resources/Shank.png");

	logoImg = new Image("Resources/Logo.png");
	logo1 = new Sprite(logoImg);
	logo2 = new Sprite(logoImg);

	x = 80.0f;
	y = 90.0f;
}

// ------------------------------------------------------------------------------

void WinGame::Update() {
	if (window->KeyDown(VK_ESCAPE))
		window->Close();

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

// ------------------------------------------------------------------------------

void WinGame::Draw() {
	backg->Draw(0.0f, 0.0f, Layer::BACK);
	shank->Draw(x, y);
	logo1->Draw(40.0f, 60.0f, Layer::UPPER);
	logo2->Draw(400.0f, 450.0f, Layer::LOWER);
}

// ------------------------------------------------------------------------------

void WinGame::Finalize() {
	// remove sprites da memória
	delete backg;
	delete shank;
	delete logo1;
	delete logo2;

	// remove imagem da memória
	delete logoImg;
}

// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
//                                  WinMain                                      
// ------------------------------------------------------------------------------

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	Timer timer;
	timer.Start();

	// cria motor e configura a janela
	Engine* engine = new Engine();
	
	// configura a janela
	engine->window->Mode(WINDOWED);
	engine->window->Size(800, 600);
	engine->window->Color(0, 0, 0);
	engine->window->Title("D3D Game");
	engine->window->Icon(IDI_ICON);
	engine->window->Cursor(IDC_CURSOR);

	// travando fps conforme o vsync
	//engine->graphics->VSync(true);

	float elapsedTimer = timer.Elapsed();
	std::stringstream text;
	text << "Tempo transcorrido: " << elapsedTimer << "\n";

	OutputDebugString(text.str().c_str());

	// cria e inicia o jogo
	int exit_code = engine->Start(new WinGame());

	// finaliza o jogo e motor
	delete engine;
	return exit_code;
}

// ----------------------------------------------------------------------------
