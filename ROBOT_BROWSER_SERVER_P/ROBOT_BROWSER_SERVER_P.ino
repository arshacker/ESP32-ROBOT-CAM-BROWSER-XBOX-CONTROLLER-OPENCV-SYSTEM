/*********
  This program, the Server resident on ESP32 Camera, is taken from the reference cited below, with bo minor modifications and some
  significant additions. The include file, index_OCV_ColorTrack.h, the Client, is an intoduction of OpenCV.js to the ESP32 Camera environment. The Client was
  developed and written by Andrew R. Sass. Permission to reproduce the index_OCV_ColorTrack.h file is granted free of thcharge if this
  entire copyright notice is included in all copies of the index_OCV_ColorTrack.h file.
  
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp32-cam-projects-ebook/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

//ANN 0
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "index_OCV_ColorTrack.h"

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h> //CANT INCR MAX QUEUED MSGS: BOARD PROB

//#define SOFTAP

//ANN 2
#include <Wire.h>    //comm to arduino
#define myWire Wire
#define SDA 14
#define SCL 13

#define SCREENCOLS 400
#define SCREENROWS 300

#define CODE_LEN 7
int code_array[CODE_LEN] = {0,0,0,0,0,0,1};
int code_nbr = 1;

//#define MSG_LEN 7  //
#define MSG_INT_LEN 5
#define MSG_LEN  2*MSG_INT_LEN-1

//char msg[MSG_LEN] = {0,};
//int msg_int[4] = {97,98,127,1}; // {97,98,127};

//char msg[MSG_LEN] = {0,};
char msg[MSG_INT_LEN] = {0,};
int msg_int[MSG_INT_LEN] = {97,98,127,1}; // {97,98,127};
                    //ALLOW POS INTEGERS 0-127

int One_Time_Transmit = 1;

int lock = 0;
char cTT[256] = {0,}; 
//char c[] = {0,};
byte d[256] = {0,};
char e;

int lenGlobal=0;

byte bufIP[4] = {0,0,0,0};  //i2c xmit
byte bufToBot[20] = {0,};  //bufToBot[19] = {0,};
byte buffer[40];             //i2c rcv(prev said transmit?
byte buffer_temp[40];    //check contents before xfr to buffer
int n = 0;                   //i2c rcv
 
int finalIndex = 0;
int initialIndex = 0;
int kIndex = 0;

long timeStart;
long timeFinish;
long timeStart1;
long timeFinish1;

//int XcmVal = 0;
//int YcmVal = 0;
//int Tracker = 0;
int DELTA_XCM = 0;
int DELTA_YCM = 0;
byte b_Tracker = 0;
byte b_DELTA_XCM = 0;
byte b_DELTA_YCM = 0;

String HTTP_req;
int ajax_counter = 0;
int page_counter = 0;
int analog_val = 0;
bool dig_val = false;
int analog_val_1 = 10;
bool dig_val_1 = false;
int analog_val_2 = 0;
bool dig_val_2 = true;
unsigned long Ti = 0;
unsigned long Tf = 0;
#define INTERVAL 100


//ANN 1
// Replace with your network credentials
const char* ssid = "ssid";
const char* password = "password";
const char* ssidAP = "ssidAP"; // ars  soft AP
const char* passwordAP = "passwordAP"; //ars softAP

 
String Feedback="";
String Command="",cmd="",P1="",P2="",P3="",P4="",P5="",P6="",P7="",P8="",P9="";
byte ReceiveState=0,cmdState=1,strState=1,questionstate=0,equalstate=0,semicolonstate=0;

//ANN:3
//       T-JOURNAL                     AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     27     //         0
#define SIOD_GPIO_NUM     25     //        26
#define SIOC_GPIO_NUM     23     //        27
  
#define Y9_GPIO_NUM       19     //        35
#define Y8_GPIO_NUM       36     //        34
#define Y7_GPIO_NUM       18     //        39
#define Y6_GPIO_NUM       39     //        36
#define Y5_GPIO_NUM        5     //        21
#define Y4_GPIO_NUM       34     //        19
#define Y3_GPIO_NUM       35     //        18
#define Y2_GPIO_NUM       17     //         5
#define VSYNC_GPIO_NUM    22     //        25
#define HREF_GPIO_NUM     26     //        23
#define PCLK_GPIO_NUM     21     //        22

WiFiServer server(80);
AsyncWebServer server3(82);

AsyncWebSocket ws("/ws");

/**********************start get ajax data***********************************/
// send the state of the switch to the web browser
void GetAjaxData(WiFiClient cl1)
{
    //unsigned long INTERVAL = 100;

/****  
    Tf = millis();
    if((Tf-Ti)>=INTERVAL){
      analog_val = analog_val + 1;
      analog_val = analog_val % 128;
      dig_val = true ^ dig_val;
      Ti = millis();
    }
****/  
    //analog_val = analog_val + 1;
//    analog_val = analog_val % 128;
/***********/    
    ajax_counter = ajax_counter + 1;
    //dig_val = true ^ dig_val;
    
    Serial.println("AJAX_SEND");
    Serial.print("AJAX_COUNTER = ");
    Serial.println(ajax_counter);

    //Serial.print("DIG_VAL = ");
    //Serial.println(dig_val);
/****************/
    /*    
    //send standard header response
    cl.println("HTTP/1.1 200 OK");
    cl.println("Content-Type: text/html");
    cl.println("Connection: keep-alive");
    cl.println();    
    */
    //cl.println("{\"SWITCH_STATE\":1,\"ANALOG\":66} "); //DUMMY JSON WORKS!!!
    
    /******
    cl.println("{\"SWITCH_STATE\":");    
    cl.print(dig_val);
    cl.print(",\"ANALOG\":");    
    cl.print(analog_val);  
    cl.println("}");
    *******/
    
    /*
    cl.println("<script>");
    cl.println("var GMIN = 5");
    cl.println("var GMINslider = document.getElementById(\"gmin\");");
    cl.println("var GMINoutput = document.getElementById(\"GMINdemo\");");
    cl.println("GMINoutput.innerHTML = 50;");
    cl.println("GMINslider.oninput = function(){");
    cl.println("  GMINoutput.innerHTML = 50;");
    cl.println("  GMIN = parseInt(GMINoutput.innerHTML,10);}");         
    cl.println("</script>");
    */
}
/*********************end get ajax data**************************************/


 /************************start i2c transmit*************************************/
