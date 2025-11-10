#include "createfield.h"

struct VisualizationConfig {
    double interval;          // задержка между обновлениями
    int display_enabled;   // включена ли визуализация
    int save_enabled;      // включено ли сохранение
};

// Функция для того, чтобы печатать поле в терминале (при display_enabled = 1). Третье значение создано для того, чтобы проверять не наткнулся ли динозавр на препятствие.
// (Оно получает значения конфига и принимает соответствующие действия)
// Третье значение получает номер строки (нужно будет для вывода ошибок)
void print_field(struct Field*, struct VisualizationConfig*, int, int);

// Функция для того, чтобы распечатать конечный результат поля в output файле после прочитанного действия (при save_enabled = 1)
void print_field_in_file(struct Field*, const char*, struct VisualizationConfig*);

// Функция для того, чтобы cчитать дополнительные опции с командной строки
void parse_arguments(int, char *[], struct VisualizationConfig*);

// Функция для того, чтобы чистить консоль
void clear_console();

// Функция задержки
void delay(double);