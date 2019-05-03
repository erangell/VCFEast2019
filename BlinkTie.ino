/*
  Blinky Light Mood Tie
*/

int mood = 0; // initial mood value = nothing until bits are set/reset
int bits = 0; // value to display on LEDs
int leddelay = 3; // delay between LED flashes to create persistence of vision
int hold = 30; // number of cycles to hold the count before changing bits


// the setup function runs once when you press reset or power the board
void setup() {
  // switches are used to select a bit to be toggled, which is displayed on the LED

  Serial.begin(9600);
  
  pinMode(2, INPUT_PULLUP); // hold switch
  pinMode(3, OUTPUT);
  pinMode(4, INPUT_PULLUP); // LED number: ones place
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, INPUT_PULLUP); // LED number: twos place
  pinMode(8, INPUT_PULLUP); // LED number; fours place
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {

  int running = 1;

  //transition from hold state to running state where each LED is rapidly flashed for persistence of vision
  //initial value of bits should be set here based on input switches (which toggle bits in the desired display value)
  
  int ctr = hold;
  int b = bits;
  
  while (running)
  {   
    b = bits;
    Serial.print (b);
    Serial.print (" ");
    
    b = b & 127;

    if (b > 63)
    {
      Serial.print("1");
      digitalWrite(3, HIGH);   
      delay(leddelay);
      digitalWrite(3, LOW);
    }
    else
    {
      Serial.print("0");
      digitalWrite(3, LOW);   
      delay(leddelay);
    }

    b = b & 63;
        
    // Remaining LEDs are controlled by PWM digital outputs
    
    if (b > 31) 
    {
      Serial.print("1");
      digitalWrite(5, HIGH);   
      delay(leddelay);
      digitalWrite(5, LOW);  
    }
    else
    {  
      Serial.print("0");
      digitalWrite(5, LOW);   
      delay(leddelay);
    }

    b = b & 31;
    
    if (b > 15) 
    {
      Serial.print("1");
      digitalWrite(6, HIGH);   
      delay(leddelay);
      digitalWrite(6, LOW); 
    }
    else
    {
      Serial.print("0");
      digitalWrite(6, LOW);   
      delay(leddelay);
    }

    b = b & 15;
    
    if (b > 7)
    {
      Serial.print("1");
      digitalWrite(9, HIGH);   
      delay(leddelay);
      digitalWrite(9, LOW);   
    }
    else
    {
      Serial.print("0");
      digitalWrite(9, LOW);   
      delay(leddelay);
    }

    b = b & 7;
    
    if (b > 3)
    { 
      Serial.print("1"); 
      digitalWrite(10, HIGH);   
      delay(leddelay);
      digitalWrite(10, LOW); 
    }
    else
    {
      Serial.print("0");
      digitalWrite(10, LOW);   
      delay(leddelay);
    }

    b = b & 3;
    
    if (b > 1)
    {
      Serial.print("1");
      digitalWrite(11, HIGH);   
      delay(leddelay);
      digitalWrite(11, LOW);  
    }
    else
    {    
      Serial.print("0");
      digitalWrite(11, LOW);   
      delay(leddelay);
    }

    b = b & 1;
    Serial.println (b);
      
    //bottom LED is 2 color, controlled by pins 12 and 13
    if (b == 1)
    {
      digitalWrite(12, HIGH);   
      digitalWrite(13, LOW);   
    
    }
    delay(leddelay);
    
    digitalWrite(12, LOW);   
    digitalWrite(13, LOW);   

    ctr--;
    if (ctr == 0)
    {
      bits++;      
      
      bits = bits & 127;
      ctr = hold;
    }

  } // while
} // loop

