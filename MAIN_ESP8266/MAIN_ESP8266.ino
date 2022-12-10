//Include the needed library, we will use softer serial communication with the ESP8266
#include <SoftwareSerial.h>
#include <Keypad.h>
#include <SPI.h>
#include <MFRC522.h>
//LCD config
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
LiquidCrystal_I2C lcd(0x27,20,4);  //sometimes the LCD adress is not 0x3f. Change to 0x27 if it dosn't work.



//Define the used
//#define SS_PIN 10
//#define RST_PIN 9
//MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
#define ESP8266_RX 8  //Connect the TX pin from the ESP to this RX pin of the Arduino
#define ESP8266_TX 9  //Connect the TX pin from the Arduino to the RX pin of ESP


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////Variables you must change according to your values/////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Add your data: SSID + KEY + host + location + id + password
//////////////////////////////////////////////
const char SSID_ESP[] = "HUAWEI-G73q";         //Give EXACT name of your WIFI
const char SSID_KEY[] = "M28hAxQ4";             //Add the password of that WIFI connection
const char* host = "https://localhost/SmartLockerProject";  //Add the host without "www" Example: electronoobs.com
String NOOBIX_id = "999999999";                     //This is the ID you have on your database, I've used 99999 becuase there is a maximum of 5 characters
String NOOBIX_password = "4125879";               //Add the password from the database, also maximum 5 characters and only numerical values
String location_url = "/TX.php?id=";            //location of your PHP file on the server. In this case the TX.php is directly on the first folder of the server
                                                //If you have the files in a different folder, add thas as well, Example: "/ESP/TX.php?id="     Where the folder is ESP 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//const byte ROWS = 4; 
//const byte COLS = 4; 
//
//char hexaKeys[ROWS][COLS] = {
//  {'1', '2', '3', 'A'},
//  {'4', '5', '6', 'B'},
//  {'7', '8', '9', 'C'},
//  {'<', '0', 'C', 'D'}
//};
//
//byte rowPins[ROWS] = {A0, A1, 7, 6}; 
//byte colPins[COLS] = {5, 4, 3, 2}; 


//Used variables in the code
String url = "";
String URL_withPacket = "";    
unsigned long multiplier[] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
//MODES for the ESP
const char CWMODE = '1';//CWMODE 1=STATION, 2=APMODE, 3=BOTH
const char CIPMUX = '1';//CWMODE 0=Single Connection, 1=Multiple Connections


//Define the used functions later in the code, thanks to Kevin Darrah, YT channel:  https://www.youtube.com/user/kdarrah1234
boolean setup_ESP();
boolean read_until_ESP(const char keyword1[], int key_size, int timeout_val, byte mode);
void timeout_start();
boolean timeout_check(int timeout_ms);
void serial_dump_ESP();
boolean connect_ESP();
void connect_webhost();
unsigned long timeout_start_val;
char scratch_data_from_ESP[20];//first byte is the length of bytes
char payload[200];
byte payload_size=0, counter=0;
char ip_address[16];

//Variable to SEND to the DATABASE
bool sent_bool_1 = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Variable RECEIVED from the DATABASE
bool received_bool_1 = 0;
String received_text = "";


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////



//Store received chars in this variables
char t1_from_ESP[5];  //For time from web
char d1_from_ESP[2];  //For received_bool_1
char d13_from_ESP[6]; //For received_nr_5
char d14_from_ESP[300]; //For received_text 


//DEFINE KEYWORDS HERE
const char keyword_OK[] = "OK";
const char keyword_Ready[] = "Ready";
const char keyword_no_change[] = "no change";
const char keyword_blank[] = "#&";
const char keyword_ip[] = "192.";
const char keyword_rn[] = "\r\n";
const char keyword_quote[] = "\"";
const char keyword_carrot[] = ">";
const char keyword_sendok[] = "SEND OK";
const char keyword_linkdisc[] = "Unlink";

const char keyword_t1[] = "t1";
const char keyword_b1[] = "b1";
const char keyword_n6[] = "n6";
const char keyword_doublehash[] = "##";


SoftwareSerial ESP8266(ESP8266_RX, ESP8266_TX);// rx tx



void setup(){//        SETUP     START

//  lcd.init();                 //Init the LCD
//  lcd.backlight();            //Activate backlight   
  
  //Pin Modes for ESP TX/RX
  pinMode(ESP8266_RX, INPUT);
  pinMode(ESP8266_TX, OUTPUT);
  

  ESP8266.begin(115200);//default baudrate for ESP
  ESP8266.listen();//not needed unless using other software serial instances
  Serial.begin(115200); //for status and debug
  
  delay(1000);//delay before kicking things off
  setup_ESP();//go setup the ESP
//  SPI.begin();      // Initiate  SPI bus
//  mfrc522.PCD_Init();   // Initiate MFRC522
//  Serial.println("Approximate your card to the reader...");
//  Serial.println(); 
 
}




void loop(){



  send_to_server_1(); 
  digitalWrite(6,received_bool_1);

//  // Look for new cards
//  if ( ! mfrc522.PICC_IsNewCardPresent()) 
//  {
//    return;
//  }
//  // Select one of the cards
//  if ( ! mfrc522.PICC_ReadCardSerial()) 
//  {
//    return;
//  }
//  //Show UID on serial monitor
//  Serial.print("UID tag :");
//  String content= "";
//  byte letter;
//  for (byte i = 0; i < mfrc522.uid.size; i++) 
//  {
//     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
//     Serial.print(mfrc522.uid.uidByte[i], HEX);
//     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
//     content.concat(String(mfrc522.uid.uidByte[i], HEX));
//  }
//  Serial.println();
//  Serial.print("Message : ");
//  content.toUpperCase();
//  if (content.substring(1) == "BD 31 15 2B") //change here the UID of the card/cards that you want to give access
//  {
////    char customKey = customKeypad.getKey();
////    if (customKey){
////      Serial.println(customKey);
////    }
//    Serial.println("Authorized access");
//    Serial.println();
//    delay(3000);
//  }
// 
// else   {
//    Serial.println(" Access denied");
//    delay(3000);
//  }
//  
  send_to_server_5();
  digitalWrite(6,received_bool_1);
  
  delay(1000);

}//End of the main loop
