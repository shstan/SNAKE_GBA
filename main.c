
#include "myLib.h"
// the main file


// for controlling speed of game
int count = 3000;




// three highest scores.
int score = 0;
int second_score = 0;
int third_score = 0;

// for snakeblock coloring.
u16 bgcolor = GREY;
u16 cell_color = DARK_G;

/** main
 * The menu function.
 * @return int returns 0 if successful.
 */
int main() {

    REG_DISPCTL = MODE3 | BG2_ENABLE;

    enum GBAState state = START;
    enum GBAState prev_state = state;

    unsigned short prev_buttons = BUTTONS;
    volatile int timer = 0;

    while(1) {
        waitForVBlank();
        switch(state) {
        case START:
            drawImage3(0, 0, SNAKE_TITLE_WIDTH, SNAKE_TITLE_HEIGHT, snake_title);
            drawRect(49, 99, 10, 44, GREY);
            drawString(50, 100, "<SNAKE>" , WHITE);
            drawRect(138, 45, 11, 140, BLACK);
            drawRect(99, 128, 10, 72, GREY);
            drawRect(100, 110, 10, 90, GREY);
            char titl_scr[10];
            sprintf(titl_scr, "BEST SCORE: %3d", score);
            drawString(100, 110, titl_scr, YELLOW);
            prev_state = state;
            state = WAIT;
            break;
        case WAIT:
            if((prev_buttons != BUTTONS) && KEY_DOWN_NOW(BUTTON_A)) {
                if (prev_state == START) {
                    state = STATE1;

                }
                else if (prev_state == STATE1) {
                    state = STATE2;

                }
                else if (prev_state == STATE2) {
                    state = STATE3;
                }
                else if (prev_state == STATE3) {
                    state = START;
                }
            } else {
                if (prev_state == START) {

                    if (0 == timer % 200) {
                        drawRect(138, 45, 11, 140, BLACK);
                    }
                    else if (0 == timer % 100) {
                        drawString(140, 50, "-- Press A to START --", YELLOW);
                    }
                }
                else if (prev_state == STATE1) {

                    bool change = false;
                    bool init = true;
                    if (KEY_DOWN_NOW(BUTTON_DOWN)) {
                        if (count <= 3500) {
                            count += 100;
                            change = true;
                        }


                    } else if (KEY_DOWN_NOW(BUTTON_UP)) {
                        if (count >= 1500) {
                            count -= 100;
                            change = true;
                        }
                    }
                    if (change || init) {
                        drawProgressBar(2000, 2000 - (count - 1500), WHITE, BLACK, 10, 90, 60, 70);
                    }
                    init = false;

                }
                else if (prev_state == STATE2) {

                }
                if ((prev_state != START) && KEY_DOWN_NOW(BUTTON_SELECT)) {
                    state = START;
                }
            }

            break;


        case STATE1:

            drawRect(0, 0, 160, 240, GREY);
            drawString(40, 70, " Choose speed", BLACK);
            drawString(50, 70, "SLOW v   ^ FAST", BLACK);
            drawString(100, 90, "^/v to adjust.", BLUE);

            drawString(110, 60, "Press A to continue...", YELLOW);
            prev_state = state;
            state = WAIT;
            break;

        case STATE2:
//            drawRect(0, 0, 160, 240, GREEN);
            fillScreen(BLACK);
            drawString(20, 60, "-------HELP-------", WHITE);
            drawString(40, 0, "Press SELECT to return to title anytime.", WHITE);
            drawString(60, 70, "Eat up the blue dots", BLUE);
            drawString(70, 90, "to earn points.", BLUE);
            drawString(90, 70, "Avoid Red dots.", RED);
            drawString(100, 50, "They change positions too!", RED);
            drawString(110, 10, "Warning will show up on right", RED);
            drawString(120, 50, "before the change!", RED);


            drawString(130, 20, "Don't hit the bounds or body, too!", ORANGE);
            drawString(150, 70, "Press A to continue...", YELLOW);
            prev_state = state;
            state = WAIT;
            break;

        case STATE3:
            ;
            int recent_score = game();
            if (recent_score < 0) {
                state = START;
                break;
            }
            drawImage3(0, 0, END_THEME_WIDTH, END_THEME_HEIGHT, end_theme);
            drawString(20, 80, "GAME OVER" , WHITE);
            char urscr[10];
            sprintf(urscr, "SCORE: %3d", recent_score);
            drawString(40, 88, urscr, YELLOW);
            if (recent_score > score) {
                int temp = score;
                score = recent_score;
                second_score = temp;
            } else if (recent_score > second_score) {
                int temp = second_score;
                second_score = recent_score;
                third_score = temp;
            } else if (recent_score > third_score) {
                third_score = recent_score;
            }
            drawString(70, 120, "LEADERBOARD", WHITE);
            char sc1[10];
            char sc2[10];
            char sc3[10];
            sprintf(sc1, "1st: %3d", score);
            sprintf(sc2, "2nd: %3d", second_score);
            sprintf(sc3, "3rd: %3d", third_score);
            drawString(100, 142, sc3, YELLOW);
            drawString(90, 142, sc2, YELLOW);
            drawString(80, 142, sc1, YELLOW);
            drawRect(59, 9, 66, 98, YELLOW);
            drawImage3(60, 10, SNAKE_DEAD_WIDTH, SNAKE_DEAD_HEIGHT, snake_dead);
            drawString(140, 50, "Press A to go to title.", YELLOW);
            prev_state = state;
            state = WAIT;
            break;



        }
        timer++;
        prev_buttons = BUTTONS;
    }

    return 0;
}






