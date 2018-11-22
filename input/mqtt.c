#include <mosquitto.h>
#include <log.h>

#include "input.h"
#include "mqtt.h"


static struct mosquitto *mq;

void
mqtt_message_cb(struct mosquitto *mq_, void *obj,
		struct mosquitto_message *msg)
{
    log_debug("topic:%s payload:%s", msg->topic, msg->payload);
}

void
_initInput(void)
{
    mosquitto_lib_init();
    mq = mosquitto_new(NULL, true, NULL);
    mosquitto_connect(mq, MQTT_HOST, MQTT_PORT, MQTT_KEEPALIVE);
    mosquitto_subscribe(mq, NULL, MQTT_TOPIC, 0);
    mosquitto_message_callback_set(mq, mqtt_message_cb);
}

uint32_t
_getInput(void)
{
    mosquitto_loop(mq, -1, 1);
}
