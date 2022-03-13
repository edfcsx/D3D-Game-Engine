/**********************************************************************************
// Window (Código Fonte)
//
// Criação:     19 Mai 2007
// Atualização: 05 Ago 2021
// Compilador:  Visual C++ 2019
//
// Descrição:   A classe abstrai todos os detalhes de configuração de
//              uma janela para um jogo.
//
**********************************************************************************/

#include "Window.h"
#include "Engine.h"

// -------------------------------------------------------------------------------
// Inicialização de membros estáticos da classe

bool Window::windowKeys[256] = { 0 };						// estado do teclado/mouse
int	 Window::windowMouseX = 0;								// estado do mouse no eixo x
int  Window::windowMouseY = 0;								// estado do mouse no eixo y

// -------------------------------------------------------------------------------
// construtor

Window::Window() {
	hInstance		= GetModuleHandle(NULL);				// identificador da aplicação
	windowHandle	= 0;									// identificador da janela do jogo
	windowWidth		= GetSystemMetrics(SM_CXSCREEN);		// a janela ocupa toda tela (tela cheia)
	windowHeight	= GetSystemMetrics(SM_CYSCREEN);		// a janela ocupa toda tela (tela cheia)
	windowIcon		= LoadIcon(NULL, IDI_APPLICATION);		// icone padrão de uma aplicação
	windowCursor	= LoadCursor(NULL, IDC_ARROW);			// cursor padrão de uma aplicação
	windowColor		= RGB(0, 0, 0);							// cor de fundo padrão é preta
	windowTitle		= std::string("Windows Game");			// título padrão da janela
	windowStyle		= WS_POPUP | WS_VISIBLE;				// estilo para tela cheia
	windowMode		= FULLSCREEN;							// modo padrão é tela cheia
	windowPosX		= 0;									// posição inicial da janela no eixo x
	windowPosY		= 0;									// posição inicial da janela no eixo y
	windowCenterX	= windowWidth / 2.0f;					// centro da janela no eixo x
	windowCenterY	= windowHeight / 2.0f;					// centro da janela no eixo y
}

// -------------------------------------------------------------------------------

void Window::Mode(int mode) {
	windowMode = mode;

	if (windowMode == WINDOWED) {
		// modo janela
		windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE;
	}
	else {
		// modo em tela cheia ou sem bordas
		windowStyle = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE;
	}
}

// -------------------------------------------------------------------------------

void Window::Size(int width, int height) {
	// window size
	windowWidth = width;
	windowHeight = height;

	// calcula a posição do centro da janela
	windowCenterX = windowWidth / 2.0f;
	windowCenterY = windowHeight / 2.0f;

	// Ajusta a posição da janela para o centro da tela
	windowPosX = (GetSystemMetrics(SM_CXSCREEN) / 2) - (windowWidth / 2);
	windowPosY = (GetSystemMetrics(SM_CYSCREEN) / 2) - (windowHeight / 2);
}

// -------------------------------------------------------------------------------

void Window::Print(std::string text, int x, int y, COLORREF color) {
	// esta função exibe o texto na posição (x,y) da tela usando a cor especificada
	// ela usa a biblioteca do windows (lenta) e deve ser usada apenas para depuração

	// pega o contexto do dispositivo gráfico
	HDC xdc = GetDC(windowHandle);
	// define a cor do texto
	SetTextColor(xdc, color);
	// define o fundo do texto como transparente
	SetBkMode(xdc, TRANSPARENT);
	// mostra o texto
	TextOut(xdc, x, y, text.c_str(), int(text.size()));
	// Libera o contexto do dispositivo
	ReleaseDC(windowHandle, xdc);
}

// -------------------------------------------------------------------------------

