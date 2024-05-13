#include<LPC17xx.h>
#include "q2.c"
void scan(void);
unsigned char row,var,flag,key,key2[2]={'0','\0'};
unsigned char msg4[10]="DRINK UP!";
unsigned long int i,var1,temp,temp1,temp2,temp3;
unsigned char SCAN_CODE[16] = {0X11,0X21,0X41,0X81,0X12,0X22,0X42,0X82,0X14,0X24,0X44,0X84,0X18,0X28,0X48,0X88};
unsigned char ASCII_CODE[10] = {'0','1','2','3','4','5','6','7','8','9'};
int ind = 3,ans = 0,a=0,b=0,c=0,count=0,ml,temp4;
unsigned char asciians[5] = {'0','0','0','0','\0'};
unsigned char op ='+';
int ak;
void InitTimer0(void){
	LPC_TIM0->CTCR=0;
	LPC_TIM0->TCR=0X2;
	LPC_TIM0->PR=2;
	LPC_TIM0->MR0=999999;
	LPC_TIM0->MCR=2;
	LPC_TIM0->EMR=0X20;
	LPC_TIM0->TCR=1;
}

void delay(void){
	InitTimer0();
	while(!(LPC_TIM0->EMR & 1));
}


void buzz_init() { 
	LPC_PINCON->PINSEL0 &= ~(3 << 22);
	LPC_GPIO0->FIODIR |= 0x00000800;
	LPC_GPIO0->FIOCLR = 0x00000800;
}

int main()
{
	SystemInit();
	SystemCoreClockUpdate();
	buzz_init();
	
	LPC_GPIO2->FIODIR |= 0XF<<10;
	LPC_GPIO1->FIODIR &= 0XF87FFFFF;
	lcd_init();
	lcd_comdata(0X80,0);
	delay_lcd(800);
	while(count<3)
	{
		while(1)
		{
			for(row = 1;row<5;row++)
			{
				if(row == 1)
				{
					var1 = 0x400;
				}
				else if(row == 2)
				{
					var1 = 0x800;
				}
				else if(row == 3)
				{
					var1 = 0x1000;
				}
				else if(row == 4)
				{
					var1 = 0x2000;
				}
				temp = var1;
				LPC_GPIO2->FIOCLR = 0X3C00;
				LPC_GPIO2->FIOSET = var1;
				flag = 0;
				scan();
				if(flag == 1)
				{
					count++;
					break;
				}
			}
			if(flag == 1)
			{
				break;
			}
		}
		for(i=0;i<16;i++)
		{
			if(key == SCAN_CODE[i])
			{
				key = ASCII_CODE[i];
				key2[0]=ASCII_CODE[i];
				key2[1]='\0';
				lcd_puts(key2);
				delay_lcd(100000);
				if(count == 1)
				{
					a = key-48;
				}
				else if(count == 2)
				{
					b = key-48;
				}
				else if(count == 3)
				{
					c = key-48;
				}
				break;
			}
		}
	
	}
	lcd_comdata(0XC0,0);
	delay_lcd(10000);
	ml=a*100+b*10+c;
	ans=(ml*0.00305932)*3600;
	temp4=ans;
	while(temp4>0){
	while(ans!=0)
	{
		asciians[ind--] = ans%10 +'0';
		ans /= 10;
	}
	for(i=0;i<100000;i++);
		
	lcd_puts(&asciians[0]);
	
	for(ak=0;ak<4;ak++)
	{
		asciians[ak] = '0';
	}
	asciians[4] = '\0';
	lcd_comdata(0XC0,0);
	
	temp4--;
	delay();
	ans=temp4;
	ind = 3;
	}
	if(temp4==0){
		lcd_comdata(0XC0,0);
		lcd_puts(&msg4[0]);
		LPC_GPIO0->FIOSET = 0x00000800;
	}
	return 0;
}
void scan()
{
	temp3 = LPC_GPIO1->FIOPIN;
	temp3 &= 0xF<<23;
	if(temp3!=0)
	{
		flag = 1;
		temp3 >>= 19;
		var1 >>= 10;
		key = temp3|var1;
	}
}

//q2.c

#include <lpc17xx.h>
void lcd_init(void);
void write(int, int);
void delay_lcd(unsigned int);
void lcd_comdata(int, int);
void clear_ports(void);
void lcd_puts(unsigned char *);
//lcd initialization
void lcd_init()
{
/*Ports initialized as GPIO */
LPC_PINCON->PINSEL1 &= 0xFC003FFF; //P0.23 to P0.28
/*Setting the directions as output */
LPC_GPIO0->FIODIR |= 0x0F<<23 | 1<<27 | 1<<28;
clear_ports();
delay_lcd(3200);
lcd_comdata(0x33, 0);
delay_lcd(30000);
lcd_comdata(0x32, 0);
delay_lcd(30000);
lcd_comdata(0x28, 0); //function set
delay_lcd(30000);
lcd_comdata(0x0c, 0);//display on cursor off
delay_lcd(800);
lcd_comdata(0x06, 0); //entry mode set increment cursor right
delay_lcd(800);
lcd_comdata(0x01, 0); //display clear
delay_lcd(10000);
return;
}
void lcd_comdata(int temp1, int type)
{
int temp2 = temp1 & 0xf0; //move data (26-8+1) times : 26 - HN place, 4 - Bits
temp2 = temp2 << 19; //data lines from 23 to 26
write(temp2, type);
temp2 = temp1 & 0x0f; 
temp2 = temp2 << 23;
write(temp2, type);
delay_lcd(1000);
return;
}
void write(int temp2, int type) //write to command/data reg
{
clear_ports();
LPC_GPIO0->FIOPIN = temp2; // Assign the value to the data lines
if(type==0)
LPC_GPIO0->FIOCLR = 1<<27; // clear bit RS for Command
else
LPC_GPIO0->FIOSET = 1<<27; // set bit RS for Data
LPC_GPIO0->FIOSET = 1<<28; // EN=1
delay_lcd(25);
LPC_GPIO0->FIOCLR = 1<<28; // EN =0
return;
}
void delay_lcd(unsigned int r1)
{
unsigned int r;
for(r=0;r<r1;r++);
return;
}
void clear_ports(void)
{
/* Clearing the lines at power on */
LPC_GPIO0->FIOCLR = 0x0F<<23; //Clearing data lines
LPC_GPIO0->FIOCLR = 1<<27; //Clearing RS line
LPC_GPIO0->FIOCLR = 1<<28; //Clearing Enable line
return;
}
void lcd_puts(unsigned char *buf1)
{
unsigned int i=0;
unsigned int temp3;
while(buf1[i]!='\0')
{
temp3 = buf1[i];
lcd_comdata(temp3, 1);
i++;
if(i==16)
{
lcd_comdata(0xc0, 0);
}
}
return;
}