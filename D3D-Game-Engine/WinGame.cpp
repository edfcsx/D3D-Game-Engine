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

#include "Resources.h"
#include "Engine.h"
#include "Game.h"

// ------------------------------------------------------------------------------

class WinGame : public Game {
	private:
	public:
		void Init();
		void Update();
		void Draw();
		void Finalize();
};

// ------------------------------------------------------------------------------

void WinGame::Init() {}

// ------------------------------------------------------------------------------

void WinGame::Update() {
	if (window->KeyDown(VK_ESCAPE))
		window->Close();
}

// ------------------------------------------------------------------------------

void WinGame::Draw() {}

// ------------------------------------------------------------------------------

void WinGame::Finalize() {}

// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
//                                  WinMain                                      
// ------------------------------------------------------------------------------

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	// cria motor e configura a janela
	Engine* engine = new Engine();
	
	// configura a janela
	engine->window->Mode(WINDOWED);
	engine->window->Size(960, 540);
	engine->window->Color(240, 240, 140);
	engine->window->Title("D3D Game");
	engine->window->Icon(IDI_ICON);
	engine->window->Cursor(IDC_CURSOR);

	// travando fps conforme o vsync
	//engine->graphics->VSync(true);

	// cria e inicia o jogo
	int exit_code = engine->Start(new WinGame());

	// finaliza o jogo e motor
	delete engine;
	return exit_code;
}

// ----------------------------------------------------------------------------
