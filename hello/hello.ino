void setup() {
  // put your setup code here, to run once:
  // 开启串行通信接口并设置通信波特率
  Serial.begin(115200);
}

void loop() {
  // put your main code here to run repeatedly:
  // 写入字符串数据+换行到串口
  Serial.println("Hello world!");
  // 延迟 2000ms
  delay(2000);
}
