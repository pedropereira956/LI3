# ✈️ Sistema de Gestão de Dados de Voo e Reservas (LI3)

> Projeto prático desenvolvido no âmbito da unidade curricular de **Laboratórios de Informática III** do curso de Engenharia Informática na **Universidade do Minho** (Ano Letivo 2025/2026).

📄 **[Clica aqui para consultar o Relatório Técnico do Projeto (Fase 2)](./relatorio-fase2.pdf)**

Este projeto consiste num sistema robusto em linguagem C focado no processamento eficiente, parsing, validação e interrogação de grandes volumes de dados de companhias aéreas, aeroportos, passageiros, voos e reservas associadas.

---

## 💻 Tecnologias e Estrutura

* **Linguagem Principal:** C (padrão C99/C11)
* **Organização Modular:**
  * `src/` e `include/`: Divisão clara entre Entidades, Catálogos de dados, Módulos de Validação, Parsers e Executáveis de teste/interativos.
  * `resultados-esperadosFASE2/`: Módulo de validação contendo ficheiros de output para controlo de qualidade e automação de testes.
* **Automação de Build:** `Makefile` integrado para compilação rápida e gestão de dependências.

---

## ⚙️ Pré-requisitos

Para compilar e correr este projeto, necessitas de ter instalado no teu computador:
* Um compilador de C (ex: `gcc` ou `clang`).
* A ferramenta `make`.

---

## 🚀 Como Compilar e Executar

**1. Clonar o repositório**
\`\`\`bash
git clone https://github.com/pedropereira956/nome-do-repositorio.git
cd nome-do-repositorio
\`\`\`

**2. Compilar o código**
Na raiz do projeto (onde se encontra o Makefile), executa:
\`\`\`bash
make
\`\`\`
*(Isto irá gerar os ficheiros executáveis: `programaPrincipal`, `programaIterativo` e `programaTestes`)*

**3. Executar o programa**

*   **Modo Batch (Ficheiro de Comandos):**
    Ideal para processamento massivo de instruções.
    \`\`\`bash
    ./programaPrincipal caminho_para_ficheiros_csv caminho_para_comandos.txt
    \`\`\`

*   **Modo Interativo:**
    Navegação por menus diretamente no terminal.
    \`\`\`bash
    ./programaIterativo
    \`\`\`

*   **Modo de Testes:**
    Validação de memória e verificação de resultados esperados.
    \`\`\`bash
    ./programaTestes
    \`\`\`

**4. Limpar ficheiros de compilação**
Para apagar os ficheiros `.o` e os binários gerados, mantendo o diretório limpo:
\`\`\`bash
make clean
\`\`\`

---

## ✒️ Autor

* **Pedro Pereira** - [@pedropereira956](https://github.com/pedropereira956) - [Teu Link do LinkedIn]
* Estudante de Engenharia Informática na Universidade do Minho.
