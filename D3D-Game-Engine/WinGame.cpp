/**********************************************************************************
// WinGame
//
// Cria��o:     19 Mai 2007
// Atualiza��o: 04 Ago 2021
// Compilador:  Visual C++ 2019
//
// Descri��o:   Um jogo para Windows � uma classe derivada de Game.
//              Crie uma classe derivada e sobrescreva os m�todos Init,
//              Update, Draw e Finalize.
//
**********************************************************************************/

#include <sstream>
#include "Resources.h"
#include "Engine.h"
#include "Game.h"
#include "Timer.h"
#include "Shank.h"

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
	int exit_code = engine->Start(new Shank());

	// finaliza o jogo e motor
	delete engine;
	return exit_code;
}

// ----------------------------------------------------------------------------
