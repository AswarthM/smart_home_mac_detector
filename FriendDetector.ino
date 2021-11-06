#include "./esppl_functions.h"
#define numTrack 5
char srcmacchar[] = "00";
char destmacchar[] = "00";
String srcmac, destmac;
String knownMac[50] = {}; // {macAddress found}
String trackMac[numTrack][2] = {"20f478b12898", ""}; // {macAddress, isVisibleNow?}
unsigned long trackTime[numTrack] = {};
int numMac = 0; // number of mac addresses found/known
unsigned long timeNow = 0;

boolean macExists(String mac){
  for(int i = 0; i < numMac; i++){
    if(mac == knownMac[i]){
      return true;
    }
  }
  return false;
}

void addMac(String mac){
  numMac += 1;
  knownMac[numMac] = mac;
}

void cb(esppl_frame_info *info) {
  srcmac = "";
  destmac = "";
 // Serial.print("\n");
  
  for(int i = 0; i < 6; i++){
    sprintf(srcmacchar, "%02x", info->sourceaddr[i]);
    sprintf(destmacchar, "%02x", info->receiveraddr[i]);
    srcmac += srcmacchar;
    destmac += destmacchar;
  }
  //Serial.print(srcmac);

  if(srcmac == trackMac[0][0]){
    trackTime[0] = millis();
    trackMac[0][1] = "yes";
    Serial.println("Device visible");
  }
  if((unsigned long)(millis() - trackTime[0]) >= 5000){
    trackMac[0][1] = "no";
    Serial.println("Device invisible");
  }  

  if(macExists(srcmac)){
    Serial.print("known mac ");
    Serial.print(" SRC: ");
    Serial.print(srcmac);
    Serial.print(" DEST: ");
    Serial.println(destmac);
  }
  else{
    addMac(srcmac);
    Serial.print("unknown mac added to list ");
    Serial.print(" SRC: ");
    Serial.print(srcmac);
    Serial.print(" DEST: ");
    Serial.println(destmac);
  }
  
//  if(srcmac == "20f478b12898" || destmac == "20f478b12898"){
//    Serial.print("\n");
//    Serial.print(" SRC: ");
//    Serial.print(srcmac);
//    // Serial.printf("%02x", info->sourceaddr[i]);
//    Serial.print(" DEST: ");
//    Serial.print(destmac);
//    //  for (int i = 0; i < 6; i++) Serial.printf("%02x", info->receiveraddr[i]);
//    //  Serial.print(" RSSI: ");
//    //  Serial.print(info->rssi);
//    //  Serial.print(" SEQ: ");
//    //  Serial.print(info->seq_num);
//    //  Serial.print(" CHNL: ");
//    //  Serial.print(info->channel);
//    if (info->ssid_length > 0) {
//      Serial.print(" SSID: ");
//      for (int i = 0; i < info->ssid_length; i++) Serial.print((char) info->ssid[i]);    
//    }
//    
//  }
  
}

void setup() {
  delay(500);
  Serial.begin(115200);
  esppl_init(cb);
}

void loop() {
  esppl_sniffing_start();
  while (true) {
    for (int i = 1; i < 15; i++ ) {
      esppl_set_channel(i);
      while (esppl_process_frames()) {
        //
      }
    }
  }  
}
