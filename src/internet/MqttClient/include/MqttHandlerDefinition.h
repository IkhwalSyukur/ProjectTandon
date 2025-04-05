#pragma once

#include <sdkconfig.h>

#ifndef CONFIG_MQTT_HANDLER_TAG
#define CONFIG_MQTT_HANDLER_TAG                 "MQTT_HANDLER"
#endif

#ifndef CONFIG_MQTT_HANDLER_BROKER
#define CONFIG_MQTT_HANDLER_BROKER              "mqtt://localhost" 
// #define CONFIG_MQTT_HANDLER_BROKER              "mqtt" 
#endif

#ifndef CONFIG_MQTT_HANDLER_PREFIX_CLIENT_ID
#define CONFIG_MQTT_HANDLER_PREFIX_CLIENT_ID    "WaterTorrentClient_"
#endif

#ifndef CONFIG_MQTT_HANDLER_USERNAME
#define CONFIG_MQTT_HANDLER_USERNAME            "username"
#endif

#ifndef CONFIG_MQTT_HANDLER_PASSWORD
#define CONFIG_MQTT_HANDLER_PASSWORD            "password"
#endif

#ifndef CONFIG_MQTT_HANDLER_PORT
#define CONFIG_MQTT_HANDLER_PORT                1883
#endif

/**
 * @brief QOS levels
 * 0 - fire and forget, no ACK from broker, message never stored even if fail to deliver
 * 1 - at least once, sender stores until PUBACK, if none will resend, potential of duplicate in broker side
 * 2 - exactly once, safest but slowest with. stores internally, making sure is received by broker with 4 handshakes
 */
#define CONFIG_MQTT_QOS_DEFAULT                 0
#define CONFIG_MQTT_QOS_0                       0
#define CONFIG_MQTT_QOS_1                       1
#define CONFIG_MQTT_QOS_2                       2
#define CONFIG_MQTT_RETAIN_MESSAGES             0
#define CONFIG_MQTT_QOS0_STORE false
#define CONFIG_MQTT_QUEUE_RECONNECT_LENGTH      1

#define CONFIG_MQTT_EVENT_ERROR_BIT                    BIT0
#define CONFIG_MQTT_EVENT_CONNECTED_BIT                BIT1
#define CONFIG_MQTT_EVENT_DISCONNECTED_BIT             BIT2
#define CONFIG_MQTT_EVENT_SUBSCRIBED_BIT               BIT3
#define CONFIG_MQTT_EVENT_UNSUBSCRIBED_BIT             BIT4
#define CONFIG_MQTT_EVENT_PUBLISHED_BIT                BIT5
#define CONFIG_MQTT_EVENT_DATA_BIT                     BIT6
#define CONFIG_MQTT_EVENT_BEFORE_CONNECT_BIT           BIT7
#define CONFIG_MQTT_EVENT_DELETED                      BIT8