#pragma once

// Сруктура нашего динозавра. x, y - координаты динозавра по горизонтали и вертикали соответственно.
struct Dino {
    int x, y;
    char steppedCell;
};

// Сруктура нашего поля/прямоугольника; width - ширина прямоугольника, height - высота прямоугольника
struct Field {
    char **tiles;
    int width, height;
};

// Функция для инициализации динозавра
void init_dino(struct Dino*, struct Field *, int, int);

// Функция для удаления динозавра, чтобы в последующем его инициализировать на новой позиции (по возможности)
void del_dino(struct Dino*, struct Field *field, struct Field *, int, int);

// Функция для создания поля
int init_field(struct Field*, int, int);

// Функция для определения размеров поля, а затем для его инициализации (чтение первой строки)
int parse_line_init_field(char *const, struct Field*);

// Функция для определения позиции динозавра (чтение второй строки)
int parse_line_init_dino(char *const, struct Dino*, struct Field*);

// Функция для освобождения памяти поля
void free_field(struct Field *);

// Функция для подсчёта слов (создана для подсчета количества слов/чисел после команды)
int count_words(char const*, int);