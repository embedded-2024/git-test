char data[8] = {300, 0xE0, 0xE8, 0xE1, 0xE9, 0xE7, 0xE3, 0xE0};
char buffer[8];

uint8_t d[10] = {300, 0xE0, 0xE8, 0xE1, 0xE9, 0xE7, 0xE3, 0xE0};

int aa;
String bb;
char cc;
bool ee;

String dd = "10000";
int length;

int i=0;

void setup() {
  Serial.begin(115200);

  length = dd.length();
  Serial.println(sizeof(d)); 
  Serial.println("############"); 
  Serial.println(sizeof(aa));
  Serial.println("############"); 
  Serial.println(sizeof(bb));
  Serial.println("############"); 
  Serial.println(sizeof(cc));
  Serial.println("############"); 
  Serial.println(sizeof(ee));
   Serial.println("############"); 
  // Serial.println(sizeof(ee));

}


void loop() {
  
  for (int i = 7 ; i >= 0 ; i--) {
    buffer[ 8 - i ] = data[i];
    Serial.print(buffer[8-i], HEX);
    Serial.print(" ");
  }

  Serial.println();

  // String dd = String(buffer);
  // Serial.println(dd);
  delay(1000);

}
