#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "createfield.h"
#include "dinoactions.h"

int move_dino(char *const line, struct Dino *dino, struct Field *field, struct Field *colors) {
    char *value = calloc(128, sizeof(char));
    char *p = line;
    int offset = 0;
    
    // Читаем команду
    if (sscanf(p, "%s %n", value, &offset) != 1) {
        printf("ERROR: Unable to read the first word (MOVE function)");
        return 0;
    }

    if (strcmp(value, "MOVE") == 0) {
        char *direction = calloc(128, sizeof(char));
        p += offset;

        // xNew - передвижение направо/налево; yNew - передвижение вверх/вниз;
        int xNew = 0, yNew = 0;

        if (sscanf(p, "%s %n", direction, &offset) != 1) {
            printf("ERROR: Unable to read the second word (MOVE function)");
            return 0;
        }

        // Проверяем нет ли никаких других мусорных данных после команды MOVE и направления (то есть в конце строки)
        if (count_words(line, 2) == 0) {
            free(value);
            return -10;
        }


        if (strcmp(direction, "UP") == 0) {
            p += offset;
            yNew = -1;
        } else if (strcmp(direction, "DOWN") == 0) {
            p += offset;
            yNew = 1;
        } else if (strcmp(direction, "LEFT") == 0) {
            p += offset;
            xNew = -1;
        } else if (strcmp(direction, "RIGHT") == 0) {
            p += offset;
            xNew = 1;
        } else {
            free(value);
            free(direction);
            return -11;
        }

        xNew = (((dino->x+xNew) + field->width) % field->width);
        yNew = (((dino->y+yNew) + field->height) % field->height);
        if (field->tiles[yNew][xNew] == '^') {
            free(value);
            free(direction);
            return -1; // Ошибка при выполнении хода. Смотри visualizefield.c что потом будет выведено
        }

        if (field->tiles[yNew][xNew] == '&') {
            free(value);
            free(direction);
            return -4; // Ошибка при выполнении хода. Смотри visualizefield.c что потом будет выведено
        }

        if (field->tiles[yNew][xNew] == '@') {
            free(value);
            free(direction);
            return -7; // Ошибка при выполнении хода. Смотри visualizefield.c что потом будет выведено
        }

        del_dino(dino, field, colors, dino->x, dino->y);

        if (field->tiles[yNew][xNew] == '%') {
            free(value);
            free(direction);
            return -3; // Ошибка падения в яму. Смотри visualizefield.c что потом будет выведено
        }

        init_dino(dino, field, xNew, yNew);

        free(value);
        free(direction);
        return 1;

    } else {
        printf("ERROR: Unknown command while reading function MOVE\n");
        free(value);
        return 0;
    }
}



int paint_cell(char *const line, struct Dino *dino, struct Field *field, struct Field *colors) {
    char *value = calloc(128, sizeof(char));
    char *p = line;
    int offset = 0;
    
    // Читаем команду
    if (sscanf(p, "%s %n", value, &offset) != 1) {
        printf("ERROR: Unable to read the first word (PAINT function)");
        return 0;
    }

    if (strcmp(value, "PAINT") == 0) {
        char *color = calloc(128, sizeof(char));
        p += offset;
        sscanf(p, "%s", color);

        if (strlen(color) != 1) {
            free(color);
            free(value);
            return -2;
        }

        // Проверяем нет ли никаких других мусорных данных после команды PAINT и буквы (то есть в конце строки)
        if (count_words(line, 2) == 0) {
            free(value);
            return -10;
        }

        else if (*color >= 'a' && *color <= 'z') {
            colors->tiles[dino->y][dino->x] = *color;
            free(color);
            free(value);
            return 1;
        }

        else {
            free(color);
            free(value);
            return -1;
        }

    } else {
        printf("ERROR: Unknown command while reading function PAINT\n");
        free(value);
        return 0;
    }
}