//ANN 4
  void i2cTransmit(void){
                /************SEND I2C DATA**********************/
    //Serial.println(F("Test with 1 transmissions of writing 10 bytes each"));
  //byte buf[20] = { 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, };
  byte buf[27] = { 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126};
  int err = 0;
  unsigned long millis1 = millis();
  boolean firsterr = false;
  //for( int i=0; i<1; i++)
  //{
    Serial.println(F("Sending data"));
    timeStart1 = millis();
    myWire.beginTransmission(4);

    for(int z=0; z<=3;z++){
      buf[z]=bufIP[z];
    }
    for(int v=0; v<=19; v++){
      buf[v+4]=bufToBot[v];  
    }
    buf[24] = b_DELTA_XCM;
    buf[25] = b_DELTA_YCM;
    buf[26] = b_Tracker;
    
    Serial.print("BUF = "); 
    for( int k=0; k<27; k++)
    {
    Serial.print( (int) buf[k]);
    Serial.print(F(", "));
    }
    Serial.println();
        
    myWire.write( buf, 27);       //replace with bufToBot from client
    //myWire.write(bufToBot,5);
    if( myWire.endTransmission() != 0)
    {
      err++;
    }
    delayMicroseconds(100);  // Even the normal Arduino Wire library needs some delay when the Slave disables interrupts.
 // }
    
     myWire.beginTransmission(6);
     myWire.write( buf, 27);
    if( myWire.endTransmission() != 0)
    {
      err++;
    }
    delayMicroseconds(100);  // Even the normal Arduino Wire library needs some delay when the Slave disables interrupts.
     
     
  unsigned long millis2 = millis();
  Serial.print(F("total time: "));
  Serial.print(millis2 - millis1);
  Serial.print(F(" ms, total errors: "));
  Serial.println(err);

  delay(2);
  
  timeFinish1 = millis();
  Serial.print("INNER TIME  = ");
  Serial.println(timeFinish1-timeStart1);

                             /**END SEND I2C DATA*****/

                             /**REQUEST I2C DATA******/
  
  Serial.println(F("Requesting data"));
  n = myWire.requestFrom(4, 10);    // request bytes from Slave //make n a global
  Serial.print(F("n="));
  Serial.print(n);
  Serial.print(F(", available="));
  Serial.println(myWire.available());
  
//  myWire.printStatus(Serial);      // This shows information about the SoftwareWire object.

//  byte buffer[40];         //make this a global for access by cmd func
//  for( int j=0; j<n; j++)
//    buffer[j] = myWire.read();
  myWire.readBytes( buffer_temp, n);

  Serial.print("RCV_BUFFER_TEMP = ");
  for( int k=0; k<n; k++)
  {
    if( k == 0)
      Serial.print(F("*"));      // indicate the number of the counter
    Serial.print( (int) buffer_temp[k]);
    Serial.print(F(", "));
  }
  Serial.println();

  if(buffer_temp[0]<128){                //NO ERROR
    for( int k=0; k<n; k++){
      buffer[k] = buffer_temp[k];
    }
  }

  Serial.print("RCV_BUFFER = ");
  for( int k=0; k<n; k++)
  {
    if( k == 0)
      Serial.print(F("*"));      // indicate the number of the counter
    Serial.print( (int) buffer[k]);
    Serial.print(F(", "));
  }
  Serial.println();
  
                                /***END REQUEST I2C DATA*********/                
  delay(2);
    
  }
 
 /***********************end i2c transmit**********************************/
