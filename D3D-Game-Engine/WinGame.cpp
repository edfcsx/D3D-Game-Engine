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

// ------------------------------------------------------------------------------
// Demonstrações de jogos e funcionalidades
#include "SpriteDemo.h"
#include "ShankGame.h"

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
	//int exit_code = engine->Start(new SpriteDemo());
	int exit_code = engine->Start(new ShankGame());

	// finaliza o jogo e motor
	delete engine;
	return exit_code;
}

// ----------------------------------------------------------------------------
