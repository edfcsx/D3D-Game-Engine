/**********************************************************************************
// Timer (Arquivo de Cabeçalho)
//
// Criação:     02 Abr 2011
// Atualização: 10 Ago 2021
// Compilador:  Visual C++ 2019
//
// Descrição:   Usa um contador de alta precisão para medir o tempo.
//
**********************************************************************************/

#ifndef _PROGJOGOS_TIMER_H_
#define _PROGJOGOS_TIMER_H_

// -------------------------------------------------------------------------------

#include <windows.h>		// acesso ao contador de alta precisão do windows

// -------------------------------------------------------------------------------

class Timer {
	private:
		LARGE_INTEGER start, end;	// valores de inicio e fim do contador
		LARGE_INTEGER freq;			// frequência do contador
		bool stoped;				// estado da contagem
	public:
		Timer();					// construtor

		void Start();				// inicia/retoma contagem do tempo
		void Stop();				// para a contagem do tempo
		float Reset();				// reinicia a contagem e retorna o tempo transcorrido	
		float Elapsed();			// retorna o tempo transcorrido em segundos
		bool Elapsed(float secs);	// verifica se transcorreu "secs" segundos
};

// -------------------------------------------------------------------------------
// Funções Inline

inline bool Timer::Elapsed(float secs) {
	return (Elapsed() >= secs ? true : false);
}

#endif