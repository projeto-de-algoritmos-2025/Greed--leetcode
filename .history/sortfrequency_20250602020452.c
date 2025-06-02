#include <stdlib.h>
#include <string.h>

// Estrutura para representar um nó (pode ser usado na árvore de Huffman ou no heap)
typedef struct Node {
    char ch;                // Caractere (para nós folha)
    int freq;               // Frequência do caractere
    struct Node* left;      // Ponteiro para o filho esquerdo
    struct Node* right;     // Ponteiro para o filho direito
} Node;

// Estrutura para representar um Min-Heap (fila de prioridade mínima)
typedef struct {
    Node** data;            // Array de ponteiros para nós
    int size;               // Tamanho atual do heap
    int capacity;           // Capacidade máxima do heap
} MinHeap;

// Cria um novo nó com caractere e frequência especificados
static Node* createNode(char ch, int freq) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->ch = ch;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Cria um min-heap com capacidade especificada
static MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->data = (Node**)malloc(capacity * sizeof(Node*));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    return minHeap;
}

// Função auxiliar para trocar dois nós
static void swapNode(Node** a, Node** b) {
    Node* t = *a;
    *a = *b;
    *b = t;
}

// Função para manter a propriedade de min-heap a partir de um índice
static void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;      // Inicializa o menor como raiz
    int left = 2*idx + 1;    // Índice do filho esquerdo
    int right = 2*idx + 2;   // Índice do filho direito

    // Se o filho esquerdo é menor que a raiz
    if (left < minHeap->size && minHeap->data[left]->freq < minHeap->data[smallest]->freq)
        smallest = left;
    
    // Se o filho direito é menor que o menor até agora
    if (right < minHeap->size && minHeap->data[right]->freq < minHeap->data[smallest]->freq)
        smallest = right;

    // Se o menor não é a raiz
    if (smallest != idx) {
        swapNode(&minHeap->data[smallest], &minHeap->data[idx]);
        // Recursivamente heapify a subárvore afetada
        minHeapify(minHeap, smallest);
    }
}

// Extrai o nó com menor frequência do heap
static Node* extractMin(MinHeap* minHeap) {
    Node* temp = minHeap->data[0];                  // Pega o menor elemento (raiz)
    minHeap->data[0] = minHeap->data[minHeap->size - 1];  // Move o último elemento para a raiz
    minHeap->size--;                                // Reduz o tamanho do heap
    minHeapify(minHeap, 0);                         // Heapify a raiz
    return temp;
}

// Insere um novo nó no min-heap
static void insertMinHeap(MinHeap* minHeap, Node* node) {
    minHeap->size++;
    int i = minHeap->size - 1;
    
    // Percorre o heap para encontrar a posição correta do novo nó
    while (i && node->freq < minHeap->data[(i - 1) / 2]->freq) {
        minHeap->data[i] = minHeap->data[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->data[i] = node;
}

// Verifica se o heap tem tamanho 1
static int isSizeOne(MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// Constrói um min-heap a partir de um array de frequências
static MinHeap* buildMinHeap(int* freq) {
    MinHeap* minHeap = createMinHeap(256);  // Cria heap para todos os caracteres ASCII
    
    // Adiciona nós para caracteres com frequência > 0
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            minHeap->data[minHeap->size++] = createNode((char)i, freq[i]);
        }
    }
    
    // Constrói o heap reorganizando os elementos
    for (int i = (minHeap->size - 1) / 2; i >= 0; i--) {
        minHeapify(minHeap, i);
    }
    return minHeap;
}

// Constrói a árvore de Huffman a partir das frequências
static Node* buildHuffmanTree(int* freq) {
    Node *left, *right, *top;
    MinHeap* minHeap = buildMinHeap(freq);

    // Enquanto houver mais de um nó no heap
    while (!isSizeOne(minHeap)) {
        // Extrai os dois nós de menor frequência
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Cria um novo nó interno com soma das frequências
        top = createNode('\0', left->freq + right->freq);
        top->left = left;
        top->right = right;

        // Insere o novo nó no heap
        insertMinHeap(minHeap, top);
    }

    // O nó restante é a raiz da árvore de Huffman
    Node* root = extractMin(minHeap);
    free(minHeap->data);
    free(minHeap);
    return root;
}

// Estrutura auxiliar para armazenar caracteres e suas frequências
typedef struct {
    char ch;
    int freq;
} CharFreq;

// Coleta todos os nós folha da árvore (caracteres e suas frequências)
static void collectLeafNodes(Node* root, CharFreq* arr, int* idx) {
    if (!root) return;
    // Se é um nó folha (tem um caractere)
    if (!root->left && !root->right) {
        arr[*idx].ch = root->ch;
        arr[*idx].freq = root->freq;
        (*idx)++;
        return;
    }
    // Percorre a árvore recursivamente
    collectLeafNodes(root->left, arr, idx);
    collectLeafNodes(root->right, arr, idx);
}

// Função de comparação para qsort (ordena em ordem decrescente de frequência)
static int cmpDesc(const void* a, const void* b) {
    CharFreq* cf1 = (CharFreq*)a;
    CharFreq* cf2 = (CharFreq*)b;
    return cf2->freq - cf1->freq;
}

// Libera a memória alocada para a árvore
static void freeTree(Node* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

// Função principal que ordena os caracteres por frequência (decrescente)
char* frequencySort(char* s) {
    int freq[256] = {0};  // Inicializa array de frequências
    int len = strlen(s);

    // Calcula a frequência de cada caractere na string
    for (int i = 0; i < len; i++) {
        freq[(unsigned char)s[i]]++;
    }

    // Constrói a árvore de Huffman (para agrupar por frequência)
    Node* root = buildHuffmanTree(freq);

    // Array para armazenar caracteres e frequências
    CharFreq arr[256];
    int idx = 0;
    collectLeafNodes(root, arr, &idx);

    // Ordena os caracteres por frequência (decrescente)
    qsort(arr, idx, sizeof(CharFreq), cmpDesc);

    // Aloca memória para a string resultante
    char* result = (char*)malloc(len + 1);
    int pos = 0;
    
    // Constrói a string resultante repetindo cada caractere conforme sua frequência
    for (int i = 0; i < idx; i++) {
        for (int j = 0; j < arr[i].freq; j++) {
            result[pos++] = arr[i].ch;
        }
    }
    result[pos] = '\0';  // Termina a string

    freeTree(root);  // Libera a árvore
    return result;
}