#ifndef INPUT_MQTT_H
#define INPUT_MQTT_H

#define MQTT_HOST       "192.168.1.45"
#define MQTT_PORT       1833
#define MQTT_KEEPALIVE  15
#define MQTT_TOPIC      "ledmatrix/#"

void mqtt_init(void);
uint32_t mqtt_loop(void);

#endif				/* INPUT_MQTT_H */
