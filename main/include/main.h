#ifndef _MAIN_H
#define _MAIN_H



#include <stdio.h>
#include <stdlib.h>
#include "rc522.h"
#include "mqtt.h"
#include "wifi.h"
#include "wifi_credentials.h"
#include "config.h"








// Configuración del periférico.
rc522_config_t config = {
    .spi.host = VSPI_HOST,
    .spi.miso_gpio = 21,
    .spi.mosi_gpio = 23, 
    .spi.sck_gpio = 19,
    .spi.sda_gpio = 22,
    .spi.device_flags= SPI_DEVICE_HALFDUPLEX
};



// Defino declaraciones de funciones. 
//La implementación se realiza en la parte inferior del archivo.






/**
 * Función callback. Se llama cuando el dispositivo recibe mensajes desde
 * algún tópico al que este suscrito.
 * @param data String con el mensaje enviado.
 * @param topic Canal al que se envió el mensaje.
 */
static void get_data( char* data,  char* topic);


/**
 * @brief Funcion callback. Se llama cuando el dispositivo se conecto al servidor MQTT.
 * 
 */
static void mqtt_connected();

/**
 * @brief Función callback. Se llama cuando el dispositivo se conecta a la red WiFi
 * y esta listo para conectarse a internet.
 * 
 */
static void callback_wifi_connected();




/**
 * @brief Function que se la tarjeta RFID leída es de un USUARIO AUTORIZADO.
 * 
 */
static void action_ok();

/**
 * @brief Function que se la tarjeta RFID leída es de un USUARIO NO AUTORIZADO.
 * 
 */
static void action_fail();


/**
 * @brief Función que se llama cuando el servidor No pudo procesar la 
 * tarjeta RFID enviada.
 * 
 */
static void action_unknown();


#endif