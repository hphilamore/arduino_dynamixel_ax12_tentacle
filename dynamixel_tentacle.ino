#include <SoftwareSerial.h>
#include <DynamixelSerial.h>


//****************************************************************************************************************************************

//   Variables to set

const int operating_mode = 1;                   // Choice of : 0 = potentiometer, 1 = serial, 2 = reset

//****************************************************************************************************************************************



const int _BUFFER_SIZE = 3;       // number of inputs to recive over serial 
int _n_bytes;                     // variable to indicate to store number of bytes of data recieved over serial
//char *_buffer;               
byte Buffer[_BUFFER_SIZE];        // buffer to store info received over serial
const int pot_pin = A4;
float pot_val;


SoftwareSerial SoftSerial(9, 8);  // software serial pins tx=9, rx=8

void setup()
{
  Dynamixel.begin(1000000,2);    // Init. Servo 1 @ 1Mbps and Pin Control 2
  SoftSerial.begin(9600);
  Dynamixel.setEndless(1,OFF);
  Dynamixel.moveSpeed(1,511,200);// Init. servos at central position
  pinMode(pot_pin, INPUT);
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
          int rlen = SoftSerial.readBytes(Buffer, _BUFFER_SIZE);     // read data
                  
  //        for(int i = 0; i < rlen; i++){                           // print the received buffer
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
            Dynamixel.setEndless(1,OFF);
            Dynamixel.moveSpeed(1,servo_val,200);                   // program servo 
            Dynamixel.end();
            Serial.begin(9600); 
  
  
////            if(float(char(Buffer[0]))>0){
//            if(x_pos>0){
//              Serial.println("positive x position");
//              Serial.end(); 
//              Dynamixel.begin(1000000,2);
//              Dynamixel.setEndless(1,OFF);
//              //Dynamixel.moveSpeed(1,0,200);
//              //Dynamixel.moveSpeed(2,400,200);
//              int serial_max = 50;                                   // Max value expected over serial
//              int servo_val = map(x_pos, 0, serial_max, 511, 1023);  // Map value to half range of servo
//              Dynamixel.moveSpeed(1,servo_val,200);                  // program servo 
//              Dynamixel.end();
//              Serial.begin(9600); 
//              }
//            else{
//              Serial.println("negative x position");
//              Serial.end(); 
//              Dynamixel.begin(1000000,2);
//              Dynamixel.setEndless(1,OFF);
//              //Dynamixel.moveSpeed(1,1023,200);
//              //Dynamixel.moveSpeed(2,200,200);
//              int serial_min = -50;                                   // Min value expected over serial
//              int servo_val = map(x_pos, serial_min, 0, 0, 511);      // Map value to half range of servo
//              Dynamixel.moveSpeed(1,servo_val,200);                   // program servo 
//              Dynamixel.end();
//              Serial.begin(9600); 
//            }
//          
//         }
//         Serial.end();                      // End the Serial Comms
//         Dynamixel.begin(1000000,2);        // Begin Servo Comms
  
    }
  
    // POTENTIOMETER
    else if (operating_mode == 1){
      Dynamixel.end();                      // End Servo comms
      Serial.begin(9600);                   // Begin Serial comms
      pot_val = analogRead(pot_pin);
      Serial.print(pot_val);
      Serial.print(" ");
      Serial.println((pot_val-300)*10);
      Serial.end();                         // End the Serial Comms
      Dynamixel.begin(1000000,2);           // Begin Servo Comms
      Dynamixel.setEndless(1,OFF);
      int pot_max = 1023;                   // Min value expected from potentiometer input
      int pot_min = 0;                      // Max value expected from potentiometer input
      int servo_val = map(pot_val, pot_min, pot_max, 0, 1023);  // Map value to full range of servo
      //Dynamixel.moveSpeed(1,pot_val,200);
      //Dynamixel.moveSpeed(1,(pot_val-300)*10,200);
      Dynamixel.moveSpeed(1,servo_val,200); // program servo 
      
      }

      // RESET 
      else{
        }
  
  }