//ANN 5
void codePrep(void){
  code_nbr = 2*2*2*2*2*2*code_array[0] + 2*2*2*2*2*code_array[1] +
             2*2*2*2*code_array[2] + 2*2*2*code_array[3]+
             2*2*code_array[4] + 2*code_array[5] + code_array[6];
  Serial.print("CODE NBR = ");
  Serial.println(code_nbr);                        
}

//ANN 6
void msgPrep(void){
  //takes msg_int[3]={X,Y,Z] and creates msg[X,',',Y,',',Z} WHERE MSG_LEN = 5
/*  
  for(int i = 1; i < MSG_LEN-2; i = i + 1){
    msg[(2*i)-1] = ',';  //comma separators for python
                         //msg[1],msg[3]...
  }  
  for(int j = 1; j < MSG_LEN-1; j = j + 1){
    if(buffer[(j-1)]!=0){
        msg[(2*j)-2] = buffer[(j-1)];
    }
    else{
        msg[(2*j)-2] = 1;     //cant transmit 0 to python?????
    }
    //msg[(2*j)-2] = msg_int[(j-1)]; //prepared message
      //msg[0]=msg_int[0],msg[2]=msg_int[1],msg[4]=msg_int[2]...
  }
 */
 //below is most recent before new strategy
 /*
  for(int i = 1; i<MSG_INT_LEN+1; i = i + 1){
    msg[(2*i)-1] = 2;  //MARKER 2 SAYS IT IS NOT ZERO
  }

  for(int i = 0; i<MSG_INT_LEN; i = i + 1){
    if (buffer[i]!=0){
      msg[2*i]=buffer[i];
    }
    else{
      msg[2*i]=1;  //cant transmit 0 (null) to python
      msg[(2*i)+1]=3;  //MARKER 3 SAYS IT SHOULD BE ZERO
    }
  }
  */

  for(int j=0; j<CODE_LEN-1; j=j+1){   //leave last location as 1
    code_array[j] = 0;                 //code_array(CODE_LEN-1)==1
  }

  for(int i = 0; i<MSG_INT_LEN; i = i + 1){
    if (buffer[i]!=0){        
      msg[i]= buffer[i];
      code_array[i] = 0;
    }
    else{
      msg[i] = 1;  //cant transmit 0 (null) to python
      code_array[i] = 1;     
    }
  }

  codePrep();
  msg[MSG_INT_LEN-1] = (char)code_nbr; //put code into MSG_INT_LEN location
  
  //Serial.print("BUFFER BUFFERR BUFFER ===");
  //Serial.println(buffer[1]); //prints a
  //msg[2] = '\x01';
  //msg[1] = 5;
  //msg[0] = buffer[0];
  //msg[2] = buffer[1];
  //msg[4] = buffer[2];
  //msg[6] = buffer[3];
}

