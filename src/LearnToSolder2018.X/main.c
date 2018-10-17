/*
 * CSI Learn To Solder 2018 board software
 * 
 * Written by Brian Schmalz of Schmalz Haus LLC
 * brian@schmalzhaus.com
 * 
 * Copyright 2018
 * All of this code is in the public domain
 * 
 * Versions:
 * 
 * Ideas:
 */

#include "mcc_generated_files/mcc.h"

// Starting time, in ms, between switching which LED is currently on in main pattern
#define SLOW_DELAY          250

// Maximum number of patterns allowed in the battery array
#define NUMBER_OF_PATTERNS    8

// Button debounce time in milliseconds
#define BUTTON_DEBOUNCE_MS   20

// Number of milliseconds to stay awake for before sleeping just to see if another
// button will be pressed
#define SHUTDOWN_DELAY_MS   100

// Time between two button presses below which is considered 'short'
#define QUICK_PRESS_MS      250

/* Switch inputs :  (pressed = high)
 *   Start = S1 = GP4 - always input
 *   Speed = S2 = GP3 - always input
 * 
 * Motor FET is on GP2 - always output, high to turn on
 * 
 * LEDs (set high to light):
 *  GP5, D1, Low Speed
 *  GP1, D2, Medium Speed
 *  GP0, D3, High Speed
 */

#define TRISA_INIT        0xD8  // 0b11011000
#define PORTA_INIT        0x00

// Maximum number of milliseconds to allow system to run
//#define MAX_AWAKE_TIME_MS     (5UL * 60UL * 1000UL)
#define MAX_AWAKE_TIME_MS     (30UL * 1000UL)

// The five states a button can be in (for debouncing))
typedef enum {
    BUTTON_STATE_IDLE = 0,
    BUTTON_STATE_PRESSED_TIMING,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_RELEASED_TIMING,
    BUTTON_STATE_RELEASED
} ButtonState_t;

uint8_t SpeedLEDState = 0;
uint8_t MotorState = 0;

// Counts number of milliseconds we are awake for, and puts us to sleep if 
// we stay awake for too long
volatile static uint32_t WakeTimer = 0;

// Used for temporary timing during startup
volatile static uint32_t TempDelayTimer = 0;

// Counts down from SHUTDOWN_DELAY_MS after everything is over before we go to sleep
volatile static uint8_t ShutdownDelayTimer = 0;

// Countdown 1ms timers to  debounce the button inputs
volatile static uint8_t StartDebounceTimer = 0;
volatile static uint8_t SpeedDebounceTimer = 0;

// Keep track of the state of each button during debounce
volatile static ButtonState_t StartButtonState = BUTTON_STATE_IDLE;
volatile static ButtonState_t SpeedButtonState = BUTTON_STATE_IDLE;

// Record the last value of WakeTimer when the button was pushed
volatile static uint32_t StartRightButtonPressTime = 0;
volatile static uint32_t SpeedLeftButtonPressTime = 0;

/* This ISR runs every 125 uS. It takes the values in LEDState and lights
 * up the LEDs appropriately.
 * It also handles a number of software timer decrementing every 1ms.
 */
void TMR0_Callback(void)
{
  static uint8_t cntr = 0;
  
  cntr++;
  
  if (cntr >= 8)
  {
    cntr = 0;
    // perform the 1ms tasks

    // Always increment wake timer to count this millisecond
    WakeTimer++;

    // Decrement button debounce timers
    if (StartDebounceTimer)
    {
      StartDebounceTimer--;
    }

    if (SpeedDebounceTimer)
    {
      SpeedDebounceTimer--;
    }

    if (ShutdownDelayTimer)
    {
      ShutdownDelayTimer--;
    }
    
    if (TempDelayTimer)
    {
      TempDelayTimer--;
    }
  }
}

void SET_MOTOR_OFF(void)
{
  LATAbits.LATA2 = 0;
}

