#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <map>
#include <iomanip>

using namespace std::string_literals;

// Constante matemática para cálculo de Gaussiana
const double PI = 3.14159265358979323846;

// Estrutura dos dados do cliente
struct Customer {
    std::string customer_id;    // Atributo 0: Identificação do cliente
    double credit_score;        // Atributo 1: Pontuação de crédito
    double engagement;          // Atributo 2: Engamento
    double churn;               // Atributo 3: Taxa de cancelamento
    double fidelity;            // Atributo 4: Fidelidade
    std::string risk;           // Alvo: Risco (classe)

};

// Estrutura para armazenar os parâmetros estatísticos de cada atributo por classe
struct AtrributeStatistics {
    double average = 0.0;       // média
    double variance = 0.0;      // variância
};

// Classificador Naive Bayes Guassiano
class NaiveBayesGaussian {
private:
    std::map<std::string, double> prioris;  // P(Classe)
    // Mapeamento: [classe][indice_atributo] -> média e variância
    std::map<std::string, std::vector<AtrributeStatistics>> model;
    std::vector<std::string> classes = {"Low", "Medium", "High"};

    // Função de Densidade de Probabilidade Guasssiana
    double calculateGaussianProbability(double x, double average, double variance) {
        if (variance == 0.0) variance = 1e-4;   // Evita divisão por zero
        double exponent = std::exp(-std::pow(x - average, 2) / (2 * variance));
        return (1.0 / std::sqrt(2 * PI * variance)) * exponent;
    }

public:
    void train(const std::vector<Customer>& data) {
        double total_observations = data.size();

        // 1. Calculando as Probabilidades a Priori P(Classe)
        std::map<std::string, int> class_count;
        for (const auto& c : classes) class_count[c] = 0;
        for (const auto& d : data) class_count[d.risk]++;

        for (const auto& c : classes) {
            prioris[c] = static_cast<double>(class_count[c]) / total_observations;
        }

        // 2. Calculando a Média e Variância para cada atributo numérico por classe
        for (const auto& class_ : classes) {
            std::vector<Customer> filtered;
            for (const auto& d : data) {
                if (d.risk == class_) filtered.push_back(d);
            }

            std::vector<AtrributeStatistics> stats(4);  // 4 atributos contínuos
            if (filtered.empty()) {
                model[class_] = stats;
                continue;
            }

            // Atributo 0: Score de Crédito | Atributo 1: Engajamento | Atributo 2: Churn | Atributo 3: Fidelidade
            for (size_t i = 0; i < 4; ++i) {
                double add = 0.0;
                for (const auto& f : filtered) {
                    if (i == 0) add += f.credit_score;
                    if (i == 1) add += f.engagement;
                    if (i == 2) add += f.churn;
                    if (i == 3) add += f.fidelity;
                }

                stats[i].average = add / filtered.size();

                double sum_squares = 0.0;
                for (const auto& f : filtered) {
                    double value = 0.0;
                    if (i == 0) value = f.credit_score;
                    if (i == 1) value = f.engagement;
                    if (i == 2) value = f.churn;
                    if (i == 3) value = f.fidelity;

                    sum_squares += std::pow(value - stats[i].average, 2);
                }

                // Variância amostral (n-1), se n=1 usa variância populacional
                double n = filtered.size();
                stats[i].variance = (n > 1) ? (sum_squares / (n - 1)) : (sum_squares / n);
            }

            model[class_] = stats;
        }
    }

    std::string predict(double credit_score, double engagement, double churn, double fidelity) {
        std::string best_class = "Unknown";
        double greater_probability = -1.0;

        // Armazenando os novos atributos em um vetor para iterar facilmente
        std::vector<double> x = {credit_score, engagement, churn, fidelity};

        for (const auto& class_ : classes) {
            if (prioris[class_] == 0.0) continue;   // Pula classes que não existiam no treino

            // Começa coma probabilidade a priori: P(Classe)
            double probability_class = prioris[class_]; 

            // Multiplicando pelas verossimilhanças: P(Atributo_i | Classe)
            for (size_t i = 0; i < 4; ++i) {
                double p_attribute = calculateGaussianProbability(x[i], model[class_][i].average, model[class_][i].variance);
                probability_class *= p_attribute;
            }

            // Busca a classe com a maior probabilidade a posteriori (MAP)
            if (probability_class > greater_probability) {
                greater_probability = probability_class;
                best_class = class_;
            }
        }

        return best_class;
    }
};

