/**********************************************************************************
// Graphics (Arquivo de Cabe�alho)
//
// Cria��o:     06 Abr 2011
// Atualiza��o: 20 Ago 2021
// Compilador:  Visual C++ 2019
//
// Descri��o:   Graphics � uma classe que faz uso das fun��es do Direct3D para
//              acessar o hardware de acelera��o gr�fica da placa de v�deo.
//
**********************************************************************************/

#ifndef _PROGJOGOS_GRAPHICS_H_
#define _PROGJOGOS_GRAPHICS_H_

// ---------------------------------------------------------------------------------
// Inclus��es

#include <dxgi.h>				// infraestrutura principal do directX
#include <d3d11.h>				// principais fun��es do direct3D
#include "Window.h"				// cria e configura uma janela do windows
#include "Types.h"				// tipos especificos da engine

// ---------------------------------------------------------------------------------

class Graphics {
	private:
		D3D_FEATURE_LEVEL			  featureLevel;			// n�vel de recursos D3D suportados pelo hardware
		IDXGISwapChain				* swapChain;			// swap chain
		ID3D11RenderTargetView		* renderTargetView;		// render target view do backbuffer
		ID3D11BlendState			* blendState;			// configura��o de mistura de cores
		float						  bgColor[4];			// cor de fundo do backbuffer
		bool						  vSync;				// vertical sync
	public:
		Graphics();											// construtor
		~Graphics();										// destrutor

		static ID3D11Device			* device;				// dispositivo gr�fico
		static ID3D11DeviceContext  * context;	        	// contexto do dispositivo gr�fico
		static D3D11_VIEWPORT		  viewport;				// viewport

		void VSync(bool state);								// liga/desliga vertical sync
		void Clear();										// limpa o backbuffer com a cor de fundo
		void Present();										// apresenta o desenho na tela
		bool Initialize(Window* window);					// inicializa o direct3D
};

// ---------------------------------------------------------------------------------
// M�todos Inline

// liga/desliga vertical sync
inline void Graphics::VSync(bool state) {
	vSync = state;
}

// limpa o backbuffer para o pr�ximo quadro
inline void Graphics::Clear() {
	context->ClearRenderTargetView(renderTargetView, bgColor);
}

// apresenta o desenho na tela (troca o frontbuffer com o backbuffer)
inline void Graphics::Present() {
	swapChain->Present(vSync, NULL);
	context->OMSetRenderTargets(1, &renderTargetView, nullptr);
}
// ---------------------------------------------------------------------------------

#endif