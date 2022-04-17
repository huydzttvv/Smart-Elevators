#include "main.h"
// Noi khai bao hang so
#define     LED     PORTD
#define     ON      1
#define     OFF     0

#define     INIT_SYSTEM         0
#define     ENTER_USERNAME      1
#define     CHECK_USERNAME      2
#define     ENTER_PASSWORD      3
#define     CHECK_PASSWORD      4
#define     UNLOCK_DOOR         6
#define     CHOOSE_FUNCTION     7
#define     WRONG_USERNAME      8
#define     WRONG_PASSWORD      9
#define     CHANGE_PASSWORD_STEP1   10
#define     CHANGE_PASSWORD_STEP2   11
#define     MAX_ACCOUNT         10
#define     ADMIN               12
#define     ADDNEWNAME          13
#define     ADDNEWPASS          14
#define     DELETE              15
#define     SLEEP               16
// Noi khai bao bien toan cuc
unsigned char arrayMapOfOutput [8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char statusOutput[8] = {0,0,0,0,0,0,0,0};
//unsigned char indexUser = 0;
// Khai bao cac ham co ban IO
void init_system(void);
void delay_ms(int value);
void OpenOutput(int index);
void CloseOutput(int index);
void TestOutput(void);
void ReverseOutput(int index);
void Test_KeyMatrix();
//Chuong trinh Password Door
unsigned char arrayMapOfNumber [16] = {1,2,3,'A',4,5,6,'B',
                                       7,8,9,'C','*',0,'E','D'};
unsigned char arrayMapOfUsername[MAX_ACCOUNT][4]= {
  {0,0,0,0},
  {1,1,1,1},
  {1,2,3,4},
};
unsigned char arrayMapOfPassword[MAX_ACCOUNT][6]= {
  {1,2,3,4,5,6},
  {1,1,1,1,1,1},
  {3,3,3,3,3,3},
};
unsigned char arrayPassword[6];
unsigned char arrayUsername[4];
unsigned char statusPassword = INIT_SYSTEM;
unsigned char indexOfNumber0 = 0;
unsigned char indexOfNumber1 = 0;
unsigned char indexOfNumber2 = 0;
unsigned char indexOfNumber3 = 0;
unsigned char timeDelay = 0;
unsigned char indexAcc = 0;
unsigned char pre_state = 0;
int admin=0;
unsigned char choose = 0;
unsigned char count=2;
unsigned char del = 0;
unsigned char lcdline1;
unsigned char lcdline1;
void App_PasswordDoor();
unsigned char CheckUsername();
unsigned char CheckPassword();
void AddUser();
unsigned char isButtonNumber();
unsigned char isNumber();
unsigned char numberValue;
unsigned char isButtonEnter();
unsigned char Mode();
void UnlockDoor();
void LockDoor();
void DeleteUser();
void ListUser();
//unsigned char StateForFunction();
//void ChooseFunction();
//void ChangePassword(unsigned char);
#define     LIGHT_ON      1
#define     LIGHT_OFF     0
void BaiTap_Den();
unsigned char isButtonLight();
unsigned char statusLight = LIGHT_OFF;
void LightOn();
void LightOff();
////////////////////////////////////////////////////////////////////
//Hien thuc cac chuong trinh con, ham, module, function duoi cho nay
////////////////////////////////////////////////////////////////////
void main(void)
{
	unsigned int k = 0;
	init_system();
        //TestOutput();
	while (1)
	{
            while (!flag_timer3);
            flag_timer3 = 0;
            // thuc hien cac cong viec duoi day
            scan_key_matrix();
            App_PasswordDoor();
            DisplayLcdScreen();
	}
}
// Hien thuc cac module co ban cua chuong trinh
void delay_ms(int value)
{
	int i,j;
	for(i=0;i<value;i++)
		for(j=0;j<238;j++);
}

void init_system(void)
{
        TRISB = 0x00;		//setup PORTB is output
        TRISD = 0x00;
        init_lcd();
        LcdClearS();
//        LED = 0x00;
	init_interrupt();
        delay_ms(1000);
        init_timer0(4695);//dinh thoi 1ms sai so 1%
        init_timer1(9390);//dinh thoi 2ms
	init_timer3(46950);//dinh thoi 10ms
	SetTimer0_ms(2);
        SetTimer1_ms(10);
	SetTimer3_ms(50); //Chu ky thuc hien viec xu ly input,proccess,output
        init_key_matrix();
}

void OpenOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		LED = LED | arrayMapOfOutput[index];
	}

}

void CloseOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		LED = LED & ~arrayMapOfOutput[index];
	}
}

void ReverseOutput(int index)
{
    if (statusOutput[index]  == ON)
    {
        CloseOutput(index);
        statusOutput[index] = OFF;
    }
    else
    {
        OpenOutput(index);
        statusOutput[index] = ON;
    }
}

void TestOutput(void)
{
	int k;
	for (k=0;k<=7 ;k++ )
	{
		OpenOutput(k);
		delay_ms(500);
		CloseOutput(k);
		delay_ms(500);
	}
}
unsigned char isButtonMotorOn()
{
    if (key_code[2] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButtonMotorOff()
{
    if (key_code[3] == 1)
        return 1;
    else
        return 0;
}

void App_PasswordDoor()
{
    switch (statusPassword)
    {
//        case INIT_SYSTEM:
//            LcdPrintStringS(0,0,"PRESS * FOR PASS");
//            LcdPrintStringS(1,0,"                ");
//            LockDoor();
//            if (isButtonEnter())
//            {
//                indexOfNumber0 = 0;
//                indexOfNumber1 = 0;
//                indexOfNumber2 = 0;
//                indexOfNumber3 = 0;
//                timeDelay = 0;
//                del=100;
//                statusPassword = ENTER_USERNAME;
//                //statusPassword = ENTER_PASSWORD;
//            }
//            break;
        case INIT_SYSTEM:
            timeDelay++;
            LcdPrintStringS(0,0,"PRESS * FOR PASS");
            LcdPrintStringS(1,0,"                ");
            LockDoor();
            if (isButtonEnter())
            {
                indexOfNumber0 = 0;
                indexOfNumber1 = 0;
                indexOfNumber2 = 0;
                indexOfNumber3 = 0;
                timeDelay = 0;
                del=100;
                statusPassword = ENTER_USERNAME;
                //statusPassword = ENTER_PASSWORD;
            }
            if (timeDelay >= 200)
                statusPassword = SLEEP;
            break;
        case SLEEP:
            timeDelay = 0;
            LcdClearS();
            if (isButtonNumber())
            {
                statusPassword = INIT_SYSTEM;
            }
            break;
        case ENTER_USERNAME:
            LcdPrintStringS(0,0,"ENTER USERNAME  ");
            timeDelay ++;
            if (isButtonNumber())
            {
                if (numberValue == 'D'){
                    indexOfNumber0 -= 1;
                    LcdPrintStringS(1,indexOfNumber0, " ");
                }
                else {
                    //LcdPrintStringS(1,indexOfNumber0, "x");
                    LcdPrintNumS(1,indexOfNumber0, numberValue);
                    arrayUsername [indexOfNumber0] = numberValue;
                    indexOfNumber0 ++;
                    timeDelay = 0;
                }
            }
            if (indexOfNumber0 >= 4){
                indexOfNumber0=0;
                statusPassword = CHECK_USERNAME;
            }
              
            if (timeDelay >= 100)
                statusPassword = INIT_SYSTEM;
            if (isButtonEnter())
                statusPassword = INIT_SYSTEM;
            break;
        case CHECK_USERNAME:
            timeDelay = 0;
            if (CheckUsername()){
 
                LcdClearS();
                statusPassword = ENTER_PASSWORD;
            }
            else
                statusPassword = WRONG_USERNAME;
            break;
        case ENTER_PASSWORD:
            LcdPrintStringS(0,0,"ENTER PASSWORD  ");
            timeDelay ++;
            if (isButtonNumber())
            {
                if (numberValue == 'D'){
                    indexOfNumber0 -= 1;
                    LcdPrintStringS(1,indexOfNumber0, " ");
                }
                else {
                    LcdPrintStringS(1,indexOfNumber1,"*");
                    arrayPassword [indexOfNumber1] = numberValue;
                    indexOfNumber1 ++;
                    timeDelay = 0;
                }
            }
            if (indexOfNumber1 >= 6){
                statusPassword = CHECK_PASSWORD;
                timeDelay = 0;
            }
            if (timeDelay >= 100)
                statusPassword = INIT_SYSTEM;
            if (isButtonEnter())
                statusPassword = INIT_SYSTEM;
            break;
        case CHECK_PASSWORD:
            timeDelay++;
            if (CheckPassword()){
                if (Mode()&&admin!=0){
                    LcdClearS();
                    timeDelay = 0;
                    if (pre_state != CHANGE_PASSWORD_STEP1)
                        statusPassword = CHANGE_PASSWORD_STEP1;
                    else statusPassword = CHANGE_PASSWORD_STEP2;
                    pre_state = 0;
                }
            //delay_ms(20);
                if (Mode()&&admin==0){
                    LcdClearS();
                    statusPassword = ADMIN;
                }
                
                if (isButtonEnter() || (timeDelay >= 200))
                    statusPassword = UNLOCK_DOOR;
            }
            else
                statusPassword = WRONG_PASSWORD;
            break;
        case UNLOCK_DOOR:
            timeDelay ++;
            LcdPrintStringS(0,0,"OPENING DOOR    ");
            LcdPrintStringS(1,0,"HAVE A GOOD DAY ");
            UnlockDoor();
            if(Mode()&&admin!=0){
                LcdClearS();
                if (pre_state != CHANGE_PASSWORD_STEP1)
                    statusPassword = CHANGE_PASSWORD_STEP1;
                else statusPassword = CHANGE_PASSWORD_STEP2;
                timeDelay = 0;
                pre_state = 0;
            }
            
            else if (Mode()&&admin==0){
                LcdClearS();
                statusPassword = ADMIN;
                timeDelay = 0;
            }
            if (timeDelay >= 100)
                statusPassword = INIT_SYSTEM;
            
            break;
        case CHANGE_PASSWORD_STEP1:
            //ChangePassword();
            LcdPrintStringS(0,0,"OLD PASSWORD    ");
            timeDelay ++;
            pre_state = CHANGE_PASSWORD_STEP1;
            if (isButtonNumber())
            {
                if (numberValue == 'D'){
                    indexOfNumber0 -= 1;
                    LcdPrintStringS(1,indexOfNumber0, " ");
                }
                else {
                    LcdPrintStringS(1,indexOfNumber2,"*");
                    arrayPassword [indexOfNumber2] = numberValue;
                    indexOfNumber2 ++;
                    timeDelay = 0;
                }
            }
            if (indexOfNumber2 >= 6){
                statusPassword = CHECK_PASSWORD;
                timeDelay = 0;
            }
            if (timeDelay >= 100)
                statusPassword = INIT_SYSTEM;
            if (isButtonEnter())
                statusPassword = INIT_SYSTEM;
            break;
        case CHANGE_PASSWORD_STEP2:
            LcdPrintStringS(0,0,"NEW PASSWORD    ");
            timeDelay ++;
            if (isButtonNumber())
            {
                if (numberValue == 'D'){
                    indexOfNumber0 -= 1;
                    LcdPrintStringS(1,indexOfNumber0, " ");
                }
                else {
                    LcdPrintStringS(1,indexOfNumber3,"*");
                    arrayMapOfPassword[indexAcc][indexOfNumber3] = numberValue;
                    indexOfNumber3 ++;
                    timeDelay = 0;
                }
            }
            if (indexOfNumber3 >= 6){
                LcdClearS();
                LcdPrintStringS(0,0,"UPDATED PASSWORD");
                LcdPrintStringS(1,0,"                ");
                delay_ms(50);
                statusPassword = INIT_SYSTEM;
                timeDelay = 0;
            }
            if (timeDelay >= 100)
                statusPassword = INIT_SYSTEM;
            if (isButtonEnter())
                statusPassword = INIT_SYSTEM;
            break;
        case WRONG_PASSWORD:
            timeDelay ++;
            LcdPrintStringS(0,0,"PASSWORD WRONG  ");
            if (timeDelay >= 50)
                statusPassword = INIT_SYSTEM;
            break;
        case WRONG_USERNAME:
            timeDelay ++;
            LcdPrintStringS(0,0,"USERNAME WRONG  ");
            if (timeDelay >= 50)
                statusPassword = INIT_SYSTEM;
            break;
        case ADMIN:
            timeDelay ++;
            LcdPrintStringS(0,0,"1.CHANGEPASS    ");
            LcdPrintStringS(1,0,"2.ADD      3.DEL");
//            while ((isNumber() != 14) && (timeDelay < 400)){
//                if (isNumber() == 3)
//                    choose = (choose+1)%3;
//                if (isNumber() == 3)
//                    choose = (choose-1)%3;
//                if (choose == 0){
//                    LcdPrintStringS(0,0,"1.CHANGEPASS    ");
//                    delay_ms(10);
//                    LcdPrintStringS(0,0,"                ");
//                    delay_ms(10);
//                }
//                if (choose == 1){
//                    LcdPrintStringS(1,0,"2.ADD      ");
//                    delay_ms(10);
//                    LcdPrintStringS(1,0,"           ");
//                    delay_ms(10);
//                }
//                if (choose == 2){
//                    LcdPrintStringS(1,11,"3.DEL");
//                    delay_ms(10);
//                    LcdPrintStringS(1,11,"     ");
//                    delay_ms(10);
//                }
//            }
//            if (choose == 0){
//                LcdClearS();
//                statusPassword = CHANGE_PASSWORD_STEP1;
//            }
//            else if (choose==1){
//                timeDelay = 0;
//                LcdClearS();
//                statusPassword =ADDNEWNAME;
//                timeDelay = 0;
//            }
//            else if(choose==2){
//                LcdClearS();
//                timeDelay = 0;
//                statusPassword =DELETE;
//                timeDelay = 0;
//            }
//            if (Mode()==1){
//                LcdClearS();
//                if (pre_state != CHANGE_PASSWORD_STEP1)
//                    statusPassword = CHANGE_PASSWORD_STEP1;
//                else statusPassword = CHANGE_PASSWORD_STEP2;
//                pre_state = 0;
//            }
//            else if (Mode()==2){
//                count+=1;
//                LcdClearS();
//                statusPassword =ADDNEWNAME;
//            }
//            else if(isButtonNumber()==3){
//                //statusPassword =;
//            }
            if (isNumber()==1){
                LcdClearS();
                if (pre_state != CHANGE_PASSWORD_STEP1)
                    statusPassword = CHANGE_PASSWORD_STEP1;
                else statusPassword = CHANGE_PASSWORD_STEP2;
                pre_state = 0;
                timeDelay = 0;
            }
            else if (isNumber()==2){
                timeDelay = 0;
                LcdClearS();
                statusPassword =ADDNEWNAME;
            }
            else if(isNumber()==3){
                LcdClearS();
                statusPassword =DELETE;
                timeDelay = 0;
            }
            if (timeDelay >= 200)
                statusPassword = INIT_SYSTEM;
            if (isButtonEnter())
                statusPassword = INIT_SYSTEM;
            break;
        case ADDNEWNAME:
            timeDelay ++;
            LcdPrintStringS(0,0,"USERNAME        ");
            
            if (isButtonNumber())
            {
                if (numberValue == 'D'){
                    indexOfNumber0 -= 1;
                    LcdPrintStringS(1,indexOfNumber0, " ");
                }
                else {
                    //LcdPrintStringS(1,indexOfNumber0,"x");
                    LcdPrintNumS(1,indexOfNumber0, numberValue);
                    arrayUsername [indexOfNumber0] = numberValue;
                    indexOfNumber0 ++;
                    timeDelay = 0;
                }
            }
            if (indexOfNumber0 >= 4){
                if(CheckUsername()){
                    LcdPrintStringS(1,0,"INVALID ID");
                    if(timeDelay >= 100)
                    {
                        indexOfNumber0=0;
                        statusPassword = ADMIN;
                    }
                }
                else if(CheckUsername()==0)
                {
                    indexOfNumber0=0;
                    LcdClearS();
                    statusPassword = ADDNEWPASS;
                }
            }
            if (timeDelay >= 100){
                statusPassword = ADMIN;
                indexOfNumber0=0;
            }
            if (isButtonEnter())
                statusPassword = INIT_SYSTEM;
            break;
        case ADDNEWPASS:
            timeDelay ++;
            LcdPrintStringS(0,0,"PASSWORD        ");
            if (isButtonNumber())
            {
                if (numberValue == 'D'){
                    indexOfNumber0 -= 1;
                    LcdPrintStringS(1,indexOfNumber0, " ");
                }
                else {
                    LcdPrintStringS(1,indexOfNumber0, "*");
                    arrayPassword [indexOfNumber0] = numberValue;
                    indexOfNumber0 ++;
                    timeDelay = 0;
                }
            }
            if (timeDelay >= 100){
                statusPassword = ADMIN;
                indexOfNumber0=0;
            }
            if (indexOfNumber0 >= 6){
                AddUser();
                timeDelay = 0;
                indexOfNumber0=0;
                statusPassword = ADMIN ;
            }
            
            if (isButtonEnter())
                statusPassword = INIT_SYSTEM;
            break;
        case DELETE:
            timeDelay ++;
            if(del == 1){
                LcdPrintStringS(0,0,"1.1234          ");
                LcdPrintStringS(1,0,"                ");
            }
            else if (del == 2){
                LcdPrintStringS(0,0,"1.1111          ");
                LcdPrintStringS(1,0,"                ");
            }
            else {
                LcdPrintStringS(0,0,"1.1111          ");
                LcdPrintStringS(1,0,"2.1234          ");
            }
            if(isNumber()==1)
            {
                LcdPrintStringS(0,0,"Deleted ID 1111 ");
                del=1;
                DeleteUser();
            }
            if(isNumber()==2)
            {
                del=2;
                LcdPrintStringS(0,0,"Deleted ID 1234 ");
                DeleteUser();
            }
            if (Mode())
                statusPassword = ADMIN;
            if (timeDelay >= 200)
                statusPassword = ADMIN;
            if (isButtonEnter())
                statusPassword = INIT_SYSTEM;
            break;
        default:
            statusPassword = INIT_SYSTEM;
         
    }
}
//////////
unsigned char Mode(){
    if (key_code[14] == 1)
        return 15;
    else 
        return 0;
}
/////////
unsigned char CheckUsername()
{
    unsigned char i,j;
    unsigned result = 1;
    for (i=0;i <=count ;i++)
    {
        result = 1;
        for (j=0;j<4;j++)
        {
            if (arrayUsername[j] != arrayMapOfUsername[i][j])
                result = 0;
        }
        if (result == 1){
            indexAcc = i;
            return (i+1);
        }
    }
    return 0;
}
unsigned char CheckPassword()
{
    unsigned char j;
    unsigned result = 1;
    for (j=0;j<6;j++)
    {
        if (arrayPassword[j] != arrayMapOfPassword[indexAcc][j])
            result = 0;
    }
    if (result == 1){
        admin=indexAcc;
        return (indexAcc+1);
    }
    return 0;
}
unsigned char isButtonNumber()
{
    unsigned char i;
    for (i = 0; i<=15; i++)
        if (key_code[i] == 1)
        {
            numberValue = arrayMapOfNumber[i];
            return 1;
        }
    return 0;
}
unsigned char isNumber()
{
    unsigned char i;
    for (i = 0; i<=15; i++)
        if (key_code[i] == 1)
        {
            numberValue = arrayMapOfNumber[i];
            return numberValue;
        }
    return 0;
}
unsigned char isButtonEnter()
{
    if (key_code[12] == 1)
        return 1;
    else
        return 0;
}

void UnlockDoor()
{
    OpenOutput(0);
    //OpenOutput(4);
}
void LockDoor()
{
    CloseOutput(0);
    //CloseOutput(4);
}
void ChangePassword(){
    
}
void AddUser()
{
    unsigned char i;
    count++;
    for (i=0;i<5;i++)
    {
       arrayMapOfUsername[count][i]=arrayUsername[i];
    }
    for (i=0;i<7;i++)
    {
        arrayMapOfPassword[count][i]=arrayPassword[i];
    }
    
}

void DeleteUser()
{
    unsigned char k;
    unsigned char i;
    for(i=del;i< count;i++){
        for(k=0;k<4;k++)
        {
            arrayMapOfUsername[i][k]=arrayMapOfUsername[i+1][k];
        }
    }
    
    for(i=del;i< count;i++){
        for(k=0;k<6;k++)
        {
            arrayMapOfPassword[i][k]=arrayMapOfPassword[i+1][k];
        }
    }
    count--;
}

