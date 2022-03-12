/**********************************************************************************
// Window (Arquivo de Cabeçalho)
//
// Criação:     19 Mai 2007
// Atualização: 05 Ago 2021
// Compilador:  Visual C++ 2019
//
// Descrição:   A classe abstrai todos os detalhes de configuração de
//              uma janela para um jogo.
//
**********************************************************************************/

#ifndef _PROGJOGOS_WINDOW_H
#define _PROGJOGOS_WINDOW_H
// ---------------------------------------------------------------------------------
// Inclusões

#include <windows.h>		// inclui funções do windows
#include <windowsx.h>		// inclui funções extras do windows
#include <string>			// inclui a classe string
#include "Types.h"			// tipos personalizados do motor

// ---------------------------------------------------------------------------------
// Constantes globais e enumerações

enum WindowModes { FULLSCREEN, WINDOWED, BORDELESS };

// ---------------------------------------------------------------------------------

/*
	Os atributos com o modificador "static" informa que o atributo pertence a classe
	e não as suas instancias, cada instancia da classe compartilhara os valores dos
	atributos estaticos
*/

class Window {
private:
	HINSTANCE		hInstance;								// identificador da aplicação
	HWND			windowHandle;							// identificador da janela
	int				windowWidth;							// largura da janela
	int				windowHeight;							// altura da janela
	HICON			windowIcon;								// icone da janela
	HCURSOR			windowCursor;							// cursor da janela
	COLORREF		windowColor;							// cor do fundo da janela
	std::string		windowTitle;							// mome da barra de titulo
	DWORD			windowStyle;							// estilo da janela
	int				windowMode;								// modo tela cheia, janela ou sem borda
	int				windowPosX;								// posição inicial da janela no eixo x
	int				windowPosY;								// posição inicial da janela no eixo y
	float			windowCenterX;							// centro da janela no eixo x
	float			windowCenterY;							// centro da janela no eixo y
	static bool		windowKeys[256];						// estado das teclas do teclado
	static int		windowMouseX;							// posição do mouse no eixo x
	static int		windowMouseY;							// posição do mouse no eixo y

public:
	Window();												// construtor de Window

	HINSTANCE appId();										// retorna o identificador da aplicação
	HWND Id();												// retorna o identificador da janela
	int  Width();											// retorna a largura atual da janela
	int  Height();											// retorna a altura atual da janela

	// O modificador const informa que o metodo não pode alterar os atributos do objeto
	void Icon(const uint icon);								// define o icone da janela
	void Cursor(const uint cursor);							// define o cursor da janela
	void Title(const std::string title);					// define o título da janela
	void Size(int width, int height);						// define o tamanho (largura e altura) da janela
	void Mode(int mode);									// define o modo da janela (FULLSCREEN/WINDOWED)

	// O modificador const ao final do metodo informa que os membros que guardam estados
	// não serão alterados por esse metodo
	int Mode() const;										// retorna o modo atual da janela (FULLSCREEN/WINDOWED)
	float CenterX() const;									// retorna o centro da janela no eixo x
	float CenterY() const;									// retorna o centro da janela no eixo y
	std::string Title() const;								// retorna o título da janela

	void HideCursor(bool hide);								// habilita ou desabilita a exibição do cursor
	void Close();											// fecha a janela imediatamente

	bool KeyDown(int vkcode);								// verifica se uma tecla/botão está pressionado
	bool KeyUp(int vkcode);									// verifica se uma tecla/botão está liberado
	int  MouseX();											// retorna a posição x do mouse
	int  MouseY();											// retorna a posição y do mouse

	COLORREF Color();										// retorna a cor de fundo da janela
	void Color(int r, int g, int b);						// define a cor de fundo da janela
	void Print(std::string, int x, int y, COLORREF color);	//mostra o texto usando a biblioteca do windows (lento)
	bool Create();											// cria a janela com os valores dos atributos

	// Tratamento de eventos do windows
	static LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParm, LPARAM lParam);
};

// ---------------------------------------------------------------------------------
// Funções membros inline

// retorna o identificador da aplicação
inline HINSTANCE Window::appId() {
	return hInstance;
}

// retorna o identificador da janela do jogo
inline HWND Window::Id() {
	return windowHandle;
}

// retorna a largura atual da janela
inline int Window::Width() {
	return windowWidth;
}

// retorna a altura atual da janela
inline int Window::Height() {
	return windowHeight;
}

// ---------------------------------------------------------------------------------

// define o icone da janela
inline void Window::Icon(const uint icon) {
	windowIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(icon));
}

// define o cursor da janela
inline void Window::Cursor(const uint cursor) {
	windowCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(cursor));
}

// define o titulo da janela
inline void Window::Title(const std::string title) {
	windowTitle = title;
}

// ---------------------------------------------------------------------------------

// retorna o modo atual da janela (FULLSCREEN/WINDOWED)
inline int Window::Mode() const {
	return windowMode;
}

// retorna o centro da janela do eixo horizontal
inline float Window::CenterX() const {
	return windowCenterX;
}

// retorna o centro da janela do eixo vertical
inline float Window::CenterY() const {
	return windowCenterY;
}

// retorna o título da janela
inline std::string Window::Title() const {
	return windowTitle;
}

// ---------------------------------------------------------------------------------

// habilita ou desabilita a exibição do cursor
inline void Window::HideCursor(bool hide) {
	ShowCursor(!hide);
}

// fecha a janela imediatamente
inline void Window::Close() {
	PostMessage(windowHandle, WM_DESTROY, 0, 0);
}

// retorna verdadeiro se a tecla está pressionada
inline bool Window::KeyDown(int vkcode) {
	return windowKeys[vkcode];
}

// retorna verdadeiro se a tecla está liberada
inline bool Window::KeyUp(int vkcode) {
	return !(windowKeys[vkcode]);
}

// retorna a posição do mouse no eixo x
inline int Window::MouseX() {
	return windowMouseX;
}

// retorna a posição do mouse no eixo y
inline int Window::MouseY() {
	return windowMouseY;
}

// ---------------------------------------------------------------------------------

// retorna a cor de fundo da janela
inline COLORREF Window::Color() {
	return windowColor;
}

// define a cor de fundo da janela
inline void Window::Color(int r, int g, int b) {
	windowColor = RGB(g, g, b);
}

// ---------------------------------------------------------------------------------

#endif
