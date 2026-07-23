#pragma once
#include "dados.h"

void mqttIniciar(DadosSistema& dados);
void mqttAtualizar(DadosSistema& dados);
void mqttPublicar(const DadosSistema& dados);
