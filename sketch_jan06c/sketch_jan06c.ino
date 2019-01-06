#include <Arduino.h>
#include <HardwareSerial.h>
#if defined(UBRRH) || defined(UBRR0H)
  extern HardwareSerial Serial;
  #define HAVE_HWSERIAL0
#endif

//Not modified
enum node_type {
  RLACS = 0,
  RLTDS,
  THL
};

//Modified
const unsigned char ID_NODE = 27;
const unsigned char NODE = RLACS;

#define ID_NODE_LENGTH      2
#define DEBUG
char rlacs_node[5] = {'R','L','A','C','S'};
char rltds_node[5] = {'R','L','T','D','S'};
char thl_node[3] = {'T','H','L'};
char node_name_str[3][5] = {{'R','L','A','C','S'},
                            {'R','L','T','D','S'},
                            {'T','H','L',' ',' '}};
unsigned char node_length[3] = {5,5,3};

#define TESTCONNECT_LEN     11
char command_testConnect[TESTCONNECT_LEN] = {'T','E','S','T','C','O','N','N','E','C','T'};
#define GETDATA_LEN         7
char command_getData[GETDATA_LEN] = {'G','E','T','D','A','T','A'};

char rec[200];
unsigned int index_rec = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void CheckConnect(){
  int j, id_temp = 0;
  for(j = 0; j < 11; j++){
    if((index_rec - 1 - j) >= 0){      
      if(rec[index_rec - 1 - j] != command_testConnect[10 - j]){
        #ifdef DEBUG
          Serial.println("CheckConnect Error - Check String 'Check Connect'");
        #endif
        return;
      }
    }
    else{
      if(rec[200 - index_rec + 1 + j] != command_testConnect[11 - j]){
        #ifdef DEBUG
          Serial.println("CheckConnect Error - Check String 'Check Connect'");
        #endif
        return;
      }
    }
  }

  for(j = 0; j < 2; j++){
    if((index_rec - 13 + j) >= 0){
      id_temp = id_temp*10 + rec[index_rec - 13 + j] - 48;               
    }
    else{
      id_temp = id_temp*10 + rec[200 - index_rec + 13 - j] - 48;     
    }
  }
  if(id_temp == ID_NODE){
    #ifdef DEBUG
      Serial.println("OK ID NODE");        
    #endif
    
    switch(NODE){
      case RLACS:{
        for(j = 0; j < 5; j++){
          if((index_rec - 14 - j) >= 0){
            if(rec[index_rec - 14 -  j] != rlacs_node[4 - j]){
              return;
            }
          }
        }        
        Serial.println("OK");        
        break;
      }
      case RLTDS:{
        
        break;
      }
      case THL:{
        
        break;
      }
    }
  }   
}

void GetData(){
  int j;
  //Calculate Length of String Receive 
  int len = GETDATA_LEN + ID_NODE_LENGTH + node_length[NODE];

  //Check String "GetData" command of string receive
  for(j = GETDATA_LEN; j > 0; j--){    
    if(rec[index_rec - GETDATA_LEN + j - 1] != command_getData[j - 1]){ //Compare
      #ifdef DEBUG 
        Serial.println("Error-GetData-Command Incorect");
      #endif
      return;
    }
  }
  
  //Check Name of Node on string receive, if different then return
  for(j = node_length[NODE]; j > 0; j--){    
    if((index_rec - len + j - 1) >= 0){                                 //Check index over max array (rec)      
      if(rec[index_rec - len + j - 1] != node_name_str[NODE][j - 1]){   //Check character of string receive vs node name string
        #ifdef DEBUG 
          Serial.println("Error-GetData-Node Name Incorect");
        #endif
        return;
      }
    }
    else{
      if(rec[200 - index_rec + len - j - 1] != node_name_str[NODE][j - 1]){
        #ifdef DEBUG 
          Serial.println("Error-GetData-Node Name Incorect");
        #endif
        return;
      }
    } 
  }

  //Check ID Node Number of node
  int node_index_temp = 0;    //init id node index data  
  //Calculate id Node number from string
  for(j = 0; j < ID_NODE_LENGTH > 0; j++){
    node_index_temp = node_index_temp*10 + rec[index_rec - len + node_length[NODE] + j] - 48;    
  }  
  
  //Compare ID Number from String receive vs ID Node(Setup)
  if(node_index_temp != ID_NODE){
    #ifdef DEBUG 
      Serial.println("Error-GetData-IDNode Incorect");
    #endif
    return;
  }

  Serial.println("GETDATA OK");
}

void UpdateData(){

}
void serialEvent() {
  while (Serial.available()) {    
    char inChar = (char)Serial.read();
    rec[index_rec] = inChar;
    if(rec[index_rec] == '\n'){
      CheckConnect();
      GetData();
    }
    index_rec++;    
    if(index_rec == 200) index_rec = 0;
    //Serial.print(inChar);
  }
}
