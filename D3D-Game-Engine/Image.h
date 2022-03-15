/**********************************************************************************
// Image (Arquivo de Cabeçalho)
//
// Criação:     16 Mar 2012
// Atualização: 13 Ago 2021
// Compilador:  Visual C++ 2019
//
// Descrição:   Define uma classe para representar imagens
//
**********************************************************************************/

#ifndef _PROGJOGOS_IMAGE_H_
#define _PROGJOGOS_IMAGE_H_

// ---------------------------------------------------------------------------------
// inclusões

#include "Types.h"			// tipos personalizados do motor
#include "Texture.h"		// funções para carregar textura
#include <string>			// classe de string c++

// ---------------------------------------------------------------------------------

class Image {
	private:
		ID3D11ShaderResourceView * textureView;		// view associada a textura
		uint					   width;			// largura da imagem
		uint					   height;			// altura da imagem
	
	public:
		Image(std::string filename);     			// constroi a imagem a partir de um arquivo
		~Image();

		uint Width() const;							// retorna largura da imagem
		uint Height() const;						// retorna altura da imagem
		ID3D11ShaderResourceView* View() const;     // retorna ponteiro para a view da imagem
};

// ---------------------------------------------------------------------------------
// Métodos Inline

// retorna a largura da textura
inline uint Image::Width() const {
	return width;
}

// retorna a altura da imagem
inline uint Image::Height() const {
	return height;
}

// retorna ponteiro para a textura D3D
inline ID3D11ShaderResourceView* Image::View() const {
	return textureView;
}

// --------------------------------------------------------------------------------
#endif
