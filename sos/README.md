## SOS 救援信号发生器

### 功能

使用 ESP8266 板载 LED 发射国际通用的求救信号 SOS



### 用法

通电后，ESP8266 每隔 1秒发射一次 SOS 信号



### 注意

项目中提供两种方法发射信号，一种是按字母一个一个的输出信号，另一种是将信号控制的参数写到数组中，用循环的方式读取并输出，前一种输出效果更加准确。