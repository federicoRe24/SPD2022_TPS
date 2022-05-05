/*
 *	Trabajo Práctico 1: Cronómetro Binario
 *
 *	Al iniciar la simulacion, todos los led deben comenzar apagados.
 *	Cada 1 segundo que transcurra, se deben encender los led
 *	de acuerdo al numero que representan.
 *
 *	Ejemplo:
 *   
 *	El numero 15 (0000001111) se representaria encendiendo los 
 *	leds AMARILLO y NARANJA.
 *	Al pasar al numero 16 (0000010000), se deben apagar todos 
 *	los led anteriores y encenderse uno de los leds VERDES. 
 *	Notese, en el ejemplo los 0 representan los led apagados 
 *	y los 1 los led encendidos). 
 *   
 *	-------------------------------------------------------
 *   
 *	Al presionarse el boton START, debe iniciar el cronometro.
 *	Volver a presionarlo hace que la secuencia se detenga. 
 *  (Como si se hubiese presionado pausa).
 *	Al presionarse el boton RESET, el cronometro
 *	debe reiniciarse y arrancar de 0.
 *
 *	Tip: Modularizar la función que controla el encendido de los 
 *	LEDS y de ser posible, todo el código para evitar repetir lineas lo mas posible.
 *  Usar millis para controlar el tiempo del contador para que el cambio de los 
 *	leds encendidos sea perceptible para el ojo humano y 
 *	documentar cada función creada en el código. 
 *	Un breve comentario que diga que es lo que hace esa función
 *  y de corresponder, que retorna).
*/

//##### Una Ayudita #####
//--- Defines ---//
#define ZERO_TO_HERO 0          //? To start the counter from 0
#define BUTTON_START 2
#define BUTTON_RESET 3
#define FIRST_LED 4             //? First pin of the leds
#define LAST_LED 13             //? Last pin of the leds
#define BASE_MILLI_SECONDS 1000 //? Secods expresed in miliseconds
#define MAX_SECONDS 1023  
#define TAM 10
//--- End Defines ---//

//! Variables Area
short cont = ZERO_TO_HERO;
int array[TAM];
int button_reset_before = LOW;
int button_start_before = LOW;
int button_start_flag = 0;
unsigned long millis_before = 0;
int flagEnd = 0; // Marca cuando finaliza el programa
int appTimer = 0;

/*
 * brief: Setea los pines como INPUT o OUTPUT
 * return: void
*/
void set_pins()
{
  pinMode(BUTTON_START, INPUT);
  pinMode(BUTTON_RESET, INPUT);
  for(int i = FIRST_LED; i <= LAST_LED; i++)
  {
  	pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  };
}

/*
 * brief: Establece todos los espacios del array en 0
 * param: int array[]
 * return: void
*/
void start_array(int array[])
{
  for(int i = 0; i <= TAM-1; i++)
  {
    array[i] = 0;
  }
}

/*
 * brief: Transcribe un numero de decimal a binario y lo copia a un array
 * param: int number
 * param: int array[]
 * return: void
*/
void decimal_to_BIN(int number, int arrayBin[])
{
	int endIndex = TAM - 1;
  	int numBin = 0;	
  
  	while(number > 0)
    {
      	numBin = number % 2;
      	number /= 2;
      	
      	arrayBin[endIndex] = numBin;
      	endIndex--;
      
    }
  
}

/*
 * brief: Printea la información que establece el contador.
 * return: void
*/
void print_array()
{	
  	Serial.print("Seconds: ");
  	Serial.print(appTimer);
 	Serial.print(" | Counter: ");
  	Serial.print(cont);
  	Serial.print(" | Binary: ");
  	for(int i = 0; i <= TAM-1; i++)
    {
      Serial.print(array[i]);
    }
  	Serial.println("");
}

/*
 * brief: Establece la energia de los pines en LOW
 * return: void
*/
void shutdown_leds()
{
	for (int i = FIRST_LED; i <= LAST_LED; i++)
    {
        digitalWrite(i, LOW);
    }
}

/*
 * brief: Establece la energia de ciertos pines según el array ingresado
 * param: int array[]
 * return: void
*/
void show_leds(int array[])
{
  
  int array_iter = TAM-1;
  
  for(int i = FIRST_LED; i <= LAST_LED; i++)
  {
    digitalWrite(i, array[array_iter]);
    array_iter--;
  }
}

/*
 * brief: Resetea el contador, incluye un Antirebote del botón
 * return: void
*/
void reset_cont()
{
  int button_now = digitalRead(BUTTON_RESET);
    // Anti rebote
    if (button_now == HIGH && button_reset_before == LOW)
    {
        cont = ZERO_TO_HERO;
    }

    button_reset_before = button_now;
}

/*
 * brief: Establece el inicio / pausa / continuación del contador
 * return: void
*/
void start_count()
{
  int button_now = digitalRead(BUTTON_START);
    // Anti rebote
    if (button_now == HIGH && button_start_before == LOW)
    {
        if(button_start_flag == 1)
        {
          // Las luces se apagan al pausar, para desactivar esta config, comentar la linea 150.
          shutdown_leds();
          button_start_flag = 0;
        }
      	else
        {
          button_start_flag = 1;
        }
    }

    button_start_before = button_now;
}

/*
 * brief: Realiza un proceso cada 1000ms
 * return: void
*/
void sequence()
{
    unsigned long millis_now = millis(); // 3000
    if (millis_now - millis_before >= BASE_MILLI_SECONDS)
    { 
      	appTimer++;
      	// Inicio de secuencia
      
      	if(cont <= MAX_SECONDS && flagEnd == 0)
        { 
          if(button_start_flag == 1)
          {
            start_array(array);
            decimal_to_BIN(cont, array);
            shutdown_leds();
            show_leds(array);
            print_array();
            cont++;
          }
        }
        else
        {
          flagEnd == 1;
        }
      
      	// Fin de secuencia
      
        millis_before = millis_now; // MB = 2000
    }
}

void setup()
{
  
  Serial.begin(9600);
  set_pins();
}

void loop()
{
  	sequence();
  	reset_cont();
  	start_count();
  
  	delay(7); // Delay permitido
}
