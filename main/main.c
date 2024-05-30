
#include "rc522.h"
#include "mqtt.h"
#include "wifi.h"
#include "wifi_credentials.h"



#define TOPIC_PUB           "/REQ"        // Topico para enviar solicitudes
#define TOPIC_SUB           "/RES"        // Topico para recibir respuestas
#define RES_OK              "/RES/OK"
#define RES_FAIL            "/RES/FAIL"
#define RES_UNKNOWN         "/RES/UNKNOWN"

#define PRODUCT_ID           "12345678"            // Numero de 8 cifras que describe el lector RFID
#define LEN_BUFFER           100
#define GPIO_LED             2

#define RESOK               1
#define RESFAIL             0
#define RESUNKNOWN          2
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
                sprintf(buffer,"[%s] %" PRIu64 "",PRODUCT_ID, tag->serial_number);
                ESP_LOGI(TAG,"%s\n",buffer );
                mqtt_publish(buffer,TOPIC_PUB,2,0);

            }
            break;
    }
}



// Defino declaraciones de funciones. 
//La implementación se realiza en la parte inferior del archivo.

static void RFID_reader_init();

static void get_data( char* data,  char* topic);

static void mqtt_connected();

static void callback_wifi_connected();

static void lec_config();



int app_main()
{
    lec_config();
    wifi_connect(WIFI_CREDENTIALS_ID,WIFI_CREDENTIALS_PASS,callback_wifi_connected,NULL);

    while(1){
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }

   
    return 0;
}






static void lec_config(){
    gpio_set_direction(GPIO_LED,GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_LED,0);
}

static void RFID_reader_init(){
 rc522_create(&config, &scanner);
 rc522_register_events(scanner, RC522_EVENT_ANY, rc522_handler, NULL);
 rc522_start(scanner);
}

static void get_data( char* data,  char* topic){
    printf("\n[%s] %s\n",topic,data);

    //Primer verifico que la respuesta sea para mi con el codigo PRODUCT_ID
    // Si no se cumple la igualda retorno, el mensaje no es para mi lector RFID
    if( strcmp(data,PRODUCT_ID) != 0) {
    printf("%s != %s\n",data,PRODUCT_ID);
    return;
    }
    printf("%s == %s\n",data,PRODUCT_ID);



   // Si llegue aqui, el mensaje es para mi asi que tengo que procesarlo.
   if( strcmp(topic,RES_OK) == 0){
    // Usuario autorizado
    printf("\n Usuario autorizado\n");
    gpio_set_level(GPIO_LED,1);
    return;
   }
    if( strcmp(topic,RES_FAIL) == 0){
    // Usuario autorizado
    printf("\n Usuario  NO autorizado\n");
    gpio_set_level(GPIO_LED,0);

    return;
   }
    if( strcmp(topic,RES_UNKNOWN) == 0){
    // Usuario autorizado
    printf("\n Usuario desconocido\n");
    gpio_set_level(GPIO_LED,0);

    return;
   }
}

static void mqtt_connected(){
    printf("init mqtt\n");
    // Me conector a todos los tópicos necesarios.
    mqtt_subcribe(RES_OK,2);
    mqtt_subcribe(RES_FAIL,2);
    mqtt_subcribe(RES_UNKNOWN,2);

    RFID_reader_init();


}

static void callback_wifi_connected(){
    printf("WIFI Ok\n");
    mqtt_init(MQTT_URL,mqtt_connected,NULL,get_data);
}

