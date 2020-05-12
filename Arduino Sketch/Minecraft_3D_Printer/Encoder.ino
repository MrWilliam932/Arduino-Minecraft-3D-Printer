void EncState(){
  EncDTState = digitalRead(EncDT);
      
      if(EncDTState == 0){
      EncResult = 1;
      EncNewCLK = true;
      }
      if(EncDTState == 1){
      EncResult = 0;
      EncNewCLK = true;
      }


}
