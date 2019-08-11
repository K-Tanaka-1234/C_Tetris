#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "display.h"
#include "keyboard.h"
#include "tetris.h"

int main(void)
{
    char mat[GAME_WIDTH * GAME_HEIGHT];
    enum Tetrimino next;
    int x, y, r, prev_y;
    int tick_rate = 1000;
    int period = 500;
    int count = 0;
    int score = 0;
    int speed_up[5] = {10000, 5000, 2500, 1250, 625};
    int level[5] = {0};

    tetris_init();
    set_new_mino();

    while(1)
    {
        switch (kbhit())
        {
            case 'a':
                decrement_x();
                break;
            case 's':
                increment_y();
                break;
            case 'd':
                increment_x();
                break;
            case 'f':
                decrement_rotate();
                break;
            case 'g':
                increment_rotate();
                break;
            default:
                break;
        }

        if(put_mino(0)) break;

        get_color_matrix(mat);
        int size = get_mino_size(next);
        char *mino_mat = (char *)malloc(sizeof(char) * size * size);
        get_mino_mat(next, mino_mat);

        rect(0, 0, 17, 21, BLACK);
        rect(11, 0, 6, 6, WHITE);
        rect(0, 0, 10, 20, WHITE);
        color_matrix(12, 0, size, size, mino_mat);
        color_matrix(0, 0, GAME_WIDTH, GAME_HEIGHT, mat);
        flush();
        free(mino_mat);

        if(count % period == 0)
        {
            prev_y = y;
            increment_y();
            get_mino_state(&x, &y, &r, &next);

            if(prev_y == y)
            {
                lock_mino();
                set_new_mino();
            }

            for(int i = 0;i < 5;i++)
            {
                if(score > speed_up[i] && level[i] == 0)
                {
                    period -= i * 100;
                    level[i] = 1;
                }
            }

            score += remove_filled_line();
            printf("score = %d\n", score);
        }

        usleep(1000000 / tick_rate);
        count++;
    }

    printf("###   GAME OVER   ###\n\nSCORE > %d\n", score);

    return 0;
}
