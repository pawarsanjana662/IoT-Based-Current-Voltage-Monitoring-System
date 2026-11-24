// Three Phase Voltage and Current Monitoring System
// MikroC PRO for PIC
// NOTE: Confirm the exact PIC MCU and hardware calibration before use.

// LCD module connections
sbit LCD_RS at RD2_bit;
sbit LCD_EN at RD3_bit;
sbit LCD_D4 at RD4_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D7 at RD7_bit;
sbit LCD_RS_Direction at TRISD2_bit;
sbit LCD_EN_Direction at TRISD3_bit;
sbit LCD_D4_Direction at TRISD4_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D7_Direction at TRISD7_bit;

long varset=0;
long int tcount=0;
long int setcount=50;
int menucnt=0;
int vr=0;
int vy=0;
int vb=0;
int tp=0;
int vib=0;
float Ampsr;
float Ampsy;
float Ampsb;
double adcVoltage = 0;
double currentValuer = 0;
double currentValuey = 0;
double currentValueb = 0;
int adcvalue1=0;
int adcvalue2=0;
int adcvalue3=0;
int sensitivity = 100;
long offsetVoltage = 2500;
double Voltage = 0;
double VRMS = 0;
double AmpsRMS1 = 0;
double AmpsRMS =0;
int mVperAmp = 100; // use 100 for 20A Module and 66 for 30A
#define VREF 5.0
#define ADC_RESOLUTION 1023.0
#define ACS_OFFSET 512
#define SENSITIVITY 100
float peakVoltage;
unsigned int adcValue;
float voltir;
float voltiy;
float voltib;
float currentval_r;
float currentval_y;
float currentval_b;
//float current;
unsigned int maxVal = 0, minVal = 1023;
float rmsVoltage;
float current;
char buffer[16];
char cnt1[]="val:00000";
char tset[]="val:0000.0";
char fset[]="val:00000";
char mset[]="val:0F000";
char cnt2[]="val:00000";       //currenttempreading
long heat=50;
int x;
int i;
int ux=0;
int lx=0;

void digitalcur(void){
    tset[5] = varset/10000 + 48;
    tset[6] = (varset/100)%10 + 48;
    tset[7] = (varset/10)%10 + 48;
    tset[9] = varset%10 + 48;
    Lcd_Out(2,1,"                ");
    Lcd_Out(2,1,tset);
}

void digitalconv(void){
    cnt1[4] = varset/10000 + 48;
    cnt1[5] = (varset/1000)%10 + 48;
    cnt1[6] = (varset/100)%10 + 48;
    cnt1[7] = (varset/10)%10 + 48;
    cnt1[8] = varset%10 + 48;
    Lcd_Out(2,1,"                ");
    Lcd_Out(2,1,cnt1);
}

void measure(void){
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,1,"voltage-r");
    vr=ADC_Read(0)*0.37;
    varset=vr;
    digitalconv();
    Delay_ms(2000);

    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,1,"voltage-y");
    vy=ADC_Read(1)*0.37;
    varset=vy;
    digitalconv();
    Delay_ms(2000);

    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,1,"voltage-b");
    vb=ADC_Read(2)*0.37;
    varset=vb;
    digitalconv();
    Delay_ms(2000);
}

void newcurrentm(void){
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,1,"current-r");
    Ampsr=0;

    for (i=0; i<20;i++){
        adcValue=0;
        adcValue=ADC_Read(6);
        Voltage = adcValue*4.8828;
        if (Voltage>=2500){
            Ampsr += ((Voltage-2500)/10);
        }
        if (Voltage<=2500){
            Ampsr += ((2500-Voltage)/10);
        }
    }
    Ampsr/=20;
    varset=Ampsr;
    digitalcur();
    Delay_ms(2000);

    Ampsy=0;
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,1,"current-y");

    for (i=0; i<20;i++){
        adcValue=0;
        adcValue=ADC_Read(5);
        Voltage = adcValue*4.8828;
        if (Voltage>=2500) {
            Ampsy += ((Voltage-2500)/10);
        }
        if (Voltage<=2500){
            Ampsy += ((2500-Voltage)/10);
        }
    }
    Ampsy/=20;
    varset=Ampsy;
    digitalcur();
    Delay_ms(2000);

    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,1,"current-b");
    Ampsb=0;

    for (i=0; i<20;i++){
        adcValue=0;
        adcValue=ADC_Read(4);
        Voltage = adcValue*4.8828;
        if (Voltage>=2500){
            Ampsb += ((Voltage-2500)/10);
        }
        if (Voltage<=2500){
            Ampsb += ((2500-Voltage)/10);
        }
    }
    Ampsb/=20;
    varset=Ampsb;
    digitalcur();
    Delay_ms(2000);
}

void check1(void){
    if(vr > 260){
        PORTB.F0 = 1;
        UART1_Write_Text("1");
        Delay_ms(3000);
    }
    if(vy > 260){
        PORTB.F0 = 1;
        UART1_Write_Text("2");
        Delay_ms(3000);
    }
    if(vb > 260){
        PORTB.F0 = 1;
        UART1_Write_Text("3");
        Delay_ms(3000);
    }

    if(vr < 210){
        PORTB.F0 = 1;
        UART1_Write_Text("4");
        Delay_ms(3000);
    }
    if(vy < 210){
        PORTB.F0 = 1;
        UART1_Write_Text("5");
        Delay_ms(3000);
    }
    if(vb < 210){
        PORTB.F0 = 1;
        UART1_Write_Text("6");
        Delay_ms(3000);
    }

    if(Ampsr > 1){
        PORTB.F0 = 1;
        UART1_Write_Text("7");
        Delay_ms(3000);
    }
    if(Ampsy > 1){
        PORTB.F0 = 1;
        UART1_Write_Text("8");
        Delay_ms(3000);
    }
    if(Ampsb > 1){
        PORTB.F0 = 1;
        UART1_Write_Text("9");
        Delay_ms(3000);
    }
}

void main() {
    TRISD = 0x00;
    PORTD = 0xFF;
    TRISA = 0xFF;
    TRISB = 0x00;
    PORTB = 0x00;
    TRISD = 0x00;
    TRISC = 0xFF;
    UART1_Init(9600);

    Lcd_Init();
    ADC_Init();

    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    Lcd_Out(1,1," 3 ph---monitor ");
    Delay_ms(3000);
    Lcd_Cmd(_LCD_CLEAR);

    while(1){
        measure();
        newcurrentm();
        check1();
    }
}
