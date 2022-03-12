/**********************************************************************************
// Graphics (C�digo Fonte)
//
// Cria��o:     06 Abr 2011
// Atualiza��o: 20 Ago 2021
// Compilador:  Visual C++ 2019
//
// Descri��o:   Graphics � uma classe que faz uso das fun��es do Direct3D para
//              acessar o hardware de acelera��o gr�fica da placa de v�deo.
//
**********************************************************************************/

#include "Graphics.h"

// -------------------------------------------------------------------------------
// Inicializa��o de membros est�ticos da classe

ID3D11Device* Graphics::d3dDev = nullptr;					// dispositivo gr�fico
ID3D11DeviceContext* Graphics::d3dDevContext = nullptr;		// contexto do dispositivo gr�fico
D3D11_VIEWPORT Graphics::viewport = { 0 };					// viewport

// -------------------------------------------------------------------------------

Graphics::Graphics() {
	// inicializa membros v�riaveis
	swapChain			= nullptr;							// ponteiro para swap chain
	renderTargetView	= nullptr;							// render target view
	blendState			= nullptr;							// mistura de cores
	featureLevel		= D3D_FEATURE_LEVEL_11_0;			// vers�o do Direct3D

	bgColor[0]			= 0.0f;								// Red
	bgColor[1]			= 0.0f;								// Green
	bgColor[2]			= 0.0f;								// Blue
	bgColor[3]			= 0.0f;								// Alpha (0 = transparente)

	vSync				= false;							// vertical sync desligado
}

// -------------------------------------------------------------------------------

Graphics::~Graphics() {
	// libera blend state
	if (blendState) {
		blendState->Release();
		blendState = nullptr;
	}

	// libera render-target
	if (renderTargetView) {
		renderTargetView->Release();
		renderTargetView = nullptr;
	}

	// libera swap chain
	if (swapChain) {
		// Direct3D � incapaz de fechar quando em tela cheia
		swapChain->SetFullscreenState(false, NULL);
		swapChain->Release();
		swapChain = nullptr;
	}

	// libera o contexto do dispositivo gr�fico
	if (d3dDevContext) {
		// restaura ao estado original
		d3dDevContext->ClearState();
		d3dDevContext->Release();
		d3dDevContext = nullptr;
	}

	// libera o dispositivo gr�fico
	if (d3dDev) {
		d3dDev->Release();
		d3dDev = nullptr;
	} 
}

// -------------------------------------------------------------------------------

