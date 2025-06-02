#include <stdio.h>
#include <stdlib.h>

// Função de comparação para qsort - ordena cursos por prazo final (crescente)
int compararCursos(const void* a, const void* b) {
    int* cursoA = *(int**)a;
    int* cursoB = *(int**)b;
    return cursoA[1] - cursoB[1]; // Compara os prazos finais (lastDay)
}

int scheduleCourse(int** courses, int coursesSize, int* coursesColSize) {
    /* 
    Função principal que calcula o número máximo de cursos que podem ser concluídos
    
    Parâmetros:
    - courses: array de cursos (cada curso é [duração, prazo final])
    - coursesSize: número total de cursos
    - coursesColSize: array com tamanhos das colunas (não utilizado)
    
    Retorno: número máximo de cursos completáveis
    */
    
    // 1. Ordena os cursos por prazo final (do mais urgente para o menos urgente)
    qsort(courses, coursesSize, sizeof(int*), compararCursos);
    
    // 2. Cria um array para simular uma max-heap (armazenará durações)
    int* heap = (int*)malloc(coursesSize * sizeof(int));
    int heapSize = 0;
    int tempoTotal = 0; // Acumula o tempo gasto nos cursos
    
    // 3. Percorre todos os cursos disponíveis
    for (int i = 0; i < coursesSize; i++) {
        int duracao = courses[i][0];
        int prazoFinal = courses[i][1];
        
        // Verifica se o curso pode ser adicionado sem ultrapassar o prazo
        if (tempoTotal + duracao <= prazoFinal) {
            // Adiciona no final do heap
            heap[heapSize++] = duracao;
            tempoTotal += duracao;
            
            // Ajusta o heap para cima (mantém propriedade de max-heap)
            int indice = heapSize - 1;
            while (indice > 0 && heap[(indice - 1) / 2] < heap[indice]) {
                // Troca com o pai se necessário
                int temp = heap[(indice - 1) / 2];
                heap[(indice - 1) / 2] = heap[indice];
                heap[indice] = temp;
                indice = (indice - 1) / 2;
            }
        }
        // Se não couber, verifica se podemos substituir um curso mais longo
        else if (heapSize > 0 && heap[0] > duracao) {
            // Remove o curso mais longo (raiz do heap)
            int cursoRemovido = heap[0];
            heap[0] = heap[--heapSize];
            tempoTotal -= cursoRemovido;
            
            // Ajusta o heap para baixo
            int indice = 0;
            while (1) {
                int filhoEsq = 2 * indice + 1;
                int filhoDir = 2 * indice + 2;
                int maior = indice;
                
                if (filhoEsq < heapSize && heap[filhoEsq] > heap[maior])
                    maior = filhoEsq;
                if (filhoDir < heapSize && heap[filhoDir] > heap[maior])
                    maior = filhoDir;
                
                if (maior == indice) break;
                
                // Troca com o maior filho
                int temp = heap[indice];
                heap[indice] = heap[maior];
                heap[maior] = temp;
                indice = maior;
            }
            
            // Adiciona o novo curso (mais curto)
            heap[heapSize++] = duracao;
            tempoTotal += duracao;
            
            // Ajusta o heap para cima novamente
            indice = heapSize - 1;
            while (indice > 0 && heap[(indice - 1) / 2] < heap[indice]) {
                int temp = heap[(indice - 1) / 2];
                heap[(indice - 1) / 2] = heap[indice];
                heap[indice] = temp;
                indice = (indice - 1) / 2;
            }
        }
    }
    
    // 4. Libera memória e retorna o resultado
    free(heap);
    return heapSize;
}