//ANN 7
void onWsEvent(AsyncWebSocket * server3, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){

  //msgPrep(); //comma separators  //put in ws_evt_data

  //int msg_int[] = {97,',',98,',',127}; //commas separators useful in python
  //char msg[5] = {0,}; 
  //char msg[] = {'a','b','c'};
  //char msg[] = {97,98,99};
  //char msg[] = {97,',',98,',',99};
  //char msg[] = {97,',',98,',',31};

  //msg[0] = msg_int[0]; //int to char for transmission
  //msg[1] = msg_int[1];
  //msg[2] = msg_int[2];
  //msg[3] = msg_int[3];
  //msg[4] = msg_int[4];
 
  lenGlobal = len;
  
  if(type == WS_EVT_CONNECT){
 
    Serial.println("Websocket client connection received");
    client->text("Hello from ESP32 Server3");
    //client->binary(msg); //move to ws_evt_data
 
  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("Client disconnected");
 
  }else if(type==WS_EVT_DATA){
    Serial.println("Data received: ");

    for(int j=0; j < len; j++){
      d[j] = data[j];          //data read out only once allowed
      cTT[j] = d[j];  //ascii to char

            
      Serial.print(cTT[j]);   //TIMING
    }//end for j loop
    Serial.println();
    Serial.println(len);
    
    
    
    for(int i=0; i < len; i++){
      Serial.print(d[i]);     //TIMING
      Serial.print("|");
    }
    Serial.println();

    parse_msg();           //convert msg from python to array format
    initializeTT();


    msgPrep(); //comma separators
    client->binary(msg);  //return msg to python

    Serial.println("I2C Transmit");
    i2cTransmit();
    initializeTT();    

   }//end else if ws evt data
}//end onwsevent


void initializeTT(void)
{
  for(int n=0;n<256;n++){
    cTT[n] = 0;
    d[n] = 0;
   }
}

/*
void initialize(void)
{
  for(int n=0;n<255;n++)
  {c[n] = 0;}
}
*/

//ANN 8
void parse_msg(void){

   Serial.print("cTT = ");
   for(int w=0; w<lenGlobal;w=w+1){
      Serial.print(cTT[w]);
   }
   Serial.println();
   
   String var = String(cTT);
   var = String('[') + var + String(']');
   Serial.print("var = ");
   Serial.println(var);
 
    /********************parse on comma*********************************/
  /*******************orig parse****************************
  int index1 = var.indexOf(',',0);
  int index2 = var.indexOf(',',index1+1);
  int index3 = var.indexOf(',',index2+1);
  int index4 = var.indexOf(',',index3+1);
  int index5 = var.indexOf(',',index4+1);
  
  int indexEND = var.indexOf('?');  //will return -1
  Serial.println(indexEND);
  String X_VALUE = var.substring(1,index1); //omit the [
  String Y_VALUE = var.substring(index1+1,index2);
  String A_VALUE = var.substring(index2+1,index3);
  String B_VALUE = var.substring(index3+1,index4);
  String C_VALUE = var.substring(index4+1,index5);


  Serial.println(X_VALUE);
  Serial.println(Y_VALUE);
  Serial.println(C_VALUE);
  int x_value_int = X_VALUE.toInt();
  x_value_int++;
  Serial.println(x_value_int);
  bufToBot[0] = X_VALUE.toInt();
  bufToBot[1] = Y_VALUE.toInt();
  bufToBot[2] = A_VALUE.toInt();
  bufToBot[3] = B_VALUE.toInt();
  //bufToBot[4] = C_VALUE.toInt(); //SEND COUNT TO BOT only goes to 255,then starts again
  ************************end orig parse*****************************/
  
  /****************************new parse*******************************/
  initialIndex = 1;  //IGNORE THIS start initialIndex at 1 b/c python transmits [ .
  kIndex = 0;
  finalIndex = 0;
  while(finalIndex!=-1){
    finalIndex = var.indexOf(',',initialIndex); 
    bufToBot[kIndex] = var.substring(initialIndex,finalIndex).toInt();
    //Serial.println(bufToBot[kIndex]);
    if(finalIndex==-1){
      //Serial.println(bufToBot[kIndex]);
      initialIndex = 0;
      finalIndex = 0;
      kIndex = 0;
      break;}
    initialIndex = finalIndex+1;
    kIndex++;
  }
/*************************end new parse************************************/
  Serial.print("BUFTOBOT = ");
  for(int m=0;m<20;m++){      //NEW
    Serial.print(bufToBot[m]);
    Serial.print(" ");
  }
  Serial.println(); 
    /*********************end parse on comma****************************/  
}

