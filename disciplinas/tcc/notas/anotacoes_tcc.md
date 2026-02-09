### Configurações Telegraf para o TCC

- configurado a rede ssid-wifi
- setar o server do mqtt
- ajustar o topico do mqtt

- chave do influxDB
token = "9G1rJGQb2Ev5cV0WBkStNCsPxork4-nKpJUmNrng5UAoAzjTDShfdnJFlLgWi5yYOajtoOZ56w12T-p3dwENMQ=="

* pesquisa arquivo telegraf

inputs.mqtt_consumer
data_format
inputs.mqtt_consumer.topic_parsing

### Padrão do tópico que queremos processar
    topic = "iot/curso/+/+/+"
    # Define que o 4º campo (nome do sensor) será o nome da 'measurement' no InfluxDB
    measurement = "_/_/_/_/measurement"
    # Define que o 2º (local) e 3º (dispositivo) campos serão as tags
    tags = ["_/_/local/dispositivo/_"]

outputs.influxdb
urls


### Senhas

* wifi
Rede: projetotcc
Senha: danilo123

* InfluxDB
- Usuario: danilo
- Senha: danilo123

### Sensores

Cada topico deve ser enviado separadamente para o MQTT para que ele seja enviado para o grafana, também deve ser verficado como está no arquivo do telegraf essa questão

*topicos dos sensores*

const char* topic_temp  = "IFSCTub/ETCC001/temperatura";
const char* topic_hum   = "IFSCTub/ETCC001/umidade";
const char* topic_gas   = "IFSCTub/ETCC001/gas";
const char* topic_dist  = "IFSCTub/ETCC001/distancia";
