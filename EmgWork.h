class EMGWork{
private:
  int kol =0;
  int min[2],max[2];
  int vr;
  bool start = false;



public:
  long long unsigned int oldTime = millis();
  int ping = 0;

  int EMG[2];
  int maxEMG[2];
  int crut[2];
  int pot;

  void readSig(){
    EMG[0] = analogRead(A4);
    //EMG[0] = 0;
    EMG[1] = analogRead(A5);
  }

  int getPotenc(float Prc){
    Prc = Prc / 100;
    pot = (analogRead(A0) * Prc) + (pot*(1-Prc));
    return pot;
  }

  void flt_MX90_R(int Power,int num){
    if(start == false){
      min[0] = 9999; min[1] = 9999;
      max[0] = 0; max[1] = 0;
      start = true;
    }
    for(int i = 0; i < num && i < Power; i++){ 
      vr = analogRead(A4);
      if(min[0] > vr){min[0] = vr;}
      if(max[0] < vr){max[0] = vr;}
      vr = analogRead(A5);
      if(min[1] > vr){min[1] = vr;}
      if(max[1] < vr){max[1] = vr;}
      kol++;
      if(kol > Power){
        kol  = 0; 
        start = false;
        EMG[0] = max[0] - min[0];
        EMG[1] = max[1] - min[1]; 
      }
    }
  }

  void flt_MX90(int Power){
    min[0] = 9999; min[1] = 9999;
    max[0] = 0; max[1] = 0;
    for(int i = 0; i < Power; i++){
      EMG[0] = analogRead(A4);
      if(min[0] > EMG[0]){min[0] = EMG[0];}
      if(max[0] < EMG[0]){max[0] = EMG[0];}
      EMG[1] = analogRead(A5);
      if(min[1] > EMG[1]){min[1] = EMG[1];}
      if(max[1] < EMG[1]){max[1] = EMG[1];}
    }
    EMG[0] = max[0] - min[0];
    EMG[1] = max[1] - min[1];
  }

  short getSig(){
    short S;
    if(EMG[0] > maxEMG[0]){ S = -1;}
    else if(EMG[1] > maxEMG[1]){ S = 1;}
    else{S = 0;}    
    return S;    
  }

  void setMax(short MODE){
    if(MODE == 0){
      maxEMG[0] = -100;
      maxEMG[1] = -100;
    }
    else if(MODE == 1){
      const short crut = 100;
      if(maxEMG[0] < EMG[0]-crut){maxEMG[0] = EMG[0]+crut;}
      if(maxEMG[1] < EMG[1]-crut){maxEMG[1] = EMG[1]+crut;}
    }
  }

  int timePing(){
    int time = millis() - oldTime;
    oldTime = millis();
    return time;
  }

  void sig(){
    Serial.print(maxEMG[0]);
    Serial.print(",");
    Serial.print(maxEMG[1]);
    Serial.print(",");
    Serial.print(EMG[0]);
    Serial.print(",");
    Serial.print(EMG[1]);
    Serial.print(",");
    Serial.print(timePing());
    Serial.print(",");
    Serial.println(1100);
  }

};
