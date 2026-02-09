### Reunião – 22/10/2025

Foi realizada a configuração inicial da plataforma para captura e leitura de dados dos sensores. Nessa etapa, foram configurados o **Telegraf**, o **Mosquitto** e a **rede Wi-Fi**.

No Telegraf, foi ajustado o arquivo de configuração `telegraf.conf`, onde são definidos os *topics* que serão capturados.

No arquivo enviado para a placa ESP32, foram configurados:

- nome da rede Wi-Fi a ser utilizada;

- senha da rede;

- pinos de conexão dos sensores.

Durante os testes, foi verificado que o Telegraf e o Mosquitto estavam funcionando corretamente e conectando ao broker MQTT. Porém, os dados ainda não estavam sendo enviados ao InfluxDB. Essa inconsistência segue em análise para correção.

Também foi identificado que a rede Wi-Fi disponível era apenas 5 GHz, o que impedia o funcionamento correto da ESP32. Para resolver o problema, foi criada uma rede Wi-Fi 2.4 GHz dedicada. Com isso, será realizado um novo teste para verificar se os dados serão enviados corretamente ao InfluxDB.

Caso os dados sejam recebidos com sucesso, será feita a configuração do *topic* para consumo no Grafana.

---

### Teste de Display – 04/02/2026

No dia 04/02/2026 foi realizado o primeiro teste do display OLED. Para validação, foram desenvolvidos três códigos de teste:

um exibindo a mensagem “OLED OK”;

um com animação do símbolo do infinito;

um com animação de um boneco andando.

Esses testes tiveram como objetivo verificar o funcionamento do display e validar a exibição correta de mensagens e imagens.

O próximo passo é integrar o display ao código principal do projeto para testar a apresentação das variáveis em tempo real.

---

### Encontro para demonstração de funcionamento dos sensores — 07/02/2026

Nesse dia, a Renata esteve em minha casa para verificarmos o funcionamento dos sensores. Foram utilizados o **sensor DHT22** e o **sensor de gás MQ-4**.

Durante os testes, observamos que os sensores precisam de um tempo mínimo de aquecimento/estabilização após serem ligados para começar a enviar dados consistentes. Também identificamos a necessidade de uma análise mais detalhada do funcionamento de cada sensor, para extrair os dados da forma mais adequada e utilizá-los corretamente na demonstração do projeto, além de possibilitar comparações com dados de estações meteorológicas da região.

Após a validação inicial do funcionamento dos dois sensores, alinhamos as tarefas que ainda estão em andamento conforme o cronograma do projeto.

**Link do relatório de pendências:**  
<https://docs.google.com/document/d/1vK7SJ0CxSDksnt4Y9osJn-Nzqyum4z0V/edit?rtpof=true>

