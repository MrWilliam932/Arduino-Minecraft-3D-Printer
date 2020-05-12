bool SDCardInit(){
SDCardState = !digitalRead(SDPresent); //check if the card is phisically plugged in
bool SDCardOK; 
if(SDCardState != SDCardLastState){ //if the card changed state
  SDCardLastState = SDCardState;
  SDCardStateChanged = true;
}
  
if (SDCardStateChanged && !SDCardState) {//and it's not in the socket
    lcd.clear();
    SDCardStateChanged = false;
    Serial.println("SD Card not present"); //print a message
    lcd.print("No SD Card");
    fileAlreadyOpen = false;
    SDCardOK = false; //Then the SD card is not usable
    delay(1000);  
    lcd.clear();
  }
  else if(SDCardStateChanged && SDCardState){ //if the card is in place
  lcd.clear();
  SDCardStateChanged = false;
  delay(100);
  if(SD.begin(10)){ //then try to initialize it, if it succeeds...
  Serial.println("Card initialized."); 
  lcd.print("SD Card Found");
  SDCardOK = true; // the card is ok and ready to be used
  delay(1000);  
  lcd.clear();
  }
  else{ //if not then the card is damaged
    lcd.clear();
    SDCardStateChanged = false;
    Serial.println("Card failed or bricked");
    lcd.print("SD Card Damaged");
    SDCardOK = false;
    delay(1000);  
    lcd.clear();
  }
  }

  return SDCardOK;
}
