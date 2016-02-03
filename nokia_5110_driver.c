#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "nokia_5110_driver.h"

void lcdSetup(void)
{
    /* configure the spi, reset, and data/command pins */
    wiringPiSPISetup(LCD_SPI_CHANNEL, LCD_SPI_SPEED);
    pinMode(LCD_RESET, OUTPUT);
    pinMode(LCD_DC, OUTPUT);
    
    /* cycle the reset pin to reset the display */
    digitalWrite(LCD_RESET, LOW);
    delay(LCD_RESET_MS);
    digitalWrite(LCD_RESET, HIGH);
    
    /* use the extended instruction set to configure Vop, TC, and BS */
    lcdWriteCommand(SEL_EXT_INSTR_SET);
    lcdWriteCommand(SET_VOP);
    lcdWriteCommand(SET_TC);
    lcdWriteCommand(SET_BS);
    
    /* return to basic instruction set and select normal mode */
    lcdWriteCommand(SEL_BAS_INSTR_SET);
    lcdWriteCommand(SEL_NORMAL_MODE);
    
    lcdDemo();
}

void lcdDemo(void)
{
    lcdClearScreen();
    lcdDisplayString(0, 0, "Hello, World!");
    delay(10);
    lcdDisplayString(0, 1, "This is a demo");
    delay(10);
    lcdDisplayString(0, 2, "Will this be truncated?");
    delay(10);
    lcdDisplayString(0, 3, "This is fun");
    delay(10);
    lcdDisplayString(0, 4, "Still testing");
    delay(10);
    lcdDisplayString(0, 5, "And done!");
}

void lcdDisplayBitmap(Bitmap *bitmap)
{
    int i, j;
    /* start at display beginning */
    lcdSetCursor(0,0);
    
    /* display all the banks */
    for (i = 0; i < LCD_Y; i++)
        for (j = 0; j < LCD_X; j++)
            lcdWriteData(*bitmap->banks[i][j]);
}

void lcdClearScreen(void)
{
    int i;
    /* start at display beginning */
    lcdSetCursor(0,0);
    /* write zeros to entire data buffer */
    for (i = 0; i < (LCD_X * LCD_Y)/8; i++)
        lcdWriteData(0);
}

int lcdSetCursor(unsigned int x, unsigned int y)
{
    /* return error for invalid cursor position */
    if(x > LCD_X || y > LCD_Y)
        return -1;
    
    /* set the x and y positions */
    lcdWriteCommand(SET_Y | y);
    lcdWriteCommand(SET_X | x);
    return 0;
}

int lcdDisplayChar(unsigned int x, unsigned int y, unsigned char character)
{
    /* set the cursor position, checking for errors */
    if(lcdSetCursor(x, y) == -1)
        return -1;

    /* display question marks in place of invalid characters */
    if(character < FONT_TABLE_START || character > FONT_TABLE_END)
        character = '?';
    
    int i;
    /* get bitmap for character and display it */
    for(i = 0; i < FONT_WIDTH; i++)
        lcdWriteData(fontTable[character - FONT_TABLE_START][i]);
    
    /* end with a blank vertical buffer between characters */
    lcdWriteData(0);
}

int lcdDisplayString(unsigned int x, unsigned int y, unsigned char *string)
{
    /* go until null termination reached */
    while(*string)
    {
        /* and display each character, checking for errors */
        if(lcdDisplayChar(x++, y, *(string++) == -1)
            return -1;
    }
    
    /* if we got to this point, no errors */
    return 0;
}

void lcdWriteCommand(unsigned char command)
{
    /* set DC pin low and write the command */
    digitalWrite(LCD_DC, LOW);
    wiringPiSPIDataRW(LCD_SPI_CHANNEL, &command, 1);
}

void lcdWriteData(unsigned char data)
{
    /* set DC pin high and write the data */
    digitalWrite(LCD_DR, HIGH);
    wiringPiSPIDataRW(LCD_SPI_CHANNEL, &data, 1);
}