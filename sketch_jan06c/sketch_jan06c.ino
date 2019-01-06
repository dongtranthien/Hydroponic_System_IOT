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

//Hardware of RLACS Node
#define PIN_D1    3
#define PIN_D2    4
#define PIN_A1    A0
#define PIN_A2    A1
//Hardware of RLTDS Node
#define PIN_D     5
#define PIN_TDS   A2
//Hardware of THL Node
#define PIN_T     A3
#define PIN_H     A4
#define PIN_L     A5

#define ID_NODE_LENGTH      2
//#define DEBUG
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

  #ifdef DEBUG
    Serial.println("GETDATA");
  #endif

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

  //OK, GetData Command correct. Now get data and Transfer data to sever  
  //Transfer String response 
  /*Format:
      RLACS: NODENAME_ID_DATA_D1_(ON/OFF)_D2_(ON/OFF)_A1_(int)_A2_(int)
      RLTDS: NODENAME_ID_DATA_D_(ON/OFF)_TDS_(int)
      THL:   NODENAME_ID_DATA_T_(int)_H_(int)_L_(int)
  */
  //Transfer Node name
  for(j = 0; j < 5; j++){
    if(node_name_str[NODE][j] == ' '){        //Break when array is null
      break;
    }
    Serial.print(node_name_str[NODE][j]);    
  }  
  Serial.print("_");
  Serial.print(ID_NODE);                      //Transfer ID Node
  Serial.print("_");
  Serial.print("DATA");                       //Transfer Data string
  Serial.print("_");
  //Transfer data of each node
  switch(NODE){
    case RLACS:{                              //RLACS Node Type
      //Get and transfer D1 data
      Serial.print("D1");
      Serial.print("_");
      if(digitalRead(PIN_D1) == HIGH){
        Serial.print("ON");
        Serial.print("_");
      }
      else{
        Serial.print("OFF");
        Serial.print("_");
      }
      //Get and transfer D2 data
      Serial.print("D2");
      Serial.print("_");
      if(digitalRead(PIN_D2) == HIGH){
        Serial.print("ON");
        Serial.print("_");
      }
      else{
        Serial.print("OFF");
        Serial.print("_");
      }
      //Get and transfer A1 data
      Serial.print("A1");
      Serial.print("_");
      Serial.print(analogRead(PIN_A1));
      Serial.print("_");
      //Get and transfer A2 data
      Serial.print("A2");
      Serial.print("_");
      Serial.print(analogRead(PIN_A2));
      break;
    }
    case RLTDS:{                              //RLTDS Node Type
      //Get and transfer D data
      Serial.print("D");
      Serial.print("_");
      if(digitalRead(PIN_D) == HIGH){
        Serial.print("ON");
        Serial.print("_");
      }
      else{
        Serial.print("OFF");
        Serial.print("_");
      }
      //Get and transfer TDS data
      Serial.print("TDS");
      Serial.print("_");
      Serial.print(analogRead(PIN_TDS)); 
      break;
    }    
    case THL:{                              //THL Node Type
      //Get and transfer T data
      Serial.print("T");
      Serial.print("_");
      Serial.print(analogRead(PIN_T)); 
      //Get and transfer H data
      Serial.print("H");
      Serial.print("_");
      Serial.print(analogRead(PIN_H)); 
      //Get and transfer L data
      Serial.print("L");
      Serial.print("_");
      Serial.print(analogRead(PIN_L)); 
      break;
    }
  }
  #ifdef DEBUG
    Serial.println("GETDATA OK - End Function");
  #endif
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
