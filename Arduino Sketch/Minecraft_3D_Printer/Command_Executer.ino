void CmdExec(GCommand Trgt){ //This function executes the correct command 

  if(Trgt.Code.equals("G1")){
    G1(Trgt);
  }
  if(Trgt.Code.equals("G28")){
    G28();
  }

}
