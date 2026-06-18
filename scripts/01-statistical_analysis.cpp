#include <iostream>  // Fluxos de entrada e saída (std::cout)
#include <vector>    // Estrutura de dados contígua em memória (std::vector)
#include <numeric>   // Algoritmos numéricos de alta performance (std::accumulate, std::inner_product)
#include <cmath>     // Funções matemáticas avançadas (std::sqrt)
#include <concepts>  // Recursos do C++20 para restrição de tipos em tempo de compilação
#include <ranges>    // Biblioteca de visualizações e views do C++20

// Definição de um Concept (C++20): Restringe o template para aceitar APENAS tipos numéricos.
// Isso evita que o desenvolvedor tente passar um vetor de strings, gerando um erro claro no build.
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

// Classe Genérica que utiliza o Concept 'Numeric'. 
// Garante abstração de custo zero (zero-cost abstraction) para float, double, int, etc.
template<Numeric T>
class DataSummary {
public:
    // Método estático que retorna um par (Média, Desvio Padrão) processado em tempo de execução nativo
    static std::pair<double, double> compute_stats(const std::vector<T>& data) {
        // Guard Clause (Cláusula de Salvaguarda) para evitar divisão por zero em vetores vazios
        if (data.empty()) return {0.0, 0.0};

        // 1. CÁLCULO DA MÉDIA MATEMÁTICA
        // std::accumulate realiza uma redução (reduce) linear O(N).
        // Ele soma todos os elementos iniciando em 0.0, mantendo excelente localidade de cache.
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        double mean = sum / data.size();

        // 2. CÁLCULO DA VARIÂNCIA POPULACIONAL (MÉTODO EXPANDIDO)
        // O algoritmo calcula matematicamente: E[X^2] - (E[X])^2
        // std::inner_product multiplica o vetor por ele mesmo elemento a elemento (dot product) e soma tudo.
        // Isso calcula a soma dos quadrados de forma extremamente otimizada pela CPU (frequentemente vetorizada via SIMD).
        double variance = std::inner_product(data.begin(), data.end(), data.begin(), 0.0) / data.size() - (mean * mean);
        
        // 3. CÁLCULO DO DESVIO PADRÃO
        // A raiz quadrada da variância nos traz de volta à unidade de medida original dos dados
        double std_dev = std::sqrt(variance); 

        // Retorna uma estrutura std::pair contendo as duas métricas estatísticas essenciais
        return {mean, std_dev};
    }
};

int main() {
    // Vetor contíguo simulando a leitura em tempo real de sensores industriais automotivos
    std::vector<double> sensor_readings = {24.5, 22.1, 25.0, 23.8, 22.8, 24.1, 23.0, 17.2, 19.9, 21.7,
                                           27.7, 19.3, 21.7, 25.5, 22.6, 20.9, 26.4, 23.3, 26.7, 20.1};

    // Structured Binding (C++17): Desempacota o std::pair diretamente em variáveis locais legíveis
    auto [mean, std_dev] = DataSummary<double>::compute_stats(sensor_readings);

    // Exibição formatada dos resultados de qualidade no terminal
    std::cout << "\n#### ESTATÍSTICAS DO SENSOR ###" << std::endl;
    std::cout << "Média         : " << mean << std::endl;
    std::cout << "Desvio Padrão : " << std_dev << std::endl;
    std::cout << "================================" << std::endl;

    return 0;
}





