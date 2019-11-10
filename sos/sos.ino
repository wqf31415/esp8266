// 使用板载的 led 模拟发出 摩斯电码的 SOS 信号
// SOS, 原文 Save Our Souls，拯救我们的灵魂，是国际通用的求救信号
// 在莫斯电码中，S 为 ... ,O 为 ---
// ESP8266 中板载LED，低电平点亮，高电平熄灭


int durations[] = {200,200,200,500,500,500,200,200,200};

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);

}

void loop() {
  showSOS();
}

/**
 * 用循环的方式输出信号
 * 将每个信号闪烁时间写到数组里，for循环逐个输出
 * 这种方式字母之间间隔没控制好，用 showSOS 方法效果比较好
 */
void showSOSByLoop(){
  for(int i = 0; i < 9; i++){
    flash(durations[i]);
    }
    delay(1000);
  }

/**
 * 按字母一个一个的输出
 */
void showSOS(){
    // S
  flash(200);
  flash(200);
  flash(200);

  // 字母之间增加延迟，否则前 4 个信号感觉是连起来的
  delay(300);
  
  // O
  flash(500);
  flash(500);
  flash(500);
  
  // S
  flash(200);
  flash(200);
  flash(200);
  
  delay(1000);
  }

/**
 * LED闪烁
 * 参数：
 * duration 闪烁间隔
 */
void flash(int duration){
  digitalWrite(LED_BUILTIN,LOW);
  delay(duration);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(duration);
  }
