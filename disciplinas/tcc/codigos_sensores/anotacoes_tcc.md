Reunião dia 22/10/2025

foi realizado a configuração primaria para o desenvolvimento do "plataforma" para realizar a captura e leitura dos dados dos 
sensore, basicamente foi configurado o telegraf, o mosquitto a rede wifi.

no telegraf foi configurado o script telegraf.conf onde fica os topic a serem capturados

no arquivo que vai ser enviado para a placa esp32 foi configurado o nome da rede wifi a ser utilizada, a senha dessa rede
os pinos que vão ser utilizados pelo sensor.

na hora de realizar o teste conseguimos verificar que o telegraf, mosquitto estavam funcionando corretamente e conectando ao mqtt
porem os dados não estavam sendo enviado para o influxDB, essa questão ainda está sendo analisada para ser corrida.

na realização do teste foi descoberto que devido a minha rede wifi ser 5G a placa não estava conseguindo funcionar corretamente, para resolver 
esse problema foi criada uma rede wifi 2.4G para ter um bom funcionamento da placa, agora com essa rede criada irei realizar um novo teste
para ver se os dados vão chegar no influxDB

caso os dados cheguem irei realizar a configuração do topico para ser consumido pelo grafana



# Configurações Telegraf para o TCC

- configurado a rede ssid-wifi
- setar o server do mqtt
- ajustar o topico do mqtt

- chave do influxDB
token = "9G1rJGQb2Ev5cV0WBkStNCsPxork4-nKpJUmNrng5UAoAzjTDShfdnJFlLgWi5yYOajtoOZ56w12T-p3dwENMQ=="

* pesquisa arquivo telegraf

inputs.mqtt_consumer
data_format
inputs.mqtt_consumer.topic_parsing
# Padrão do tópico que queremos processar
    topic = "iot/curso/+/+/+"
    # Define que o 4º campo (nome do sensor) será o nome da 'measurement' no InfluxDB
    measurement = "_/_/_/_/measurement"
    # Define que o 2º (local) e 3º (dispositivo) campos serão as tags
    tags = ["_/_/local/dispositivo/_"]

outputs.influxdb
urls


# Senhas

* wifi
Rede: projetotcc
Senha: danilo123

* InfluxDB
- Usuario: danilo
- Senha: danilo123
