#include "huffman.h"
#define TABLESIZE 256


// Структура узла дерева Хаффмана
typedef struct Node {
    unsigned char symbol;
    unsigned int freq;
    struct Node *left;
    struct Node *right;
} Node;


// Структура для элемента списка
typedef struct ListNode {
    Node *tree_node;
    struct ListNode *next;
} ListNode;


// Структура для таблицы кодов
typedef struct CodeTable {
    unsigned char symbol;
    char *code;
    int code_length;
} CodeTable;


// Создание нового узла дерева
Node* create_node(unsigned char symbol, unsigned int freq) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (node) {
        node->symbol = symbol;
        node->freq = freq;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}


// Создание нового элемента списка
ListNode* create_list_node(Node *tree_node) {
    ListNode *list_node = (ListNode*)malloc(sizeof(ListNode));
    if (list_node) {
        list_node->tree_node = tree_node;
        list_node->next = NULL;
    }
    return list_node;
}


// Вставка в упорядоченный список по возрастанию частоты
void insert_sorted(ListNode **head, ListNode *new_node) {
    if (*head == NULL || new_node->tree_node->freq < (*head)->tree_node->freq) {
        new_node->next = *head;
        *head = new_node;
    } else {
        ListNode *current = *head;
        while (current->next != NULL && 
               current->next->tree_node->freq <= new_node->tree_node->freq) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}


// Построение дерева Хаффмана с использованием упорядоченного списка
Node* build_huffman_tree(unsigned int *freq_table) {
    ListNode *head = NULL;
    int unique_chars = 0;
    
    // Создаем список листьев
    for (int i = 0; i < 256; i++) {
        if (freq_table[i] > 0) {
            Node *leaf = create_node((unsigned char)i, freq_table[i]);
            ListNode *list_node = create_list_node(leaf);
            insert_sorted(&head, list_node);
            unique_chars++;
        }
    }
    
    // Обработка особых случаев
    if (unique_chars == 0) {
        return NULL;  // Файл пустой
    }
    
    if (unique_chars == 1) {
        // Для одного символа создаем искусственное дерево
        Node *single_node = head->tree_node;
        
        // Создаем корень с левым потомком = сам символ, правый = NULL
        Node *root = create_node(0, single_node->freq);
        root->left = single_node;
        root->right = NULL;
        
        free(head);  // Освобождаем только ListNode, Node остается в дереве
        return root;
    }

    while (head != NULL && head->next != NULL) {
        // Извлекаем два узла с наименьшими частотами
        ListNode *first = head;
        ListNode *second = head->next;
        head = second->next;
        
        // Создаем объединенный узел
        Node *combined = create_node(0, first->tree_node->freq + second->tree_node->freq);
        combined->left = first->tree_node;
        combined->right = second->tree_node;
        
        // Вставляем объединенный узел обратно в список
        ListNode *new_list_node = create_list_node(combined);
        insert_sorted(&head, new_list_node);
        
        free(first);
        free(second);
    }
    
    // В списке остался один элемент - корень дерева
    Node *root = head->tree_node;
    free(head);  // Освобождаем последний ListNode
    return root;
}


// Освобождение памяти дерева
void free_tree(Node *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}


// Рекурсивная генерация кодов Хаффмана
void generate_codes(Node *root, char *code, int depth, CodeTable *code_table) {
    if (root == NULL) return;
    
    // Если это лист
    if (root->left == NULL && root->right == NULL) {
        code_table[root->symbol].symbol = root->symbol;
        code_table[root->symbol].code = (char*)malloc(depth + 1);
        if (code_table[root->symbol].code) {
            strncpy(code_table[root->symbol].code, code, depth);
            code_table[root->symbol].code[depth] = '\0';
            code_table[root->symbol].code_length = depth;
        }
        return;
    }
    
    // Левое поддерево - добавляем '0'
    if (depth < 256) { // Защита от переполнения
        code[depth] = '0';
        generate_codes(root->left, code, depth + 1, code_table);
    }
    
    // Правое поддерево - добавляем '1'
    if (depth < 256) {
        code[depth] = '1';
        generate_codes(root->right, code, depth + 1, code_table);
    }
}


// Освобождение таблицы кодов
void free_code_table(CodeTable *code_table, int size) {
    for (int i = 0; i < size; i++) {
        if (code_table[i].code != NULL) {
            free(code_table[i].code);
        }
    }
}


// Построение таблицы частот
void build_frequency_table(FILE *file, unsigned int *freq_table) {
    memset(freq_table, 0, 256 * sizeof(unsigned int));
    
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        freq_table[(unsigned char)ch]++;
    }
    
    rewind(file);
}


// Запись бита в буфер
void write_bit(FILE *file, unsigned char bit, unsigned char *bit_buffer, int *bit_count) {
    *bit_buffer = (*bit_buffer << 1) | (bit & 1);
    (*bit_count)++;
    
    if (*bit_count == 8) {
        fputc(*bit_buffer, file);
        *bit_buffer = 0;
        *bit_count = 0;
    }
}


// Запись оставшихся битов
void flush_bits(FILE *file, unsigned char *bit_buffer, int *bit_count) {
    if (*bit_count > 0) {
        *bit_buffer = *bit_buffer << (8 - *bit_count);
        fputc(*bit_buffer, file);
        *bit_buffer = 0;
        *bit_count = 0;
    }
}