bool Window::Create() {
	WNDCLASSEX wndClass;

	// definindo uma classe de janela chamada "GameWindow"
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = Window::WinProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = windowIcon;
	wndClass.hCursor = windowCursor;
	wndClass.hbrBackground = (HBRUSH)CreateSolidBrush(windowColor);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "GameWindow";
	wndClass.hIconSm = windowIcon;

	// registrando "GameWindow"
	if (!RegisterClassEx(&wndClass))
		return false;

	// criando uma janela baseada na classe "GameWindow"
	windowHandle = CreateWindowEx(
		NULL,								// estilos extras
		"GameWindow",						// nome da "window class"
		windowTitle.c_str(),				// título da janela
		windowStyle,						// estilo da janela
		windowPosX, windowPosY,				// posição (x,y) inicial
		windowWidth, windowHeight,			// largura e altura da janela
		NULL,								// identificador da janela pai
		NULL,								// identificador do menu
		hInstance,							// identificador da aplicação
		NULL								// parâmetros da aplicação
	);

	// Ao usar o modo em janela é preciso levar em conta que as barras 
	// e bordas ocupam espaço na janela. O código abaixo ajusta o tamanho
	// da janela de forma que a área cliente do jogo fique no tamanho 
	// (windowWidth x windowHeight)

	if (windowMode == WINDOWED) {
		// retângulo com o tamanho da área cliente desejada
		RECT winRect = { 0, 0, windowWidth, windowHeight };

		// ajusta o tamanho do retângulo
		AdjustWindowRectEx(
			&winRect,
			GetWindowStyle(windowHandle),
			GetMenu(windowHandle) != NULL,
			GetWindowExStyle(windowHandle)
		);

		// atualiza a posição da janela
		windowPosX = (GetSystemMetrics(SM_CXSCREEN) / 2) - ((winRect.right - winRect.left) / 2);
		windowPosY = (GetSystemMetrics(SM_CYSCREEN) / 2) - ((winRect.bottom - winRect.top) / 2);

		// redimensiona janela com uma chamada a moveWindow
		MoveWindow(
			windowHandle,							// identificador da janela
			windowPosX,								// posição x
			windowPosY,								// posição y
			winRect.right - winRect.left,			// largura
			winRect.bottom - winRect.top,			// altura
			true									// repintar
		);
	}

	// retorna estado da inicialização (bem sucedida ou não)
	return (windowHandle ? true : false);
}

// -------------------------------------------------------------------------------

LRESULT CALLBACK Window::WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_KEYDOWN:		// tecla pressionada
		windowKeys[wParam] = true;
		return 0;
	case WM_KEYUP:			// tecla liberada
		windowKeys[wParam] = false;
		return 0;
	case WM_MOUSEMOVE:		// movimento do mouse
		windowMouseX = GET_X_LPARAM(lParam);
		windowMouseY = GET_Y_LPARAM(lParam);
		return 0;
	case WM_LBUTTONDOWN:	// botão esquerdo do mouse pressionado
	case WM_LBUTTONDBLCLK:
		windowKeys[VK_LBUTTON] = true;
		return 0;
	case WM_MBUTTONDOWN:	// botão do meio do mouse pressionado
	case WM_MBUTTONDBLCLK:
		windowKeys[VK_MBUTTON] = true;
		return 0;
	case WM_RBUTTONDOWN:	// botão direito do mouse pressionado
	case WM_RBUTTONDBLCLK:
		windowKeys[VK_RBUTTON] = true;
		return 0;
	case WM_LBUTTONUP:		// botão esquerdo do mouse liberado
		windowKeys[VK_LBUTTON] = false;
		return 0;
	case WM_MBUTTONUP:		// botão do meio do mouse liberado
		windowKeys[VK_MBUTTON] = false;
		return 0;
	case WM_RBUTTONUP:		// botão direito do mouse liberado
		windowKeys[VK_RBUTTON] = false;
		return 0;
	case WM_SETFOCUS:		// mudança de foco da janela
		Engine::Resume();
		return 0;
	case WM_KILLFOCUS:		// perca de foco da janela
		Engine::Pause();
		return 0;
	case WM_DESTROY:		// destruição da janela
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