int dig_cell(char *const line, struct Dino *dino, struct Field *field) {
    char *value = calloc(128, sizeof(char));
    char *p = line;
    int offset = 0;
    
    // Читаем команду
    if (sscanf(p, "%s %n", value, &offset) != 1) {
        printf("ERROR: Unable to read the first word (DIG function)");
        return 0;
    }

    if (strcmp(value, "DIG") == 0) {
        char *direction = calloc(128, sizeof(char));
        p += offset;

        // (xNew, yNew) - координаты ямы
        int xNew = 0, yNew = 0;

        if (sscanf(p, "%s %n", direction, &offset) != 1) {
            printf("ERROR: Unable to read the second word (DIG function)");
            return 0;
        }

        // Проверяем нет ли никаких других мусорных данных после команды DIG и направления (то есть в конце строки)
        if (count_words(line, 2) == 0) {
            free(value);
            return -10;
        }


        if (strcmp(direction, "UP") == 0) {
            p += offset;
            yNew = -1;
        } else if (strcmp(direction, "DOWN") == 0) {
            p += offset;
            yNew = 1;
        } else if (strcmp(direction, "LEFT") == 0) {
            p += offset;
            xNew = -1;
        } else if (strcmp(direction, "RIGHT") == 0) {
            p += offset;
            xNew = 1;
        } else {
            free(value);
            free(direction);
            return -11;
        }

        xNew = (((dino->x+xNew) + field->width) % field->width);
        yNew = (((dino->y+yNew) + field->height) % field->height);
        if (field->tiles[yNew][xNew] == '&' || field->tiles[yNew][xNew] == '@') {
            free(value);
            free(direction);
            return -21;
        }
        field->tiles[yNew][xNew] = '%';

        free(value);
        free(direction);
        return 1;

    } else {
        printf("ERROR: Unknown command while reading function DIG\n");
        free(value);
        return 0;
    }
}



int mound_cell(char *const line, struct Dino *dino, struct Field *field, struct Field *colors) {
    char *value = calloc(128, sizeof(char));
    char *p = line;
    int offset = 0;
    
    // Читаем команду
    if (sscanf(p, "%s %n", value, &offset) != 1) {
        printf("ERROR: Unable to read the first word (MOUND function)");
        return 0;
    }

    if (strcmp(value, "MOUND") == 0) {
        char *direction = calloc(128, sizeof(char));
        p += offset;

        // (xNew, yNew) - координаты горы
        int xNew = 0, yNew = 0;

        if (sscanf(p, "%s %n", direction, &offset) != 1) {
            printf("ERROR: Unable to read the second word (MOUND function)");
            return 0;
        }

        // Проверяем нет ли никаких других мусорных данных после команды MOUND и направления (то есть в конце строки)
        if (count_words(line, 2) == 0) {
            free(value);
            return -10;
        }


        if (strcmp(direction, "UP") == 0) {
            p += offset;
            yNew = -1;
        } else if (strcmp(direction, "DOWN") == 0) {
            p += offset;
            yNew = 1;
        } else if (strcmp(direction, "LEFT") == 0) {
            p += offset;
            xNew = -1;
        } else if (strcmp(direction, "RIGHT") == 0) {
            p += offset;
            xNew = 1;
        } else {
            free(value);
            free(direction);
            return -11;
        }

        xNew = (((dino->x+xNew) + field->width) % field->width);
        yNew = (((dino->y+yNew) + field->height) % field->height);
        if (field->tiles[yNew][xNew] == '%') {
            field->tiles[yNew][xNew] = '_';
            colors->tiles[yNew][xNew] = '_';
        }
        else if (field->tiles[yNew][xNew] == '&' || field->tiles[yNew][xNew] == '@') {
            free(value);
            free(direction);
            return -20;
        } else {
            field->tiles[yNew][xNew] = '^';
        }

        free(value);
        free(direction);
        return 1;

    } else {
        printf("ERROR: Unknown command while reading function MOUND\n");
        free(value);
        return 0;
    }
}