// Чтение бита из файла
unsigned char read_bit(FILE *file, unsigned char *bit_buffer, int *bit_count, int *eof) {
    if (*bit_count == 0) {
        int byte = fgetc(file);
        if (byte == EOF) {
            *eof = 1;
            return 0;
        }
        *bit_buffer = (unsigned char)byte;
        *bit_count = 8;
    }
    
    unsigned char bit = (*bit_buffer >> 7) & 1;
    *bit_buffer = *bit_buffer << 1;
    (*bit_count)--;
    
    return bit;
}


// Кодирование файла
void encode_file(const char *input_file, const char *output_file) {
    FILE *in = fopen(input_file, "rb");
    if (!in) {
        printf("ERROR: Failed to open input file %s\n", input_file);
        return;
    }
    
    FILE *out = fopen(output_file, "wb");
    if (!out) {
        printf("ERROR: Failed to create output file %s\n", output_file);
        fclose(in);
        return;
    }
    
    printf("Encoding file %s...\n", input_file);
    
    // Замеряем время, потраченное на кодировку
    clock_t start_time = clock();

    // Построение таблицы частот
    unsigned int freq_table[TABLESIZE];
    build_frequency_table(in, freq_table);
    
    // Построение дерева Хаффмана
    Node *root = build_huffman_tree(freq_table);
    if (root == NULL) {
        printf("ERROR: Input file is empty\n");
        fclose(in);
        fclose(out);
        return;
    }
    
    // Генерация кодов
    CodeTable code_table[TABLESIZE];
    memset(code_table, 0, sizeof(code_table));
    
    char code_buffer[TABLESIZE];
    generate_codes(root, code_buffer, 0, code_table);
    
    // Запись таблицы частот в выходной файл
    fwrite(freq_table, sizeof(unsigned int), 256, out);
    
    // Кодирование данных
    unsigned char bit_buffer = 0;
    int bit_count = 0;
    
    int ch;
    while ((ch = fgetc(in)) != EOF) {
        char *code = code_table[(unsigned char)ch].code;
        if (code) {
            for (int i = 0; code[i] != '\0'; i++) {
                write_bit(out, code[i] == '1' ? 1 : 0, &bit_buffer, &bit_count);
            }
        }
    }
    
    flush_bits(out, &bit_buffer, &bit_count);
    
    // Статистика
    fseek(in, 0, SEEK_END);
    long original_size = ftell(in);
    fseek(out, 0, SEEK_END);
    long compressed_size = ftell(out);

    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    printf("Original size: %ld bytes\n", original_size);
    printf("Compressed size: %ld bytes\n", compressed_size);
    printf("Compression ratio: %.2f\n", (double)compressed_size / original_size);
    printf("Time encoding: %.4f seconds\n", time_spent);
    
    // Освобождение ресурсов
    free_tree(root);
    free_code_table(code_table, 256);
    fclose(in);
    fclose(out);
    
    printf("Coding is complete. The result is in %s\n", output_file);
}


// Декодирование файла
void decode_file(const char *input_file, const char *output_file) {
    FILE *in = fopen(input_file, "rb");
    if (!in) {
        printf("ERROR: Failed to open input file %s\n", input_file);
        return;
    }
    
    FILE *out = fopen(output_file, "wb");
    if (!out) {
        printf("ERROR: Failed to create output file %s\n", output_file);
        fclose(in);
        return;
    }
    
    printf("Decoding file %s...\n", input_file);

    // Замеряем время, потраченно на декодировку
    clock_t start_time = clock();
    
    // Чтение таблицы частот
    unsigned int freq_table[TABLESIZE];
    if (fread(freq_table, sizeof(unsigned int), 256, in) != 256) {
        printf("ERROR: Invalid compressed file format\n");
        fclose(in);
        fclose(out);
        return;
    }
    
    // Построение дерева Хаффмана
    Node *root = build_huffman_tree(freq_table);
    if (root == NULL) {
        printf("ERROR: Failed to build Huffman tree\n");
        fclose(in);
        fclose(out);
        return;
    }
    
    // Декодирование данных
    unsigned char bit_buffer = 0;
    int bit_count = 0;
    int eof = 0; // Флаг, созданный для цикла
    
    Node *current = root;

    // Подсчитаем общее количество символов для проверки
    int total_symbols = 0;
    for (int i = 0; i < 256; i++) {
        total_symbols += freq_table[i];
    }
    
    int decoded_symbols = 0;
    
    while (!eof) {
        unsigned char bit = read_bit(in, &bit_buffer, &bit_count, &eof);
        
        if (!eof) {
            if (bit == 0) {
                current = current->left;
            } else {
                current = current->right;
            }
            
            // Если достигли листа
            if (current->left == NULL && current->right == NULL) {
                fputc(current->symbol, out);
                current = root;
                decoded_symbols++;

                // Дополнительная проверка: если декодировали все символы, выходим
                if (decoded_symbols >= total_symbols) break;
            }
        }
    }

    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    printf("Time decoding: %.4f seconds\n", time_spent);
    printf("Decoding completed. Result in %s\n", output_file);
    
    free_tree(root);
    fclose(in);
    fclose(out);
}