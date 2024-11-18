/***************************************************************************************
                                e s p 3 2   B l i n k
                                                                      қuran oct 2024
**************************************************************************************/
#include <Arduino.h>
#include <sdtio.h>
#include <string.h>

#define TRUE                             1
#define FALSE                            0
#define WAIT_ONE_SEC                 10000
#define ON_BOARD_LED                     5
#define DAC                             25   // Trig
#define WHEEL_L                          2
#define WHEEL_R                         A4
#define WHEEL_L_DIRECTION               15 
#define WHEEL_R_DIRECTION               A5
#define BATTERY_LEVEL                   A3   // GPIO 39

void IRAM_ATTR myTimer(void);

volatile int flag; 
volatile int vL, vR;

void setup() 
{
    hw_timer_t *timer = NULL;

    pinMode(ON_BOARD_LED, OUTPUT);
    pinMode(DAC, OUTPUT);
    pinMode(WHEEL_L, OUTPUT);
    pinMode(WHEEL_R, OUTPUT);
    pinMode(WHEEL_L_DIRECTION, OUTPUT);
    pinMode(WHEEL_R_DIRECTION, OUTPUT);


    digitalWrite(ON_BOARD_LED, LOW); // invers logic!
    digitalWrite(WHEEL_L_DIRECTION, LOW );
    digitalWrite(WHEEL_R_DIRECTION, HIGH);
    digitalWrite(WHEEL_L, LOW); // stop !
    digitalWrite(WHEEL_R, LOW); // stop !


    vR = vL = 0;

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &myTimer, true);
    timerAlarmWrite(timer, 100, true);  // 0.1 msec
    timerAlarmEnable(timer);

    Serial.begin(115200);
    sei();                              // normaly not needed cause Arduino
                                        // uses it befor the loop starts.
                                        // but 'cause we want to use the 
                                        // serial imediately... 
                                        // therefore we call sei() first!
    printf("start!");
}  

void loop() 
{
    static int led = 0;
    if (flag)
    {
        led ^= 1; // toggle
        digitalWrite(ON_BOARD_LED, led);
        flag = 0; 
        printf("Elektronik und Technische Informatik ist super stark!!!");

    }

    if (Serial.available() > 0) 
    {
        String data = Serial.readString();
        if (data.indexOf('x') != -1)
        {
            // x received
            flag = TRUE;
            printf("\nx received!\n");
        }

    }

}


void IRAM_ATTR myTimer(void)   // periodic timer interrupt, expires each 0.1 msec
{
    static int32_t count  = 0;
    static unsigned char ramp = 0;

    count++;
    ramp++;

    dacWrite(DAC, ramp);

    if (count >= WAIT_ONE_SEC) 
    {
        flag = TRUE;
        count = 0;
    }


    // PWM:

    if (ramp >= vL) digitalWrite(WHEEL_L, LOW);  else digitalWrite(WHEEL_L, HIGH);
    if (ramp >= vR) digitalWrite(WHEEL_R, LOW);  else digitalWrite(WHEEL_R, HIGH);

}

