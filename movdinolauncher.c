// Для запуска:
// gcc movdinolauncher.c createfield.c dinoactions.c visualizefield.c -o movdinolauncher.exe
// ./movdinolauncher.exe input.txt output.txt ...
// Первыми должны быть написаны данные тры параметра (./movdinolauncher.exe input.txt output.txt), а далее, вместо многоточия, могут быть написаны опции interval N (N - минимум 1), no-display и no-save.
// Любая другая опция будет проигнорирована. А также будет выведено предупреждение.

// Тут будет небольшая памятка:

// При засыпании ямы горой, если клетка была закрашена, то она становится простой клеткой _.
// При раскапывании горы - гора будет роскопана до состояния ямы.
// Формировать гору на клетке, где уже есть гора можно. Будет обычная гора.
// При покраске клетки - поле не меняется. Состояние поля поменятся сразу после следующей команды (MOVE, DIG, JUMP, MOUND, CUT).
// Сажать дерево можно только на пустую либо покрашенную клетку, иначе - ошибка.
// Раскапывать клетку, на котором распологается дерево или камень нельзя, иначе - ошибка.
// Формировать гору на клетке, на котором распологается дерево нельзя, иначе - ошибка.
// При срубании дерева, цвет клетки, на котором было посажено дерево, сохраняется
// При попадании камня в яму, клетка перекрашивается в пустую _.
// Команды все пишем заглавными буквами. Иначе - ошибка с указанием на неизвестную команду.
// Пусть размеры поля (SIZE x y) (10 <= x <= 100, 10 <= y <= 100). Тогда, при установке позиции динозавра (START x1 y1), значения должны принимать такие значения: 0 <= x1 <= x - 1, 0 <= y1 <= y.

// Данная лабороторная работа выполнена вся до пункта 6.3 (не включительно). Проверялась она на macOS.

// Наконец, если Вы думаете, что есть какой-то баг - это не баг, а фича. Удачной игры!


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dinoactions.h"
#include "createfield.h"
#include "visualizefield.h"

char *readthefirstword(char *line) {
    char *value = calloc(1024, sizeof(char));
    char *p = line;
    int offset = 0;

    sscanf(p, "%s %n", value, &offset);
    free(value);
    return value;
}