//ANN:9
void ExecuteCommand(WiFiClient cl) {
  //int Tracker = 0;
  if (cmd!="colorDetect") {  //Omit printout
    //Serial.println("cmd= "+cmd+" ,P1= "+P1+" ,P2= "+P2+" ,P3= "+P3+" ,P4= "+P4+" ,P5= "+P5+" ,P6= "+P6+" ,P7= "+P7+" ,P8= "+P8+" ,P9= "+P9);
    //Serial.println("");
  }
  
  if (cmd=="resetwifi") {
    WiFi.begin(P1.c_str(), P2.c_str());
    Serial.print("Connecting to ");
    Serial.println(P1);
    long int StartTime=millis();
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        if ((StartTime+5000) < millis()) break;
    } 
    Serial.println("");
    Serial.println("STAIP: "+WiFi.localIP().toString());
    Feedback="STAIP: "+WiFi.localIP().toString();
  }    
  else if (cmd=="restart") {
    ESP.restart();
  }
//ANN 10  
  else if (cmd=="cm"){
    int XcmVal = P1.toInt();
    int YcmVal = P2.toInt();
    int Tracker = P3.toInt();//Tracker declared above
    //Serial.println("cmd= "+cmd+" ,VALXCM= "+XcmVal);
    //Serial.println("cmd= "+cmd+" ,VALYCM= "+YcmVal);
    //Serial.println("cmd= "+cmd+" ,TRACKER= "+Tracker);//print and 
                                                      //put into i2c xmit array below
    DELTA_XCM = (XcmVal/2) -(SCREENCOLS/4);
    if(DELTA_XCM<0){
      DELTA_XCM = 256+DELTA_XCM;
    }

    DELTA_YCM = (YcmVal/2) -(SCREENROWS/4);
    if(DELTA_YCM<0){
      DELTA_YCM = 256+DELTA_YCM;
    }

    b_DELTA_XCM = (byte)DELTA_XCM;    
    b_DELTA_YCM = (byte)DELTA_YCM;
    b_Tracker = (byte)Tracker;


    
    /****          
    Serial.print("XCMVAL = ");
    Serial.println(XcmVal);   
    Serial.print("YCMVAL = ");
    Serial.println(YcmVal);            
    Serial.print("b_DELTA_XCM = ");
    Serial.println(b_DELTA_XCM);
    Serial.print("b_DELTA_YCM = ");
    Serial.println(b_DELTA_YCM);
    Serial.print("b_Tracker = ");
    Serial.println(b_Tracker);
    ****/

    //GetAjaxData(cl);

/****  
    Tf = millis();
    if((Tf-Ti)>=INTERVAL){
      analog_val = analog_val + 1;
      analog_val = analog_val % 128;
      dig_val = true ^ dig_val;
      Ti = millis();
    }
****/

/****/  
    Tf = millis();
    if((Tf-Ti)>=INTERVAL){
      analog_val_1 = analog_val_1 + 1;
      analog_val_1 = analog_val_1 % 128;
      dig_val_1 = true ^ dig_val_1;
      analog_val_2 = analog_val_2 + 1;
      analog_val_2 = analog_val_2 % 128;
      dig_val_2 = true ^ dig_val_2;      
      Ti = millis();
    }
/****/
   
    /******
    cl.println("{\"SWITCH_STATE\":");    
    cl.print(dig_val);
    cl.print(",\"ANALOG\":");    
    cl.print(analog_val);  
    cl.println("}");
    *******/
//ANN 11
    /******/
    cl.println("[{\"DIGSENSOR1\":");    
    cl.print(dig_val_1);
    cl.print(",\"DIGSENSOR2\":");    
    cl.print(dig_val_2);  
    cl.println("},");

    cl.println("{\"ANASENSOR1\":");    
    cl.print(analog_val_1);
    cl.print(",\"ANASENSOR2\":");    
    cl.print(analog_val_2);  
    cl.println("}]");    
    /*******/    


/***********    
    ajax_counter = ajax_counter + 1;
    //dig_val = true ^ dig_val;
    
    Serial.println("AJAX_SEND");
    Serial.print("AJAX_COUNTER = ");
    Serial.println(ajax_counter);

    Serial.print("DIG_VAL = ");
    Serial.println(dig_val);
****************/     
       
  }
  else if (cmd=="quality") { 
    sensor_t * s = esp_camera_sensor_get();
    int val = P1.toInt(); 
    s->set_quality(s, val);
  }
  else if (cmd=="contrast") {
    sensor_t * s = esp_camera_sensor_get();
    int val = P1.toInt(); 
    s->set_contrast(s, val);
  }
  else if (cmd=="brightness") {
    sensor_t * s = esp_camera_sensor_get();
    int val = P1.toInt();  
    s->set_brightness(s, val);  
  }   
  else {
    Feedback="Command is not defined.";
    //Tracker = 0;
  }
  if (Feedback=="") {
    Feedback=Command;
  }
  //Serial.print("TRACKER = ");
  //Serial.println(Tracker);
}

