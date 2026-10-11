#include "AT_MQTT.h"
#define DEBUG_ENABLE 1
#define DEBUG_LOG "[ AT-MQTT ]"
#include "debug_print.h"

uint8_t MQTTDISCONNECTED_CallBack(void *device, void *data)
{
	AT_Device_t *at_device = device;
	at_device->init_step = 0;
	at_device->state = AT_INIT;
	
	return 0;
}

at_err_t mqtt_pub(const char *topic, const char *data, uint16_t len)
{
    char cmd[128] = {0};

    // 鏋勯€燤QTT鍙戝竷鍛戒护
    snprintf(cmd, sizeof(cmd), "AT+MPUBEX=\"%s\",0,0,%d\r\n", topic, len);

    if (AT_SendCmd(cmd, ">", 1000, NULL) == AT_CMD_OK)
    {
        if (AT_SendCmd(data, "OK", 1000, NULL) == AT_CMD_OK)
        {
            return AT_MQTT_SEND_SUCCESS;
        }
    }
    return AT_MQTT_SEND_FAIL;
}

void mqtt_cmd_init(AT_Device_t *at_device)
{
    at_device->mqtt_params.IP_Address = MQTT_SERVER_DOMAIN;
    at_device->mqtt_params.Port = MQTT_SERVER_PORT;
    at_device->mqtt_params.Product_ID = MQTT_USERNAME;
    at_device->mqtt_params.Device_Name = MQTT_CLIENTID;
    at_device->mqtt_params.SECRET_KEY = PRODUCT_KEY;
    at_device->mqtt_params.keepalive = KEEPALIVE;//MQTT_KEEP_ALIVE;
    at_device->mqtt_params.reporting_interval = KEEPALIVE;

	
	memcpy(at_device->mqtt_params.Token , MQTT_PASSWORD , sizeof(MQTT_PASSWORD));
//    OneNET_Authorization("2018-10-31", at_device->mqtt_params.Product_ID, 1956499200, at_device->mqtt_params.SECRET_KEY, at_device->mqtt_params.Device_Name, at_device->mqtt_params.Token, sizeof(at_device->mqtt_params.Token), 1);

		AT_Cmd_Register("OK" , 1000 , NULL , -1 , "AT+MCONFIG=\"%s\",\"%s\",\"%s\"\r\n" , at_device->mqtt_params.Device_Name, at_device->mqtt_params.Product_ID, at_device->mqtt_params.Token);

		AT_Cmd_Register("CONNECT OK" , 10000 , NULL , -1 , "AT+MIPSTART=\"%s\",%d\r\n" , MQTT_SERVER_DOMAIN, MQTT_SERVER_PORT);

		AT_Cmd_Register("CONNACK OK" , 10000 , NULL , -1 , "AT+MCONNECT=1,%d\r\n" , at_device->mqtt_params.keepalive);

    AT_Cmd_Register("SUBACK", 1000, NULL, -1,"AT+MSUB=\"%s\",0\r\n", MQTT_SUB_TOPIC);
}

at_err_t set_ack(char *id , uint16_t code , char *msg)
{
	at_err_t ret = AT_CMD_OK;
	char *topic = MQTT_ACK_TOPIC_1;

	char *str = MQTT_OneNet_MsgAck(id , msg , code);
	if(str != NULL)
	{
			ret = mqtt_pub(topic , str , strlen(str));
			if(ret == AT_CMD_OK)
			{
					mqtt_message.need_ack = 0;
					memset(mqtt_message.id , 0 , sizeof(mqtt_message.id));
					memset(mqtt_message.ack_message , 0 , sizeof(mqtt_message.ack_message));
					mqtt_message.ack_code = 0;

					DEBUG_PRINT("mqtt publish success , topic:%s payload:%s\r\n" , topic , str);
			}
			else
			{
					DEBUG_PRINT("mqtt publish fail , ret:%d\r\n" , ret);
			}
			free(str);
	}
	else
	{
			DEBUG_PRINT("CJSON malloc fail\r\n");
	}

	return ret;	
}

uint8_t mqtt_parse(void *device, void *buf)
{
    char *data = buf , *js_start = NULL;
    AT_Device_t *at_device = device;
    at_device->state = AT_MQTT_PARSE;
    if (data != NULL)
    {
			DEBUG_PRINT("mqtt receive data:%s\r\n" , data);
			js_start = strstr(data , "{");
			if(js_start != NULL)
			{
				MQTT_OneNet_MsgParse(js_start);
			}
    }
    at_device->state = AT_IDLE;

    return 0;
}

at_err_t mqtt_refresh(void)
{
		at_err_t ret = AT_CMD_OK;
		char *str = MQTT_OneNet_BoardInfoRefresh();
		if(str != NULL)
		{
			ret = mqtt_pub(MQTT_PUB_TOPIC_1 , str , strlen(str));
			DEBUG_PRINT("mqtt publish finish\r\n");
			free(str);
		}
		else
		{
			
		}
    return ret;
}
