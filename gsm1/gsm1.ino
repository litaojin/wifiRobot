#include <SoftwareSerial.h>
#include <String.h>

SoftwareSerial mySerial(7, 8);

char sensor_id[] = "22913";
char device_id[] = "13719";
char api_key[] = "3a2e8d958b8b9cdefde4a1135ad7b17e";
char apn[] = "\"UNINET\"";
int i;
int len_head, len_data; 
int times;
int temperature = 10;

char post[] = {// HTTP HEAD
    "POST /v1.0/device/ddd/sensor/sss/datapoints HTTP/1.1\r\n"
    "Host: api.yeelink.net\r\n"
    "Accept: *\r\n"
    "U-ApiKey: kkk\r\n"
    "Content-Length: ccc\r\n"
    "Content-Type: application/x-www-form-urlencoded\r\n"
    "Connection: close\r\n"
    "\r\n"
  };
       
char post_data[] = {
  "{\"value\":xx}\r\n"
  };
  		
void post_temperature(int temp) 
{  
  connect();
  len_head = strlen(post);
  len_data = strlen(post_data);
  
  for(times=0; times<len_head; times++)
  {
    if(post[times]=='d' && post[times+1]=='d' && post[times+2]=='d')
    {
      mySerial.print(device_id);                 
      times += 3;
    }
    if(post[times]=='s' && post[times+1]=='s' && post[times+2]=='s')
    {
      mySerial.print(sensor_id);                 
      times += 3;
    } 
    if(post[times]=='c' && post[times+1]=='c' && post[times+2]=='c')
    {
      mySerial.print(len_data);                 
      times += 3;
    }
    if(post[times]=='k' && post[times+1]=='k' && post[times+2]=='k')
    {
      mySerial.print(api_key);                 
      times += 3;
    }
    mySerial.print(post[times]);
  } 
  //mySerial.print(post);
  
  for(times=0; times<len_data; times++)
  {
    if(post_data[times] == 'x' && post_data[times+1] == 'x')
    {
      mySerial.print(temp);                 
      times += 2;
    } 
    mySerial.print(post_data[times]);
  } 
  mySerial.write(26);//send order
  delay(5000); 
}
	   
void connect() //TCP连接
{
  mySerial.print("ati\r\n");   delay(1000);
  mySerial.print("ati\r\n");   delay(1000);
  mySerial.print("ati\r\n");   delay(1500);  
  mySerial.print("AT+CGCLASS=\"B\"\r\n");   delay(500); 
  mySerial.print("AT+CGDCONT=1,\"IP\",\"UNINET\"\r\n");   delay(500);
  mySerial.print("AT+CGATT=1\r\n");   delay(500); 
  mySerial.print("AT+CIPCSGP=1,\"UNINET\"\r\n"); //uninet
  delay(500);  
  mySerial.print("AT+CLPORT=\"TCP\",\"2000\"\r\n"); 
  delay(500);  
  mySerial.print("AT+CIPSTART=\"TCP\",\"42.96.164.52\",\"80\"\r\n"); //the ip addr to yeelink
  delay(10000);   
  mySerial.print("AT+CIPSEND\r\n"); 
  delay(500);
}
 
void setup()
{
  mySerial.begin(19200);    // the GPRS baud rate   
  //Serial1.begin(19200);    // the GPRS baud rate 
  //while(!Serial1);
  delay(1000);
}
void loop()
{  
  post_temperature(temperature);
  SendTextMessage("+8613801037634", "This is hello from Arduino by Tao1.");
  //SendTextMessage("+8618600015808", "This is hello from Arduino by Tao2.");
  delay(20000);
  temperature += 2;
  if(temperature > 41) 
    temperature = 0;
}
///SendTextMessage()
///this function is to send a sms message
void SendTextMessage(char * mobile, char * msg)
{
 mySerial.print("AT+CMGF=1\r");    //Because we want to send the SMS in text mode
 delay(100);
 //mySerial.println("AT + CMGS = \"+8613801037634\"");//send sms message, be careful need to add a country code before the cellphone number
 mySerial.print("AT + CMGS = \"");
 mySerial.print(mobile);
 mySerial.println("\"");
 delay(100);
 mySerial.println(msg);//the content of the message
 delay(100);
 mySerial.println((char)26);//the ASCII code of the ctrl+z is 26
 delay(100);
 mySerial.println();
}
///DialVoiceCall
///this function is to dial a voice call
void DialVoiceCall(char * mobile)
{
// mySerial.println("ATD + +8613801037634;");//dial the number
 mySerial.print("ATD + ");
 mySerial.print(mobile);
 mySerial.println(";");//dial the number
 delay(100);
 mySerial.println();
}

