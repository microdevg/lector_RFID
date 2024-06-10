/**
 * @file keypad.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-06-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */


/**
 * @brief Callback que recibe un char de parametro
 * 
 */
typedef void (*cb_get_key)(char key);


/**
 * @brief Callback que recibe un buffer char de parametro
 * 
 */
typedef void (*cb_get_buffer)(char* buffer);


/**
 * @brief Callback que no recibe parametro
 * 
 */
typedef void (*cb_clear)();




/**
 * @brief Teclado compatible con callbacks. 
 * Utiliza una tarea en segundo plano para el polling.
 * 
 * @param cb_key_pressed Callback cuando se presiona una tecla
 * @param cb_get_buffer  Callback cuando se presiona la tecla #, recibe el buffer como parametro
 * @param fclear  Callback que se llama cuando se presiona tecla C, se borra el buffer
 */
void keypad_init( cb_get_key cb_key_pressed,
                  cb_get_buffer cb_get_buffer,
                  cb_clear  fclear);