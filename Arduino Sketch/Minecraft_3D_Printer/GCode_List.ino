void G28(){
   Extruder.X = 0 + Offset.X;
   Extruder.Y = 0 + Offset.Y;
   Extruder.Z = 0 + Offset.Z;
}

void G1(GCommand Target){
  float m;
  float deltaX;
  float deltaY;
  Point ExtruderTemp;

  ExtruderTemp.X = Extruder.X;
  ExtruderTemp.Y = Extruder.Y;
  ExtruderTemp.Z = Extruder.Z;
  
if(Target.IgnoreCmd == false){ //Take Action
  if(Target.Extrude == true){ //Extrusion

    if(Target.NewX == true && Target.NewY == false && Target.NewZ == false){ //X only extrussion

      if(Extruder.X < Target.XPos){
      for(int i = Extruder.X; i <= Target.XPos; i++){
        ExtruderTemp.X = i;
//        Serial.print("X = ");
//        Serial.print(ExtruderTemp.X);
//        Serial.print(" Y = ");
//        Serial.println(ExtruderTemp.Y);
        Keyboard.println("");
        delay(delayT);
        Keyboard.println("/setblock ~" + String(int(ExtruderTemp.X)) + " ~" + String(int(ExtruderTemp.Z)) + " ~" + String(int(-1 * (ExtruderTemp.Y))) + " minecraft:gold_block replace");
        delay(delayT);
      }
    }
    else{
      for(int f = Extruder.X; f >= Target.XPos; f--){
        ExtruderTemp.X = f;
//        Serial.print("X = ");
//        Serial.print(ExtruderTemp.X);
//        Serial.print(" Y = ");
//        Serial.println(ExtruderTemp.Y);
        Keyboard.println("");
        delay(delayT);
        Keyboard.println("/setblock ~" + String(int(ExtruderTemp.X)) + " ~" + String(int(ExtruderTemp.Z)) + " ~" + String(int(-1 * (ExtruderTemp.Y))) + " minecraft:gold_block replace");
        delay(delayT);
      }
    }
      Extruder.X = ExtruderTemp.X;
    }

    if(Target.NewX == false && Target.NewY == true && Target.NewZ == false){ //Y only extrussion
      
      if(Extruder.Y < Target.YPos){
        for(int d = Extruder.Y; d <= Target.YPos; d++){
          ExtruderTemp.Y = d;
//        Serial.print("X = ");
//        Serial.print(ExtruderTemp.X);
//        Serial.print(" Y = ");
//        Serial.println(ExtruderTemp.Y);
        Keyboard.println("");
        delay(delayT);
        Keyboard.println("/setblock ~" + String(int(ExtruderTemp.X)) + " ~" + String(int(ExtruderTemp.Z)) + " ~" + String(int(-1 * (ExtruderTemp.Y))) + " minecraft:gold_block replace");
        delay(delayT);
         }
      }
      else{
        for(int k = Extruder.Y; k >= Target.YPos; k--){
        ExtruderTemp.Y = k;
//        Serial.print("X = ");
//        Serial.print(ExtruderTemp.X);
//        Serial.print(" Y = ");
//        Serial.println(ExtruderTemp.Y);
          Keyboard.println("");
          delay(delayT);
          Keyboard.println("/setblock ~" + String(int(ExtruderTemp.X)) + " ~" + String(int(ExtruderTemp.Z)) + " ~" + String(int(-1 * (ExtruderTemp.Y))) + " minecraft:gold_block replace");
          delay(delayT);
         }
      }
      
      Extruder.Y = ExtruderTemp.Y;
    }


    if(Target.NewX == true && Target.NewY == true && Target.NewZ == false){ //XY extrussion
      deltaY = Target.YPos - Extruder.Y;
      deltaX = Target.XPos - Extruder.X;
      m = (deltaY)/(deltaX);  
//      Serial.println("Delta X = " + String(deltaX) + "Delta Y = " + String(deltaY));
//      lcd.clear();
//      lcd.print("m = ");
//      lcd.println(String(m));

      if(abs(m) <= 1){
        
      if(Extruder.X < Target.XPos){
        
        for(int i = 0; i<= deltaX; i++){
          
          ExtruderTemp.X = i + Extruder.X;
          ExtruderTemp.Y = (m * i) + Extruder.Y;
//          Serial.print("X = ");
//          Serial.print(ExtruderTemp.X);
//          Serial.print(" Y = ");
//          Serial.println(ExtruderTemp.Y);
            Keyboard.println("");
            delay(delayT);
            Keyboard.println("/setblock ~" + String(int(ExtruderTemp.X)) + " ~" + String(int(ExtruderTemp.Z)) + " ~" + String(int(-1 * (ExtruderTemp.Y))) + " minecraft:gold_block replace");
            delay(delayT);
          }
          
      }
      else{
        for(int i = 0; i >= deltaX; i--){
          
          ExtruderTemp.X = i + Extruder.X;
          ExtruderTemp.Y = (m * i) + Extruder.Y;
//          Serial.print("X = ");
//          Serial.print(ExtruderTemp.X);
//          Serial.print(" Y = ");
//          Serial.println(ExtruderTemp.Y);
          Keyboard.println("");
          delay(delayT);
          Keyboard.println("/setblock ~" + String(int(ExtruderTemp.X)) + " ~" + String(int(ExtruderTemp.Z)) + " ~" + String(int(-1 * (ExtruderTemp.Y))) + " minecraft:gold_block replace");
          delay(delayT);
          }
          
      }
    }
    else{

      if(Extruder.Y < Target.YPos){
        for(int w = 0; w <= deltaY ; w++){
            ExtruderTemp.Y = w + Extruder.Y;
            ExtruderTemp.X = (w / m) + Extruder.X;
//          Serial.print("X = ");
//          Serial.print(ExtruderTemp.X);
//          Serial.print(" Y = ");
//          Serial.println(ExtruderTemp.Y);
            Keyboard.println("");
            delay(delayT);
            Keyboard.println("/setblock ~" + String(int(ExtruderTemp.X)) + " ~" + String(int(ExtruderTemp.Z)) + " ~" + String(int(-1 * (ExtruderTemp.Y))) + " minecraft:gold_block replace");
            delay(delayT);
        }
      }
      else{
        for(int s = 0; s >= deltaY; s--){
            ExtruderTemp.Y = s + Extruder.Y;
            ExtruderTemp.X = (s / m) + Extruder.X;
//          Serial.print("X = ");
//          Serial.print(ExtruderTemp.X);
//          Serial.print(" Y = ");
//          Serial.println(ExtruderTemp.Y);
            Keyboard.println("");
            delay(delayT);
            Keyboard.println("/setblock ~" + String(int(ExtruderTemp.X)) + " ~" + String(int(ExtruderTemp.Z)) + " ~" + String(int(-1 * (ExtruderTemp.Y))) + " minecraft:gold_block replace");
            delay(delayT);
        }
        
      }
      
    }

      
      Extruder.X = ExtruderTemp.X;
      Extruder.Y = ExtruderTemp.Y;
    }
    
      
  }
  else{ //Movement
    if(Target.NewX){
      Extruder.X = Target.XPos;
    }
    if(Target.NewY){
      Extruder.Y = Target.YPos;
    }
    if(Target.NewZ){
      Extruder.Z = Target.ZPos;
    }
  }
}
else{ //No Action
  
}
Serial.println("Extruder X = " + String(Extruder.X) + " Y = " + String(Extruder.Y) + " Z = " + String(Extruder.Z));
}