bool Graphics::Initialize(Window* window) {
	// ----------------------------
	// Dispositivo Direct3D
	// ----------------------------

	uint createDeviceFlags = 0;

#ifdef _DEBUG
	// exibe mensagens de erro do Direct3D em modo de depura��o
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// cria dispositivo para acessar dispositivo Direct3D
	if FAILED(
		D3D11CreateDevice(
			NULL,							// adaptador de video (NULL = adaptador padr�o)
			D3D_DRIVER_TYPE_HARDWARE,		// tipo de driver D3D (Hardware, Reference ou Software)
			NULL,							// ponteiro para rasterizador em software
			createDeviceFlags,				// modo de depura��o ou modo normal
			NULL,							// featureLevels do direct3D (NULL = maior suportada)
			0,								// tamanho do vetor featureLevels
			D3D11_SDK_VERSION,				// vers�o do SDK do Direct3D
			&d3dDev,						// guarda o dispositivo D3D criado
			&featureLevel,					// vers�o do direct3D utilizada
			&d3dDevContext))				// contexto do dispositivo D3D
		return false;

	// ----------------------------
	// Cor de fundo do Direct3D
	// ----------------------------

	// ajusta a cor de fundo do backbuffer para a mesma cor da janela
	COLORREF color = window->Color();

	bgColor[0] = GetRValue(color) / 255.0f;		// Red
	bgColor[1] = GetGValue(color) / 255.0f;		// Green
	bgColor[2] = GetBValue(color) / 255.0f;		// Blue
	bgColor[3] = 1.0f;							// Alpha (1 = cor s�lida)

	// ----------------------------
	// Interfaces DXGI
	// ----------------------------

	// Cria objeto para a infraestrutura gr�fica
	IDXGIDevice* dxgiDevice = nullptr;
	if FAILED(d3dDev->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice))
		return false;

	// cria objeto para adaptador de video (placa gr�fica)
	IDXGIAdapter* dxgiAdapter = nullptr;
	if FAILED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter))
		return false;

	// cria objeto para a f�brica DXGI
	IDXGIFactory* dxgiFactory = nullptr;
	if FAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory))
		return false;

	// ----------------------------
	// Swap Chain
	// ----------------------------

	// descri��o de uma swap chain
	DXGI_SWAP_CHAIN_DESC swapDesc = { 0 };
	swapDesc.BufferDesc.Width = uint(window->Width());				// largura do backbuffer
	swapDesc.BufferDesc.Height = uint(window->Height());			// altura do backbuffer
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;					// taxa de atualiza��o em hertz
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;				// numerador � um inteiro e n�o um racional
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// formato de cores RGB 8 bits
	swapDesc.SampleDesc.Count = 1;									// amostras por pixel (antialiasing)
	swapDesc.SampleDesc.Quality = 0;								// n�vel de qualidade da imagem
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			// utilize superficie como RENDER-TARGET
	swapDesc.BufferCount = 2;										// n�meros de buffers (front + back)
	swapDesc.OutputWindow = window->Id();							// identificador da janela
	swapDesc.Windowed = (window->Mode() != FULLSCREEN);				// modo janela ou tela cheia
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;			// descarta superficie ap�s apresenta��o
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// muda resolu��o do monitor em tela

	// cria uma swap chain
	if FAILED(dxgiFactory->CreateSwapChain(d3dDev, &swapDesc, &swapChain))
		return false;

	// impede a DXGI de monitorar ALT + ENTER e alterar entre windowed/fullscreen
	if FAILED(dxgiFactory->MakeWindowAssociation(window->Id(), DXGI_MWA_NO_ALT_ENTER))
		return false;

	// ----------------------------
	// Render Target
	// ----------------------------

	// pega a superficie backbuffer de uma swapchain
	ID3D11Texture2D* backBuffer = nullptr;
	if FAILED(swapChain->GetBuffer(0, __uuidof(backBuffer), (void**)(&backBuffer)))
		return false;

	// cria uma render-target view do backbuffer
	if FAILED(d3dDev->CreateRenderTargetView(backBuffer, NULL, &renderTargetView))
		return false;

	// liga uma render-target ao est�gio de output-merger
	d3dDevContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

	// ----------------------------
	// Viewport / Rasterizar
	// ----------------------------

	// configura uma viewport
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = float(window->Width());
	viewport.Height = float(window->Height());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// liga a viewportao est�gio de rasteriza��o
	d3dDevContext->RSSetViewports(1, &viewport);

	// ----------------------------
	// Blend State
	// ----------------------------

	// Equa��o de mistura de cores (blending):
	// finalColor = SrcColor * SrcBlend <OP> DestColor * DestBlend

	// Combinando superficies transparentes (Alpha Blending)
	// finalColor = SrcColor * SrcAlpha + DestColor * (1-SrcAlpha)

	D3D11_BLEND_DESC blendDesc = { 0 };
	blendDesc.AlphaToCoverageEnable = false;							// destaca a silhueta das sprites
	blendDesc.IndependentBlendEnable = false;							// usa mesma mistura para todos os render targets
	blendDesc.RenderTarget[0].BlendEnable = true;						// habilita o blending
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			// fator de mistura da fonte
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	// destino da mistura RGB � o alpha invertido
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				// opera��o de adi��o da mistura de cores
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;			// fonte da mistura Alpha � o alpha pixel shader
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;		// destino da mistura Alpha � o alpha invertido
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;		// opera��o de adi��o na mistura de cores
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;				// componentes de cada pixel que podem ser sobrescritos

	// cria o blend state
	if FAILED(d3dDev->CreateBlendState(&blendDesc, &blendState))
		return false;

	// liga o blend state ao estagio de Output-Merger
	d3dDevContext->OMSetBlendState(blendState, nullptr, 0xffffffff);

	// ----------------------------
	// Libera interface DXGI
	// ----------------------------
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();
	backBuffer->Release();

	// inicializa��o bem sucedida
	return true;
}
// -------------------------------------------------------------------------------
