#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "lcd.h"

typedef struct Buffer {
    int w;
    int h;
    const unsigned char data[];
} Buffer;

Buffer image = {
    .w = 16,
    .h = 16,
    .data = {
        0xC1, 0x5F, 0x41, 0x00, 0x1F, 0xD5, 0x11, 0x00, 0xD2, 0x15, 0x09, 0x00, 0xC1, 0x5F, 0x41, 0x00,
        0x07, 0x01, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x07, 0x04, 0x04, 0x00, 0x07, 0x05, 0x04, 0x00,
    }
};


typedef struct Ball {
    int x;
    int y;
    int vx;
    int vy;
    int r;
} Ball;

static Ball balls[8];

void create_ball(Ball *ball) {
    ball->vx = ((rand() % 2) * 2 - 1) * (rand() % 2 + 1);
    ball->vy = ((rand() % 2) * 2 - 1) * (rand() % 2 + 1);
    ball->r = rand() % 8 + 4;
    ball->x = rand() % (LCD_X - ball->r * 2) + ball->r;
    ball->y = rand() % (LCD_Y - ball->r * 2) + ball->r;
}

void update_ball(Ball *ball) {
    ball->x += ball->vx;
    ball->y += ball->vy;
    if (ball->x - ball->r < 0 || ball->x + ball->r >= LCD_X) {
        ball->vx = -ball->vx;
        ball->x += 2 * ball->vx;
    }
    if (ball->y - ball->r < 0 || ball->y + ball->r >= LCD_Y) {
        ball->vy = -ball->vy;
        ball->y += 2 * ball->vy;
    }
}

void draw_ball(Ball *ball) {

    LCD_FillCircle(ball->x, ball->y, ball->r, WHITE);
    LCD_DrawCircle(ball->x, ball->y, ball->r, BLACK);
    LCD_FillCircle(ball->x, ball->y, ball->r - 2, BLACK);

    // LCD_Blit(image.data, ball->x, ball->y, image.w, image.h, OR);

    // LCD_FillRect(ball->x - ball->r, ball->y - ball->r, ball->x + ball->r, ball->y + ball->r, XOR);
    // LCD_DrawRect(ball->x - ball->r, ball->y - ball->r,
    //              ball->x + ball->r, ball->y + ball->r, BLACK);

    // LCD_Pixel(x, y, BLACK);
}

int main()
{
    int size = sizeof(balls) / sizeof(*balls);
    int i;
    srand(time(NULL));

    for (i = 0; i < size; ++i)
    {
        create_ball(&balls[i]);
    }

    if (LCD_Init() != 0) {
        printf("Error initializing LCD\n");
        return 1;
    }

    LCD_SetBacklight(1);

    while (1) {

        for (i = 0; i < size; ++i)
        {
            update_ball(&balls[i]);
        }

        LCD_Clear();

        for (i = 0; i < size; ++i)
        {
            draw_ball(&balls[i]);
        }

        LCD_Display();

        usleep(25000);
    }

    return 0;
}
