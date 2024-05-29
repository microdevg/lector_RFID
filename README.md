## Librería para el modulo lector de tarjetas RFID.

## En este ejemplo se muestra como usar la librería para leer tarjetas RFID.  
El driver genera una task aparte donde realiza el proceso de lectura de manera periódica.

En la configuración por defecto usamos el puerto SPI3 con los siguientes pines:
```

rc522_config_t config = {
    .spi.host = VSPI_HOST,
    .spi.miso_gpio = 25,
    .spi.mosi_gpio = 23, 
    .spi.sck_gpio = 19,
    .spi.sda_gpio = 22,
    .spi.device_flags= SPI_DEVICE_HALFDUPLEX
};
```

Revisar que su placa de desarrollo coincida con el siguiente pinout.

![pinout_esp32](./imgs/esp32_pinout.jpg)


### Circuito montado

![circuito](./imgs/circuito.jpg)





### Pruebas

En las pruebas realizada la librería funciona bien y obtiene el ID de la tarjeta correctamente.


#### Nuevas pruebas
 
En wifi.c se agrego  evento de IP obtenida, el callback de WiFi connected se llama en ese momento.



### Funcionamiento
```
El lector RFID se subscribe a 3 TOPICOS:
#define RES_OK              "/RES/OK"
#define RES_FAIL            "/RES/FAIL"
#define RES_UNKNOWN         "/RES/UNKNOWN"
```
y publica en:
```
#define TOPIC_PUB           "/REQ"        // Topico para enviar solicitudes
````
