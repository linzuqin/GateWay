#ifndef _APP_W5500_MQTT_H_
#define _APP_W5500_MQTT_H_
#include <stdint.h>

/*数组大小*/
#define BUFFER_SIZE 2048




typedef enum
{
    MQTT_PARAMS_INIT = 0,
    MQTT_DNS_RESOLVE,
    MQTT_DISCONNECT,
    MQTT_CONNECTED,
    MQTT_SUBSCRIBE,
    MQTT_IDLE,
    MQTT_PUBLISH,
}MQTT_State_t;
extern uint8_t mqtt_dest_ip[4];
extern uint16_t mqtt_port;
int app_w5500_mqtt_publish(const char *topic , char *data , uint16_t size);
void app_w5500_mqtt_proc(void);
void app_w5500_1s_callback(void);


#endif
