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

Game* Engine::game = nullptr;			// jogo em execução
Window* Engine::window = nullptr;		// janela do jogo
Graphics* Engine::graphics = nullptr;	// dispositivo gráfico

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

	// retorna o resultado da execução do jogo
	return Loop();
}

// -------------------------------------------------------------------------------

int Engine::Loop() {
	// Inicialização do jogo
	game->Init();

	// mensagens do windows
	MSG msg = { 0 };

	// laço principal do jogo
	do {
		// testa se tem mensagem do windows para tratar
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// Atualização do jogo
			game->Update();

			// limpa a tela para o próximo quadro
			graphics->Clear();

			// desenha o jogo
			game->Draw();

			// apresenta o jogo na tela (troca backbuffer/frontbuffer)
			graphics->Present();

			// controle de fps (quebra galho inicial)
			Sleep(16);
		}
	} while (msg.message != WM_QUIT);

	// finalização do jogo
	game->Finalize();

	//encerra a aplicação
	return int(msg.wParam);
}

// -----------------------------------------------------------------------------
