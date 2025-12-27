//gsm.c
#include <string.h>
#include "uart0.h"
#include "delay.h"
#include "lcd.h"


extern char buff[25],i,ch;

void GSM_init(void)
{
        LCD_cmd(0x80);
        LCD_str("GSM init....");
        i=0;memset(buff,'\0',25);
        UART0_Str("AT\r\n");
        while(i<2);
        delay_ms(500);
        buff[i] = '\0';
        if(strstr(buff,"OK"))
        {
                LCD_cmd(0xC0);
                LCD_str("OK");
                delay_ms(500);
                LCD_cmd(0xC0);
                LCD_str("  ");
                i=0;memset(buff,'\0',25);
                UART0_Str("ATE0\r\n");
                while(i<2);
                delay_ms(500);
                buff[i] = '\0';
                if(strstr(buff,"OK"))
                {
                        LCD_cmd(0xC0);
                        LCD_str("OK");
                        delay_ms(500);
                        LCD_cmd(0xC0);
                        LCD_str("  ");
                        i=0;memset(buff,'\0',25);
                        UART0_Str("AT+CMGF=1\r\n");
                        while(i<2);
                        delay_ms(500);
                        buff[i] = '\0';
                        if(strstr(buff,"OK"))
                        {
                                LCD_cmd(0xC0);
                                LCD_str("OK");
                                delay_ms(500);
                                LCD_cmd(0xC0);
                                LCD_str("  ");
                                delay_ms(500);
                                LCD_cmd(0x01);
                                LCD_str("GSM OK");
                                delay_ms(1000);
                        }
                        else
                        {
                                LCD_cmd(0xC0);
                                LCD_str("ERROR");
                                delay_ms(500);
                                LCD_cmd(0xC0);
                                LCD_str("     ");
                        }
                }
                else
                {
                                LCD_cmd(0xC0);
                                LCD_str("ERROR");
                                delay_ms(500);
                                LCD_cmd(0xC0);
                                LCD_str("     ");
                }
        }
        else
        {
                LCD_cmd(0xC0);
                LCD_str("ERROR");
                delay_ms(500);
                LCD_cmd(0xC0);
                LCD_str("     ");
        }

        LCD_cmd(0x01);
}

void send_sms(char *num,char *msg)
{
        LCD_cmd(0x01);
        LCD_str("Sending sms....");
        delay_ms(500);
        i=0;memset(buff,'\0',25);
        UART0_Str("AT+CMGS=");
        UART0_Tx('"');
        UART0_Str(num);
        UART0_Tx('"');
        UART0_Str("\r\n");

        delay_ms(500);
        i=0;
        UART0_Str(msg);
    UART0_Tx(0x1A);
        delay_ms(5000);
        while(i<3);
        buff[i] = '\0';
        if(strstr(buff,"OK"))
        {
                LCD_cmd(0x01);
                LCD_str("Message Sent..");
                delay_ms(500);
        }
        else
        {
                LCD_cmd(0x01);
                LCD_str("Message failed..");
                delay_ms(500);
        }
        LCD_cmd(0x01);
}
~