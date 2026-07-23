#include "dados.h"
#include "config.h"

const char* avaliarEstado(const DadosSensores& s) {
  if (!s.temperaturaValida || !s.umidadeValida) return "ATENCAO";
  const bool temperaturaIdeal = s.temperatura >= TEMP_IDEAL_MIN && s.temperatura <= TEMP_IDEAL_MAX;
  const bool umidadeIdeal = s.umidade >= UMIDADE_IDEAL_MIN && s.umidade <= UMIDADE_IDEAL_MAX;
  if (temperaturaIdeal && umidadeIdeal) return "IDEAL";
  if (s.temperatura < 10.0f || s.temperatura > 45.0f || s.umidade < 20.0f || s.umidade > 90.0f) return "ALERTA";
  return "ATENCAO";
}

void registrarTransicaoEstado(DadosSistema& dados) {
  static const char* anterior = nullptr;
  if (anterior != dados.estado) {
    Serial.print(F("[ALGORITMO] estado: "));
    Serial.println(dados.estado);
    anterior = dados.estado;
  }
}
