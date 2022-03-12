/**********************************************************************************
// Engine (C�digo Fonte)
//
// Cria��o:     15 Mai 2014
// Atualiza��o: 05 Ago 2021
// Compilador:  Visual C++ 2019
//
// Descri��o:   A fun��o da Engine � rodar jogos criados a partir da classe
//              abstrata Game. Todo jogo deve ser uma classe derivada de Game
//              e portanto deve implementar as fun��es membro Init, Update, Draw
//              e Finalize, que ser�o chamadas pelo motor em um la�o de tempo real.
//              Para usar a classe Engine, o programador deve criar uma inst�ncia
//              e chamar o m�todo Start(), passando um objeto derivado de Game.
//
**********************************************************************************/

#include "Engine.h"
#include <windows.h>
#include <sstream>

// ------------------------------------------------------------------------------
// Inicializa��o de vari�veis est�ticas da classe

Game* Engine::game = nullptr;		// jogo em execu��o
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

	// retorna o resultado da execu��o do jogo
	return Loop();
}

// -------------------------------------------------------------------------------

int Engine::Loop() {
	MSG msg = { 0 };		// Mensagem do windows
	HDC hdc;				// contexto do dispositivo
	RECT rect;				// �rea cliente da janela

	// captura o contexto do dispositivo
	hdc = GetDC(window->Id());

	// pega o tamanho da �rea cliente
	GetClientRect(window->Id(), &rect);

	// Inicializa��o do jogo
	game->Init();

	// la�o principal do jogo
	do {
		// trata todos os eventos e atualiza o jogo
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// Atualiza��o do jogo
			game->Update();

			// limpa a �rea cliente
			FillRect(hdc, &rect, CreateSolidBrush(window->Color()));

			// desenha o jogo
			game->Draw();

			// controle de fps (quebra galho inicial)
			Sleep(16);
		}
	} while (msg.message != WM_QUIT);

	// finaliza��o do jogo
	game->Finalize();

	// libera o contexto do dispositivo
	ReleaseDC(window->Id(), hdc);

	//encerra a aplica��o
	return int(msg.wParam);
}

// -----------------------------------------------------------------------------
