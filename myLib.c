#include "myLib.h"
// the library file

unsigned short *videoBuffer = (unsigned short *)0x6000000;


/** setPixel
 * to set an individual pixel
 * @param row row to draw a pixel
 * @param col column to draw a pixel
 * @param color color of the pixel
 */
void setPixel(int row, int col, u16 color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

/** drawRect
 * draw a rectangle with DMA3
 * @param row row to draw the rectangle
 * @param col column to draw the rectangle
 * @param height height of the rectangle
 * @param width width of the rectangle
 * @param color the color of the box
 */
void drawRect(int row, int col, int height, int width, u16 color)
{
    REG_DMA3CNT = 0;
	for(int r=0; r<height; r++)
	{
		REG_DMA3SAD = (u32)&color;
		REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row+r, col, 240)]);
		REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}



/** drawImage3
 * draw image with DMA3
 * @param row row to draw the image
 * @param col column to draw the image
 * @param width width of the image
 * @param height height of the image
 * @param *image pointer to the image data
 */
void drawImage3(int row, int col, int width, int height, const unsigned short *image) {
    REG_DMA3CNT = 0;
    for (int r = 0; r < height; r++) {
            REG_DMA3SAD = (u32)&image[OFFSET(r, 0, width)];
            REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(r + row, 0 + col, 240)]);
            REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_INCREMENT;
    }




}



/** waitForVBlank
 * wait for good time considering vblank to mess with screen.
 */
void waitForVBlank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}




