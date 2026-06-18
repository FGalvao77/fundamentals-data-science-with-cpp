#include <iostream>
#include <Eigen/Dense> // Inclui o núcleo de álgebra linear computacional (matrizes, vetores e solvers)

int main() {
    // 1. DEFINIÇÃO DA MATRIZ DE DESIGN (X)
    // MatrixXd significa: Matriz de valores do tipo 'double' com tamanho dinâmico (X)
    // Instanciamos uma matriz 4x3: 4 linhas (amostras/observações) e 3 colunas (recursos/features)
    Eigen::MatrixXd X(4, 3);

    // Alimentação de dados usando o operador de inserção em fluxo (Comma-initializer) do Eigen.
    // Primeira coluna preenchida com '1' representa o termo de BIAS (Intercepto).
    // As colunas 2 e 3 são as variáveis preditoras (recursos do problema).
    X << 1, 2.5, 1.2,
         1, 3.1, 1.8,
         1, 4.2, 2.9,
         1, 5.3, 3.5;

    // 2. DEFINIÇÃO DO VETOR ALVO (y)
    // VectorXd significa: Vetor coluna do tipo 'double' com tamanho dinâmico
    // Possui 4 elementos, correspondendo exatamente às 4 amostras da matriz X
    Eigen::VectorXd y(4);
    y << 10.5, 12.8, 16.4, 19.1;

    // 3. RESOLUÇÃO DA EQUAÇÃO NORMAL VIA DECOMPOSIÇÃO LDLT
    // (.transpose()) realiza a transposição transposta da matriz X -> X^T
    // (*) executa a multiplicação matricial otimizada por SIMD
    // (.ldlt()) decompõe a matriz resultante (X^T * X) no formato L * D * L^T
    // (.solve()) resolve o sistema linear AX = B, onde A = (X^T * X) e B = (X^T * y)
    Eigen::VectorXd beta = (X.transpose() * X).ldlt().solve(X.transpose() * y);

    // 4. EXIBIÇÃO DOS COEFICIENTES OTIMIZADOS
    std::cout << "### COEFICIENTES DA REGRESSÃO (Beta) ###" << std::endl;
    // beta(0) é o peso do Bias (onde a reta cruza o eixo Y)
    std::cout << "Intercepto (Bias): " << beta(0) << std::endl;
    // beta(1) é o coeficiente angular associado à Feature 1
    std::cout << "Coeficiente X1   : " << beta(1) << std::endl;
    // beta(2) é o coeficiente angular associado à Feature 2
    std::cout << "Coeficiente X2   : " << beta(2) << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}







