#ifndef HUFFMAN_H   // если макрос HUFFMAN_H не определён, то код после этой строчки будет скомпилирован
#define HUFFMAN_H   // определяем макрос HUFFMAN_H. Когда в следующий раз будет проверка #ifndef HUFFMAN_H, то
                    // благодаря этому определению проверка не будет пройдена и код второй раз не скомпилируется

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>


// Структура узла дерева Хаффмана
typedef struct Node Node;


// Структура для элемента списка
typedef struct ListNode ListNode;


// Структура для таблицы кодов
typedef struct CodeTable CodeTable;


// Функции для работы с деревом Хаффмана
Node* create_node(unsigned char symbol, unsigned int freq);
ListNode* create_list_node(Node *tree_node);
void insert_sorted(ListNode **head, ListNode *new_node);
Node* build_huffman_tree(unsigned int *freq_table);
void free_tree(Node *root);


// Функции для работы с кодами
void generate_codes(Node *root, char *code, int depth, CodeTable *code_table);
void free_code_table(CodeTable *code_table, int size);


// Функции кодирования/декодирования
void encode_file(const char *input_file, const char *output_file);
void decode_file(const char *input_file, const char *output_file);


// Вспомогательные функции
void build_frequency_table(FILE *file, unsigned int *freq_table);
void write_bit(FILE *file, unsigned char bit, unsigned char *bit_buffer, int *bit_count);
void flush_bits(FILE *file, unsigned char *bit_buffer, int *bit_count);
unsigned char read_bit(FILE *file, unsigned char *bit_buffer, int *bit_count, int *eof);

#endif