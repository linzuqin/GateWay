#ifndef _MQTT_H_
#define _MQTT_H_
#include "cJSON.h"
#include "stdint.h"

#define MQTT_SERVER_DOMAIN	"mqtts.heclouds.com"
#define MQTT_SERVER_PORT	1883

/*MQTT用户名*/
#define MQTT_USERNAME "2Its5wq8a3"

/*MQTT密码*/
#define MQTT_PASSWORD "version=2018-10-31&res=products%2F2Its5wq8a3%2Fdevices%2Flot_device&et=1988355119&method=md5&sign=zyATn1UFf7fvJEEjorA7Ww%3D%3D"

/*MQTT客户端*/
#define MQTT_CLIENTID "lot_device"

/*MQTT发布主题*/
#define MQTT_PUB_TOPIC_1 "$sys/"MQTT_USERNAME"/"MQTT_CLIENTID"/thing/property/post"

/*MQTT订阅主题*/
#define MQTT_SUB_TOPIC "$sys/"MQTT_USERNAME"/"MQTT_CLIENTID"/thing/property/#"

#define MQTT_SUB_TOPIC_1 "$sys/"MQTT_USERNAME"/"MQTT_CLIENTID"/thing/property/set"

/*MQTT命令应答主题*/
#define MQTT_ACK_TOPIC_1 "$sys/"MQTT_USERNAME"/"MQTT_CLIENTID"/thing/property/set_reply"

/*产品级密钥*/
#define PRODUCT_KEY "czkTF5mpf5FrcQ+7hT99aeeJG5V7LQdF113JPQvpcQE="


/*心跳包间隔*/
#define KEEPALIVE 60

typedef struct 
{
	char id[8];
	char ack_message[64];
	uint16_t ack_code;
	char version[8];
	uint8_t need_ack;
	uint8_t need_refresh;
}mqtt_message_stuct;
extern mqtt_message_stuct mqtt_message;

char *MQTT_OneNet_BoardInfoRefresh(void);
char *MQTT_OneNet_MsgAck(char *id , char *msg , int code);
void MQTT_OneNet_MsgParse(char *payload);






#endif
