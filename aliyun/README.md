## ESP8266 连接阿里云物联网

### 代码来源
QQ群: 树莓的小创意室(712199750)

### 需要安装的库

> 可以使用 Arduino 自带的库管理工具安装，也可以在本仓库 `libraries` 目录中找到

- ESP8266 开发环境
- PubSubClient
- ArduinoJson 5.x
- SimpleDHT
- Crypto


### 注意事项
- 开发板要选择 NodeMCU，否则可能会报错：`error: 'D0' was not declared in this scope`
- 安装 ArduinoJson 库时一定要安装 5.x 版本，安装 6.x 会报错



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

