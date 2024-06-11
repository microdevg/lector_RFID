#ifndef _MQTT_H
#define _MQTT_H
#include "base.h"



/**
 * @brief Configuración y conexión a servidor MQTT.
 * 
 * @param uri   Dirección del servidor
 * @param conn  Callback de conexión. Se llama cuando el sistema se conecta al servidor.
 * @param disconn  Callback de recepción. Se llama cuando el sistema se desconecta del servidor.
 * @param data_received     Callback de recepción de datos. Se llama cuando se reciben datos desde tópicos suscritos.
 * @return esp_err_t  Retorna ESP_OK si todo sale bien.
 */




esp_err_t mqtt_init(const char* uri, esp_callback_t conn, esp_callback_t disconn,callback_data_t data_received);


/**
 * @brief   Publico un mensaje en un cierto tópico. Antes debe llamarse a mqtt_init() para conectarse a servidor.
 * 
 * @param msg   Mensaje a publicar. 
 * @param topic Tópico a donde queremos publicar.
 * @param qos   Calidad del servicio.
 * @param retain Retención de mensaje.
 * @return esp_err_t  Retorna ESP_OK si todo sale bien.
 */
esp_err_t mqtt_publish(const char* msg, const char* topic,int qos, int retain);


/**
 * @brief Publico un mensaje en un cierto tópico. Retorna inmediatamente la función y 
 * deja el mensaje para ser procesado y enviado desde una cola de datos de salida.
 * 
 * @param msg   Mensaje a publicar. 
 * @param topic Tópico a donde queremos publicar.
 * @param qos   Calidad del servicio.
 * @param retain Retención de mensaje.
 * @return esp_err_t  Retorna ESP_OK si todo sale bien.
 */
esp_err_t mqtt_publish_on_queue(const char* msg, const char* topic,int qos, int retain);


/**
 * @brief Suscripcion a un determinado topico.
 * 
 * @param topic Nombre del topico al cual nos suscribimos.
 * @param qos   Calidad de la suscripcion:
 *          0 - El sistema no verifica que se reciban los mensajes
 *          1 - El sistema verifica que se reciban los mensajes al menos una vez
 *          2 - El sistema verifica que se reciban los mensaje solo una vez y en el orden en el que se enviaron.
 * @return esp_err_t Retorna ESP_OK si todo sale bien.
 */
esp_err_t mqtt_subcribe( char* topic, int qos);

#endif
