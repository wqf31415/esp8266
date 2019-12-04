#include <ESP8266WiFi.h>   //安装esp8266arduino开发环境
#include <PubSubClient.h>  //安装PubSubClient库
#include <ArduinoJson.h>   //json  V5版本
#include "aliyun_mqtt.h"
#include <SimpleDHT.h>
//需要安装crypto库

#define LED     D4
#define pinDHT11 D0     
#define WIFI_SSID        "CMCC-b4Q9"//替换自己的WIFI
#define WIFI_PASSWD      "31415926"//替换自己的WIFI

#define PRODUCT_KEY      "a1cMYN50OBh" //替换自己的PRODUCT_KEY
#define DEVICE_NAME      "w_esp8266_test_03" //替换自己的DEVICE_NAME
#define DEVICE_SECRET    "u3Aa5ST5ohDS6uPvmiY8sxswyyaNW8GG"//替换自己的DEVICE_SECRET

#define DEV_VERSION       "S-TH-WIFI-v1.0-20190220"        //固件版本信息

#define ALINK_BODY_FORMAT         "{\"id\":\"123\",\"version\":\"1.0\",\"method\":\"%s\",\"params\":%s}"
#define ALINK_TOPIC_PROP_POST     "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
#define ALINK_TOPIC_PROP_POSTRSP  "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post_reply"
#define ALINK_TOPIC_PROP_SET      "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"
#define ALINK_METHOD_PROP_POST    "thing.event.property.post"
#define ALINK_TOPIC_DEV_INFO      "/ota/device/inform/" PRODUCT_KEY "/" DEVICE_NAME ""    
#define ALINK_VERSION_FROMA      "{\"id\": 123,\"params\": {\"version\": \"%s\"}}"
unsigned long lastMs = 0;

WiFiClient   espClient;
PubSubClient mqttClient(espClient);
SimpleDHT11 dht11(pinDHT11);

byte temperature = 0;
byte humidity = 0;
int err = SimpleDHTErrSuccess;

void init_wifi(const char *ssid, const char *password) {
  //连接WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi does not connect, try again ...");
        delay(500);
    }

    Serial.println("Wifi is connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

//mqtt回调函数
void mqtt_callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    payload[length] = '\0';
    Serial.println((char *)payload);
    // https://arduinojson.org/v5/assistant/  json数据解析网站

    Serial.println("   ");
    Serial.println((char *)payload);
    Serial.println("   ");
    if (strstr(topic, ALINK_TOPIC_PROP_SET)) {
        StaticJsonBuffer<100> jsonBuffer;
        JsonObject &root = jsonBuffer.parseObject(payload);
        int params_LightSwitch = root["params"]["LightSwitch"];
        if(params_LightSwitch==1)
        {Serial.println("led off");
        digitalWrite(LED, HIGH); 
        } else {
          Serial.println("led on");
          digitalWrite(LED, LOW); 
        }
        if (!root.success()) {
            Serial.println("parseObject() failed");
            return;
        }
    }
}

void mqtt_version_post() {
    char param[512];
    char jsonBuf[1024];

    //sprintf(param, "{\"MotionAlarmState\":%d}", digitalRead(13));
    sprintf(param, "{\"id\": 123,\"params\": {\"version\": \"%s\"}}", DEV_VERSION);
    // sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    Serial.println(param);
    mqttClient.publish(ALINK_TOPIC_DEV_INFO, param);
}

void mqtt_check_connect() {
    while (!mqttClient.connected()) {
        while (connect_aliyun_mqtt(mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET)) {
            Serial.println("MQTT connect succeed!");
            //client.subscribe(ALINK_TOPIC_PROP_POSTRSP);
            mqttClient.subscribe(ALINK_TOPIC_PROP_SET);
            
            Serial.println("subscribe done");
            mqtt_version_post();
        }
    }
}

void mqtt_interval_post() {
    char param[512];
    char jsonBuf[1024];

    //sprintf(param, "{\"MotionAlarmState\":%d}", digitalRead(13));
    sprintf(param, "{\"LightSwitch\":%d,\"CurrentTemperature\":%d,\"CurrentHumidity\":%d}", !digitalRead(LED),temperature,humidity);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    Serial.println(jsonBuf);
    mqttClient.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
}


void setup() {
    pinMode(LED, OUTPUT);

    // 初始时设置高电平，让esp8266 led 熄灭
    digitalWrite(LED,HIGH);
    
    /* initialize serial for debugging */
    Serial.begin(115200);

    Serial.println("Demo Start");

    init_wifi(WIFI_SSID, WIFI_PASSWD);

    mqttClient.setCallback(mqtt_callback);
}

// the loop function runs over and over again forever
void loop() {
  if (millis() - lastMs >= 5*60*1000) {
      //5S
      lastMs = millis();
      if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
        Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
        return;
      }
      Serial.print("Sample OK: ");
      Serial.print((int)temperature); Serial.print(" *C, "); 
      Serial.print((int)humidity); Serial.println(" H");
  
      mqtt_check_connect();
      /* Post */        
      mqtt_interval_post();
  }
  mqttClient.loop();
}
