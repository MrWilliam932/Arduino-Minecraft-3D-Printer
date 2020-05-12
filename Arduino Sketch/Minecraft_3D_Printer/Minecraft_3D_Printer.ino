#include <SPI.h>
#include <SD.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "Keyboard.h"

#define SW1 6
#define SW2 5
#define SW3 4
#define EncDT 8
#define EncCLK 7
#define LED 9
#define BZ 18
#define SDPresent 19

#define MaxComParam 5
#define MaxValidCmds 2 //TBD

LiquidCrystal_I2C lcd(0x27,16,2);

bool fileAlreadyOpen = false;

bool SDCardState = false;
bool SDCardLastState = false;
bool SDCardStateChanged = false;

volatile bool EncDTState = false;
volatile bool EncNewCLK = false;
volatile int EncResult;
volatile int cuenta = 0;

String CommandParts [MaxComParam];
String ValidCmds [MaxValidCmds] = {"G1","G28"};

File dataFile;

String Command;
char c;

struct GCommand{
  String Code;
  int XPos;
  int YPos;
  int ZPos;
  bool NewX;
  bool NewY;
  bool NewZ;
  bool Extrude;
  bool IgnoreCmd;
};

struct Point{
  int X;
  int Y;
  int Z;
};

GCommand NextEndPoint = {"",0,0,0,0,0,0,0,1};

Point Extruder = {0,0,0};
Point Offset = {3,3,-22};

int delayT = 100;

void setup() {
  Keyboard.begin();
  Serial.begin(115200);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SDPresent, INPUT_PULLUP);
  pinMode(EncDT, INPUT);
  pinMode(EncCLK, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(BZ, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(EncCLK), EncState, FALLING); //Interrupt for reading the encoder
  
   lcd.init();
   lcd.backlight();

   lcd.print("   Minecraft    ");
   lcd.setCursor(0,1);
   lcd.print("   3DPrinter    ");

   delay(2000);
   lcd.clear();
//Initialize the Extruder Position with the offset
   Extruder.X = Extruder.X + Offset.X; 
   Extruder.Y = Extruder.Y + Offset.Y;
   Extruder.Z = Extruder.Z + Offset.Z;

}

void loop() {
  
  if(SDCardInit() == true && fileAlreadyOpen == false){ //SDCard Init routine and datafile opening
   dataFile  = SD.open("Sharkie.gco");
   lcd.print("file opened ");
   fileAlreadyOpen = true;
  }

  if (dataFile && fileAlreadyOpen == true) { //If the file was opened then start reading the file
    while (dataFile.available()) {
      c = dataFile.read(); 
      Command += c; //Assemble the command to be decoded
      if(c == '\r'){ //if it reads a carriage return then
        dataFile.read(); //removes the new line character
        if(Command.startsWith("G")/*|| Command.startsWith("W")*/){ //If the command starts with the configured letters then start decoding if not, the command is deleted and the program continues to read the SD card
          
        CommandData(Command); //This function separates the Commands into various strings containing the Command itself and its parameters
        NextEndPoint = CommandDecoder(CommandParts); //This function takes the command and it's parameters and converts them into coordinates
        CmdExec(NextEndPoint); //This function executes the commands.
                
        Command = "";
        }
        else{
          Command = "";
        }
      }
    
    }
    dataFile.close();
  }
  
//  else if (fileAlreadyOpen == true) {
//    Serial.println("error opening file");
//  }
}