void SET_MOTOR_ON(void)
{
  LATAbits.LATA2 = 1;
}

void LOW_LED_OFF(void)
{
  LATAbits.LATA5 = 0;
}

void MEDIUM_LED_OFF(void)
{
  LATAbits.LATA1 = 0;
}

void HIGH_LED_OFF(void)
{
  LATAbits.LATA0 = 0;
}

void LOW_LED_ON(void)
{
  LATAbits.LATA5 = 1;
}

void MEDIUM_LED_ON(void)
{
  LATAbits.LATA1 = 1;
}

void HIGH_LED_ON(void)
{
  LATAbits.LATA0 = 1;
}

void SetAllLEDsOff(void)
{
  LOW_LED_OFF();
  MEDIUM_LED_OFF();
  HIGH_LED_OFF();
}

void UpdateLEDs(void)
{
  switch (SpeedLEDState)
  {
    case 0:
      LOW_LED_ON();
      MEDIUM_LED_OFF();
      HIGH_LED_OFF();
      PWM3_DutyCycleSet(800);
      PWM3_LoadBufferSet();
      break;
      
    case 1:
      LOW_LED_OFF();
      MEDIUM_LED_ON();
      HIGH_LED_OFF();
      PWM3_DutyCycleSet(1000);
      PWM3_LoadBufferSet();
      break;
      
    case 2:
      LOW_LED_OFF();
      MEDIUM_LED_OFF();
      HIGH_LED_ON();
      PWM3_DutyCycleSet(1249);
      PWM3_LoadBufferSet();
      break;
      
    default:
      break;
  }
}

// Return the raw state of the right button input
bool StartButtonPressedRaw(void)
{
  return (uint8_t)(PORTAbits.RA4 == 0);
}

// Return the raw state of the left button input
bool SpeedButtonPressedRaw(void)
{
  return (uint8_t)(PORTAbits.RA3 == 0);
}

// Return the logical (debounced) state of the speed switch
bool SpeedButtonPressed(void)
{
  return (SpeedButtonState == BUTTON_STATE_PRESSED);
}

// Return the logical (debounced) state of the left button
bool StartButtonPressed(void)
{
  return (StartButtonState == BUTTON_STATE_PRESSED);
}

void SpeedButtonJustGotPressed(void)
{
  SpeedLEDState++;
  if (SpeedLEDState >= 3)
  {
    SpeedLEDState = 0;
  }
  UpdateLEDs();
  WakeTimer = 0;
}

void SpeedButtonJustGotReleased(void)
{
  WakeTimer = 0;
}

void StartButtonJustGotPressed(void)
{  
  switch (MotorState)
  {
    case 0:
      PWM3_Start();
      MotorState = 1;
      break;
    
    case 1:
      PWM3_Stop();
      MotorState = 0;
      break;
      
    default:
      break;    
  }
  WakeTimer = 0;
}

void StartButtonJustGotReleased(void)
{
  WakeTimer = 0;
}

