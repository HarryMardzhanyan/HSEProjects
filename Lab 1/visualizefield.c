#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "visualizefield.h"
#include "createfield.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void print_field(struct Field* field, struct VisualizationConfig* visualizationConfig, int cfg, int lineNum) {

    if (visualizationConfig->interval < 1) {
        visualizationConfig->interval = 1;
        printf("WARNING: You entered an number of seconds less than 1 in the 'interval' parameter. The default delay will be 1 second.\n");
        delay(visualizationConfig->interval);
    }
    
    if (visualizationConfig->display_enabled) {
        clear_console();
        //delay(1);    // Замораживаем терминал на секунду при отображении следующего хода (создано для красоты)

        if (cfg == -1) {
            printf("WARNING: You have come across a mountain. The command at line %d will be skipped and the next command will be read.\n", lineNum);
        }

        if (cfg == -3) {
            printf("WARNING: You have step into a pit and fell in. Change line %d to avoid early exit from the game.\n", lineNum);
        }

        if (cfg == -4) {
            printf("WARNING: You have come across a tree. The command at line %d will be skipped and the next command will be read.\n", lineNum);
        }

        if (cfg == -5) {
            printf("WARNING: You have come across a barrier and you fell into the pit. Change line %d to avoid early exit from the game.\n", lineNum);
        }

        if (cfg == -6) {
            printf("WARNING: An attempt was made to cut down a tree, but no tree was found in the specified direction. The command in line %d will be ignored.\n", lineNum);
        }

        if (cfg == -7) {
            printf("WARNING: You have come across a rock. The command at line %d will be skipped and the next command will be read.\n", lineNum);
        }

        if (cfg == -8) {
            printf("WARNING: An attempt was made to push a rock, but no rock was found in the specified direction. The command in line %d will be ignored.\n", lineNum);
        }

        if (cfg == -9) {
            printf("WARNING: It is impossible to move the rock because there is a barrier in front of it. The command in line %d will be ignored.\n", lineNum);
        }

        // Проходимся по значениям высоты поля
        for (int i = 0; i < field->height; ++i) {

            // Проходимся по значениям ширины поля
            for (int j = 0; j < field->width; ++j) {
                printf("%c", field->tiles[i][j]);
            }
            
            // Переходим на следующую строку
            printf("\n");
        }
        printf("\n");
        
        if (cfg == -3 || cfg == -5) {
            printf("GAME OVER!\n");
        }

        if (visualizationConfig->interval >= 1) {
            delay(visualizationConfig->interval);
        }
    }
}

void print_field_in_file(struct Field* field, const char *output_filename, struct VisualizationConfig* visualizationConfig) {

    if (visualizationConfig->save_enabled) {
        FILE *output_file = fopen(output_filename, "w");

        if (output_file == NULL) {
            printf("ERROR: Failed to open output file");
            fclose(output_file);
            return;
        }

        for (int i=0; i < field->height; ++i) {
            for (int j=0; j < field->width; ++j) {
                fputc(field->tiles[i][j], output_file);
            } fputc('\n', output_file);
        }

        fclose(output_file);
    }
}

void parse_arguments(int argn, char *args[], struct VisualizationConfig *visualizationConfig) {

    // Значения по умолчанию
    visualizationConfig->interval = 1;
    visualizationConfig->display_enabled = 1;
    visualizationConfig->save_enabled = 1;
    
    for (int i = 3; i < argn; i++) {
        if (strcmp(args[i], "interval") == 0 && i + 1 < argn) {
            visualizationConfig->interval = atoi(args[++i]);
        } else if (strcmp(args[i], "no-display") == 0) {
            visualizationConfig->display_enabled = 0;
        } else if (strcmp(args[i], "no-save") == 0) {
            visualizationConfig->save_enabled = 0;
        } else {
            char* option = args[i];
            printf("ERROR: Unknown option '%s' in command line. This command will be ignored.\n", option);
        }
    }
}

void clear_console() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void delay(double seconds) {
    #ifdef _WIN32
        Sleep(seconds * 1000);
    #else
        sleep(seconds);
    #endif
}