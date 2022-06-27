#include <LiquidCrystal.h>
#include <Keypad.h>

#define ROWS 4
#define COLS 4
#define MAX_LENGTH 6
#define INITIAL_PASS '1C2022'
#define BASE_MILLI_SECONDS 1000
#define CONFIG_BUTTON A4
#define RESET_BUTTON 12
#define TEST_PSW_BUTTON 11

LiquidCrystal lcd(8, 13 , A0, A1, A2, A3);
char initPass[7] = {'1','C','2','0','2','2','\0'};
char password[7];
char newPassword[7];
char customKey;
int charCont = 0;
int passChanged = 0;
int globalStatus = 0;
int globalStatus_before = 0;
int cont = 0;
int flagSwitch = 0;
int flagNewPassword = 0;
int flagConfig = 0;
int flagLedCorrect = 0;
int flagLedIncorrect = 0;
unsigned long millis_before = 0;
unsigned long millis_before_leds = 0;
short button_config_before = LOW;
short button_reset_before = LOW;
short button_psw_before = LOW;
short flagReset = 0;
short flagTest = 0;
short flagPassChanged = 0;
short ledStatus = 1;


char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {7, 6, 5, 4}; 
byte colPins[COLS] = {3, 2, 1, 0}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
/*
 * brief: Inicializa la pantalla
 * return: void
*/
void initDisplay()
{
	lcd.begin(16,1);
  	lcd.print("Password:");
}

void setup()
{
  	pinMode(9, OUTPUT);
  	pinMode(10, OUTPUT);
	initDisplay();
}

/*
 * brief: Habilita la presión de las teclas
 * return: void
*/
void pressKey() 
{
 	
  customKey = customKeypad.getKey();
  if(customKey != NO_KEY)
  {
    lcd.write(customKey);
    password[charCont] = customKey;
    charCont++;
  }
  
}
/*
 * brief: Establece la contraseña que se coloca durante el modo de configuración
 * return: void
*/
void changePass() 
{
 	
  customKey = customKeypad.getKey();
  if(customKey != NO_KEY)
  {
    lcd.write(customKey);
    newPassword[charCont] = customKey;
    charCont++;
  }
  
}
/*
 * brief: Verifica la password ingresada con la password inicial. En caso de que se haya cambiado la password, se verifica con la contraseña nueva
 * param: char password[]
 * param: char initPass[]
 * param: char newPass[]
 * return: void
*/
void verifyPass(char password[], char initPass[], char newPass[]) 
{
  	int status = 0;
  	password[charCont] = '/0';
  	for(int i = 0; i < MAX_LENGTH; i++)
  	{
		
      if(flagPassChanged == 0)
      {
        if(password[i] != initPass[i])
    	{
      	status = 1;
      	break;
    	}
      }
      else
      {
        if(password[i] != newPass[i])
    	{
      	status = 1;
      	break;
    	}
      }
  	}
      
    if(status == 0)
    {
      flagLedCorrect = 1;
      globalStatus = 1;
    }
    else
    {
      flagLedIncorrect = 1;
      globalStatus = -1;
    }
  
  	switchStatus(globalStatus);
      
}
/*
 * brief: Cambia a los distintos estados que puede tener el programa
 * param: int status
 * return: void
*/
void switchStatus(int status) 
{
 	
  lcd.clear();
  flagSwitch = 1;
  switch(status)
  {
   
    case 0:
      lcd.print("Password:");
      break;
    case 1:
      lcd.print("PASSWORD OK");
      break;
    case -1:
      lcd.print("PASSWORD FAILED");
      break;
    case 2:   
      lcd.print("PASSWORD CHANGED");    
      break;  
  }
  
}
/*
 * brief: Resetea el status a 0
 * return: void
*/
void resetStatus () 
{
  	switchStatus(0);
  	globalStatus = 0;
  	flagSwitch = 0;
}
/*
 * brief: Realiza un parpadeo de luces constante
 * param: int led
 * return: void
*/
void showLeds(int led)
{
  digitalWrite(led, ledStatus);
  if(ledStatus == 0)
  {
    ledStatus = 1;
  }
  else
  {
    ledStatus = 0;
  }
}
/*
 * brief: Establece el dispositivo en un modo de configuración de contraseña
 * return: void
*/
void config()
{
  int button_config_now = digitalRead(CONFIG_BUTTON);
  //Anti rebote
  if(button_config_now == HIGH && button_config_before == LOW)
  {
  	flagConfig = 1;
  }
  button_config_before = button_config_now;
}
/*
 * brief: Activa una flag que borra la password que se está ingresando
 * return: void
*/
void reset()
{
  int button_reset_now = digitalRead(RESET_BUTTON);
  //Anti rebote
  if(button_reset_now == HIGH && button_reset_before == LOW)
  {
  	flagReset = 1;
  }
  button_reset_before = button_reset_now;
}
/*
 * brief: Activa una flag que testea la contraseña
 * return: void
*/
void testPsw()
{
  int button_psw_now = digitalRead(TEST_PSW_BUTTON);
  //Anti rebote
  if(button_psw_now == HIGH && button_psw_before == LOW)
  {
  	flagTest = 1;
  }
  button_psw_before = button_psw_now;
}
/*
 * brief: Realiza un proceso cada 1000ms
 * return: void
*/
void sequence()
{
  	unsigned long millis_now = millis();
 	if(millis_now - millis_before >= BASE_MILLI_SECONDS)
    {
      if(flagSwitch == 1)
      {
        cont ++;
      }
      
      if(flagConfig == 0)
      {  
        if(flagReset == 1)
        {
          resetStatus();
          charCont = 0;
          flagReset = 0;
        }

        if(charCont >= MAX_LENGTH || flagTest == 1)
        {
          verifyPass(password, initPass, newPassword);
          charCont = 0;
          flagTest = 0;
        }
      }
      else
      {
        if(charCont >= MAX_LENGTH)
        {
         flagPassChanged = 1;
         flagConfig = 0;
         newPassword[charCont]= '\0';
         charCont = 0;
         switchStatus(2);
        }
      }
      
      if(flagLedCorrect == 1)
      {
       showLeds(9);
      }
      if(flagLedIncorrect == 1)
      {
       showLeds(10);
      }
      
      if(cont >= 5)
      {
        if(globalStatus == 1)
        {
          digitalWrite(9, LOW);
          flagLedCorrect = 0;
          lcd.noDisplay();
        }
        else
        {
          digitalWrite(10, LOW);
          flagLedIncorrect = 0;
          resetStatus();
        }
        cont = 0;
        flagSwitch = 0;
      }
      
      millis_before = millis_now;
    }
}


void loop()
{
  sequence();
  // Deshabilita todos los botones y el teclado al momento de validar la contraseña
  if(globalStatus == 0)
  {
    // Deshabilita los 3 botones durante el proceso de cambio de contraseña
    if(flagConfig == 1)
    {
      changePass();
    }
    else
    {
      pressKey();
      config();
      testPsw();
      reset();
    }
  }
  // Delay permitido
  delay(80);
}