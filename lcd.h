//lcd.h
#ifndef _LCD_H_
#define _LCD_H_

void LCD_Init(void);
void LCD_cmd(char);
void LCD_char(char);
void LCD_str(char *);
void F32LCD(float);
void U32LCD(unsigned int);
void Store_CGRAM(void);
void USR_INPUT(char *);

#endif