int main(int argn, char *args[]) {

    if (argn < 3) {
        printf("Wrong arguments!\n");
        return 1;
    }

    FILE *input_file = fopen(args[1], "r");

    if (input_file == NULL) {
        printf("ERROR: Failed to open output file\n");
        fclose(input_file);
        return 1;
    }

    char line[1024];
    int lineNumber = 0;
    struct Field field;
    struct Field colors;
    struct Dino dino;
    struct VisualizationConfig visualizationConfig;

    parse_arguments(argn, args, &visualizationConfig);

    int flag_size = 0, flag_start = 0;

    while (fgets(line, sizeof(line), input_file)) {
        char *action = calloc(1024, sizeof(char));
        action = readthefirstword(line);
        ++lineNumber;

        if (strcmp(action, "SIZE") == 0) {
            if (flag_size == 0) {
                flag_size = 1;
                int config = parse_line_init_field(line, &field);
                int temp = parse_line_init_field(line, &colors);
                if (config == -10) {
                    printf("ERROR: Line number %d contains garbage data at the end. Remove it to avoid early program exit.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == 0) {
                    printf("NO FIELD CREATED\n");

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                } continue;
            } else {
                clear_console();
                printf("ERROR: The SIZE command cannot be used again. Change line %d to avoid early exit from the game.\n", lineNumber);

                fclose(input_file);
                free_field(&field);
                return 1;
            }
        }
        
        else if (strcmp(action, "START") == 0) {
            if (flag_start == 0 && flag_size == 1) {
                flag_start = 1;
                int config = parse_line_init_dino(line, &dino, &field);
                if (config == -10) {
                    printf("ERROR: Line number %d contains garbage data at the end. Remove it to avoid early program exit.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == 0) {
                    print_field(&field, &visualizationConfig, 0, lineNumber);
                    print_field_in_file(&field, args[2], &visualizationConfig);
                    printf("NO DINOSAUR CREATED\n");

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                print_field(&field, &visualizationConfig, 0, lineNumber);
                print_field_in_file(&field, args[2], &visualizationConfig);
            }
            
            else if (flag_start == 0 && flag_size == 0) {
                printf("ERROR: The first command must be SIZE with field dimensions, brooo. Change line %d to avoid early exit from the game.\n", lineNumber);

                fclose(input_file);
                free_field(&field);
                return 1;
            }
            
            else {
                clear_console();
                printf("ERROR: The START command cannot be used again. Change line %d to avoid early exit from the game.\n", lineNumber);

                fclose(input_file);
                free_field(&field);
                return 1;
            }
            clear_console();
        }

        else if (strncmp(action, "//", 2) == 0) {
            continue;
        }
        
        else if (flag_start == 1 && flag_size == 1) {

            if (strcmp(action, "MOVE") == 0) {
                int config = move_dino(line, &dino, &field, &colors);
                if (config == -10) {
                    printf("ERROR: Line number %d contains garbage data at the end. Remove it to avoid early program exit.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == -11) {
                    printf("ERROR: Unknown dinosaur direction after the %s command. Change line %d to avoid early exit from the game.\n", action, lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == -3) {
                    print_field(&field, &visualizationConfig, config, lineNumber);
                    print_field_in_file(&field, args[2], &visualizationConfig);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == 0) {
                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                print_field(&field, &visualizationConfig, config, lineNumber);
            }

            else if (strcmp(action, "PAINT") == 0) {
                int config = paint_cell(line, &dino, &field, &colors);
                int temp = paint_cell(line, &dino, &field, &colors);
                if (config == -10) {
                    printf("ERROR: Line number %d contains garbage data at the end. Remove it to avoid early program exit.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == -1) {
                    printf("ERROR: Expected lowercase letter after PAINT. Change line %d to avoid early exit from the game.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == -2) {
                    printf("ERROR: Expected letter after PAINT, not a combination. Change line %d to avoid early exit from the game.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == 0) {
                    printf("FAILED TO PAINT THE CELL\n");

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                } continue;
            }

            else if (strcmp(action, "DIG") == 0) {
                int config = dig_cell(line, &dino, &field);
                if (config == -10) {
                    printf("ERROR: Line number %d contains garbage data at the end. Remove it to avoid early program exit.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == -11) {
                    printf("ERROR: Unknown direction after the %s command. Change line %d to avoid early exit from the game.\n", action, lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == -21) {
                    printf("ERROR: It is not possible to form a pit on an occupied cell. The cell must be empty or have a mountain or have a pit. Change line %d to avoid early exit from the game.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == 0) {
                    print_field(&field, &visualizationConfig, config, lineNumber);
                    print_field_in_file(&field, args[2], &visualizationConfig);
                    printf("FAILED TO DIG THE CELL\n");

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                print_field(&field, &visualizationConfig, config, lineNumber);
            }

            else if (strcmp(action, "MOUND") == 0) {
                int config = mound_cell(line, &dino, &field, &colors);
                if (config == -10) {
                    printf("ERROR: Line number %d contains garbage data at the end. Remove it to avoid early program exit.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == -11) {
                    printf("ERROR: Unknown direction after the %s command. Change line %d to avoid early exit from the game.\n", action, lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == -20) {
                    printf("ERROR: Impossiple to form a mountain on an occupied cell. The cell must be empty or have a hole or have a mountain. Change line %d to avoid early exit from the game.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == 0) {
                    print_field(&field, &visualizationConfig, config, lineNumber);
                    print_field_in_file(&field, args[2], &visualizationConfig);
                    printf("FAILED TO MOUND THE CELL\n");

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                print_field(&field, &visualizationConfig, config, lineNumber);
            }

            else if (strcmp(action, "JUMP") == 0) {
                int config = jump_dino(line, &dino, &field, &colors);
                if (config == -10) {
                    printf("ERROR: Line number %d contains garbage data at the end. Remove it to avoid early program exit.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == -11) {
                    printf("ERROR: Unknown dinosaur direction after the %s command. Change line %d to avoid early exit from the game.\n", action, lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == -3 || config == -5) {
                    print_field(&field, &visualizationConfig, config, lineNumber);
                    print_field_in_file(&field, args[2], &visualizationConfig);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == 0) {
                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                print_field(&field, &visualizationConfig, config, lineNumber);
            }

            else if (strcmp(action, "GROW") == 0) {
                int config = grow_tree(line, &dino, &field, &colors);
                if (config == -10) {
                    printf("ERROR: Line number %d contains garbage data at the end. Remove it to avoid early program exit.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == -11) {
                    printf("ERROR: Unknown direction after the %s command. Change line %d to avoid early exit from the game.\n", action, lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == -12) {
                    printf("ERROR: It is impossible to plant a tree on an occupied cell. The cell must be empty. Change line %d to avoid early exit from the game.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == 0) {
                    print_field(&field, &visualizationConfig, config, lineNumber);
                    print_field_in_file(&field, args[2], &visualizationConfig);
                    printf("FAILED TO GROW THE CELL\n");

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                print_field(&field, &visualizationConfig, config, lineNumber);
            }

            else if (strcmp(action, "CUT") == 0) {
                int config = cut_tree(line, &dino, &field, &colors);
                if (config == -10) {
                    printf("ERROR: Line number %d contains garbage data at the end. Remove it to avoid early program exit.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == -11) {
                    printf("ERROR: Unknown direction after the %s command. Change line %d to avoid early exit from the game.\n", action, lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == 0) {
                    print_field(&field, &visualizationConfig, config, lineNumber);
                    print_field_in_file(&field, args[2], &visualizationConfig);
                    printf("FAILED TO CUT THE TREE\n");

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                print_field(&field, &visualizationConfig, config, lineNumber);
            }

            else if (strcmp(action, "MAKE") == 0) {
                int config = make_rock(line, &dino, &field, &colors);
                if (config == -10) {
                    printf("ERROR: Line number %d contains garbage data at the end. Remove it to avoid early program exit.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == -11) {
                    printf("ERROR: Unknown direction after the %s command. Change line %d to avoid early exit from the game.\n", action, lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == -13) {
                    printf("ERROR: It is impossible to put a rock on an occupied cell. The cell must be empty. Change line %d to avoid early exit from the game.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == 0) {
                    print_field(&field, &visualizationConfig, config, lineNumber);
                    print_field_in_file(&field, args[2], &visualizationConfig);
                    printf("FAILED TO MAKE THE ROCK\n");

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                print_field(&field, &visualizationConfig, config, lineNumber);
            }


            else if (strcmp(action, "PUSH") == 0) {
                int config = push_rock(line, &dino, &field, &colors);
                if (config == -10) {
                    printf("ERROR: Line number %d contains garbage data at the end. Remove it to avoid early program exit.\n", lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == -11) {
                    printf("ERROR: Unknown direction after the %s command. Change line %d to avoid early exit from the game.\n", action, lineNumber);

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                if (config == 0) {
                    print_field(&field, &visualizationConfig, config, lineNumber);
                    print_field_in_file(&field, args[2], &visualizationConfig);
                    printf("FAILED PUSH THE ROCK\n");

                    fclose(input_file);
                    free_field(&field);
                    return 1;
                }
                print_field(&field, &visualizationConfig, config, lineNumber);
            }

            else if (strncmp(action, "//", 2) == 0) {
                continue;
            }

            else {
                clear_console();
                printf("ERROR: Unknown command (command: %s) at line %d. Change that line to avoid early exit from the game.\n", action, lineNumber);

                fclose(input_file);
                free_field(&field);
                return 1;
            }

            clear_console();
            print_field_in_file(&field, args[2], &visualizationConfig);
        }
        
        else if (flag_size == 0) {
            printf("ERROR: The first command must be SIZE with field dimensions, brooo. Change line %d to avoid early exit from the game.\n", lineNumber);

            fclose(input_file);
            free_field(&field);
            return 1;
        }

        else if (flag_start == 0) {
            clear_console();
            printf("ERROR: The second command must be START with the dinosaur's coordinates. Change line %d to avoid early exit from the game.\n", lineNumber);

            fclose(input_file);
            free_field(&field);
            return 1;
        }
    }

    printf("GAME OVER!\n");

    fclose(input_file);

    free_field(&field);

    return 0;
}