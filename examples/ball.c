#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "lcd/lcd.h"
#include "lcd/font.h"

typedef struct Buffer {
    int w;
    int h;
    const unsigned char data[];
} Buffer;

// Buffer image = {
//     .w = 8,
//     .h = 32,
//     .data = {
//         0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
//         0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
//     }
// };

// Buffer image = {
//     .w = 16,
//     .h = 14,
//     .data = {
//         0x23, 0x23, 0xEC, 0x2C, 0x23, 0x83, 0x4C, 0xCC, 0x03, 0x83, 0x4C, 0x4C, 0x03, 0x43, 0xEC, 0x4C,
//         0x30, 0x30, 0xC7, 0xC0, 0x30, 0x33, 0xC5, 0xC4, 0x30, 0x34, 0xC5, 0xC2, 0x30, 0x30, 0xC7, 0xC4,
//     }
// };

Buffer image = {
    .w = 16,
    .h = 16,
    .data = {
        0xC0, 0xF0, 0xFC, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC, 0xFC, 0xF0, 0xC0,
        0x03, 0x0F, 0x3F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x3F, 0x3F, 0x0F, 0x03,
    }
};


// Buffer image = {
//     .w = 32,
//     .h = 30,
//     .data = {
//         0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0,
//         0x0C, 0x0C, 0x0C, 0x0C, 0xFC, 0xFC, 0x0C, 0x0C, 0x0C, 0x0C, 0xC0, 0xC0, 0x30, 0x30, 0xF0, 0xF0, 0x00, 0x00, 0xC0, 0xC0, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0xFC, 0xFC, 0x30, 0x30,
//         0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x33, 0x33, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x33, 0x33, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x30, 0x30,
//         0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0,
//     }
// };


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
    ball->x = rand() % (LCD_WIDTH - ball->r * 2) + ball->r;
    ball->y = rand() % (LCD_HEIGHT - ball->r * 2) + ball->r;
}

void update_ball(Ball *ball) {
    ball->x += ball->vx;
    ball->y += ball->vy;
    if (ball->x - ball->r < 0 || ball->x + ball->r >= LCD_WIDTH) {
        ball->vx = -ball->vx;
        ball->x += 2 * ball->vx;
    }
    if (ball->y - ball->r < 0 || ball->y + ball->r >= LCD_HEIGHT) {
        ball->vy = -ball->vy;
        ball->y += 2 * ball->vy;
    }
}

void draw_ball(Ball *ball) {

    // LCD_FillCircle(ball->x, ball->y, ball->r, WHITE);
    // LCD_DrawCircle(ball->x, ball->y, ball->r, BLACK);
    LCD_FillCircle(ball->x, ball->y, ball->r , XOR);

    // LCD_Blit(image.data, ball->x - ball->r, ball->y - ball->r, image.w, image.h, XOR);

    // LCD_FillRect(ball->x - ball->r, ball->y - ball->r, ball->x + ball->r, ball->y + ball->r, XOR);
    // LCD_DrawRect(ball->x - ball->r, ball->y - ball->r,
    //              ball->x + ball->r, ball->y + ball->r, BLACK);

    // LCD_Pixel(x, y, BLACK);
}

int main()
{
    int size = sizeof(balls) / sizeof(*balls);
    int i, j;

    const char string[] = "Hello World! How are you? ";
    int len = strlen(string);

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

    j = 0;

    for (;;) {

        for (i = 0; i < size; ++i)
        {
            update_ball(&balls[i]);
        }

        LCD_Clear();
        // LCD_Invert();

        for (i = 0; i < size; ++i)
        {
            draw_ball(&balls[i]);
        }

        LCD_FillRect(0, (LCD_HEIGHT - LCD_CHAR_HEIGHT) / 2 - 2, LCD_WIDTH, (LCD_HEIGHT - LCD_CHAR_HEIGHT) / 2 + LCD_CHAR_HEIGHT + 1, BLACK);
        LCD_FillRect(0, (LCD_HEIGHT - LCD_CHAR_HEIGHT) / 2 - 1, LCD_WIDTH, (LCD_HEIGHT - LCD_CHAR_HEIGHT) / 2 + LCD_CHAR_HEIGHT, WHITE);

        LCD_TextLocate(j, (LCD_HEIGHT - LCD_CHAR_HEIGHT) / 2);
        LCD_Text(string);
        LCD_TextLocate(j + len * 4, (LCD_HEIGHT - LCD_CHAR_HEIGHT) / 2);
        LCD_Text(string);

        j = (j - 1) % (len * 4);

        // for (i = 0; i <= 84/16; ++i)
        // {
        //     LCD_Blit(image.data, i * 16, i, image.w, image.h, OR);
        // }

        LCD_Display();

        usleep(25000);
    }

    return 0;
}