## aliyunLight

> ESP8266 接入阿里云物联网平台，远程控制两个 led 灯

### 开发板

- NodeMCU

### 依赖库

- PubSubClient
- ArduinoJson 5.x
- SimpleDHT
- Crypto

###注意事项

当前代码中的 ArduinoJson 使用 5.x 版本，使用 6.x 版本会不兼容

### 错误调试

#### 串口提示 `MQTT connect failed, error code:255`

解决方案： 修改我们安装的 **PubSubClient** 库中的 `Arduino\libraries\PubSubClient\src\PubSubClient.h` 文件，修改其中对 MQTT 报文最大长度限制，修改为 1024，原来是 128；修改 MQTT keepAlive 的周期为 60 秒，原来是 15 。修改部分代码如下所示：

```c++
// MQTT_MAX_PACKET_SIZE : Maximum packet size
#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE 1024 // 修改成了 1024
#endif

// MQTT_KEEPALIVE : keepAlive interval in Seconds
#ifndef MQTT_KEEPALIVE
#define MQTT_KEEPALIVE 60 // 修改成了 60
#endif
```