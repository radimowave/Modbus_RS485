#include <SoftwareSerial.h>
#include <ModbusMaster.h> //https://4-20ma.io/ModbusMaster/class_modbus_master.html


#define  ID 3  // Modbus slave ID= 3

SoftwareSerial mySerial(2, 3); // RX PIN 2 Uno, TX PIN 3 Uno

#define MAX485_DE      4 //PIN 4 Uno
#define MAX485_RE_NEG  5 //PIN 5 Uno
// จริง ๆ ใช้ขาเดียวกันได้ เพราะปกติจะ set 0 หรือ 1 พร้อมกัน

ModbusMaster Meta;

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1); // Set High
  digitalWrite(MAX485_DE, 1); // Set High
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0); // Set LoW
  digitalWrite(MAX485_DE, 0); // Set LoW
}

void setup()
{
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);

  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
  Serial.println("start init serial 0");
  Serial.begin(9600);
  
  while (!Serial) {
    Serial.println("loop for init serial 0"); 
  }
  Serial.println("start init software serial");
  mySerial.begin(9600);
  while (!mySerial) {
    Serial.println("loop for init software serial");
  }
  Meta.begin(ID, mySerial);  // 
  Meta.preTransmission(preTransmission);
  Meta.postTransmission(postTransmission);

}

void loop()
{
  uint8_t result;
  uint16_t data[2]; //เก็บข้อมูล 2 ค่า
   
  Serial.println("Get data from Sensor (XY-MD02)");
   Serial.println("*****************************");
  result = Meta.readInputRegisters(1, 2); ///< Modbus function 0x04 Read Input Registers 
  //readInputRegisters	(	uint16_t 	u16ReadAddress, uint8_t u16ReadQty)	

  if (result == Meta.ku8MBSuccess)
  {
    Serial.print("Temperature (C): ");
    Serial.println(Meta.getResponseBuffer(0)/10.0f);  // getResponseBuffer	(	uint8_t 	u8Index	)	, //Retrieve data from response buffer
   

    Serial.print("Humidity (%): ");
    Serial.println(Meta.getResponseBuffer(1)/10.0f); //< buffer to store Modbus slave response; read via GetResponseBuffer()
    Serial.println();
  
  }
  delay(4000);
}
