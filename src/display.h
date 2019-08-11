#ifndef DISPLAY_H
#define DISPLAY_H

#define DISPLAY_WIDTH 18
#define DISPLAY_HEIGHT 21

enum PrintColor {RESET, BLACK = 40, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE,  DEFAULT = 49};

void dot(int, int, enum PrintColor);
void rect(int, int, int, int, enum PrintColor);
void matrix(int, int, int, int, char *, enum PrintColor);
void color_matrix(int, int, int, int, char *);
void flush();

#endif
