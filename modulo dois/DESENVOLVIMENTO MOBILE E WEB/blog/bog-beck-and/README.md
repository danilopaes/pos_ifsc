# 📝 Blog - API de Mensagens

Bem-vindo à API do Blog! Este projeto é uma aplicação Node.js que oferece autenticação de usuários, cadastro, login e um sistema de mensagens. Ideal para estudos e como base para aplicações web modernas.

---

## 🚀 Visão Geral

Esta API permite:
- Cadastro de usuários com senha protegida por hash
- Login e autenticação via JWT
- Postagem de mensagens protegidas por autenticação
- Listagem de mensagens públicas

---

## 📁 Estrutura do Projeto

```
meu-projeto/
├── db.js                 # Configuração do banco de dados SQLite e Knex
├── index.js              # Código principal da aplicação Express
├── meubanco.db           # Banco de dados SQLite
├── package.json          # Dependências e scripts do projeto
├── package-lock.json     # Lockfile do npm
├── node_modules/         # Dependências instaladas
└── README.md             # Documentação do projeto
```

---

## ⚙️ Requisitos

- Node.js (v14+ recomendado)
- npm (gerenciador de pacotes)

As principais dependências são:
- express
- knex
- sqlite3
- bcryptjs
- jsonwebtoken

---

## 🛠️ Instalação e Uso

1. **Clone o repositório:**
```bash
git clone <url-do-repo>
cd meu-projeto
```

2. **Instale as dependências:**
```bash
npm install
```

3. **Execute a aplicação:**
```bash
node index.js
```

4. **A API estará disponível em:**
```
http://localhost:3000/
```

---

## 🧩 Endpoints Principais

- `POST /signup` → Cadastro de usuário
- `POST /login` → Login de usuário (retorna token JWT)
- `POST /mensagens` → Criar mensagem (autenticado)
- `GET /mensagens` → Listar mensagens (público)

---

## 💡 Observações

- O banco de dados utilizado é SQLite, fácil de rodar localmente.
- Senhas são armazenadas com hash usando bcryptjs.
- O projeto é ideal para fins didáticos e prototipagem.

---

## 👨‍💻 Autor

Desenvolvido para fins acadêmicos no IFSC.

---

## 📜 Licença

ISC.
