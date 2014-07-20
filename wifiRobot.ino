int servo1=2,servo2=4;
int pulsewidth;//�����������
int AIN1 = 6;  //PWMA
int AIN2 = 5;  //DIRA
int BIN1 = 10;  //PWMB             
int BIN2 = 9;  //DIRB
int myServo1Angle = 90, myServo2Angle = 60;
int myServo1Inc = 5, myServo2Inc = 20;
boolean scan = false,fengmingqi=false;

void servo(int servopin,int myangle)//����һ�����庯��
{
    pulsewidth=(myangle*11)+500;//���Ƕ�ת��Ϊ500-2480������ֵ
    digitalWrite(servopin,HIGH);//������ӿڵ�ƽ����
    delayMicroseconds(pulsewidth);//��ʱ����ֵ��΢����
    digitalWrite(servopin,LOW);//������ӿڵ�ƽ����
    delay(20-pulsewidth/1000);
}
void setMotor(int MOTORA,int MOTORB)
{
  if(MOTORA>=0)  //�жϷ��򣬴���0��ʾ����
  {
    digitalWrite(AIN2,HIGH);  //AIN2�����ø�
    analogWrite(AIN1,255-MOTORA);//AIN1����PWM�ź� 
       // PWM�Ǹߵ�ƽ��ռ�ձȣ�������Ҫȡ��������255��MOTORA
  }
  else            //�жϷ���С��0��ʾ����            
  {
   digitalWrite(AIN1,HIGH);  
    analogWrite(AIN2,MOTORA+255);  //PWM�Ǹߵ�ƽ��ռ�ձ�
          //������Ҫȡ������ʱMOTORAΪ��ֵ������MOTORA+255
  }
  if(MOTORB>=0)    //�жϷ��򣬴���0��ʾ����
  {
    digitalWrite(BIN2,HIGH);   //BIN2�����ø�
    analogWrite(BIN1,255-MOTORB);  //BIN1����PWM�ź�
        // PWM�Ǹߵ�ƽ��ռ�ձȣ�������Ҫȡ��������255��MOTORB
  }
  else     //�жϷ���С��0��ʾ����
  {
    digitalWrite(BIN1,HIGH);  
    analogWrite(BIN2,255+MOTORB); //PWM�Ǹߵ�ƽ��ռ�ձ�
        //������Ҫȡ������ʱMOTORBΪ��ֵ������MOTORB+255
  }
}


void motorCmd(unsigned long command){
  int angle;
  
  if( (command & 0xFFFF00) == 0x010700 || (command & 0xFFFF00) == 0x010100) {
    angle = command & 0xFF;
    servo(servo1,  angle);  
    return;
  }
  if( (command & 0xFFFF00) == 0x010800 || (command & 0xFFFF00) == 0x010400) {
    angle = command & 0xFF;
    servo(servo2,  angle / 2);  
    return;
  }
    
  switch(command){
    case 0x040100:
    for(int i=0;i<=50;i++){
     servo(servo1,90); 
     servo(servo2,70); }
     break;
    case 0x040000:
      scan = false; break;      
    case 0x000200:
      setMotor(150,150);
       break;
    case 0x000100:
      setMotor(-150,-150);
      break;
    case 0x000300:
      setMotor(-170,170);
      break;
    case 0x000400:
      setMotor(170,-170);
      break;
     case 0x000500:
      setMotor(100,170);
       break;
    case 0x000600:
      setMotor(170,100);
      break;
    case 0x000700:
      setMotor(-100,-170);
      break;
    case 0x000800:
      setMotor(-170,-100);
      break;
    case 0x000000:
      setMotor(0,0);
      break;
    case 0x000900:
      fengmingqi=!fengmingqi;
      if(fengmingqi)
        tone(12,400);
       else
         noTone(12);
      break;
      case 0x001000:
        int melody[]={330,330,330,262,330,392,196};
        int noteDurations[]={8,4,4,8,4,2,2};
        for(int thisNote=0;thisNote< 7;thisNote++) {
          int noteDuration= 1000/noteDurations[thisNote];
          tone(12, melody[thisNote],noteDuration);
          int pauseBetweenNotes= noteDuration*1.30;
          delay(pauseBetweenNotes);
          noTone(12); 
         }
        
    }
} 
void setup(){
  Serial.begin(9600);
  pinMode (servo1,OUTPUT);
  pinMode(servo2,OUTPUT);
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  setMotor(0,0);
  for(int i=0;i<=50;i++){
  servo(servo1, 90);
  servo(servo2, 70);}
}

void loop(){
  unsigned long command;
  byte head;
  if (Serial.available()>4) {
    head = Serial.read();
    if( head == 0xFF && Serial.peek() != 0xFF) {
      command = Serial.read()*0x10000;
      command += Serial.read()*0x100;
      command += Serial.read();
      head = Serial.read();
      if( head == 0xFF ) {
        motorCmd(command);
      }
    }
  }
  if( scan == true ){
    if(myServo1Angle > 170 || myServo1Angle < 10 ){
      myServo1Inc = -myServo1Inc;
    
      if(myServo2Angle > 70 || myServo1Angle < 20 ){
        myServo2Inc = -myServo2Inc;
      }
      myServo2Angle += myServo2Inc;
      servo(servo2,myServo2Angle);
    }
    myServo1Angle += myServo1Inc;
    servo(servo1,myServo1Angle);   
  }
  
}
