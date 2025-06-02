#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char ch;
    int freq;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct {
    Node** data;
    int size;
    int capacity;
} MinHeap;

static Node* createNode(char ch, int freq) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->ch = ch;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->data = (Node**)malloc(capacity * sizeof(Node*));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    return minHeap;
}

static void swapNode(Node** a, Node** b) {
    Node* t = *a;
    *a = *b;
    *b = t;
}

static void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2*idx + 1;
    int right = 2*idx + 2;

    if (left < minHeap->size && minHeap->data[left]->freq < minHeap->data[smallest]->freq)
        smallest = left;
    if (right < minHeap->size && minHeap->data[right]->freq < minHeap->data[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapNode(&minHeap->data[smallest], &minHeap->data[idx]);
        minHeapify(minHeap, smallest);
    }
}

static Node* extractMin(MinHeap* minHeap) {
    Node* temp = minHeap->data[0];
    minHeap->data[0] = minHeap->data[minHeap->size - 1];
    minHeap->size--;
    minHeapify(minHeap, 0);
    return temp;
}

static void insertMinHeap(MinHeap* minHeap, Node* node) {
    minHeap->size++;
    int i = minHeap->size - 1;
    while (i && node->freq < minHeap->data[(i - 1) / 2]->freq) {
        minHeap->data[i] = minHeap->data[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->data[i] = node;
}

static int isSizeOne(MinHeap* minHeap) {
    return (minHeap->size == 1);
}

static MinHeap* buildMinHeap(int* freq) {
    MinHeap* minHeap = createMinHeap(256);
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            minHeap->data[minHeap->size++] = createNode((char)i, freq[i]);
        }
    }
    for (int i = (minHeap->size - 1) / 2; i >= 0; i--) {
        minHeapify(minHeap, i);
    }
    return minHeap;
}

static Node* buildHuffmanTree(int* freq) {
    Node *left, *right, *top;
    MinHeap* minHeap = buildMinHeap(freq);

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = createNode('\0', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    Node* root = extractMin(minHeap);
    free(minHeap->data);
    free(minHeap);
    return root;
}

typedef struct {
    char ch;
    int freq;
} CharFreq;

static void collectLeafNodes(Node* root, CharFreq* arr, int* idx) {
    if (!root) return;
    if (!root->left && !root->right) {
        arr[*idx].ch = root->ch;
        arr[*idx].freq = root->freq;
        (*idx)++;
        return;
    }
    collectLeafNodes(root->left, arr, idx);
    collectLeafNodes(root->right, arr, idx);
}

static int cmpDesc(const void* a, const void* b) {
    CharFreq* cf1 = (CharFreq*)a;
    CharFreq* cf2 = (CharFreq*)b;
    return cf2->freq - cf1->freq;
}

static void freeTree(Node* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

char* frequencySort(char* s) {
    int freq[256] = {0};
    int len = strlen(s);

    for (int i = 0; i < len; i++) {
        freq[(unsigned char)s[i]]++;
    }

    Node* root = buildHuffmanTree(freq);

    CharFreq arr[256];
    int idx = 0;
    collectLeafNodes(root, arr, &idx);

    qsort(arr, idx, sizeof(CharFreq), cmpDesc);

    char* result = (char*)malloc(len + 1);
    int pos = 0;
    for (int i = 0; i < idx; i++) {
        for (int j = 0; j < arr[i].freq; j++) {
            result[pos++] = arr[i].ch;
        }
    }
    result[pos] = '\0';

    freeTree(root);
    return result;
}
