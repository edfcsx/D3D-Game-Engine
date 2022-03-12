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
#include <sstream>
#include <vector>
#include <random>

// ------------------------------------------------------------------------------

struct Line {
	int xInitial;
	int xFinal;
	int velXInitial;
	int velXFinal;
	int yInitial;
	int yFinal;
	int velYInitial;
	int velYFinal;
	HPEN pen;
};

class WinGame : public Game {
private:
	HDC hdc;
	std::vector<Line> lines;
	int totalLines = 32;
public:
	void Init();
	void Update();
	void Draw();
	void Finalize();
};

// ------------------------------------------------------------------------------

void WinGame::Init() {
	hdc = GetDC(window->Id());

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> randomLineProperty(1, 7);
	std::uniform_int_distribution<int> randomColor(0, 255);
	std::uniform_int_distribution<int> randomLineX(0, window->Width());
	std::uniform_int_distribution<int> randomLineY(0, window->Height());

	while (lines.size() < totalLines) {
		Line genericLine;
		genericLine.xInitial = randomLineX(mt);
		genericLine.xFinal = randomLineX(mt);
		genericLine.velXInitial = randomLineProperty(mt);
		genericLine.velXFinal = randomLineProperty(mt);
		genericLine.yInitial = randomLineY(mt);
		genericLine.yFinal = randomLineY(mt);
		genericLine.velYInitial = randomLineProperty(mt);
		genericLine.velYFinal = randomLineProperty(mt);
		genericLine.pen = CreatePen(PS_SOLID, 3, RGB(randomColor(mt), randomColor(mt), randomColor(mt)));

		lines.push_back(genericLine);
	}
}

// ------------------------------------------------------------------------------

void WinGame::Update() {
	if (window->KeyDown(VK_ESCAPE))
		window->Close();

	for (Line& line : lines) {
		line.xInitial += line.velXInitial;
		line.xFinal += line.velXFinal;
		line.yInitial += line.velYInitial;
		line.yFinal += line.velYFinal;

		// eixo x

		if (line.xInitial > window->Width())
			line.velXInitial = -line.velXInitial;

		if (line.xInitial < 0)
			line.velXInitial = -line.velXInitial;

		if (line.xFinal > window->Width())
			line.velXFinal = -line.velXFinal;

		if (line.xFinal < 0)
			line.velXFinal = -line.velXFinal;

		// eixo y

		if (line.yInitial > window->Height())
			line.velYInitial = -line.velYInitial;

		if (line.yInitial < 0)
			line.velYInitial = -line.velYInitial;

		if (line.yFinal > window->Height())
			line.velYFinal = -line.velYFinal;

		if (line.yFinal < 0)
			line.velYFinal = -line.velYFinal;
	}
}

// ------------------------------------------------------------------------------

void WinGame::Draw() {
	for (const Line line : lines) {
		SelectObject(hdc, line.pen);
		MoveToEx(hdc, line.xInitial, line.yInitial, NULL);
		LineTo(hdc, line.xFinal, line.yFinal);
	}
}

// ------------------------------------------------------------------------------

void WinGame::Finalize() {
	ReleaseDC(window->Id(), hdc);
}

// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
//                                  WinMain                                      
// ------------------------------------------------------------------------------

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	// cria motor e configura a janela
	Engine* engine = new Engine();
	engine->window->Mode(WINDOWED);
	engine->window->Size(960, 540);
	engine->window->Color(240, 240, 140);
	engine->window->Title("Window Game");
	engine->window->Icon(IDI_ICON);
	engine->window->Cursor(IDC_CURSOR);

	// cria e inicia o jogo
	int exit_code = engine->Start(new WinGame());

	// finaliza o jogo e motor
	delete engine;
	return exit_code;
}

// ----------------------------------------------------------------------------