int jump_dino(char *const line, struct Dino *dino, struct Field *field, struct Field *colors) {
    char *value = calloc(128, sizeof(char));
    char *p = line;
    int offset = 0;
    
    // Читаем команду
    if (sscanf(p, "%s %n", value, &offset) != 1) {
        printf("ERROR: Unable to read the first word (JUMP function)");
        return 0;
    }

    if (strcmp(value, "JUMP") == 0) {
        char *direction = calloc(128, sizeof(char));
        p += offset;

        // dx - передвижение направо/налево; dy - передвижение вверх/вниз; num - количество клеток насколько нужно прыгнуть
        int dx = 0, dy = 0, num;

        if (sscanf(p, "%s %n", direction, &offset) != 1) {
            printf("ERROR: Unable to read the second word (JUMP function)");
            return 0;
        }

        if (strcmp(direction, "UP") == 0) {
            p += offset;
            dy = -1;
        } else if (strcmp(direction, "DOWN") == 0) {
            p += offset;
            dy = 1;
        } else if (strcmp(direction, "LEFT") == 0) {
            p += offset;
            dx = -1;
        } else if (strcmp(direction, "RIGHT") == 0) {
            p += offset;
            dx = 1;
        } else {
            free(value);
            free(direction);
            return -11;
        }
        
        // Проверяем что прочитали 1 число
        if (sscanf(p, "%d", &num) != 1) {
            printf("ERROR: Expected one integer number after JUMP\n");
            free(value);
            free(direction);
            return 0;
        }
        
        // Проверяем валидность размера прыжка
        if (num < 0) {
            printf("ERROR: The number of cells to jump must be a non-negative number.\n");
            free(value);
            free(direction);
            return 0;
        }

        // Проверяем нет ли никаких других мусорных данных после команды JUMP, направления и количества плиток (то есть в конце строки)
        if (count_words(line, 3) == 0) {
            free(value);
            free(direction);
            return -10;
        }

        int current_x = dino->x;
        int current_y = dino->y;

        for (int i=0; i<num; ++i) {
            int next_x = (current_x + dx + field->width) % field->width;
            int next_y = (current_y + dy + field->height) % field->height;

            if (field->tiles[next_y][next_x] == '^') {
                if (dino->steppedCell == '%') {
                    field->tiles[current_y][current_x] = '%';
                    free(value);
                    free(direction);
                    return -5; // Ошибка при выполнении хода. Смотри visualizefield.c что потом будет выведено
                }
                free(value);
                free(direction);
                return -1; // Ошибка при выполнении хода. Смотри visualizefield.c что потом будет выведено
            }

            if (field->tiles[next_y][next_x] == '&') {
                if (dino->steppedCell == '%') {
                    field->tiles[current_y][current_x] = '%';
                    free(value);
                    free(direction);
                    return -5; // Ошибка при выполнении хода. Смотри visualizefield.c что потом будет выведено
                }
                free(value);
                free(direction);
                return -4; // Ошибка при выполнении хода. Смотри visualizefield.c что потом будет выведено
            }

            if (field->tiles[next_y][next_x] == '@') {
                if (dino->steppedCell == '%') {
                    field->tiles[current_y][current_x] = '%';
                    free(value);
                    free(direction);
                    return -5; // Ошибка при выполнении хода. Смотри visualizefield.c что потом будет выведено
                }
                free(value);
                free(direction);
                return -7; // Ошибка при выполнении хода. Смотри visualizefield.c что потом будет выведено
            }

            if (dino->steppedCell == '%') {
                del_dino(dino, field, colors, dino->x, dino->y);
                init_dino(dino, field, next_x, next_y);
                field->tiles[current_y][current_x] = '%';
                current_x = next_x;
                current_y = next_y;
                continue;
            }

            del_dino(dino, field, colors, dino->x, dino->y);
            init_dino(dino, field, next_x, next_y);
            current_x = next_x;
            current_y = next_y;
        }

        if (dino->steppedCell == '%') {
            del_dino(dino, field, colors, dino->x, dino->y);
            field->tiles[current_y][current_x] = '%';
            free(value);
            free(direction);
            return -3; // Ошибка при выводе поля. Смотри visualizefield.c что потом будет выведено
        }

        free(value);
        free(direction);
        return 1;

    } else {
        printf("ERROR: Unknown command while reading function JUMP\n");
        free(value);
        return 0;
    }
}



