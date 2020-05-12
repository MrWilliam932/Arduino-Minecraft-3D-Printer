GCommand CommandDecoder(String CommParts [MaxComParam]){
  GCommand dataReturn;
  dataReturn.Code = CommParts[0];
  dataReturn.NewX = false;
  dataReturn.NewY = false;
  dataReturn.NewZ = false;
  dataReturn.IgnoreCmd = true;
  dataReturn.Extrude = false;

Serial.print(dataReturn.Code);
//Serial.print(" ");

  for(int o = 0; o < MaxValidCmds; o++){ //look if the command is in the list of executable commands
    if(dataReturn.Code.equals(ValidCmds[o])){
      dataReturn.IgnoreCmd = false;
    }
  }

  if(dataReturn.IgnoreCmd == false){ //if the command is not ignored
  
  for(int h = 1; h < MaxComParam; h++){ //then look if it has parameters 
    if(CommParts[h] == ""){ 
      continue;
    }
    
    else{ //and if it has, then converts it to variables
      if(CommParts[h].startsWith("X")){
      CommandParts[h].remove(0,1);
      dataReturn.XPos = ((int)CommandParts[h].toFloat()) + Offset.X;
      if(abs(dataReturn.XPos - Extruder.X) >= 1){
      dataReturn.NewX = true;
      Serial.print(" X= ");
      Serial.print(dataReturn.XPos);
      }
      continue;
      }
      else if(CommParts[h].startsWith("Y")){
      CommandParts[h].remove(0,1);
      dataReturn.YPos = ((int)CommandParts[h].toFloat()) + Offset.Y;
      if(abs(dataReturn.YPos - Extruder.Y) >= 1){
      dataReturn.NewY = true;
      Serial.print(" Y= ");
      Serial.print(dataReturn.YPos);
      }
      continue;
      }
      else if(CommParts[h].startsWith("Z")){
      CommandParts[h].remove(0,1);
      dataReturn.ZPos = ((int)CommandParts[h].toFloat()) + Offset.Z;
      if(abs(dataReturn.ZPos - Extruder.Z) > 0){
      dataReturn.NewZ = true;
      Serial.print(" Z= ");
      Serial.print(dataReturn.ZPos);
      }
      continue;
      }
      else if(CommParts[h].startsWith("E")){
        CommandParts[h].remove(0,1);
        //if (CommandParts[h].toFloat() > 0){
        dataReturn.Extrude = true;
        Serial.print(" Extrude");
        continue;
        //}
      }

      else if((dataReturn.NewX == true || dataReturn.NewY == true || dataReturn.NewZ == true) && dataReturn.Extrude == false){//if the movement does not extrude any material...
          Serial.print(" Movement");
          continue;
      }
      
      else if(dataReturn.NewX == false && dataReturn.NewY == false && dataReturn.NewZ == false && dataReturn.Extrude == false){//if the command does not make any of these things ignore the command
        dataReturn.IgnoreCmd = true;
        Serial.print(" Ignored Command");
      }
      
      else{
        //ignore
        continue;
      }
    }   
  }
}
else{
  Serial.print(" Ignored Command");
}
Serial.println("");
return dataReturn; // at the end returns the data collected
}
