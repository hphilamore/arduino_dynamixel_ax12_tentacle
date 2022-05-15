#include <DynamixelSerial.h>
#include <SoftwareSerial.h>


//****************************************************************************************************************************************

//   Variables to set

const int operating_mode = 1;     // Choice of : 0 = serial, 1 = potentiometer, 2 = assign new ID, 3 = reset to centre
const int id = 3;                 // servo ID 

//****************************************************************************************************************************************



const int _BUFFER_SIZE = 1;       // number of inputs to recive over serial 
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
  Dynamixel.setEndless(id,OFF);
  Dynamixel.moveSpeed(id,511,200);// Init. servos at central position
  pinMode(pot_pin, INPUT);       // Input pin for slider/ potentiometer 
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
            Dynamixel.setEndless(id,OFF);
            Dynamixel.moveSpeed(id,servo_val,200);                            // Program servo 
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
    }
  
    // POTENTIOMETER
    else if (operating_mode == 1){
      //int id = 3;
      
      Dynamixel.end();                      // End Servo comms
      
      Serial.begin(9600);                   // Begin Serial comms
      pot_val = analogRead(pot_pin);
      int pot_max = 1023;                   // Min value expected from potentiometer input
      int pot_min = 0;                      // Max value expected from potentiometer input
      int servo_val = map(pot_val, pot_min, pot_max, 0, 1023);  // Map value to full range of servo
      Serial.print("pot_val = ");
      Serial.print(pot_val);
      Serial.print(", servo_val = ");
      Serial.println(servo_val);
      Serial.end(); 
      
      //Dynamixel.moveSpeed(1,pot_val,200);
      //Dynamixel.moveSpeed(1,(pot_val-300)*10,200);
      Dynamixel.begin(1000000,2);           // Begin Servo Comms
      Dynamixel.setEndless(id,OFF);
      Dynamixel.moveSpeed(id,servo_val,200); // program servo 
      
      }

      else if(operating_mode == 2){
        //Dynamixel.setId (ID , newID );
        Dynamixel.setID (2 , 3 );
//      for(int i=1; i<255; i++){
//        Dynamixel.end();                      // End Servo comms
//        Serial.begin(9600);                   // Begin Serial comms
//        Serial.print(i);
//        Serial.print(" ");
//        Serial.println(Dynamixel.ping(i));
//        }
      }

      // RESET 
      else{
        }
  
  }
