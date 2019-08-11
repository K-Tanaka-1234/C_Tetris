#include <stdio.h>

#include "display.h"

static char frame[DISPLAY_HEIGHT][DISPLAY_WIDTH];

void dot(int x, int y, enum PrintColor color)
{
    if(x < DISPLAY_WIDTH && x >= 0 && y < DISPLAY_HEIGHT && y >= 0)
    {
        frame[y][x] = color;
    }
}

void rect(int x, int y, int w, int h, enum PrintColor color)
{
    for(int i = y;i < y + h;i++)
    {
        for(int j = x;j < x + w;j++)
        {
            dot(j, i, color);
        }
    }
}

void matrix(int x, int y, int w, int h, char *mat, enum PrintColor color)
{
    for(int i = y;i < y + h;i++)
    {
        for(int j = x;j < x + w;j++)
        {
            if(mat[(i - y) * w + (j - x)]) dot(j, i, color);
        }
    }
}

void color_matrix(int x, int y, int w, int h, char *cmat)
{
    int c;

    for(int i = y;i < y + h;i++)
    {
        for(int j = x;j < x + w;j++)
        {
            c = cmat[(i - y) * w + (j - x)];
            if(c >= BLACK && c <= WHITE) dot(j, i, c);
        }
    }
}

void flush()
{
    //printf("\033[2J");
    printf("\033[?25l");
    printf("\033[%d;%dH", 0, 0);
    for(int i = 0;i < DISPLAY_HEIGHT;i++)
    {
        for(int j = 0;j < DISPLAY_WIDTH;j++)
        {
            printf("\033[%dm  ", frame[i][j]);
        }
        printf("\n");
    }
    printf("\033[?25h");
    printf("\033[%dm", RESET);
}
