
### Enviar e-mail com os dados pelo Grafana

Para **enviar e-mail com os dados pelo Grafana**, você deve usar o **sistema de Alertas do Grafana**. O fluxo é este:

**MQTT → Grafana (dashboard/painel) → Alerta → E-mail**

***

## 1️⃣ Pré-requisitos

- Dados já **aparecendo no Grafana** (via MQTT → InfluxDB/Prometheus/etc.)

- Acesso de **admin** no Grafana

- Servidor SMTP (Gmail, Outlook, institucional, etc.)

***

## 2️⃣ Configurar o e-mail (SMTP) no Grafana

No servidor onde o Grafana roda, edite o arquivo:

```bash
grafana.ini
```

Seção `[smtp]`:

```ini
[smtp]
enabled = true
host = smtp.gmail.com:587
user = seuemail@gmail.com
password = SUA_SENHA_DE_APP
from_address = seuemail@gmail.com
from_name = Grafana IFSC
skip_verify = true
```

⚠️ **Gmail:** use **senha de app**, não a senha normal.

Depois:

```bash
sudo systemctl restart grafana-server
```

***

## 3️⃣ Criar um canal de notificação (Contact Point)

No Grafana:

```pgsql
Alerting → Contact points → New contact point
```

- Type: **Email**

- Endereços:

  ```css
  destino@email.com
  ```

Salvar.

***

## 4️⃣ Criar o alerta com os dados do sensor

1. Vá no **painel (panel)** que mostra o dado

2. Clique em **Edit**

3. Aba **Alert**

4. **Create alert rule**

Exemplo:

- Condição:

  ```less
  WHEN distancia < 20
  ```

- Avaliação: a cada 1 min

- Duração: 1 min

- Contact point: **Email**

***

## 5️⃣ Conteúdo do e-mail (automático)

O Grafana envia automaticamente:

- Nome do alerta

- Valor do sensor

- Horário

- Painel e dashboard

Exemplo de assunto:

```yaml
🚨 Alerta: Nível crítico de chorume
```

Exemplo de corpo:

```cpp
Sensor: Ultrassônico
Valor: 18 cm
Horário: 2026-02-10 14:32
Dashboard: Monitoramento Chorume
```

***

## 6️⃣ (Opcional) Enviar dados periódicos por e-mail

Se quiser **relatórios automáticos** (ex: diário/semanal):

- Use **Grafana Reporting** (Enterprise)  

  ou

- Integre com **Webhook → script → e-mail**

***

## Resumo rápido

✔ Dados no Grafana  
✔ SMTP configurado  
✔ Contact Point (Email)  
✔ Alert Rule no painel