//ANN 12
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  myWire.begin(SDA,SCL);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;  //0-63 lower number means higher quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;  //0-63 lower number means higher quality
    config.fb_count = 1;
  }
  
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  //drop down frame size for higher initial frame rate
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_CIF);  //UXGA|SXGA|XGA|SVGA|VGA|CIF|QVGA|HQVGA|QQVGA  設定初始化影像解析度

   // Wi-Fi connection

 #if defined(SOFTAP)
/******************SOFTAP**********************************/
  WiFi.softAP(ssidAP,NULL,1,0,1);
  // ssid,pwd,channel(1-13), broadcast/hidden, max connections(4)
  Serial.print("Setting AP..");
  IPAddress ip = WiFi.softAPIP();

  /*
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  */
  Serial.print("softIP = ");
  Serial.println(ip);
  
  Serial.println("");
  Serial.println("WiFi connected");
  
  Serial.print("Camera Stream Ready! Go to: http://");
  //Serial.println(WiFi.localIP()); ars using softAP
  //Serial.println(IP);
/**********************END SOFTAP*******************************/ 

  
 #else
 /***************STATION POINT***********************/
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.println(WiFi.localIP());
  IPAddress ip = WiFi.localIP();
   /********************END STATION POINT**************************/

 #endif
   
  

  /********************transmit ip address******************************/
  
    //bufIP[0] = ip[0];
  //Serial.println(String(ip[0]));
  String ipString = String(ip[0])+String('.')+String(ip[1])+String('.')+
                      String(ip[2])+String('.')+String(ip[3]);
  Serial.println(ipString);
  int z = String(ip[0]).toInt();                    
  z=z+1;
  Serial.println(z);
  bufIP[0] = (byte)String(ip[0]).toInt();  //bufIP[] is byte array
  Serial.println(bufIP[0]);
  for(int k=0; k<=3; k++){
    //bufIP[k] = (byte)String(ip[k]).toInt();  //bufIP[] is byte array
    bufIP[k] = String(ip[k]).toInt();  //bufIP[] is byte array
  }
  //Serial.println((int)bufIP[2]); 
  Serial.println(bufIP[1]);
  Serial.println(bufIP[2]);
  Serial.println(bufIP[3]);
  
  /***********************end transmit ip address************************************/
  

  ws.onEvent(onWsEvent);
  server3.addHandler(&ws);  
  
  server.begin();
  server3.begin();               
}



