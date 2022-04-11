#include <SoftwareSerial.h>
#include <DynamixelSerial.h>


//****************************************************************************************************************************************
/*
 *   Variables to set
 */
const int operating_mode = 0;                   // Choice of : 0 = potentiometer, 1 = serial

//****************************************************************************************************************************************



const int _BUFFER_SIZE = 3;       // number of inputs to recive over serial 
int _n_bytes;                     // variable to indicate to store number of bytes of data recieved over serial
//char *_buffer;               
byte Buffer[_BUFFER_SIZE];        // buffer to store info received over serial
int mode = 1; // operating mode: 1 = manual operation with potentiometer, 2 = operation via serial


SoftwareSerial SoftSerial(9, 8);  // software serial pins tx=9, rx=8

void setup()
{
  Dynamixel.begin(1000000,2);    // Init. Servo 1 @ 1Mbps and Pin Control 2
  Dynamixel.begin(1000000,3);    // Init. Servo 2 @ 1Mbps and Pin Control 2
  SoftSerial.begin(9600);
  Dynamixel.setEndless(1,OFF);
  Dynamixel.moveSpeed(1,500,200);// Init. servos at central position
  Dynamixel.moveSpeed(2,500,200);
}

void loop()
{

       

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

        
//        Serial.println(char(Buffer[0]), DEC);                     // print first character in buffer 
//        Serial.print(" ");


          if(float(char(Buffer[0]))>0){
            Serial.println("positive x position");
            Serial.end(); 
            Dynamixel.begin(1000000,2);
            Dynamixel.setEndless(1,OFF);
            Dynamixel.moveSpeed(1,200,200);
            Dynamixel.moveSpeed(2,400,200);
            //delay(200);
            Dynamixel.end();
            Serial.begin(9600); 
            }
          else{
            Serial.println("negative x position");
            Serial.end(); 
            Dynamixel.begin(1000000,2);
            Dynamixel.setEndless(1,OFF);
            Dynamixel.moveSpeed(1,400,200);
            Dynamixel.moveSpeed(2,200,200);
            //delay(200);
            Dynamixel.end();
            Serial.begin(9600); 
          }
        
       }
       Serial.end();                         // End the Serial Comunication
       Dynamixel.begin(1000000,2);           // Begin Servo C

}
