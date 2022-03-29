/**********************************************************************************
// Sprite (Arquivo de Cabeçalho)
//
// Criação:     11 Jul 2007
// Atualização: 13 Ago 2021
// Compilador:  Visual C++ 2019
//
// Descrição:   Define uma classe para representar um sprite
//
**********************************************************************************/

#ifndef _PROGJOGOS_SPRITE_H_
#define _PROGJOGOS_SPRITE_H_

// ---------------------------------------------------------------------------------

#include "Image.h"

// ---------------------------------------------------------------------------------

struct SpriteData {
	float x, y;
	float scale;
	float depth;
	float rotation;
	uint width;
	uint height;
	ID3D11ShaderResourceView* texture;
};

// ---------------------------------------------------------------------------------

struct Layer {
	static const float FRONT;
	static const float UPPER;
	static const float MIDDLE;
	static const float LOWER;
	static const float BACK;
};

// ---------------------------------------------------------------------------------

class Sprite {
	private:
		SpriteData sprite;							// dados da sprite
		bool localImage;							// imagem local ou externa
		const Image * image;						// ponteiro para uma imagem
	public:
		Sprite(std::string filename);				// constroi a sprite a partir de um arquivo
		Sprite(const Image * img);					// constroi a sprite a partir de uma imagem existente
		~Sprite();									// destrutor de sprite
		
		int Width();								// largura do sprite
		int Height();								// altura do sprite

		// desenha a imagem na posição (x,y) e profundidade (z)
		void Draw(float x, float y, float z = Layer::MIDDLE);
};


// ---------------------------------------------------------------------------------
// Funções Inline

// retorna a largura da sprite
inline int Sprite::Width() {
	return image->Width();
}

// retorna a altura da sprite
inline int Sprite::Height() {
	return image->Height();
}

// ---------------------------------------------------------------------------------

#endif

// ---------------------------------------------------------------------------------
