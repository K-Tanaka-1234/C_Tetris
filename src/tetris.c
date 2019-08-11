#include "tetris.h"
#include "display.h"

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define DEG_TO_RAD(d) ((double)d * M_PI / 180.0)

static char mino_t[9] = {
    0,          0,          0,
    0,          MAGENTA,    0,
    MAGENTA,    MAGENTA,    MAGENTA
};

static char mino_z[9] = {
    0,      0,      0,
    RED,    RED,    0,
    0,      RED,    RED,
};

static char mino_s[9] = {
    0,      0,      0,
    0,      GREEN,  GREEN,
    GREEN,  GREEN,  0
};

static char mino_j[9] = {
    0,      0,      0,
    BLUE,   0,      0,
    BLUE,   BLUE,   BLUE
};

static char mino_l[9] = {
    0,      0,      0,
    0,      0,      BLACK,
    BLACK,  BLACK,  BLACK
};

static char mino_i[25] = {
    0,  0,  0,      0,  0,
    0,  0,  CYAN,   0,  0,
    0,  0,  CYAN,   0,  0,
    0,  0,  CYAN,   0,  0,
    0,  0,  CYAN,   0,  0
};

static char mino_o[9] = {
    0,  0,      0,
    0,  YELLOW, YELLOW,
    0,  YELLOW, YELLOW
};

static char *minos[] = {mino_t, mino_z, mino_s, mino_j, mino_l, mino_i, mino_o};

static char block_map[GAME_HEIGHT + 1][GAME_WIDTH + 2];
static char block_buf[GAME_HEIGHT + 1][GAME_WIDTH + 2];
static enum Tetrimino mino;
static enum Tetrimino next_mino;
static int mino_size;
static int mino_pos_offset;
static int history;
static int mino_rotate;
static int mino_x;
static int mino_y;
static int prev_mino_rotate;
static int prev_mino_x;
static int prev_mino_y;

void tetris_init()
{
    srand((unsigned)time(NULL));
    next_mino = rand() % 7;

    for(int i = 0;i < GAME_HEIGHT + 1;i++)
    {
        for(int j = 0;j < GAME_WIDTH + 2;j++)
        {
            if(i == GAME_HEIGHT || j <= 0 || j >= GAME_WIDTH + 1)
            {
                block_map[i][j] = DEFAULT;
            }
            else
            {
                block_map[i][j] = 0;
            }
        }
    }
}

void get_color_matrix(char *ret)
{
    for(int i = 0;i < GAME_HEIGHT;i++)
    {
        for(int j = 1;j < GAME_WIDTH + 1;j++)
        {
            ret[i * GAME_WIDTH + (j - 1)] = block_map[i][j] + block_buf[i][j];
        }
    }
}

void set_new_mino()
{
    mino = next_mino;
    next_mino = rand() % 7;
    mino_rotate = 0;
    mino_x = 5;
    mino_y = -2;

    mino_size = (mino == I_MINO) ? 5 : 3;
    mino_pos_offset = mino_size / 2;
}

void gen_mino(char *ret)
{
    int a = (int)cos(DEG_TO_RAD(mino_rotate));
    int b = (int)sin(DEG_TO_RAD(mino_rotate));

    int hx = (mino_size - 1) / 2;
    int hy = (mino_size - 1) / 2;

    for(int i = 0;i < mino_size;i++)
    {
        for(int j = 0;j < mino_size;j++)
        {
            int x = ((j - hx) * a - (i - hy) * b + hx);
            int y = ((j - hx) * b + (i - hy) * a + hy);
            ret[y * mino_size + x] = minos[mino][i * mino_size + j];
        }
    }
}

int get_mino_size(enum Tetrimino mino_sel)
{
    return (mino_sel == I_MINO) ? 5 : 3;
}

void get_mino_mat(enum Tetrimino mino_sel, char *ret)
{
    int size = (mino_sel == I_MINO) ? 5 : 3;
    for(int i = 0;i < size * size;i++) ret[i] = minos[mino_sel][i];
}

void shift_block_map(int line)
{
    for(int i = line;i > 0;i--)
    {
        for(int j = 1;j < GAME_WIDTH + 1;j++) block_map[i][j] = block_map[i - 1][j];
    }
}

int remove_filled_line()
{
    int point = 0;

    for(int i = 0;i < GAME_HEIGHT;i++)
    {
        int count = 0;

        for(int j = 1;j < GAME_WIDTH + 1;j++) if(block_map[i][j] == 0) count++;

        if(count <= 0)
        {
            for(int j = 1;j < GAME_WIDTH + 1;j++) block_map[i][j] = 0;
            shift_block_map(i);
            point++;
        }
    }

    return point * point * SCORE_MAG;
}

int check_collision()
{
    for(int i = 0;i < GAME_HEIGHT + 1;i++)
    {
        for(int j = 0;j < GAME_WIDTH + 2;j++)
        {
            if(block_map[i][j] != 0 && block_buf[i][j] != 0) return 1;
        }
    }
    return 0;
}

int put_mino(int err_count)
{
    if(err_count > 1) return 1;

    for(int i = 0;i < (GAME_HEIGHT + 1) * (GAME_WIDTH + 2);i++)
    {
        block_buf[i / (GAME_WIDTH + 2)][i % (GAME_WIDTH + 2)] = 0;
    }

    char *current_mino = (char *)malloc(sizeof(char) * mino_size * mino_size);
    gen_mino(current_mino);

    for(int i = 0;i < mino_size;i++)
    {
        for(int j = 0;j < mino_size;j++)
        {
            int x = mino_x - mino_pos_offset + j;
            int y = mino_y - mino_pos_offset + i;
            if(x >= 0 && x < GAME_WIDTH + 2 && y >= 0 && y < GAME_HEIGHT + 1)
            {
                block_buf[y][x] = current_mino[i * mino_size + j];
            }
        }
    }

    if(check_collision())
    {
        switch (history)
        {
            case 1:
                mino_x = prev_mino_x;
                break;
            case 2:
                mino_y = prev_mino_y;
                break;
            case 3:
                mino_rotate = prev_mino_rotate;
                break;
            default:
                break;
        }
        return put_mino(err_count + 1);
    }

    free(current_mino);
    return 0;
}

void lock_mino()
{
    char *current_mino = (char *)malloc(sizeof(char) * mino_size * mino_size);
    gen_mino(current_mino);

    for(int i = 0;i < GAME_HEIGHT;i++)
    {
        for(int j = 1;j < GAME_WIDTH + 1;j++)
        {
            block_map[i][j] += block_buf[i][j];
        }
    }

    free(current_mino);
}

void get_mino_state(int *pos_x, int *pos_y, int *rotate, enum Tetrimino *next)
{
    *pos_x = mino_x;
    *pos_y = mino_y;
    *rotate = mino_rotate;
    *next = next_mino;
}

void increment_x()
{
    prev_mino_x = mino_x;
    mino_x++;
    history = 1;
}

void decrement_x()
{
    prev_mino_x = mino_x;
    mino_x--;
    history = 1;
}

void increment_y()
{
    prev_mino_y = mino_y;
    mino_y++;
    history = 2;
}

void decrement_y()
{
    prev_mino_y = mino_y;
    mino_y--;
    history = 2;
}

void increment_rotate()
{
    prev_mino_rotate = mino_rotate;
    mino_rotate = (mino_rotate + 90) % 360;
    history = 3;
}

void decrement_rotate()
{
    prev_mino_rotate = mino_rotate;
    mino_rotate = (mino_rotate - 90) % 360;
    history = 3;
}
