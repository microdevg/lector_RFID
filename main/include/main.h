#ifndef _MAIN_H
#define _MAIN_H



#include <stdio.h>
#include <stdlib.h>
#include "rc522.h"
#include "mqtt.h"
#include "wifi.h"
#include "wifi_credentials.h"
#include "config.h"








/**
 * @brief Nueva estructura de configuración del puerto SPI utilizado por el lector de tarjetas.
 * 
 */
rc522_config_t config = {
    .spi.host = VSPI_HOST,                          /*<! Host utilizado. El SPI0 y SP1 no están disables para el usuario */  
    .spi.miso_gpio = 21,                            /*<!  MISO */
    .spi.mosi_gpio = 23,                            /*<!  MOSI */
    .spi.sck_gpio = 19,                             /*<!  SCK */
    .spi.sda_gpio = 22,                             /*<!  SDA */
    .spi.device_flags= SPI_DEVICE_HALFDUPLEX        /*<!  Flags de configuración */
};









/**
 * Función callback. Se llama cuando el dispositivo recibe mensajes desde
 * algún tópico al que este suscrito.
 * @param data String con el mensaje enviado.
 * @param topic Canal al que se envió el mensaje.
 */
static void get_data( char* data,  char* topic);


/**
 * @brief Función callback. Se llama cuando el dispositivo se conecto al servidor MQTT.
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