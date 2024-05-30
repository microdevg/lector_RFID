
#include "rc522.h"
#include "mqtt.h"
#include "wifi.h"
#include "wifi_credentials.h"
#include "config.h"

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


// Defino declaraciones de funciones. 
//La implementación se realiza en la parte inferior del archivo.


/**
 * @brief Configuro el lector RFID
 * 
 */
static void RFID_reader_init();



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
 * @brief Configuración del gpio que maneja el LED de la placa.
 * 
 */
static void lec_config();

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


// Cuando detecto una tarjeta genero un evento que se procesa aquí.
// Es algo parecido a una interrupción por software (basado en eventos de FreeRTOS)
static void rc522_handler(void* arg, esp_event_base_t base, int32_t event_id, void* event_data)
{
    rc522_event_data_t* data = (rc522_event_data_t*) event_data;

    switch(event_id) {
        case RC522_EVENT_TAG_SCANNED: {
                rc522_tag_t* tag = (rc522_tag_t*) data->ptr;
                sprintf(buffer,REQUEST_FORMAT,PRODUCT_ID, tag->serial_number);
                ESP_LOGI(TAG,"%s\n",buffer );
                mqtt_publish(buffer,TOPIC_PUB,2,0);
            break;
            }
        default:
            break;
    }
}




int app_main()
{
    lec_config();
    wifi_connect(WIFI_CREDENTIALS_ID,WIFI_CREDENTIALS_PASS,callback_wifi_connected,NULL);

    while(1){
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }   
    return 0;
}





// Implementaciones

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
    //Primer verifico que la respuesta sea para mi con el código PRODUCT_ID
    // Si no se cumple la igualad retorno, el mensaje no es para mi lector RFID
    if( strcmp(data,PRODUCT_ID) != 0) {
    printf("%s != %s\n",data,PRODUCT_ID);
    return;
    }
    //SI no coincide el  PRODUCT_ID, ignoro el mensaje
    if(! EQUAL(data,PRODUCT_ID))return; 

   // Proceso el mensaje.
    CHECK(topic,RES_OK,action_ok);
    CHECK(topic,RES_FAIL,action_fail);
    CHECK(topic,RES_UNKNOWN,action_unknown);
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


static void action_ok(){
    printf("\n Usuario autorizado\n");
    gpio_set_level(GPIO_LED,1);
}

static void action_fail(){
      // Usuario autorizado
    printf("\n Usuario  NO autorizado\n");
    gpio_set_level(GPIO_LED,0);
}

static void action_unknown(){
    printf("Error inesperado\n");
}