void loop() {
  Feedback="";Command="";cmd="";P1="";P2="";P3="";P4="";P5="";P6="";P7="";P8="";P9="";
  ReceiveState=0,cmdState=1,strState=1,questionstate=0,equalstate=0,semicolonstate=0;

  //int i = 0;
  //initializeTT();
  
  if(One_Time_Transmit==1){
     delay(4000);
     initializeTT();
     i2cTransmit();
     One_Time_Transmit = 0;
  }
/****************31DEBUG EXPT*******************/  
    WiFiClient client = server.available();  // try to get client

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
//ANN 13
                getCommand(c);
                
                HTTP_req += c;  // save the HTTP request 1 char at a time
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    //client.println("HTTP/1.1 200 OK");
                    //client.println("Content-Type: text/html");
                    //client.println("Connection: keep-alive");
                    //client.println();
                    // AJAX request for switch state
//ANN 15                    
                    if (cmd=="colorDetect") {
                       camera_fb_t * fb = NULL;
                       fb = esp_camera_fb_get();  
                       if(!fb) {
                         Serial.println("Camera capture failed");
                         delay(1000);
                         ESP.restart();
                       }//end(!fb)
                       //ANN:1
                       client.println("HTTP/1.1 200 OK");
                       client.println("Access-Control-Allow-Origin: *");              
                       client.println("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
                       client.println("Access-Control-Allow-Methods: GET,POST,PUT,DELETE,OPTIONS");
                       client.println("Content-Type: image/jpeg");
                       client.println("Content-Disposition: form-data; name=\"imageFile\"; filename=\"picture.jpg\""); 
                       client.println("Content-Length: " + String(fb->len));             
                       client.println("Connection: close");
                       client.println();
              
                       uint8_t *fbBuf = fb->buf;
                       size_t fbLen = fb->len;
                       for (size_t n=0;n<fbLen;n=n+1024) {
                          if (n+1024<fbLen) {
                             client.write(fbBuf, 1024);
                             fbBuf += 1024;
                          } //end if(n+1024..
                          else if (fbLen%1024>0) {
                             size_t remainder = fbLen%1024;
                             client.write(fbBuf, remainder);
                          } //end else if (fblen..
                       } //end for(size_t n=0...    
                       esp_camera_fb_return(fb);                       
                    } //end if(cmd=="colorDetect")
                    else {  // HTTP request for web page
//ANN 14                      
                        // send web page - contains JavaScript with AJAX calls
                        //THIS HAPPENS ONLY ONCE!!!!!
                        page_counter = page_counter + 1;
                        Serial.print("PAGE COUNTNER = ");
                        Serial.println(page_counter);
                        //HDR
                        client.println("HTTP/1.1 200 OK");
                        client.println("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
                        client.println("Access-Control-Allow-Methods: GET,POST,PUT,DELETE,OPTIONS");
                        client.println("Content-Type: text/html; charset=utf-8");
                        client.println("Access-Control-Allow-Origin: *");
                        client.println("Connection: close");
                        client.println(); 
                        //END HDR
                        //PG LOAD
                        String Data="";
                        if (cmd!="")
                          Data = Feedback;
                        else{  
                        Data = String((const char *)INDEX_HTML);
                        } //if else--cmd!=""
                        int Index;
                        for (Index = 0; Index < Data.length(); Index = Index+1000) {
                        client.print(Data.substring(Index, Index+1000));
                        } //end for(Index...       
                        client.println();
                        //END PG LOAD                                                   
                    } //end else HTTP request for web page
                    Feedback="";
                    ExecuteCommand(client);
                    // display received HTTP request on serial port
//ANN 14                    
                    //Serial.println(HTTP_req);
                    HTTP_req = "";            // finished with request, empty string
                    break;
                } //end if (c == '\n' && currentLineIsBlank) 
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } //end c=='\n'
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                } //end c!='\r'
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
/***************END 31DEUG EXPT*******************/    
/**********ORIG DEBUG EXPT**************************  
  WiFiClient client = server.available();

  if (client) { 
    String currentLine = "";

    if(client.connected()==0){
      Serial.println("NOT CONNECTED");
    }

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //client.write(c);             
        //Serial.println(c);
 

        
        if(lock==0){
          getCommand(c);
        }
                        
        if (c == '\n') {
          if (currentLine.length() == 0) {    
            
            if (cmd=="colorDetect") {
              camera_fb_t * fb = NULL;
              fb = esp_camera_fb_get();  
              if(!fb) {
                Serial.println("Camera capture failed");
                delay(1000);
                ESP.restart();
              }
              //ANN:1
              client.println("HTTP/1.1 200 OK");
              client.println("Access-Control-Allow-Origin: *");              
              client.println("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
              client.println("Access-Control-Allow-Methods: GET,POST,PUT,DELETE,OPTIONS");
              client.println("Content-Type: image/jpeg");
              client.println("Content-Disposition: form-data; name=\"imageFile\"; filename=\"picture.jpg\""); 
              client.println("Content-Length: " + String(fb->len));             
              client.println("Connection: close");
              client.println();
              
              uint8_t *fbBuf = fb->buf;
              size_t fbLen = fb->len;
              for (size_t n=0;n<fbLen;n=n+1024) {
                if (n+1024<fbLen) {
                  client.write(fbBuf, 1024);
                  fbBuf += 1024;
                }
                else if (fbLen%1024>0) {
                  size_t remainder = fbLen%1024;
                  client.write(fbBuf, remainder);
                }
              }    
              esp_camera_fb_return(fb);                        
            }
            else {
              //ANN:1
              client.println("HTTP/1.1 200 OK");
              client.println("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
              client.println("Access-Control-Allow-Methods: GET,POST,PUT,DELETE,OPTIONS");
              client.println("Content-Type: text/html; charset=utf-8");
              client.println("Access-Control-Allow-Origin: *");
              client.println("Connection: close");
              client.println();           
              String Data="";
              if (cmd!="")
                Data = Feedback;
              else {
                Data = String((const char *)INDEX_HTML);
              }
              int Index;
              for (Index = 0; Index < Data.length(); Index = Index+1000) {
                client.print(Data.substring(Index, Index+1000));
              }        
              client.println();
            }
                        
            Feedback="";
            break;
          } else {
            currentLine = "";
          }
        } 
        else if (c != '\r') {
          currentLine += c;
        }
        if ((currentLine.indexOf("/?")!=-1)&&(currentLine.indexOf(" HTTP")!=-1)) {
          if (Command.indexOf("stop")!=-1) {  
            client.println();
            client.println();
            client.stop();
          }
          currentLine="";
          Feedback="";
          ExecuteCommand();
        }
      }
    }
    delay(1);
    client.stop();
  }
*********END ORIG DEBUG EXPT*************************/          
}

