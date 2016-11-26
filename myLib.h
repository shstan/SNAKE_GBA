typedef unsigned short u16;
typedef unsigned int   u32;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "text.h"
#include "snake_title.h"
#include "snake_dead.h"
#include "side_bar.h"
#include "danger.h"
#include "end_theme.h"


#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)
#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define CYAN COLOR(0,31,31)
#define MAGENTA COLOR(31, 0,31)
#define YELLOW COLOR(31,31,0)
#define WHITE COLOR(31,31,31)
#define BLACK 0
#define LTGRAY COLOR(25, 25, 25)
#define ORANGE  COLOR(31, 15, 0 )
#define BROWN   COLOR(18, 9 , 0 )
#define PURPLE  COLOR(15, 0 , 15)
#define TEAL    COLOR(0 , 15, 15)
#define MAROON  COLOR(15, 0 , 0 )
#define GREY    COLOR(15, 15, 15)
#define PINK    COLOR(31, 18, 19)
#define DARK_G  COLOR(0 , 23, 0  )
#define SCANLINECOUNTER  (*(volatile unsigned short *)0x4000006)

#define OFFSET(r, c, numcols) ((r)*(numcols)+(c))

// Buttons

#define BUTTON_A        (1<<0)
#define BUTTON_B        (1<<1)
#define BUTTON_SELECT   (1<<2)
#define BUTTON_START    (1<<3)
#define BUTTON_RIGHT    (1<<4)
#define BUTTON_LEFT     (1<<5)
#define BUTTON_UP       (1<<6)
#define BUTTON_DOWN     (1<<7)
#define BUTTON_R        (1<<8)
#define BUTTON_L        (1<<9)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

#define BUTTONS *(volatile unsigned int *)0x4000130

/* DMA */
/*for controlling DMA*/
typedef struct
{
    const volatile void *src;
    const volatile void *dst;
    unsigned int cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines some DMA stuff.
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)



/* DMA */

#define REG_DMA0SAD         *(volatile u32*)0x40000B0       // source address
#define REG_DMA0DAD         *(volatile u32*)0x40000B4       // destination address
#define REG_DMA0CNT         *(volatile u32*)0x40000B8       // control register

// DMA channel 1 register definitions
#define REG_DMA1SAD         *(volatile u32*)0x40000BC       // source address
#define REG_DMA1DAD         *(volatile u32*)0x40000C0       // destination address
#define REG_DMA1CNT         *(volatile u32*)0x40000C4       // control register

// DMA channel 2 register definitions
#define REG_DMA2SAD         *(volatile u32*)0x40000C8       // source address
#define REG_DMA2DAD         *(volatile u32*)0x40000CC       // destination address
#define REG_DMA2CNT         *(volatile u32*)0x40000D0       // control register

// DMA channel 3 register definitions
#define REG_DMA3SAD         *(volatile u32*)0x40000D4       // source address
#define REG_DMA3DAD         *(volatile u32*)0x40000D8       // destination address
#define REG_DMA3CNT         *(volatile u32*)0x40000DC       // control register





extern unsigned short *videoBuffer;



/** snake block structure.
 *
 * row: row of the snake block
 * col: column of the snake block
 *
 */
typedef struct {

	int row;
	int col;
	bool active;
} snakeBlock;
void update_body(snakeBlock snakebody[], int snake_length, int head_row, int head_col, int cell_size);



/** cellState
 * what each grid cell contains.
 * EMPTY: empty
 * FOOD: food
 * SNAKE: a snake block is there
 * POISON: the red poison block
 */
enum cellState {
    EMPTY,
    FOOD,
    SNAKE,
    POISON
};





/** GBAState
 * menu enum.
 * START: title screen
 * WAIT: listens to key
 * STATE1: speed control page
 * STATE2: help screen
 * STATE3: game & game over screen
 */
enum GBAState {
    START,
    WAIT,
    STATE1,
    STATE2,
    STATE3
};



// Prototypes
void setPixel(int row, int col, u16 color);
void drawRect(int row, int col, int height, int width, u16 color);
void waitForVBlank();
void drawImage3(int row, int col, int width, int height, const unsigned short *image);
void delay(int n);
void drawBox(int col, int row, unsigned short color, int cell_size);
void initializeBoard(int tot_row, int tot_col, int cell_size);
void goToNewRow(int oldrow, int oldcol, int row, int col, int cell_size);
int  game();
void fillScreen(u16 color);
void drawProgressBar(int total, int processed, u16 backgrounnd, u16 barcolor, int h, int w, int row, int col);
int snakesize(int food_eaten, int tot_row, int tot_col);
void newBody(int row, int col, int cell_size);