// Return true if either button is currently down (raw)
bool CheckForButtonPushes(void)
{
  static bool LastStartButtonState = false;
  static bool LastSpeedButtonState = false;
  
  // Debounce start button press
  if (StartButtonPressedRaw())
  {
    if (StartButtonState == BUTTON_STATE_PRESSED_TIMING)
    {
      if (StartDebounceTimer == 0)
      {
        StartButtonState = BUTTON_STATE_PRESSED;
        StartButtonJustGotPressed();
      }
    }
    else if (StartButtonState != BUTTON_STATE_PRESSED)
    {
      StartButtonState = BUTTON_STATE_PRESSED_TIMING;
      StartDebounceTimer = BUTTON_DEBOUNCE_MS;
    }
  }
  else
  {
    if (StartButtonState == BUTTON_STATE_RELEASED_TIMING)
    {
      if (StartDebounceTimer == 0)
      {
        StartButtonState = BUTTON_STATE_RELEASED;
        StartButtonJustGotReleased();
      }
    }
    else if (StartButtonState != BUTTON_STATE_RELEASED)
    {
      StartButtonState = BUTTON_STATE_RELEASED_TIMING;
      StartDebounceTimer = BUTTON_DEBOUNCE_MS;
    }
  }
    
  // Debounce right button  press
  if (SpeedButtonPressedRaw())
  {
    if (SpeedButtonState == BUTTON_STATE_PRESSED_TIMING)
    {
      if (SpeedDebounceTimer == 0)
      {
        SpeedButtonState = BUTTON_STATE_PRESSED;
        SpeedButtonJustGotPressed();
      }
    }
    else if (SpeedButtonState != BUTTON_STATE_PRESSED)
    {
      SpeedButtonState = BUTTON_STATE_PRESSED_TIMING;
      SpeedDebounceTimer = BUTTON_DEBOUNCE_MS;
    }
  }
  else
  {
    if (SpeedButtonState == BUTTON_STATE_RELEASED_TIMING)
    {
      if (SpeedDebounceTimer == 0)
      {
        SpeedButtonState = BUTTON_STATE_RELEASED;
        SpeedButtonJustGotReleased();
      }
    }
    else if (SpeedButtonState != BUTTON_STATE_RELEASED)
    {
      SpeedButtonState = BUTTON_STATE_RELEASED_TIMING;
      SpeedDebounceTimer = BUTTON_DEBOUNCE_MS;
    }
  }

  return ((bool)(StartButtonPressedRaw() || SpeedButtonPressedRaw()));
}

/*
                         Main application
 */
void main(void)
{
  // initialize the device
  SYSTEM_Initialize();

  TMR0_SetInterruptHandler(TMR0_Callback);

  // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
  // Use the following macros to:

  // Enable the Global Interrupts
  INTERRUPT_GlobalInterruptEnable();

  // Enable the Peripheral Interrupts
  INTERRUPT_PeripheralInterruptEnable();

  // Disable the Global Interrupts
  //INTERRUPT_GlobalInterruptDisable();

  // Disable the Peripheral Interrupts
  //INTERRUPT_PeripheralInterruptDisable();
    
  uint8_t i;
  
  TRISA = TRISA_INIT;
  LATA = PORTA_INIT;
  
  // Put board to sleep as soon as battery is inserted
  WakeTimer = MAX_AWAKE_TIME_MS + 1;

  while (1)
  {    
    if (WakeTimer > MAX_AWAKE_TIME_MS)
    {
      SetAllLEDsOff();
      PWM3_Stop();

      // Allow LEDsOff command to percolate to LEDs
      __delay_ms(5);

      ShutdownDelayTimer = SHUTDOWN_DELAY_MS;

      while (ShutdownDelayTimer && !CheckForButtonPushes())
      {
      }

      if (ShutdownDelayTimer == 0)
      {
          // Hit the VREGPM bit to put us in low power sleep mode
        VREGCONbits.VREGPM = 1;

        SLEEP();

        // Start off with time = 0;
        WakeTimer = 0;
        SpeedLEDState = 0;
        
        // Perform startup LED flashes
        LOW_LED_ON();
        MEDIUM_LED_ON();
        HIGH_LED_ON();
        
        TempDelayTimer = 200;
        while(TempDelayTimer);

        LOW_LED_OFF();
        MEDIUM_LED_OFF();
        HIGH_LED_OFF();
        
        TempDelayTimer = 200;
        while(TempDelayTimer);

        LOW_LED_ON();
        MEDIUM_LED_ON();
        HIGH_LED_ON();
        
        TempDelayTimer = 200;
        while(TempDelayTimer);

        LOW_LED_OFF();
        MEDIUM_LED_OFF();
        HIGH_LED_OFF();
        
        TempDelayTimer = 200;
        while(TempDelayTimer);
        
        UpdateLEDs();
      }
    }

    CheckForButtonPushes();
  }
}