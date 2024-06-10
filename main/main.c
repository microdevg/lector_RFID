/**
 * @file main.c
 * @author German Velardez (germanvelardez16@gmail.com)
 * @brief  Proyecto de lector RFID con conexión a servidor MQTT.
 * @version 0.1
 * @date 2024-05-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "main.h"

// Variables
char buffer[LEN_BUFFER]= {0};


static void get_rfid(uint64_t rfid){
    sprintf(buffer,REQUEST_FORMAT,PRODUCT_ID, rfid);
    printf("%s\n",buffer );
    mqtt_publish(buffer,TOPIC_PUB,2,0);
}


static void get_data( char* data,  char* topic){
    printf("\n[%s] %s\n",topic,data);
    //Primer verifico que la respuesta sea para mi con el código PRODUCT_ID
    // Si no se cumple la igualad retorno, el mensaje no es para mi lector RFID
    if( strcmp(data,PRODUCT_ID) != 0) {
    printf("%s != %s\n",data,PRODUCT_ID);
    return;}
    //SI no coincide el  PRODUCT_ID, ignoro el mensaje
    if(! EQUAL(data,PRODUCT_ID))return; 
   // Proceso el mensaje.
    CHECK(topic,RES_OK,action_ok);
    CHECK(topic,RES_FAIL,action_fail);
    CHECK(topic,RES_UNKNOWN,action_unknown);
}



int app_main()
{

    wifi_connect(WIFI_CREDENTIALS_ID,WIFI_CREDENTIALS_PASS,callback_wifi_connected,NULL);

    while(1){
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }   
    return 0;
}











static void mqtt_connected(){
    printf("init mqtt\n");
    // Me conector a todos los tópicos necesarios.
    mqtt_subcribe(RES_OK,2);
    mqtt_subcribe(RES_FAIL,2);
    mqtt_subcribe(RES_UNKNOWN,2);
    rc522_init(&config,get_rfid);

}

static void callback_wifi_connected(){
    printf("WIFI Ok\n");
    mqtt_init(MQTT_URL,mqtt_connected,NULL,get_data);
}


static void action_ok(){
    printf("\n Usuario autorizado\n");
 
}

static void action_fail(){
    printf("\n Usuario  NO autorizado\n");
   
}

static void action_unknown(){
    printf("Error inesperado\n");
}