#include "huffman.h"
#include <string.h>

void test_compression(const char *input_file) {
    printf("Testing compression for a file: %s\n", input_file);
    
    // Кодирование
    encode_file(input_file, "test_compressed.bin");
    
    // Декодирование
    decode_file("test_compressed.bin", "test_decompressed.txt");
    
    // Проверка совпадения
    FILE *original = fopen(input_file, "rb");
    FILE *decompressed = fopen("test_decompressed.txt", "rb");
    
    if (!original || !decompressed) {
        printf("ERROR: Failed to open files\n");
        if (original) fclose(original);
        if (decompressed) fclose(decompressed);
        return;
    }
    
    int match = 1;
    int ch1, ch2;
    while ((ch1 = fgetc(original)) != EOF && (ch2 = fgetc(decompressed)) != EOF) {
        if (ch1 != ch2) {
            match = 0;
            break;
        }
    }
    
    // Проверяем, что оба файла закончились одновременно
    if (match && (fgetc(original) != EOF || fgetc(decompressed) != EOF)) {
        match = 0;
    }
    
    fclose(original);
    fclose(decompressed);
    
    if (match) {
        printf("The files match! The code works correctly!\n");
    } else {
        printf("Something is not working correctly!\n");
    }
    
    // Удаляем временные файлы
    remove("test_compressed.bin");
    remove("test_decompressed.txt");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("ERROR: Wrong arguments!\n");
        return 1;
    }
    
    if (strcmp(argv[1], "encode") == 0) {
        if (argc != 4) {
            printf("ERROR: Please specify input and output files for encoding.\n");
            return 1;
        }
        encode_file(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "decode") == 0) {
        if (argc != 4) {
            printf("ERROR: Please specify input and output files for decoding.\n");
            return 1;
        }
        decode_file(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "test") == 0) {
        if (argc != 3) {
            printf("ERROR: Please provide an input file for testing.\n");
            return 1;
        }
        test_compression(argv[2]);
    }
    else {
        printf("ERROR: Unknown command %s\n", argv[1]);
        return 1;
    }
    
    return 0;
}

// gcc launcher.c huffman.c -o launcher.exe
// ./launcher.exe encode text.txt text.bin
// ./launcher.exe decode text.bin output.txt
// ./launcher.exe test text.txt