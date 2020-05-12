void CommandData(String Com){
  int CommandLength;
  String FullCommand;

  if(Com.indexOf(';') == -1){ //if there isn't a ; then look for a carriage return and get the position of either of them
    CommandLength = Com.indexOf('\r');
  }
  else{
    CommandLength = Com.indexOf(';') - 1;
  }

FullCommand = Com.substring(0,CommandLength); //Then take the command extracted for later use
//Serial.println(FullCommand);

for(int e = 0; e < MaxComParam; e++){ //clean the string array from data from, the previous iteration 
    CommandParts[e] = "";
}


for(int i = 0; i < MaxComParam; i++){
  if(FullCommand.length() > 1){ //if the command still has usable data
  CommandParts[i] = FullCommand.substring(0, FullCommand.indexOf(' ')); //Save into the String array the first part of the command by looking for the first space 
  //Serial.println(CommandParts[i]);
  if(FullCommand.indexOf(' ') != -1){ //If there is still a space in the string 
  FullCommand.remove(0, FullCommand.indexOf(' ')+1); //then remove the string to read the following one
  }
  else{
    FullCommand.remove(0, FullCommand.length()); //if not just remove the rest of the command
  }
  //Serial.println(FullCommand);
  }
  if(FullCommand.length() <= 1){ //if there is no more usable data stop reading the loop
    break;
  }
}



//for(int g = 0; g < MaxComParam; g++){
//    if(CommandParts[g] == ""){
//      break;
//    }
//    Serial.print(CommandParts[g]);
//    Serial.print(" ");
//  }
//  Serial.println("");
}
