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

Game* Engine::game = nullptr;		// jogo em execução
Window* Engine::window = nullptr;		// janela do jogo

// -------------------------------------------------------------------------------

Engine::Engine() {
	window = new Window();
}

// -------------------------------------------------------------------------------

Engine::~Engine() {
	delete game;
	delete window;
}

// -------------------------------------------------------------------------------

int Engine::Start(Game* level) {
	game = level;

	// cria a janela do jogo
	window->Create();

	// retorna o resultado da execução do jogo
	return Loop();
}

// -------------------------------------------------------------------------------

int Engine::Loop() {
	MSG msg = { 0 };		// Mensagem do windows
	HDC hdc;				// contexto do dispositivo
	RECT rect;				// área cliente da janela

	// captura o contexto do dispositivo
	hdc = GetDC(window->Id());

	// pega o tamanho da área cliente
	GetClientRect(window->Id(), &rect);

	// Inicialização do jogo
	game->Init();

	// laço principal do jogo
	do {
		// trata todos os eventos e atualiza o jogo
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// Atualização do jogo
			game->Update();

			// limpa a área cliente
			FillRect(hdc, &rect, CreateSolidBrush(window->Color()));

			// desenha o jogo
			game->Draw();

			// controle de fps (quebra galho inicial)
			Sleep(16);
		}
	} while (msg.message != WM_QUIT);

	// finalização do jogo
	game->Finalize();

	// libera o contexto do dispositivo
	ReleaseDC(window->Id(), hdc);

	//encerra a aplicação
	return int(msg.wParam);
}

// -----------------------------------------------------------------------------