/** game
 * plays game
 * @return int the recent score. -1 if SELECT is pressed to return to title page
 */
int game()
{
    /*pixel# = row * row_length + col*/
    int snake_length = 0;
    int cell_size = 10;
    int tot_row = (160 / cell_size - 1);
    int tot_col = (200 / cell_size - 1);


    REG_DISPCTL = MODE3 | BG2_ENABLE;




    int row = rand() % (tot_row + 1);
    int col = rand() % (tot_col + 1);
    int rd = 0;
    int cd = 0;


    int oldrow = 0;
    int oldcol = 0;
    int width = 8;
    int height = 8;
    int oldheight = height;
    int oldwidth = width;


    int my_score = 0;


    enum cellState states[tot_row + 1][tot_col + 1];
    for (int i = 0; i < tot_row + 1; i++) {
        for (int j = 0; j < tot_col + 1; j++) {
            states[i][j] = EMPTY;
        }
     }
     states[row][col] = SNAKE;








    snakeBlock snakebody[(tot_row + 1) * (tot_col + 1) - 1];
    for (int i = 0; i < ((tot_row + 1) * (tot_col + 1)); i++) {
        snakebody[i].active = false;
    }






    REG_DMA3SAD = (u32)&bgcolor;
    REG_DMA3DAD = (u32)videoBuffer;
    REG_DMA3CNT = 38400 | DMA_ON | DMA_SOURCE_FIXED;










    unsigned int prev_button = BUTTONS;
    initializeBoard(tot_row, tot_col, cell_size);
    volatile int f = 0;
    bool nofood = true;
    bool nopoisons = true;
    int food_row = 0;
    int food_col = 0;
    drawImage3(0, 200, SIDE_BAR_WIDTH, SIDE_BAR_HEIGHT, side_bar);
    char score_txt[30];
    sprintf(score_txt, "PT:%3d", my_score);
    drawString(40, 200, score_txt, WHITE);
    drawRect(68, 200, 20, 40, YELLOW);
    while(1) {

        if (nofood) {
            int fp = rand() % ((tot_row + 1) * (tot_col + 1));
            food_row = fp % (tot_row + 1);
            food_col = (fp / (tot_row + 1)) % (tot_col + 1);
            while (states[food_row][food_col] != EMPTY) {
                fp++;
                fp = fp % ((tot_row + 1) * (tot_col + 1));
                food_row = fp % (tot_row + 1);
                food_col = (fp / (tot_row + 1)) % (tot_col + 1);
            }


            states[food_row][food_col] = FOOD;
            drawBox(food_row, food_col, BLUE, cell_size);
            nofood = false;
        }

        if (nopoisons) {
            int num_poision = 1 + rand() % 5;
            for (int i = 0; i < num_poision; i++) {
                int fp = rand() % ((tot_row + 1) * (tot_col + 1));
                int poison_row = fp % (tot_row + 1);
                int poison_col = (fp / (tot_row + 1)) % (tot_col + 1);
                while ((states[poison_row][poison_col] != EMPTY) && (poison_row != rd + row) && (poison_col != cd + row)) {
                    fp++;
                    fp = fp % ((tot_row + 1) * (tot_col + 1));
                    poison_row = fp % (tot_row + 1);
                    poison_col = (fp / (tot_row + 1)) % (tot_col + 1);
                }
                states[poison_row][poison_col] = POISON;
                drawBox(poison_row, poison_col, RED, cell_size);
            }

            nopoisons = false;

        }








        if(KEY_DOWN_NOW(BUTTON_UP) && (rd != 1)) {

            prev_button = BUTTON_UP;

            cd = 0;
            rd = -1;

        }
        else if(KEY_DOWN_NOW(BUTTON_DOWN) && (rd != -1)) {

            prev_button = BUTTON_DOWN;

            cd = 0;
            rd = 1;


        }
        else if(KEY_DOWN_NOW(BUTTON_RIGHT) && (cd != -1)) {
            // cd = 8;
            prev_button = BUTTON_R;

            rd = 0;
            cd = 1;



        }
        else if(KEY_DOWN_NOW(BUTTON_LEFT) && (cd != 1)) {
            prev_button = BUTTON_L;

            rd = 0;
            cd = -1;



        }

        else if(KEY_DOWN_NOW(BUTTON_SELECT)) {
            return -1;
        }
        else {
            prev_button = BUTTONS;

        }

        if (f % (count) == 0) {



            row += rd;
            col += cd;
            if(row < 0) {
                return my_score;
            }
            if(row > tot_row) {
                return my_score;
            }
            if(col < 0) {
                return my_score;
            }
            if(col > tot_col) {
                return my_score;
            }
            waitForVBlank();







            if ((states[row][col] == SNAKE) && ((cd != 0) || (rd != 0))) {

                return my_score;
            }
            else if (states[row][col] == FOOD) {

                nofood = true;
                my_score++;
                snake_length++;
                snakebody[snake_length - 1].active = true;
                drawRect(40, 200, 10, 40, bgcolor);
                char score_txt[30];
                sprintf(score_txt, "PT:%3d", my_score);
                drawString(40, 200, score_txt, WHITE);

            }
            else if (states[row][col] == POISON) {
                return my_score;
            }
            states[row][col] = SNAKE;


            if (snake_length != 0) {
                newBody(row, col, cell_size);
                states[snakebody[snake_length - 1].row][snakebody[snake_length - 1].col] = EMPTY;
            }

            update_body(snakebody, snake_length, oldrow, oldcol, cell_size);

            if (snake_length == 0) {
                goToNewRow(oldrow, oldcol, row, col, cell_size);
                states[oldrow][oldcol] = EMPTY;
            }

            if (f % (count * 100) == (count * 90)) {
                drawString(70, 200, "CHANGE", RED);
                drawString(80, 200, "SOON  ", RED);
                drawImage3(90, 200, DANGER_WIDTH, DANGER_HEIGHT, danger);
            }

            if ((f != 0) && ((f) % (count * 100) == 0)) {
                for (int i = 0; i < tot_row + 1; i++) {
                    for (int j = 0; j < tot_col + 1; j++) {
                        if (states[i][j] == POISON) {
                            states[i][j] = EMPTY;
                            drawBox(i, j, DARK_G, cell_size);
                        }
                    }
                }
                drawRect(68, 200, 20, 40, YELLOW);
                drawRect(90, 200, DANGER_HEIGHT, DANGER_WIDTH, GREY);
                nopoisons = true;
            }




            oldrow = row;
            oldcol = col;
            oldheight = height;
            oldwidth = width;


            BUTTONS = 0;

        }
        f++;
    }
    return my_score;



}



