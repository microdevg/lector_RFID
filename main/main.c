
#include "rc522.h"
#include "mqtt.h"
#include "wifi.h"
#include "wifi_credentials.h"



#define TOPIC_PUB           "IFTS14/REQ"        // Topico para enviar solicitudes
#define TOPIC_SUB           "IFTS14/RES"        // Topico para recibir respuestas
#define PRODUCT_ID           12345678            // Numero de 8 cifras que describe el lector RFID
#define LEN_BUFFER      100



// Variables

static const char* TAG = "Lector de tarjetas";
char buffer[LEN_BUFFER]= {0};
static rc522_handle_t scanner;


rc522_config_t config = {
    .spi.host = VSPI_HOST,
    .spi.miso_gpio = 25,
    .spi.mosi_gpio = 23, 
    .spi.sck_gpio = 19,
    .spi.sda_gpio = 22,
    .spi.device_flags= SPI_DEVICE_HALFDUPLEX
};



// Cuando detecto una tarjeta genero un evento que se procesa aquí.
// Es algo parecido a una interrupción por software (basado en eventos de FreeRTOS)
static void rc522_handler(void* arg, esp_event_base_t base, int32_t event_id, void* event_data)
{
    rc522_event_data_t* data = (rc522_event_data_t*) event_data;

    switch(event_id) {
        case RC522_EVENT_TAG_SCANNED: {
                rc522_tag_t* tag = (rc522_tag_t*) data->ptr;
                sprintf(buffer,"Verificar tarjeta [sn: %" PRIu64 "]", tag->serial_number);
                ESP_LOGI(TAG,"%s\n",buffer );
                mqtt_publish(buffer,TOPIC_PUB,2,0);
            }
            break;
    }
}



// Defino declaraciones de funciones. 
//La implementacion se realiza en la parte inferior del archivo.
static void RFID_reader_init();

static void get_data( char* data,  char* topic);

static void mqtt_connected();

static void callback_wifi_connected();




int app_main()
{
    wifi_connect(WIFI_CREDENTIALS_ID,WIFI_CREDENTIALS_PASS,callback_wifi_connected,NULL);
    RFID_reader_init();

    while(1){
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }

   
    return 0;
}





static void RFID_reader_init(){
 rc522_create(&config, &scanner);
 rc522_register_events(scanner, RC522_EVENT_ANY, rc522_handler, NULL);
 rc522_start(scanner);
}

static void get_data( char* data,  char* topic){
    printf("\n[%s] %s\n",topic,data);
}

static void mqtt_connected(){
    printf("init mqtt\n");
    mqtt_subcribe(TOPIC_SUB,2);

}

static void callback_wifi_connected(){
    printf("WIFI Ok\n");
    mqtt_init(MQTT_URL,mqtt_connected,NULL,get_data);
}

