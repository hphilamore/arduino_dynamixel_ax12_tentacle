
/* Dynamixel AX-12 servo motor
 * Operating Mode 0 - Position is mapped to value recived over Serial (e.g. from Isadora)
 * Operating Mode 1 - Position is mapped to analog value of potentiometer
 * Operating Mode 2 - Servo ID changed  
 * Operating Mode 3 - Position is reset to central position 
 * 
 * Required libraries from: https://sourceforge.net/projects/dynamixelforarduino/files/
 */



// https://sourceforge.net/projects/dynamixelforarduino/files/
#include <DynamixelSerial1.h> 
#include <SoftwareSerial.h>


//****************************************************************************************************************************************

//   Variables to set

const int operating_mode = 1;     // Choice of : 0 = serial, 1 = potentiometer, 2 = assign new ID, 3 = reset to centre
const int id = 2;                 // servo ID 

//****************************************************************************************************************************************



const int _BUFFER_SIZE = 1;       // number of inputs to recive over serial 
int _n_bytes;                     // variable to indicate to store number of bytes of data recieved over serial
//char *_buffer;               
byte Buffer[_BUFFER_SIZE];        // buffer to store info received over serial
const int pot_pin = A4;
float pot_val;
int Position;

SoftwareSerial SoftSerial(9, 8);  // software serial pins tx=9, rx=8

void setup()
{
  Dynamixel.begin(1000000,2);     // Init. Servo 1 @ 1Mbps and Pin Control 2
  SoftSerial.begin(9600);
  Dynamixel.setEndless(id,OFF);
  Dynamixel.moveSpeed(id,511,200);// Init. servos at central position
  pinMode(pot_pin, INPUT);        // Input pin for slider/ potentiometer 
  delay(1000);
}

void loop()
{

    // SERIAL
    if(operating_mode == 0){  
  
        Dynamixel.end();                      // End Servo comms
        Serial.begin(9600);                   // Begin Serial comms
  
         // 
         if (SoftSerial.available() > 0)      // if data available
         {
          int rlen = SoftSerial.readBytes(Buffer, _BUFFER_SIZE);           // read data
                  
  //        for(int i = 0; i < rlen; i++){                                 // print the received buffer
  //          Serial.print(char(Buffer[i]), DEC);
  //          Serial.print(" ");
  //          }
  //         Serial.println("");
  
          
  //        Serial.println(char(Buffer[0]), DEC);                     
  //        Serial.print(" ");

            int x_pos = float(char(Buffer[0]));                              // First character in buffer 
            int serial_max = 50;                                             // Max value expected over serial
            int serial_min = -50;                                            // Min value expected over serial
            int servo_val = map(x_pos, serial_min, serial_max, 0, 1023);     // Map value to full range of servo
            Serial.print("x_pos = ");
            Serial.print(x_pos);
            Serial.print(", servo_val = ");
            Serial.println(servo_val);
            Serial.end(); 
            Dynamixel.begin(1000000,2);
            Dynamixel.setEndless(id,OFF);
            Dynamixel.moveSpeed(id,servo_val,200);                            // Program servo 
            Dynamixel.end();
            Serial.begin(9600);   
      }
    }
  
    // POTENTIOMETER
    else if (operating_mode == 1){
            
      Dynamixel.end();                      // End Servo comms
      
      Serial.begin(9600);                   // Begin Serial comms
      pot_val = analogRead(pot_pin);
      int pot_max = 1023;                   // Min value expected from potentiometer input
      int pot_min = 0;                      // Max value expected from potentiometer input
      int servo_val = map(pot_val, pot_min, pot_max, 0, 1023);  // Map servo position to poentiometer value
      Serial.print("pot_val = ");
      Serial.print(pot_val);
      Serial.print(", servo_val = ");
      Serial.print(servo_val);
      Serial.print(", servo_pos = ");
      Serial.print(Position);
      Serial.println("");
      Serial.end(); 
      Dynamixel.begin(1000000,2);           // Begin Servo Comms
      Dynamixel.setEndless(id,OFF);
      Dynamixel.moveSpeed(id,servo_val,200); // program servo 
      Position = Dynamixel.readPosition(id);       // Request and Print the Position 
      
      }

      else if(operating_mode == 2){
        //Dynamixel.setId (ID , newID );
        Dynamixel.setID (1 , 2 );
      }

      // RESET 
      else{
        }
  
  }