/** update_body
 * the function to update snake position and its blocks.
 * @param snakebody the array of snakeblock
 * @param snake_length the length of the snake
 * @param head_row the row of head
 * @param head_col the column of head
 * @param cell_size the size of a cell block
 */
void update_body(snakeBlock* snakebody, int snake_length, int head_row, int head_col, int cell_size) {
    int old_row;
    int old_col;
    for (int i  = snake_length - 1; 0 < i; i--) {
        snakeBlock *cur = snakebody + i;
        snakeBlock *prev = snakebody + i - 1;
        old_row = snakebody[i].row;
        old_col = snakebody[i].col;
        cur -> row = prev -> row;
        cur -> col = prev -> col;
        if (i == snake_length - 1) {
            goToNewRow(old_row, old_col, cur-> row, cur -> col, cell_size);

        }



    }
    if (snake_length > 0) {
        int temp_1 = snakebody[0].row;
        int temp_2 = snakebody[0].col;

        snakebody[0].row = head_row;
        snakebody[0].col = head_col;
        if (snake_length == 1) {

            goToNewRow(temp_1, temp_2, snakebody[0].row, snakebody[0].col, cell_size);
        }

    }
}

/** goToNewRow
 * when sending a snakeblock graphic to new position and erasing old one.
 * @param oldrow old row
 * @param oldcol old column
 * @param row new row
 * @param col new column
 * @param cell_size size of the grid cell
 */
