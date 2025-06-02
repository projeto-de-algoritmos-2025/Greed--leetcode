#include <stdlib.h>
#include <limits.h>

int min(int a, int b) {
    return a < b ? a : b;
}

int paintWalls(int* cost, int costSize, int* time, int timeSize) {
    int n = costSize;
    
    /* 
    DP[i][j] representa o custo mínimo para pintar 'i' paredes,
    onde o pintor pago está ocupado por 'j' unidades de tempo.
    Vamos otimizar para usar apenas um array 1D.
    */
    
    int max_time = n; // Máximo tempo necessário (todas as paredes)
    int* dp = (int*)malloc((max_time + 1) * sizeof(int));
    
    // Inicializa o array DP com infinito
    for (int j = 0; j <= max_time; j++) {
        dp[j] = INT_MAX;
    }
    dp[0] = 0; // Custo zero para pintar 0 paredes
    
    for (int i = 0; i < n; i++) {
        int current_cost = cost[i];
        int current_time = time[i];
        
        /* 
        Percorre o array de trás para frente para evitar 
        usar o mesmo pintor múltiplas vezes
        */
        for (int j = max_time; j >= 0; j--) {
            if (dp[j] != INT_MAX) {
                int new_j = j + current_time + 1; // +1 porque o pintor gratuito pinta 1 parede
                if (new_j > max_time) new_j = max_time;
                
                // Atualiza o custo mínimo
                dp[new_j] = min(dp[new_j], dp[j] + current_cost);
            }
        }
    }
    
    /* 
    Encontra o menor custo onde o número de paredes pintadas pelo pintor pago
    mais as pintadas pelo gratuito (j) é >= n
    */
    int result = INT_MAX;
    for (int j = n; j <= max_time; j++) {
        if (dp[j] < result) {
            result = dp[j];
        }
    }
    
    free(dp);
    return result;
}