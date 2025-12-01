#include "createfield.h"

// Функция для передвижения динозавра на одну клетку
int move_dino(char *const, struct Dino *, struct Field *, struct Field *);

// Функция для окрашивания клетки, на котором стоит динозавра
int paint_cell(char *const, struct Dino *, struct Field *, struct Field *);

// Функция для того, чтобы откопать клетку по заданному направлению
int dig_cell(char *const, struct Dino *, struct Field *);

// Функция для того, чтобы сформировать гору по заданному направлению
int mound_cell(char *const, struct Dino *, struct Field *, struct Field *);

// Функция для передвижения динозавра на одну несколько клеток по заданному направлению (количество клеток будет задано после направления)
int jump_dino(char *const, struct Dino *, struct Field *, struct Field *);

// Функция для того, чтобы вырастить дерево по заданному направлению
int grow_tree(char *const, struct Dino *, struct Field *, struct Field *);

// Функция для того, чтобы срубить дерево
int cut_tree(char *const, struct Dino *, struct Field *, struct Field *);

// Функция для того, чтобы создать камень по заданному направлению
int make_rock(char *const, struct Dino *, struct Field *, struct Field *);

// Функция для того, чтобы пнуть камень по заданному направлению
int push_rock(char *const, struct Dino *, struct Field *, struct Field *);