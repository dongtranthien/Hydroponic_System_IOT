//Not modified
enum node_type {
  RLACS,
  RLTDS,
  THL
};

//Modified
const unsigned char ID_NODE = 27;
const unsigned char NODE = RLACS;

char rlacs_node[5] = {'R','L','A','C','S'};
char rltds_node[5] = {'R','L','T','D','S'};
char thl_node[3] = {'T','H','L'};

char rec[200];
unsigned int index_rec = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    
    char inChar = (char)Serial.read();
    rec[index_rec] = inChar;
    if(rec[index_rec] == '\n'){
      int j, id_temp = 0;
      for(j = 0; j < 2; j++){
        if((index_rec - 2 + j) >= 0){
          id_temp = id_temp*10 + rec[index_rec - 2 + j] - 48;               
        }
      }
      if(id_temp == ID_NODE){
        unsigned char err = 0;
        
        switch(NODE){
          case RLACS:{
            for(j = 0; j < 5; j++){
              
            }
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
    index_rec++;    
    if(index_rec == 200) index_rec = 0;
    //Serial.print(inChar);
  }
}
