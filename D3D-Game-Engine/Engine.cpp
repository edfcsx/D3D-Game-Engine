/**********************************************************************************
// Engine (Código Fonte)
//
// Criação:     15 Mai 2014
// Atualização: 05 Ago 2021
// Compilador:  Visual C++ 2019
//
// Descrição:   A função da Engine é rodar jogos criados a partir da classe
//              abstrata Game. Todo jogo deve ser uma classe derivada de Game
//              e portanto deve implementar as funções membro Init, Update, Draw
//              e Finalize, que serão chamadas pelo motor em um laço de tempo real.
//              Para usar a classe Engine, o programador deve criar uma instância
//              e chamar o método Start(), passando um objeto derivado de Game.
//
**********************************************************************************/

#include "Engine.h"
#include <windows.h>
#include <sstream>

// ------------------------------------------------------------------------------
// Inicialização de variáveis estáticas da classe

Game	 * Engine::game		 = nullptr;			// jogo em execução
Window   * Engine::window    = nullptr;			// janela do jogo
Graphics * Engine::graphics  = nullptr;			// dispositivo gráfico
float	   Engine::frameTime = 0.0f;			// tempo do quadro atual
bool       Engine::paused    = false;
Timer      Engine::timer;

// -------------------------------------------------------------------------------

Engine::Engine() {
	window	 = new Window();
	graphics = new Graphics();
}

// -------------------------------------------------------------------------------

Engine::~Engine() {
	delete game;
	delete graphics;
	delete window;
}

// -------------------------------------------------------------------------------

int Engine::Start(Game* level) {
	game = level;

	// cria a janela do jogo
	if (!window->Create()) {
		MessageBox(nullptr, "Falha na criação da janela", "Engine", MB_OK);
		return EXIT_FAILURE;
	}

	// inicializa o dispositivo gráfico
	if (!graphics->Initialize(window)) {
		MessageBox(window->Id(), "Falha na inicialização do dispositivo gráfico", "Engine", MB_OK);
		return EXIT_FAILURE;
	}

	// ajusta a resolução da função Sleep para 1 milisegundo
	// requer uso da biblioteca winmm.lib
	timeBeginPeriod(1);

	int exitCode = Loop();

	// volta a resolução ao valor original
	timeEndPeriod(1);

	return exitCode;
}

// -------------------------------------------------------------------------------

void Engine::Pause() {
	paused = true;
	timer.Stop();
}

// -------------------------------------------------------------------------------

void Engine::Resume() {
	paused = false;
	timer.Start();
}

// -------------------------------------------------------------------------------

int Engine::Loop() {
	// inicia a contagem do tempo
	timer.Start();

	// inicialização do jogo
	game->Init();

	// mensagens do windows
	MSG msg = { 0 };

	// controle da tecla Pause (VK_PAUSE)
	bool pauseKeyCtrl = true;

	// laço principal do jogo
	do {
		// testa se tem mensagem do windows para tratar
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// ------------------------
			// Pause/Resumo do jogo
			// ------------------------

			if (pauseKeyCtrl) {
				if (window->KeyDown(VK_PAUSE)) {
					paused = !paused;
					pauseKeyCtrl = false;

					if (paused)
						timer.Stop();
					else
						timer.Start();
				}
			}
			else {
				// registra novo pressionamento somente
				// após liberação da tecla pause
				if (window->KeyUp(VK_PAUSE))
					pauseKeyCtrl = true;
			}

			// -----------------------------------------------------------------------------

			if (!paused) {
				// calcula o tempo do quadro
				frameTime = FrameTime();

				// Atualização do jogo
				game->Update();

				// limpa a tela para o próximo quadro
				graphics->Clear();

				// desenha o jogo
				game->Draw();

				// apresenta o jogo na tela (troca backbuffer/frontbuffer)
				graphics->Present();
			}
			else {
				game->OnPause();
			}

			// -----------------------------------------------------------------------------

		}
	} while (msg.message != WM_QUIT);

	// finalização do jogo
	game->Finalize();

	//encerra a aplicação
	return int(msg.wParam);
}

// -----------------------------------------------------------------------------
float Engine::FrameTime() {
	// ----- START DEBUG ----------
#ifdef _DEBUG
	static float totalTime  = 0.0f;    // tempo total transcorrido 
	static uint  frameCount = 0;       // contador de frames transcorridos
#endif
	// ------ END DEBUG -----------

	// tempo do frame atual em segundos
	frameTime = timer.Reset();

	// ----- START DEBUG ----------
#ifdef _DEBUG
	// tempo acumulado dos frames 
	totalTime += frameTime;

	// incrementa contador de frames
	frameCount++;

	// a cada 1000ms (1 segundo) atualiza indicador de FPS na janela
	if (totalTime >= 1.0f) {
		std::stringstream text;        // fluxo de texto para mensagens
		text << std::fixed;            // sempre mostra a parte fracionária
		text.precision(3);             // três casas depois da vírgula

		text << window->Title().c_str() << "    "
			<< "FPS: " << frameCount << "    "
			<< "Frame Time: " << frameTime * 1000 << " (ms)";

		SetWindowText(window->Id(), text.str().c_str());

		frameCount = 0;
		totalTime -= 1.0f;
	}
#endif
	// ------ END DEBUG ----------- 

	return frameTime;
}