void goToNewRow(int oldrow, int oldcol, int row, int col, int cell_size) {
    drawBox(oldrow, oldcol, cell_color, cell_size);
    drawRect(row * cell_size, col * cell_size, cell_size, cell_size, YELLOW);
    drawRect(row * cell_size + 1, col * cell_size + 1, cell_size - 2, cell_size - 2, GREEN);

}


/** newBody
 * generating snakeblock graphic without erasing old one.
 * @param row row of the new position
 * @param col column of the new position
 * @param cell_size the size of the grid cell
 *
 */
void newBody(int row, int col, int cell_size) {
    drawRect(row * cell_size, col * cell_size, cell_size, cell_size, YELLOW);
    drawRect(row * cell_size + 1, col * cell_size + 1, cell_size - 2, cell_size - 2, GREEN);

}


/** drawBox
 * generating a grid cell
 * @param row row of the cell
 * @param col column of the cell
 * @param color color of the cell
 * @param cell_size size of each cell
 *
 */
void drawBox(int row, int col, unsigned short color, int cell_size) {
    drawRect(row * cell_size, col * cell_size, cell_size, cell_size, bgcolor);
    drawRect(row * cell_size + 1, col * cell_size + 1, cell_size - 2, cell_size - 2, color);
}

/** initializeBoard
 * to generate empty grid
 * @param tot_row the highest row number index
 * @param tot_col the highest column number index
 * @param cell_size the size of the cell
 */
void initializeBoard(int tot_row, int tot_col, int cell_size) {
    for(int i = 0; i < tot_row + 1; i++) {
        for (int j = 0; j < tot_col + 1; j++) {
            drawBox(i, j, cell_color, cell_size);
        }
    }
}

/** fillScreen
 * to fill screen with one color in mode3 DMA
 * @param color u16 color to fill screen with
 */
void fillScreen(u16 color) {

    u16 color_cpy = color;
    DMA[3].cnt = 0;
    DMA[3].dst = 0;
    DMA[3].src = 0;
    DMA[3].src = (void *)&color_cpy;
    DMA[3].dst = (void *)(videoBuffer);
    DMA[3].cnt = 38400 | DMA_ON | DMA_SOURCE_FIXED;
}

/** drawProgressBar
 * draws progress bar
 * @param total the total value
 * @param processed the value that we are interested in
 * @param background background color of the progress bar
 * @param barcolor the color of the moving bar
 * @param h height of bar
 * @param w width of bar
 * @param row row of the bar (position)
 * @param col column of the bar (position)
 */
void drawProgressBar(int total, int processed, u16 background, u16 barcolor, int h, int w, int row, int col) {
    if (((w * processed / total) < w) && ((w * processed / total) > 0)) {
        drawRect(row, col, h, w * processed / total, background);
        drawRect(row, col + (w * processed / total), h, w - (w * processed / total), barcolor);

    } else if ((w * processed / total) >= w) {
        drawRect(row, col, h, w, background);
    } else if ((w * processed / total) <= 0) {
        drawRect(row, col, h, w, barcolor);
    }


}




