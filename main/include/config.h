#ifndef _CONFIG_H
#define _CONFIG_H

/**
 * @brief Parámetros para configuración la conexión con servidor MQTT
 * 
 */
#define MQTT_URL            "mqtt://broker.hivemq.com"
#define TOPIC_PUB           "/REQ"                      // Tópico para enviar solicitudes
#define TOPIC_SUB           "/RES"                      // Tópico para recibir respuestas
#define RES_OK              "/RES/OK"
#define RES_FAIL            "/RES/FAIL"
#define RES_UNKNOWN         "/RES/UNKNOWN"


#define PRODUCT_ID          "12345678"                  // Numero de 8 cifras que describe el lector RFID
#define LEN_BUFFER          100
#define GPIO_LED            2



#define MSG_OK          "USUARIO AUTORIZADO\n"
#define MSG_FAIL        "USUARIO NO AUTORIZADO\n"
#define MSG_UNKNOWN     "ERROR DESCONOCIDO"


#endif