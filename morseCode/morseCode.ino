// 莫斯电码翻译机
// 可以通过串口（115200）向 ESP8266 发信息（英文字母、数字、空格），ESP8266 将输出对应的莫斯电码信号
// 莫斯电码部分规则：一个线是点的 3 倍长度，每个线或者点之间的时间等于一个点的持续时间，两个字母之间的间隔时间与一个线的长度相同，两个单词之间的间隔等于7个点的持续时间

// 字母数组
char* letters[] = {
  ".-","-...","-.-.","-..",   // a,b,c,d
  ".","..-.","--.","....",    // e,f,g,h
  "..",".---","-.-",".-..",   // i,j,k,l
  "--","-.","---",".--.",     // m,n,o,p
  "--.-",".-.","...","-",     // q,r,s,t
  "..-","...-",".--","-..-",  // u,v,w,x
  "-.--","--.."               // y,z
};

// 数字数组
char* numbers[] = {
  "-----",".----","..---","...--","....-",  // 0,1,2,3,4
  ".....","-....","--...","---..","----."   // 5,6,7,8,9
};

// 点的闪烁时间，用于调节速度
int dotDelay = 200;

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);

  // 初始时让灯不亮
  digitalWrite(LED_BUILTIN,HIGH);
}

void loop() {
  char ch;
  if(Serial.available()){
    ch = Serial.read();
    Serial.print(ch);
    if(ch != '\n'){
      Serial.print(": ");
    }
    if(ch >= 'a' && ch <= 'z'){
      flashSequence(letters[ch - 'a']);
    }else if(ch >= 'A' && ch <= 'Z'){
      flashSequence(letters[ch - 'A']);
    }else if(ch >= '0' && ch <= '9'){
      flashSequence(numbers[ch - '0']);
    }else if(ch == ' '){
      delay(dotDelay * 4);
    }
    Serial.println();
  }
}

/**
 * 通过LED输出语句信号
 */
void flashSequence(char* sequence){
  int i = 0;
  while(sequence[i] != NULL){
    Serial.print(sequence[i]);
    flashDotOrDash(sequence[i]);
    i++;  
  }
  delay(dotDelay);
}

/**
 * 通过LED输出 . 或 - 的信号
 */
void flashDotOrDash(char dotOrDash){
  digitalWrite(LED_BUILTIN,LOW);
  if(dotOrDash == '.'){
    // 如果是 . 延迟一倍的 dotDelay 时长
    delay(dotDelay);
  }else{
    // 如果是 - 延迟三倍的 dotDelay 时长
    delay(dotDelay * 3);
  }
  digitalWrite(LED_BUILTIN,HIGH);
  delay(dotDelay);
}
