#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm> // Necessário para utilizar o algoritmo de ordenação std::sort
#include <concepts>

// Restrição de tipo (C++20 Concepts): Garante que a classe manipule apenas tipos aritméticos (int, double, float)
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

// Estrutura de Dados (Struct) para agrupar e retornar os resultados de forma limpa e coesa
struct StatisticalMetrics {
    double mean;        // Média aritmética
    double std_dev;     // Desvio padrão populacional
    double q1;          // 1º Quartil (Percentil 25%): Limite dos 25% menores valores
    double median;      // 2º Quartil (Percentil 50% / Mediana): Divide o conjunto ao meio
    double q3;          // 3º Quartil (Percentil 75%): Limite dos 75% menores valores
    double iqr;         // Amplitude Interquartil (Interquartile Range): Distância entre Q3 e Q1
};

template<Numeric T>
class DataSummary {
public:
    static StatisticalMetrics compute_stats(const std::vector<T>& data) {
        // Cláusula de salvaguarda: Evita processamento e divisões por zero se o vetor estiver vazio
        if (data.empty()) return {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

        size_t n = data.size();

        // ====================================================================
        // 1. CÁLCULO DE MÉDIA E DESVIO PADRÃO (MÉTRICAS DE DISPERSÃO E CENTRALIDADE)
        // ====================================================================
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        double mean = sum / n;
        
        // Variância calculada via produto interno: E[X^2] - (E[X])^2. Abordagem O(N) de alta performance.
        double variance = std::inner_product(data.begin(), data.end(), data.begin(), 0.0) / n - (mean * mean);
        double std_dev = std::sqrt(variance);

        // ====================================================================
        // 2. PREPARAÇÃO DOS DADOS PARA MÉTODOS POSICIONAIS
        // ====================================================================
        // Clonamos o vetor original para preservar os dados originais do usuário.
        // std::sort rearranja os elementos em ordem crescente (gerando o ROL estatístico) em complexidade O(N log N).
        std::vector<T> sorted_data = data;
        std::sort(sorted_data.begin(), sorted_data.end());

        double q1 = 0.0, median = 0.0, q3 = 0.0;

        // ====================================================================
        // 3. FUNÇÃO AUXILIAR (LAMBDA) PARA MEDIANA EM SUBINTERVALOS
        // ====================================================================
        // Captura o vetor por referência e calcula a mediana no intervalo semiaberto [start, end)
        auto find_median = [](const std::vector<T>& vec, size_t start, size_t end) -> double {
            size_t size = end - start;
            if (size == 0) return 0.0;
            
            size_t mid = start + size / 2;
            
            // Se o tamanho do subconjunto for ímpar, retorna o elemento do meio.
            // Se for par, realiza a média aritmética dos dois elementos centrais.
            if (size % 2 != 0) {
                return static_cast<double>(vec[mid]);
            } else {
                return (static_cast<double>(vec[mid - 1]) + static_cast<double>(vec[mid])) / 2.0;
            }
        };

        // ====================================================================
        // 4. SEMENTAÇÃO DOS QUARTIS (MÉTODO INCLUSIVO/MÉTODO DE TUKEY)
        // ====================================================================
        // Cálculo da Mediana Geral (Q2) sobre o vetor completo (de 0 a n)
        median = find_median(sorted_data, 0, n);

        // Estratégia de partição dependendo da paridade de N:
        if (n % 2 != 0) {
            // Se N for ÍMPAR: O elemento central da mediana geral é EXCLUÍDO dos subconjuntos.
            // Q1 é a mediana da metade inferior: intervalo [0, n/2)
            q1 = find_median(sorted_data, 0, n / 2);
            // Q3 é a mediana da metade superior: intervalo [n/2 + 1, n)
            q3 = find_median(sorted_data, n / 2 + 1, n);
        } else {
            // Se N for PAR: O conjunto divide-se perfeitamente de forma simétrica ao meio.
            // Q1 é a mediana da metade inferior: intervalo [0, n/2)
            q1 = find_median(sorted_data, 0, n / 2);
            // Q3 é a mediana da metade superior: intervalo [n/2, n)
            q3 = find_median(sorted_data, n / 2, n);
        }

        // IQR mede a dispersão dos 50% dados centrais, eliminando o efeito de extremos (outliers).
        double iqr = q3 - q1;

        // Retorna a struct populada por meio de agregação direta
        return {mean, std_dev, q1, median, q3, iqr};
    }
};

int main() {
    // Vetor de amostras contendo 20 elementos (par) para validação empírica dos algoritmos
    std::vector<double> sensor_readings = {24.5, 22.1, 25.0, 23.8, 22.8, 24.1, 23.0, 17.2, 19.9, 21.7,
                                           27.7, 19.3, 21.7, 25.5, 22.6, 20.9, 26.4, 23.3, 26.7, 20.1};

    // Executa a análise estatística completa
    StatisticalMetrics stats = DataSummary<double>::compute_stats(sensor_readings);

    // Saída rica de dados no terminal para validação de Qualidade / Data Science
    std::cout << "--- Estatísticas Avançadas do Sensor ---" << std::endl;
    std::cout << "Média Numérica:   " << stats.mean << std::endl;
    std::cout << "Desvio Padrão :   " << stats.std_dev << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Primeiro Quartil (Q1): " << stats.q1 << std::endl;
    std::cout << "Mediana (Q2)         : " << stats.median << std::endl;
    std::cout << "Terceiro Quartil (Q3): " << stats.q3 << std::endl;
    std::cout << "Amplitude IQR        : " << stats.iqr << std::endl;

    return 0;
}