int grow_tree(char *const line, struct Dino *dino, struct Field *field, struct Field *colors) {
    char *value = calloc(128, sizeof(char));
    char *p = line;
    int offset = 0;
    
    // Читаем команду
    if (sscanf(p, "%s %n", value, &offset) != 1) {
        printf("ERROR: Unable to read the first word (GROW function)");
        return 0;
    }

    if (strcmp(value, "GROW") == 0) {
        char *direction = calloc(128, sizeof(char));
        p += offset;

        // (xNew, yNew) - координаты дерева
        int xNew = 0, yNew = 0;

        if (sscanf(p, "%s %n", direction, &offset) != 1) {
            printf("ERROR: Unable to read the second word (GROW function)");
            return 0;
        }

        // Проверяем нет ли никаких других мусорных данных после команды GROW и направления (то есть в конце строки)
        if (count_words(line, 2) == 0) {
            free(value);
            return -10;
        }


        if (strcmp(direction, "UP") == 0) {
            p += offset;
            yNew = -1;
        } else if (strcmp(direction, "DOWN") == 0) {
            p += offset;
            yNew = 1;
        } else if (strcmp(direction, "LEFT") == 0) {
            p += offset;
            xNew = -1;
        } else if (strcmp(direction, "RIGHT") == 0) {
            p += offset;
            xNew = 1;
        } else {
            free(value);
            free(direction);
            return -11;
        }

        xNew = (((dino->x+xNew) + field->width) % field->width);
        yNew = (((dino->y+yNew) + field->height) % field->height);
        if (field->tiles[yNew][xNew] == '%' || field->tiles[yNew][xNew] == '^' || field->tiles[yNew][xNew] == '&' || field->tiles[yNew][xNew] == '@') {
            free(value);
            free(direction);
            return -12;
        } else {
            field->tiles[yNew][xNew] = '&';
        }

        free(value);
        free(direction);
        return 1;

    } else {
        printf("ERROR: Unknown command while reading function MOUND\n");
        free(value);
        return 0;
    }
}



int cut_tree(char *const line, struct Dino *dino, struct Field *field, struct Field *colors) {
    char *value = calloc(128, sizeof(char));
    char *p = line;
    int offset = 0;
    
    // Читаем команду
    if (sscanf(p, "%s %n", value, &offset) != 1) {
        printf("ERROR: Unable to read the first word (CUT function)");
        return 0;
    }

    if (strcmp(value, "CUT") == 0) {
        char *direction = calloc(128, sizeof(char));
        p += offset;

        // (xNew, yNew) - координаты дерева
        int xNew = 0, yNew = 0;

        if (sscanf(p, "%s %n", direction, &offset) != 1) {
            printf("ERROR: Unable to read the second word (CUT function)");
            return 0;
        }

        // Проверяем нет ли никаких других мусорных данных после команды CUT и направления (то есть в конце строки)
        if (count_words(line, 2) == 0) {
            free(value);
            return -10;
        }


        if (strcmp(direction, "UP") == 0) {
            p += offset;
            yNew = -1;
        } else if (strcmp(direction, "DOWN") == 0) {
            p += offset;
            yNew = 1;
        } else if (strcmp(direction, "LEFT") == 0) {
            p += offset;
            xNew = -1;
        } else if (strcmp(direction, "RIGHT") == 0) {
            p += offset;
            xNew = 1;
        } else {
            free(value);
            free(direction);
            return -11;
        }

        xNew = (((dino->x+xNew) + field->width) % field->width);
        yNew = (((dino->y+yNew) + field->height) % field->height);
        if (field->tiles[yNew][xNew] == '&') {
            field->tiles[yNew][xNew] = colors->tiles[yNew][xNew];
        } else {
            free(value);
            free(direction);
            return -6;
        }

        free(value);
        free(direction);
        return 1;

    } else {
        printf("ERROR: Unknown command while reading function DIG\n");
        free(value);
        return 0;
    }
}


