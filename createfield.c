#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "createfield.h"

void init_dino(struct Dino *dino, struct Field *field, int x, int y) {
    dino->x = x;
    dino->y = y;

    for (int i = 0; i < field->height; ++i) {

        // Проходимся по значениям ширины поля
        for (int j = 0; j < field->width; ++j) {

            // Проверяем совпадение координат точек и вставляем динозавра
            if (i == y && j == x) {
                dino->steppedCell = field->tiles[i][j];
                field->tiles[i][j] = '#';
            }
        }
    }
}

void del_dino(struct Dino *dino, struct Field *field, struct Field *colors, int x, int y) {
    dino->x = x;
    dino->y = y;

    for (int i = 0; i < field->height; ++i) {

        // Проходимся по значениям ширины поля
        for (int j = 0; j < field->width; ++j) {

            // Проверяем совпадение координат точек и удаляем динозавра
            if (i == y && j == x) {
                field->tiles[i][j] = colors->tiles[i][j];
            }
        }
    }
}

int init_field(struct Field *field, int w, int h) {
    field->width = w;
    field->height = h;
    char **rows = calloc(h, sizeof(char*));

    if (rows == NULL) {
        perror("ERROR");
        return 0;
    }

    for (int i=0; i<h; ++i) {
        rows[i] = calloc(w, sizeof(char));
        if (rows[i] == NULL) {
            printf("ERROR: Cannot allocate memory for row\n");
        
            for (int j=0; j<w; ++j) {
                if (rows[j] == NULL) return 0;
            }

            free(rows);
            return 0;
        }
    }

    for (int i=0; i<h; ++i) {
        for (int j=0; j<w; ++j) {
            rows[i][j] = '_';
        }
    }

    field->tiles = rows;

    return 1;
}

int parse_line_init_field(char * const line, struct Field * field) {
    char *value = calloc(128, sizeof(char));
    char *p = line;
    int offset = 0;
    
    // Читаем команду
    if (sscanf(p, "%s %n", value, &offset) != 1) {
        free(value);
        return 0;  // Ошибка чтения команды
    }
    
    if (strcmp(value, "SIZE") == 0) {
        p += offset;
        int x, y;
        
        // Проверяем что прочитали 2 числа
        if (sscanf(p, " %d %d %n", &x, &y, &offset) != 2) {
            printf("ERROR: Expected two numbers after command SIZE\n");
            free(value);
            return 0;
        }

        // Проверяем нет ли никаких других мусорных данных после одной команды и двух чисел (то есть в конце строки)
        if (count_words(line, 3) == 0) {
            free(value);
            return -10;
        }
        
        // Проверяем валидность размеров
        if (x < 10 || y < 10 || x > 100 || y > 100) {
            printf("ERROR: Invalid field size. Minimum field length is 10, maximum is 100.\n");
            free(value);
            return 0;
        }
        
        // Инициализируем поле и возвращаем конфиг
        if (init_field(field, x, y) == 0) {
            printf("ERROR: Failed to initialize field\n");
            free(value);
            return 0;
        }
        
        return 1;  // Успешное завершение функции
        
    } else {
        printf("ERROR: Unknown command '%s'\n", value);
        free(value);
        return 0;
    }
}

int parse_line_init_dino(char * const line, struct Dino* dino, struct Field* field) {
    char *value = calloc(128, sizeof(char));
    char *p = line;
    int offset = 0;
    sscanf(p, "%s %n", value, &offset);

    // Проверяем является ли первое слово "START"
    if (strcmp(value, "START") == 0) {
        p += offset;
        int x, y;
        sscanf(p, "%d %d", &x, &y);

        // Проверяем, что после слова "START" написано 2 числа - координаты динозавра
        if (sscanf(p, "%d %d", &x, &y) != 2) {
            printf("ERROR: Expected two numbers after command START\n");
            free(value);
            return 0;
        }

        // Проверяем нет ли никаких других мусорных данных после одной команды и двух чисел (то есть в конце строки)
        if (count_words(line, 3) == 0) {
            free(value);
            return -10;
        }

        // Проверяем, что значения координат входят в границы поля
        if ((x < 0) || (x >= field->width) || (y < 0) || (y >= field->height)) {
            printf("ERROR: The dinosaur's position coordinates must be within the field's boundaries.\n");
            free(value);
            return 0;
        }

        init_dino(dino, field, x, y);
        
        free(value);
        return 1;
    } else {
        printf("ERROR: Unknown command '%s'\n", value);
        free(value);
        return 0;
    }
}

void free_field(struct Field *field) {
    if (field->tiles != NULL) {
        for (int i = 0; i < field->height; ++i) {

            // Освобождаем каждую строку
            free(field->tiles[i]);
        }

        // Освобождаем массив указателей
        free(field->tiles);
        field->tiles = NULL;
    }
}

int count_words(char const* point, int quantity) {
    int counter = 0;
    char newLine[128];
    strcpy(newLine, point);
    char *token = strtok(newLine, " \t\n");
    
    while (token != NULL) {
        ++counter;
        token = strtok(NULL, " \t\n");
    }

    // Если counter > quantity, тогда это значит что в конце строки (после прочтения всех нужных значений) написаны ещё какие-то параметры (мусор)
    if (counter > quantity) return 0;
    else return 1;
}