void getCommand(char c){
  if (c=='?') ReceiveState=1;
  if ((c==' ')||(c=='\r')||(c=='\n')) ReceiveState=0;
  
  if (ReceiveState==1) {
    Command=Command+String(c);    
    if (c=='=') cmdState=0;
    if (c==';') strState++;
    if ((cmdState==1)&&((c!='?')||(questionstate==1))) cmd=cmd+String(c);
    if ((cmdState==0)&&(strState==1)&&((c!='=')||(equalstate==1))) P1=P1+String(c);
    if ((cmdState==0)&&(strState==2)&&(c!=';')) P2=P2+String(c);
    if ((cmdState==0)&&(strState==3)&&(c!=';')) P3=P3+String(c);
    if ((cmdState==0)&&(strState==4)&&(c!=';')) P4=P4+String(c);
    if ((cmdState==0)&&(strState==5)&&(c!=';')) P5=P5+String(c);
    if ((cmdState==0)&&(strState==6)&&(c!=';')) P6=P6+String(c);
    if ((cmdState==0)&&(strState==7)&&(c!=';')) P7=P7+String(c);
    if ((cmdState==0)&&(strState==8)&&(c!=';')) P8=P8+String(c);
    if ((cmdState==0)&&(strState>=9)&&((c!=';')||(semicolonstate==1))) P9=P9+String(c);   
    if (c=='?') questionstate=1;
    if (c=='=') equalstate=1;
    if ((strState>=9)&&(c==';')) semicolonstate=1;
  }
}
