/**
 * @file keypad.h
 * @author German Velardez (germanvelardez16@gmail.com)
 * @brief  Driver para manejo de teclado 4x4. Utiliza callback para manejar eventos.
 * @version 0.1
 * @date 2024-06-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */




/**
 * @brief Teclado compatible con callbacks. 
 * Utiliza una tarea en segundo plano para el polling.
 * 
 * @param cb_key_pressed Callback cuando se presiona una tecla, recibe un char como parámetro
 * @param cb_get_buffer  Callback cuando se presiona la tecla, recibe el buffer como parámetro
 * @param fclear  Callback que se llama cuando se presiona tecla C, se borra el buffer
 */
void keypad_init( cb_get_key cb_key_pressed,
                  cb_get_buffer cb_get_buffer,
                  cb_clear  fclear);