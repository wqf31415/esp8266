#include <ESP8266WiFi.h>   //安装esp8266arduino开发环境
#include <PubSubClient.h>  //安装PubSubClient库
#include <ArduinoJson.h>   //json  V5版本
#include "aliyun_mqtt.h"

//需要安装crypto库

#define LED     D4
#define LED2     D1
   
#define WIFI_SSID        "CMCC-b4Q9"//替换自己的WIFI
#define WIFI_PASSWD      "31415926"//替换自己的WIFI

#define PRODUCT_KEY      "a1REmKBuTek" //替换自己的PRODUCT_KEY
#define DEVICE_NAME      "w_esp8266_test_02" //替换自己的DEVICE_NAME
#define DEVICE_SECRET    "TbdSgi90KqpoG3P0r3Q3cyhdVOojvG0t"//替换自己的DEVICE_SECRET

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


byte temperature = 0;
byte humidity = 0;


         
void init_wifi(const char *ssid, const char *password)      //连接WiFi
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi does not connect, try again ...");
        delay(500);
    }

    Serial.println("Wifi is connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void mqtt_callback(char *topic, byte *payload, unsigned int length) //mqtt回调函数
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    payload[length] = '\0';
    Serial.println((char *)payload);
   // https://arduinojson.org/v5/assistant/  json数据解析网站

    
    Serial.println("   ");
  Serial.println((char *)payload);
   Serial.println("   ");
    if (strstr(topic, ALINK_TOPIC_PROP_SET))
    {
        StaticJsonBuffer<100> jsonBuffer;
        JsonObject &root = jsonBuffer.parseObject(payload);

        if(root["params"]["LightSwitch"].success()){
        int params_LightSwitch = root["params"]["LightSwitch"];
        if(params_LightSwitch==0)
        {Serial.println("led off");
        digitalWrite(LED, HIGH); 
        }
        else
        {Serial.println("led on");
        digitalWrite(LED, LOW); }
        }

        if(root["params"]["LightSwitch2"].success()){
          int params_LightSwitch2 = root["params"]["LightSwitch2"];
          if(params_LightSwitch2==0){
            Serial.println("led2 off");
            digitalWrite(LED2, HIGH); 
          } else {
            Serial.println("led2 on");
            digitalWrite(LED2, LOW); 
            }
          }
        if (!root.success())
        {
            Serial.println("parseObject() failed");
            return;
        }
    }
}
void mqtt_version_post()
{
    char param[512];
    char jsonBuf[1024];

    //sprintf(param, "{\"MotionAlarmState\":%d}", digitalRead(13));
    sprintf(param, "{\"id\": 123,\"params\": {\"version\": \"%s\"}}", DEV_VERSION);
   // sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    Serial.println(param);
    mqttClient.publish(ALINK_TOPIC_DEV_INFO, param);
}
void mqtt_check_connect()
{
    while (!mqttClient.connected())//
    {
        while (connect_aliyun_mqtt(mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET))
        {
            Serial.println("MQTT connect succeed!");
            //client.subscribe(ALINK_TOPIC_PROP_POSTRSP);
            mqttClient.subscribe(ALINK_TOPIC_PROP_SET);
            
            Serial.println("subscribe done");
            mqtt_version_post();
        }
    }
    
}

void mqtt_interval_post()
{
    char param[512];
    char jsonBuf[1024];

    //sprintf(param, "{\"MotionAlarmState\":%d}", digitalRead(13));
    sprintf(param, "{\"LightSwitch\":%d,\"LightSwitch2\":%d}", !digitalRead(LED), !digitalRead(LED2));
    sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    Serial.println(jsonBuf);
    mqttClient.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
}


void setup()
{

   

    pinMode(LED, OUTPUT);
    pinMode(LED2, OUTPUT);
    /* initialize serial for debugging */
    Serial.begin(115200);

    Serial.println("Demo Start");

    init_wifi(WIFI_SSID, WIFI_PASSWD);

    mqttClient.setCallback(mqtt_callback);
}

// the loop function runs over and over again forever
void loop()
{
    if (millis() - lastMs >= 10000)  //10s
    {
        lastMs = millis();
  
        mqtt_check_connect();
        /* Post */        
        mqtt_interval_post();
    }

    mqttClient.loop();

   
}
