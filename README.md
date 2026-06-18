# Data Science & Machine Learning com C++ Moderno

Este repositório contém uma coleção de implementações práticas voltadas para a aplicação de Ciência de Dados e Machine Learning Clássico utilizando o ecossistema **C++ Moderno (C++20)** e computação linear de alta performance.

O objetivo deste projeto é demonstrar o controle estrito de memória, previsibilidade de tempo de execução e a eficiência matemática que o C++ oferece para engenharia de recursos e automação industrial.

---

## 📁 Estrutura do Repositório

```text
data-science-with-cpp/
├── 01-statistical_analysis.cpp  # Estatística Descritiva Básica (Média e Desvio Padrão)
├── 02-advanced_statistics.cpp   # Estatística Posicional (Mediana, Quartis Q1/Q3 e IQR)
├── 03-linear_regression.cpp     # Regressão Linear Múltipla via Equação Normal (Eigen)
└── README.md                    # Documentação oficial do projeto
```

### 🛠️ **Pré-requisitos do Sistema (Ubuntu Linux)**

Antes de compilar os arquivos, é necessário garantir que o seu sistema operacional possui o compilador de C++ atualizado e a biblioteca de álgebra linear Eigen 3.

Abra o terminal do seu Ubuntu e instale as dependências com o comando abaixo:

```bash
sudo apt update
sudo apt install build-essential clang libeigen3-dev
```

- `build-essential`: Instala o compilador `g++` e ferramentas essenciais de build.

- `libeigen3-dev`: Biblioteca de matrizes utilizada no script de regressão linear (instalada em `/usr/include/eigen3`).

### 🚀 **Como Executar cada Script**

Você pode executar os códigos de duas maneiras dentro do ambiente do VS Code:

🛠️ **Método Automático (Atalhos do VS Code)**

1. Abra a pasta do projeto no VS Code.

2. Abra o arquivo .cpp que deseja executar (ex: 01-statistical_analysis.cpp).

3. Certifique-se de que o arquivo está ativo na tela e pressione Ctrl + Shift + B para compilar.

4. Abra o terminal integrado do VS Code (`Ctrl + ``) e execute o binário gerado correspondente:

```bash
./01-statistical_analysis
# ou
./02-advanced_statistics
# ou
./03-linear_regression
```

### 💻 **Método Manual (Direto pelo Terminal)**

Se preferir compilar manualmente pelo terminal integrado, utilize as instruções específicas para cada arquivo abaixo:

**1. Análise Estatística Básica (`01-statistical_analysis.cpp`)**

Calcula a Média Numérica e o Desvio Padrão Populacional de leituras de sensores em tempo contíguo.

```bash
g++ -std=c++20 01-statistical_analysis.cpp -o 01-statistical_analysis
./01-statistical_analysis
```

**2. Análise Estatística Avançada (`02-advanced_statistics.cpp`)**

Gera o ROL estatístico (ordenação via `std::sort`) e calcula Mediana, Quartis ($Q_1$ e $Q_3$) e a amplitude interquartil ($IQR$) para detecção de anomalias.

```bash
g++ -std=c++20 02-advanced_statistics.cpp -o 02-advanced_statistics
./02-advanced_statistics
```

**3. Regressão Linear Múltipla (`03-linear_regression.cpp`)**

Resolve a Equação Normal de Mínimos Quadrados ($X^T X \beta = X^T y$) utilizando Decomposição Cholesky Modificada (LDLT) para máxima estabilidade numérica e performance SIMD.

```bash
g++ -std=c++20 -I/usr/include/eigen3 03-linear_regression.cpp -o 03-linear_regression
./03-linear_regression
```

### 🛡️ **Recursos Avançados Utilizados**

- C++20 Concepts: Restrição de tipos de dados (`template<Numeric T>`) em tempo de compilação.

- Structured Bindings (C++17): Desempacotamento limpo de tuplas e pares de dados.

- Expression Templates (Eigen 3): Eliminação de matrizes temporárias em memória durante computação linear.# data-science-with-cpp

