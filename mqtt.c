#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "led.h"
#include "bh1750.h"

const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883; 
const char* lux_topic = "cambien";
const char* bbb_topic = "led_blink";
char* gpio_pin = "60";

pthread_t sensor_thread, mqtt_thread;
int pipe_fd[2];
struct mosquitto *mosq;

void* sensor_thread_function(void* arg) 
{
    char lux_str[20];
    while (1) 
    {
        double lux_value = bh1750_read();
        snprintf(lux_str, sizeof(lux_str), "%.2f", lux_value);
        write(pipe_fd[1], lux_str, strlen(lux_str) + 1);
        sleep(2);
    }
    return NULL;
}

void* mqtt_thread_function(void* arg) 
{
    char buf[20];
    while (1) 
    {
        mosquitto_loop(mosq, -1, 1);
        read(pipe_fd[0], buf, sizeof(buf));
        mosquitto_publish(mosq, NULL, lux_topic, strlen(buf), buf, 0, false);
    }
    return NULL;
}

void on_connect(struct mosquitto *mosq, void *obj, int rc) 
{
    printf("Connected to MQTT Broker with code %d\n", rc);
    mosquitto_subscribe(mosq, NULL, bbb_topic, 0);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) 
{
    if(message->payloadlen)
    {
        printf("Received message on topic %s: %s\n", message->topic, (char *)message->payload);
        char* payload = (char *)message->payload;
        if (strcmp(payload, "ON") == 0) 
        {
            GPIO_SET(PIN);
        } 
        else if (strcmp(payload, "OFF") == 0) 
        {
            GPIO_CLR(PIN);
        } 
    }
}

int main() 
{
    int rc;
    char lux_str[20];

// Khởi tạo GPIO
    setup_io();
    bh1750_init();
    mosquitto_lib_init();
    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) 
    {
        fprintf(stderr, "Error: Out of memory.\n");
        return 1;
    }

    if (pipe(pipe_fd) == -1) 
    {
        perror("pipe");
        return 1;
    }

    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    rc = mosquitto_connect(mosq, mqtt_server, mqtt_port, 60);
    if (rc != MOSQ_ERR_SUCCESS) 
    {
        fprintf(stderr, "Unable to connect to MQTT Broker. Error code: %d\n", rc);
        return 1;
    }

    if (pthread_create(&sensor_thread, NULL, sensor_thread_function, NULL) != 0) 
    {
        perror("pthread_create");
        return 1;
    }

    if (pthread_create(&mqtt_thread, NULL, mqtt_thread_function, NULL) != 0) 
    {
        perror("pthread_create");
        return 1;
    }

    pthread_join(sensor_thread, NULL);
    pthread_join(mqtt_thread, NULL);

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
}
