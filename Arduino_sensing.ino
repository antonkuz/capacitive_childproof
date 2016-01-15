//****************************************************************************************
// Illutron take on Disney style capacitive touch sensor using only passives and Arduino
// Dzl 2012
//****************************************************************************************


//                              10n
// PIN 9 --[10k]-+-----10mH---+--||-- OBJECT
//               |            |
//              3.3k          |
//               |            V 1N4148 diode
//              GND           |
//                            |
// Analog 0 ---+------+--------+
//            |      |
//          100pf   1MOmhm
//            |      |
//           GND    GND

#define NOTHING_INDEX 0
#define ADULT_INDEX 1
#define CHILD_INDEX 2
#define NUM_STATES 3

#define NOTHING_BUTTON_PIN 2
#define SPEAKERS_PIN 3
#define ADULT_BUTTON_PIN 4
#define CHILD_BUTTON_PIN 5
#define NOTHING_LED_PIN 6
#define ADULT_LED_PIN 12
#define CHILD_LED_PIN 11

#define HOLD_CONFIRM 10

#define SET(x,y) (x |=(1<<y))				//-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))       		// |
#define CHK(x,y) (x & (1<<y))           		// |
#define TOG(x,y) (x^=(1<<y))            		//-+

#define N 160  //How many frequencies
int sizeOfArray = N;

float results[N];
float freq[N];

char holderIndex;
char holderCount;

/* for proper starting */
char nothingButtonPressed = 0;
char adultButtonPressed = 0;
char childButtonPressed = 0;

void setup()
{
  TCCR1A=0b10000010;        //-Set up frequency generator
  TCCR1B=0b00011001;        //-+
  ICR1=110;
  OCR1A=55;

  pinMode(9,OUTPUT);        //-Signal generator pin
  pinMode(8,OUTPUT);        //-Sync (test) pin

  Serial.begin(9600);

  for(int i=0;i<N;i++)      //-Preset results
    results[i]=0;         //-+

  /* music setup */
  pinMode(SPEAKERS_PIN, OUTPUT);
  pinMode(NOTHING_LED_PIN, OUTPUT);
  pinMode(ADULT_LED_PIN, OUTPUT);
  pinMode(CHILD_LED_PIN, OUTPUT); 
  digitalWrite(SPEAKERS_PIN, LOW);
  digitalWrite(NOTHING_LED_PIN, LOW);
  digitalWrite(ADULT_LED_PIN, LOW);
  digitalWrite(CHILD_LED_PIN, LOW);

  holderIndex = NOTHING_INDEX;
  holderCount = 0;
}

void loop()
{
  int counter = 0;
  for(unsigned int d=0;d<N;d++)
  {
    int v=analogRead(0);    //-Read response signal
    CLR(TCCR1B,0);          //-Stop generator
    TCNT1=0;                //-Reload new frequency
    ICR1=d;                 // |
    OCR1A=d/2;              //-+
    SET(TCCR1B,0);          //-Restart generator

    results[d]=results[d]*0.5+(float)(v)*0.5; //Filter results
   
    freq[d] = d;
  }
  process_data();

  TOG(PORTB,0);            //-Toggle pin 8 after each sweep (good for scope)
}
   

   
    
 
