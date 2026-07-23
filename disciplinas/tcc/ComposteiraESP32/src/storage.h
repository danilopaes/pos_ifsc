#pragma once
#include "dados.h"

void storageIniciar(DadosSistema& dados);
void storageRegistrar(const DadosSistema& dados);
void appendJsonLog(const DadosSistema& dados);
