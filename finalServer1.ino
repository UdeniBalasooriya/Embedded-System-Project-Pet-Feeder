
#include <Stepper.h>
#include <SPI.h>
#include <WiFi.h>
#include <Hx711.h>

//------------------------------------------------------------
char ssid[] = "SuperBeam";      //  your network SSID (name)    // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
int alart=0;
WiFiServer server(8080);
byte supply=1;
char msg[600];
byte i;
char state[]="OK";
//indicator---------------------------------------------------
int indicator=A2;
int indicatorval = 0;
//stepper------------------------------------------------------
const int stepsPerRevolution = 330; 
Stepper myStepper(stepsPerRevolution ,3,4,5,6);            
//loadcell-------------------------------------------------------------
Hx711 scale(A4, A3);
//-------------------------------------------------------------
void setup() {
  //---------------------------------------------------------
  pinMode(indicator, INPUT);
  pinMode(1, OUTPUT);
  digitalWrite(1,HIGH);
  //-----------------------------------------------------------
  pinMode(12,OUTPUT);
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    //Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    //Serial.print("Attempting to connect to SSID: ");
    //Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(5000);
  } 
  i=0;
  server.begin();
  //----------------------------------------------------------
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  //-----------------------------------------------------------
  
}


void loop() {
  //Serial.println(analogRead(indicator));
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        msg[i] = client.read();
     
        if (msg[i] == '\n' && currentLineIsBlank) {
          // send a standard http response header
          httpPage(client);
           break;
        }
        if (msg[i] == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        
        else if (msg[i] != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
        i++;
      }
    }
    // give the web browser time to receive the data
    delay(10);
      // close the connection:
      client.stop();

      i=0;
     supply=check(msg);
     //-------------------------------------------------------stepper motor calling
   if(supply==0){
     rotate(10);
   }
   else if(supply==1){
     rotate(20);
   }
//-------------------------------------------------------------------------------   
  }
}
void httpPage(WiFiClient client){
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html><body bgcolor=\"#EEF262\">");
          // add a meta refresh tag, so the browser pulls again every 5 seconds:
          //client.println("<img src=\"/images/1.jpg\">");
          client.println("<font color='‪#4A0303‬'>"); 
          client.println("<br><center><FONT size = +6>Pet Feeder</FONT></center><br>");
          client.println("</font color>"); 
          client.println("<center><FONT size = +3>Supply Food</FONT></center><br>"); 
          client.println("<center><form action=\"http://192.168.43.140:8080/SMALL\">");
          client.println("<input type=\"submit\" value=\"Small amount\"/></form></center><br>");
          //client.println("<h3>Check foods</h3><br>"); 
          client.println("<center><form action=\"http://192.168.43.140:8080/LARGE\">");
          client.println("<input type=\"submit\" value=\"Large amount\"/></form></center><br><br>");
          client.print("<center><FONT size = +3>Food availability : </FONT>");
          if(analogRead(indicator)<250){client.println("<FONT size = +2>No Food</FONT>");}
          else{client.println("<FONT size = +2>Food available</FONT></br>");}
          if( -100 < scale.getGram() )client.println("<br><FONT size = +2>Plate is empty</FONT><br>");
          else client.println("<br><FONT size = +2>Food remains in the plate.</FONT></br>");
          client.println("</h3></center></body></html>");
}

byte check(char msg[]){
  byte x;
  for(x=50;x<200;x++){
  if(msg[x]=='S' && msg[x+1]=='M'&& msg[x+2]=='A' && msg[x+3]=='L' && msg[x+4]=='L'){return 0;}
  if(msg[x]=='L' && msg[x+1]=='A'&& msg[x+2]=='R' && msg[x+3]=='G' && msg[x+4]=='E'){return 1;}
  //else{Serial.println("not found\n");}
  //return 2;
  }
}
void rotate(int times){
  int j=0;
  for(j=0;j<times;j++){
  //Serial.println("clockwise");
   myStepper.step(stepsPerRevolution);
   //delay(10);
  
   //myStepper.step(-stepsPerRevolution);
   //delay(10);
  }
  
}
                                                                                                                  
