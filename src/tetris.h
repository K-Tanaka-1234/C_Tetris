#ifndef TETRIS_H
#define TETRIS_H

#define GAME_WIDTH 10
#define GAME_HEIGHT 20
#define SCORE_MAG 100

enum Tetrimino {T_MINO, Z_MINO, S_MINO, J_MINO, L_MINO, I_MINO, O_MINO};

void tetris_init();
void get_color_matrix(char *);
void set_new_mino();
void gen_mino(char *);
int put_mino(int);
int remove_filled_line();
void lock_mino();
void get_mino_state(int *, int *, int *, enum Tetrimino *);
int get_mino_size(enum Tetrimino);
void get_mino_mat(enum Tetrimino, char *);
void increment_x();
void decrement_x();
void increment_y();
void decrement_y();
void increment_rotate();
void decrement_rotate();

#endif
