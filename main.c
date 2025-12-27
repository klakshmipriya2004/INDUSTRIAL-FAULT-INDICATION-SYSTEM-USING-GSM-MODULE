//main.c
#include<lpc214x.h>
#include "lcd.h"
#include "delay.h"
#include "uart0.h"
#include "gsm.h"
#include "i2c.h"
#include "i2c_eeprom.h"
#include "defines.h"
#include "dht11.h"

unsigned char keyscan(void);
unsigned int Readnum(void);
unsigned int colscan(void);
void Backspace(void);

u8 temp_set,humi_set;
f32 temp;
u8 i_flag=0;

void Edit_temp(u8 n)
{
        u32 var;
        LCD_cmd(0x01);
        if(n==1)
        {
                LCD_str("Enter the temp");
                var=Readnum();
                i2c_eeprom_write(0x50,0x00,var);
                temp_set=i2c_eeprom_read(0x50,0x00); // read temperature from EEPROM
                LCD_cmd(0x01);
                LCD_str("Temp updated");
                delay_ms(1000);
                LCD_cmd(0x01);
        }
        else if(n==2)
        {
                LCD_str("Enter the humi");
                var=Readnum();
                i2c_eeprom_write(0x50,0x10,var);
                humi_set=i2c_eeprom_read(0x50,0x10);
                LCD_cmd(0x01);
                LCD_str("Humi updated");
                delay_ms(1000);
                LCD_cmd(0x01);
        }

}

void edit(void)
{
   unsigned char ch;
   while(1)
   {
                LCD_cmd(0x01);
                LCD_str("1.Edit temp");
                LCD_cmd(0xc0);
                LCD_str("2.Edit humi 3.EX ");
                ch=keyscan();
                delay_ms(100);
                while(colscan()==0);
                switch(ch)
                {
                        case '1':Edit_temp(1);
                                break;
                        case '2':Edit_temp(2);
                                break;
                        case '3':LCD_cmd(0x01);
                                        return;
                }
        }
}


void eint0_isr(void) __irq
{
        i_flag=1;
        SSETBIT(EXTINT,0);//clear flag
        VICVectAddr=0;//dummy write;
}

void Enable_EINT0(void)
{
        CFGPIN(PINSEL1,0,FUNC2);
        //CFGPIN(PINSEL0,0,FUNC4);
        //SETBIT(IODIR0,EINT0_LED);
        SSETBIT(VICIntEnable,14);
        VICVectCntl1=0x20|14;
        VICVectAddr1=(unsigned)eint0_isr;

        //Enable EINT 0
        SSETBIT(EXTINT,0); //default

        //EINT0 as EDGE_TRIG
    SETBIT(EXTMODE,0);
        //EINT0 as REDGE
  //SETBIT(EXTPOLAR,0);
}

int main()
{
        unsigned char humidity_integer, humidity_decimal, temp_integer, temp_decimal, checksum;
        u32 j=0;
        Enable_EINT0();
        LCD_Init( );
        LCD_str("I.F.I.S.S.A");
        delay_s(2);
        LCD_cmd(0x01);
        InitUART0();
        init_i2c();
        GSM_init();
        //i2c_eeprom_write(0x50,0x00,35);// write temperature into EEPROM
        //i2c_eeprom_write(0x50,0x10,45);// write Humidity into EEPROM
        temp_set=i2c_eeprom_read(0x50,0x00); // read temperature from EEPROM
        humi_set=i2c_eeprom_read(0x50,0x10);    // read Humidity from EEPROM
        LCD_cmd(0x01);
        U32LCD(temp_set);
        LCD_char(' ');
        U32LCD(humi_set);
        delay_ms(2000);
        while(1)
        {
                do
                {
                        dht11_request();
                        dht11_response();
                        humidity_integer = dht11_data();
                        humidity_decimal = dht11_data();
                        temp_integer = dht11_data();
                        temp_decimal = dht11_data();
                        checksum = dht11_data();
                        if( (humidity_integer + humidity_decimal + temp_integer + temp_decimal) != checksum )
                                LCD_str("Checksum Error\r\n");
                        else
                        {
                                LCD_cmd(0x80);
                                LCD_str("H : ");
                                U32LCD(humidity_integer);
                                LCD_char('.');
                                U32LCD(humidity_decimal);
                                LCD_str(" % RH ");
                                LCD_cmd(0xC0);
                                LCD_str("T : ");
                                U32LCD(temp_integer);
                                LCD_char('.');
                                U32LCD(temp_decimal);
                                LCD_char(223);
                                LCD_str("C");
                                delay_ms(1000);
                                if(j==0)
                                {
                                        if(temp_integer>=temp_set)
                                        {
                                                send_sms("6305141921", "temperature crossed setpoint value");
                                                j++;
                                        }
                                        if(humidity_integer>=humi_set)
                                        {
                                                send_sms("6305141921", "humidity crossed setpoint value");
                                                j++;
                                        }
                                }
                                if(j>=1)
                                {
                                        j++;
                                }
                                if(j==15)
                                j=0;
                        }
                }while(i_flag==0);
                i_flag=0;
                j=0;
                edit();
                        //LCD_str(0x01);
        }
}