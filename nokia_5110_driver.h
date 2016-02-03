#ifndef NOKIA_5110_DRIVER_H
#define NOKIA_5110_DRIVER_H

/* Hardware Configuration */
#define LCD_SPI_CHANNEL     1
#define LCD_RESET           8
#define LCD_DC              9

/* Configuration Commands */
#define SET_VOP             0xB0
#define SET_TC              0x04
#define SET_BS              0x14

/* Timing */
#define LCD_SPI_SPEED       1000000
#define LCD_RESET_MS        5

/* Usage Commands */
#define SEL_EXT_INSTR_SET   0x21
#define SEL_BAS_INSTR_SET   0x20

#define SEL_NORMAL_MODE     0x0C
#define SEL_INVERT_MODE     0x0D

/* Bitmap structure */
#define LCD_X 84
#define LCD_Y 6

typedef struct Bitmap {
    unsigned char banks[LCD_Y][LCD_X];
} Bitmap;

#endif