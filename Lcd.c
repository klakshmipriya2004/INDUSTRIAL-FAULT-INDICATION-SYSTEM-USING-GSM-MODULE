//lcd.c
#include <LPC214x.H>

#include "defines.h"
#include "delay.h"
#include "lcd.h"
#include "types.h"

//p0.8 - p0.15 pins are connected to lcd data pins
#define LCD_DATA 8
//p0.16 is connected to RS of LCD
#define RS 17
//p0.17 is connected to EN of LCD
#define EN 18

char CGRAM_LUT[] = {0x17,0x14,0x14,0x1f,0x05,0x05,0x1d,0x00};


void LCD_Init(void)
{
        WRITEBYTE(IODIR0,LCD_DATA,0XFF);
        WRITEBIT(IODIR0,RS,1);
        WRITEBIT(IODIR0,EN,1);

        delay_ms(16);
        LCD_cmd(0x30);
        delay_ms(5);
        LCD_cmd(0x30);
        delay_ms(1);
        LCD_cmd(0x30);
        delay_ms(1);
        LCD_cmd(0x38);//
        LCD_cmd(0x10);//
        LCD_cmd(0x01);//
        LCD_cmd(0x06);//
        LCD_cmd(0x0F);//0X0E,0X0C

}
void U32LCD(u32 n)
{
        u8 a[10]={0};
        s16 i=0;

        if(n==0)
                LCD_char('0');
        else
        {
                while(n>0)
                {
                 a[i]=(n%10)+48;
                 n=n/10;
                 i++;
                }
    for(--i;i>=0;i--)
        LCD_char(a[i]);
  }
}
void LCD_cmd(char cmd)
{
        WRITEBYTE(IOPIN0,LCD_DATA,cmd);
        WRITEBIT(IOCLR0,RS,1);
        WRITEBIT(IOSET0,EN,1);
        delay_us(1);
        WRITEBIT(IOCLR0,EN,1);
        delay_ms(2);
}

void LCD_char(char data)
{
        WRITEBYTE(IOPIN0,LCD_DATA,data);
        WRITEBIT(IOSET0,RS,1);
        WRITEBIT(IOSET0,EN,1);
        WRITEBIT(IOCLR0,EN,1);
        delay_ms(1);
}

void LCD_str(char *str)
{
        while(*str != '\0')
        {
                LCD_char(*str);
                str++;
        }
}

void Store_CGRAM(void)
{
        int i;
        LCD_cmd(0x40);
        for(i=0;i<8;i++)
                LCD_char(CGRAM_LUT[i]);
}
void F32LCD(float f)
{
        s32 i; s8 j;
        i=f;
        U32LCD(i);
        LCD_char('.');
        for(j=0;j<6;j++)
        {
                f=(f-i)*10;
                i=f;
                LCD_char(i+48);
        }
}