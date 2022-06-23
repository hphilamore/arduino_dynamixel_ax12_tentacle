
/* Dynamixel AX-12 servo motor
 * Operating Mode 0 - Position is mapped to value recived over Serial (e.g. from Isadora)
 * Operating Mode 1 - Position is mapped to analog value of potentiometer
 * Operating Mode 2 - Servo ID changed  
 * Operating Mode 3 - Perform autonomous motion sequence 
 * Operating Mode 4 - Reset to centre position 
 * 
 * Required libraries from: https://sourceforge.net/projects/dynamixelforarduino/files/
 */



// https://sourceforge.net/projects/dynamixelforarduino/files/
#include <DynamixelSerial.h> 
#include <SoftwareSerial.h>


//****************************************************************************************************************************************

//   Variables to set
// Choice of : 0 = serial, 1 = potentiometer, 2 = assign new ID, 3 = autonomous motion sequence , 4 = reset central position 
const int operating_mode = 3;     
const int id = 2;                 // servo ID 

//****************************************************************************************************************************************



const int _BUFFER_SIZE = 1;       // number of inputs to recive over serial 
int _n_bytes;                     // variable to indicate to store number of bytes of data recieved over serial
//char *_buffer;               
byte Buffer[_BUFFER_SIZE];        // buffer to store info received over serial
const int pot_pin = A4;
float pot_val;
int servo_max = 1023;                                             // Max value expected over serial
int servo_min = 0; 

SoftwareSerial SoftSerial(9, 8);  // software serial pins tx=9, rx=8

void setup()
{
  Dynamixel.begin(1000000,2);     // Init. Servo 1 @ 1Mbps and Pin Control 2
  SoftSerial.begin(9600);
  Dynamixel.setEndless(id,OFF);
//  Dynamixel.moveSpeed(id,511,200);// Init. servos at central position
//  pinMode(pot_pin, INPUT);        // Input pin for slider/ potentiometer 
//  delay(1000);
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
            int servo_val = map(x_pos, serial_min, serial_max, servo_min, servo_max);     // Map value to full range of servo
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
      int servo_val = map(pot_val, pot_min, pot_max, servo_min, servo_max);  // Map servo position to poentiometer value
      Serial.print("pot_val = ");
      Serial.print(pot_val);
      Serial.print(", servo_val = ");
      Serial.print(servo_val);
      Serial.print(", servo_pos = ");

      Serial.println("");
      Serial.end(); 
      Dynamixel.begin(1000000,2);           // Begin Servo Comms
      Dynamixel.setEndless(id,OFF);
      Dynamixel.moveSpeed(id,servo_val,200); // program servo 
 
      
      }

      // SET NEW ID NUMBER 
      else if(operating_mode == 2){
        //Dynamixel.setId (ID , newID );
        Dynamixel.setID (1 , 2);
      }


      // AUTONOMOUS SWEEP 
      else if(operating_mode == 3){

        float servo_1 = Dynamixel.readPosition(1); // Request Position
        float servo_2 = Dynamixel.readPosition(2); // Request Position
        float servo_3 = Dynamixel.readPosition(3); // Request Position

        // initialise servos to last position 
        Dynamixel.moveSpeed(1,servo_1,200); // program servo 
        Dynamixel.moveSpeed(2,servo_2,200); // program servo
        Dynamixel.moveSpeed(3,servo_3,200); // program servo
        
        //unsigned long Time = millis();     // write ms since start of recording to SD card
        const float scaler = 1.0/400;          // scale rotation speed in x plane  
   
        // position in servo range time t0
        unsigned long Time = millis();
        float servo_1_t0 = servo_pos(&sin, scaler, Time);
        float servo_2_t0 = servo_pos(&cos, scaler, Time);
        
        while(1){ 
          

          // position in servo range time t1
          unsigned long Time = millis();
          float servo_1_t1 = servo_pos(&sin, scaler, Time);
          float servo_2_t1 = servo_pos(&cos, scaler, Time);

          // find the difference in position value at t0 and t1
          float d1 = servo_1_t1 - servo_1_t0; 
          float d2 = servo_2_t1 - servo_2_t0; 

          // increment servo position 
          servo_1 += d1;
          servo_2 += d2;

          // position in servo range time t0
          servo_1_t0 = servo_1_t1; 
          servo_2_t0 = servo_2_t1; 

          // cap servo range at upper bound = servo max, lower bound = servo min
          if(servo_1 > servo_max){
            servo_1 = servo_max; // 2*servo_max - servo_1;
            }
          else if(servo_1 < servo_min){
            servo_1 = servo_min; // 2*servo_min - servo_1;
            }

          if(servo_2 > servo_max){
            servo_2 = servo_max; // 2*servo_max - servo_1;
            }
          else if(servo_2 < servo_min){
            servo_2 = servo_min; // 2*servo_min - servo_1;
            }
            
          // find servo 3 as a function of servo 2
          if(servo_1 < servo_max/2){
            servo_3 = servo_max;
            }
          else{
            servo_3 = servo_min;
            }

         Dynamixel.moveSpeed(1,servo_1,200); // program servo 
         Dynamixel.moveSpeed(2,servo_2,200); // program servo
         Dynamixel.moveSpeed(3,servo_3,200); // program servo
       

          Dynamixel.end();                      // End Servo comms
      
          Serial.begin(9600);                   // Begin Serial comms
          Serial.print("\t");
          Serial.print(servo_1);
          Serial.print("\t");
          Serial.print(servo_2);
          Serial.print("\t");
          Serial.print(servo_3);
          Serial.print("\t");
          Serial.println("");
          Serial.end();

          Dynamixel.begin(1000000,2);     // Init. Servo 1 @ 1Mbps and Pin Control 2

          //delay(200);

        }
        
      }

      // RESET TO CENTRE
      else{
        Dynamixel.moveSpeed(id,511,200);// Init. servos at central position
        pinMode(pot_pin, INPUT);        // Input pin for slider/ potentiometer 
        delay(1000);
        }
  
  }




float servo_pos(float (*callback)(float), float scaler, unsigned long Time) {
  //unsigned long Time = millis();     // write ms since start of recording to SD card 
   
  // convert time to more workable number using scaler
  float x = Time * scaler;  

  // map to range (-1, 1)
  float x_val = callback(x);  // x position    

  // map to servo range 
  float servo_pos = map(x_val*10, -10.0, 10.0, servo_min, servo_max);

  return servo_pos;
}