int main() {
    // --- GERANDO OS DADOS ANTERIORES (conjunto de dados de treino) ---
    std::mt19937 gen(42);
    std::uniform_int_distribution<> credit_dist(300, 999);
    std::uniform_real_distribution<> engagement_dist(1.0, 10.0);
    std::uniform_real_distribution<> churn_dist(0.05, 0.95);
    std::uniform_int_distribution<> fidelity_dist(100, 4999);
    std::discrete_distribution<> risk_dist({0.5, 0.3, 0.2});
    std::vector<std::string> risk_categories = {"Low", "Medium", "High"};

    std::vector<Customer> train_dataset;
    for (int i = 1; i <= 10; ++i) {
        Customer cust;
        cust.customer_id = "CUST_"s + (i < 10 ? "00"s : "0"s) + std::to_string(i);
        cust.credit_score = credit_dist(gen);
        cust.engagement = std::round(engagement_dist(gen) * 10.0) / 10.0;
        cust.churn = std::round(churn_dist(gen) * 100.0) / 100.0;
        cust.fidelity = fidelity_dist(gen);
        cust.risk = risk_categories[risk_dist(gen)];
        train_dataset.push_back(cust);
    }

    // --- INSTANCIANDO E TREINANDO O MODELO ---
    NaiveBayesGaussian model_nb;
    model_nb.train(train_dataset);

    std::cout << "\n\n### Naive Bayes Model Successfully Trained in C++ ###\n";

    // --- TESTANDO O MODELO COM NOVOS CLIENTES SINTÉTICOS --
    std::cout << "Predicting risk for new customers:\n";
    std::cout << std::string(75, '-') << "\n";
    std::cout << std::left
              << std::setw(15) << "Credit Score"
              << std::setw(15) << "Engagement"
              << std::setw(15) << "Churn Prob"
              << std::setw(15) << "Fidelity"
              << "Risk Predict" << "\n";
    std::cout << std::string(75, '-') << "\n";

    // Caso 1: Cliente com ótimos indicadores financeiros e operacionais
    double new_score_1 = 850.0;
    double new_eng_1 = 9.0;
    double new_churn_1 = 0.12;
    double new_fid_1 = 4200.0;
    std::string predict_1 = model_nb.predict(new_score_1, new_eng_1, new_churn_1, new_fid_1);

    //Caso 2: Cliente com comportamento crítico (score baixo, churn alto)
    double new_score_2 = 340.0;
    double new_eng_2 = 2.1;
    double new_churn_2 = 0.85;
    double new_fid_2 = 300.0;
    std::string predict_2 = model_nb.predict(new_score_2, new_eng_2, new_churn_2, new_fid_2);

    //Caso 3: Cliente com comportamento crítico (score baixissímo, churn médio)
    double new_score_3 = 203.0;
    double new_eng_3 = 5.3;
    double new_churn_3 = 0.67;
    double new_fid_3 = 2750.0;
    std::string predict_3 = model_nb.predict(new_score_3, new_eng_3, new_churn_3, new_fid_3);

    //Caso 4: Cliente com comportamento ótimo (score alto, churn baixo)
    double new_score_4 = 913.0;
    double new_eng_4 = 7.7;
    double new_churn_4 = 0.27;
    double new_fid_4 = 3111.0;
    std::string predict_4 = model_nb.predict(new_score_4, new_eng_4, new_churn_4, new_fid_4);

    //Caso 5: Cliente com comportamento crítico (score baixissímo, churn altissímo)
    double new_score_5 = 107.0;
    double new_eng_5 = 3.3;
    double new_churn_5 = 0.97;
    double new_fid_5 = 783.0;
    std::string predict_5 = model_nb.predict(new_score_5, new_eng_5, new_churn_5, new_fid_5);

    // Exibindo os resultados
    std::cout << std::left << std::fixed << std::setprecision(2)
              << std::setw(15) << new_score_1
              << std::setw(15) << new_eng_1
              << std::setw(15) << new_churn_1
              << std::setw(15) << new_fid_1
              << predict_1 << "\n";

    std::cout << std::left << std::fixed << std::setprecision(2)
              << std::setw(15) << new_score_2
              << std::setw(15) << new_eng_2
              << std::setw(15) << new_churn_2
              << std::setw(15) << new_fid_2
              << predict_2 << "\n";

    std::cout << std::left << std::fixed << std::setprecision(2)
              << std::setw(15) << new_score_3
              << std::setw(15) << new_eng_3
              << std::setw(15) << new_churn_3
              << std::setw(15) << new_fid_3
              << predict_3 << "\n";

    std::cout << std::left << std::fixed << std::setprecision(2)
              << std::setw(15) << new_score_4
              << std::setw(15) << new_eng_4
              << std::setw(15) << new_churn_4
              << std::setw(15) << new_fid_4
              << predict_4 << "\n";

    std::cout << std::left << std::fixed << std::setprecision(2)
              << std::setw(15) << new_score_5
              << std::setw(15) << new_eng_5
              << std::setw(15) << new_churn_5
              << std::setw(15) << new_fid_5
              << predict_5 << "\n";

    return 0;
}