int make_rock(char *const line, struct Dino *dino, struct Field *field, struct Field *colors) {
    char *value = calloc(128, sizeof(char));
    char *p = line;
    int offset = 0;
    
    // Читаем команду
    if (sscanf(p, "%s %n", value, &offset) != 1) {
        printf("ERROR: Unable to read the first word (MAKE function)");
        return 0;
    }

    if (strcmp(value, "MAKE") == 0) {
        char *direction = calloc(128, sizeof(char));
        p += offset;

        // (xNew, yNew) - координаты камня
        int xNew = 0, yNew = 0;

        if (sscanf(p, "%s %n", direction, &offset) != 1) {
            printf("ERROR: Unable to read the second word (MAKE function)");
            return 0;
        }

        // Проверяем нет ли никаких других мусорных данных после команды MAKE и направления (то есть в конце строки)
        if (count_words(line, 2) == 0) {
            free(value);
            return -10;
        }


        if (strcmp(direction, "UP") == 0) {
            p += offset;
            yNew = -1;
        } else if (strcmp(direction, "DOWN") == 0) {
            p += offset;
            yNew = 1;
        } else if (strcmp(direction, "LEFT") == 0) {
            p += offset;
            xNew = -1;
        } else if (strcmp(direction, "RIGHT") == 0) {
            p += offset;
            xNew = 1;
        } else {
            free(value);
            free(direction);
            return -11;
        }

        xNew = (((dino->x+xNew) + field->width) % field->width);
        yNew = (((dino->y+yNew) + field->height) % field->height);
        if (field->tiles[yNew][xNew] == '%' || field->tiles[yNew][xNew] == '^' || field->tiles[yNew][xNew] == '&' || field->tiles[yNew][xNew] == '@') {
            free(value);
            free(direction);
            return -13;
        } else {
            field->tiles[yNew][xNew] = '@';
        }

        free(value);
        free(direction);
        return 1;

    } else {
        printf("ERROR: Unknown command while reading function MOUND\n");
        free(value);
        return 0;
    }
}



int push_rock(char *const line, struct Dino *dino, struct Field *field, struct Field *colors) {
    char *value = calloc(128, sizeof(char));
    char *p = line;
    int offset = 0;
    
    // Читаем команду
    if (sscanf(p, "%s %n", value, &offset) != 1) {
        printf("ERROR: Unable to read the first word (PUSH function)");
        return 0;
    }

    if (strcmp(value, "PUSH") == 0) {
        char *direction = calloc(128, sizeof(char));
        p += offset;

        // (xNew, yNew) - координаты камня
        int xNew = 0, yNew = 0, xNew1 = 0, yNew1 = 0;

        if (sscanf(p, "%s %n", direction, &offset) != 1) {
            printf("ERROR: Unable to read the second word (PUSH function)");
            return 0;
        }

        // Проверяем нет ли никаких других мусорных данных после команды PUSH и направления (то есть в конце строки)
        if (count_words(line, 2) == 0) {
            free(value);
            return -10;
        }


        if (strcmp(direction, "UP") == 0) {
            p += offset;
            yNew = -1;
        } else if (strcmp(direction, "DOWN") == 0) {
            p += offset;
            yNew = 1;
        } else if (strcmp(direction, "LEFT") == 0) {
            p += offset;
            xNew = -1;
        } else if (strcmp(direction, "RIGHT") == 0) {
            p += offset;
            xNew = 1;
        } else {
            free(value);
            free(direction);
            return -11;
        }

        xNew1 = (((dino->x+2*xNew) + field->width) % field->width);
        yNew1 = (((dino->y+2*yNew) + field->height) % field->height);
        xNew = (((dino->x+xNew) + field->width) % field->width);
        yNew = (((dino->y+yNew) + field->height) % field->height);
        if (field->tiles[yNew][xNew] == '@') {
            if (field->tiles[yNew1][xNew1] == '^' || field->tiles[yNew1][xNew1]  == '&' || field->tiles[yNew1][xNew1]  == '@') {
                free(value);
                free(direction);
                return -9;
            }
            else if (field->tiles[yNew1][xNew1]  == '%') {
                field->tiles[yNew][xNew] = colors->tiles[yNew][xNew];
                field->tiles[yNew1][xNew1]  = '_';
            }
            else {
                field->tiles[yNew][xNew] = colors->tiles[yNew][xNew];
                field->tiles[yNew1][xNew1]  = '@';
            }
        } else {
            free(value);
            free(direction);
            return -8;
        }

        free(value);
        free(direction);
        return 1;

    } else {
        printf("ERROR: Unknown command while reading function PUSH\n");
        free(value);
        return 0;
    }
}