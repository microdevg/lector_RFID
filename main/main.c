#include <esp_log.h>
#include <inttypes.h>
#include "rc522.h"
#include "mqtt.h"
#include "wifi.h"
#include "wifi_credentials.h"




#define MQTT_URL                        "mqtt://broker.hivemq.com"


static const char* TAG = "Lector de tarjetas";


static rc522_handle_t scanner;


rc522_config_t config = {
    .spi.host = VSPI_HOST,
    .spi.miso_gpio = 25,
    .spi.mosi_gpio = 23, 
    .spi.sck_gpio = 19,
    .spi.sda_gpio = 22,
    .spi.device_flags= SPI_DEVICE_HALFDUPLEX
};


#define LEN_BUFFER      100
char buffer[LEN_BUFFER]= {0};
// Cuando detecto una tarjeta genero un evento que se procesa aqui
// Es algo parecido a una interrupción por software (basado en eventos de FreeRTOS)
static void rc522_handler(void* arg, esp_event_base_t base, int32_t event_id, void* event_data)
{
    rc522_event_data_t* data = (rc522_event_data_t*) event_data;

    switch(event_id) {
        case RC522_EVENT_TAG_SCANNED: {
                rc522_tag_t* tag = (rc522_tag_t*) data->ptr;
                sprintf(buffer,"Verificar tarjeta [sn: %" PRIu64 "]", tag->serial_number);
                ESP_LOGI(TAG,"%s\n",buffer );
                mqtt_publish(buffer,"request",2,0);
            }
            break;
    }
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
    mqtt_subcribe("request",2);

}

static void callback_wifi_connected(){
    printf("WIFI Ok\n");
    mqtt_init(MQTT_URL,mqtt_connected,NULL,get_data);
}




int app_main()
{
    wifi_connect(WIFI_CREDENTIALS_ID,WIFI_CREDENTIALS_PASS,callback_wifi_connected,NULL);
    RFID_reader_init();

    while(1){
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }

   
    return 0;
}