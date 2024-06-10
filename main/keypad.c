#include "keypad.h"
#include <string.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>




#define DELAY_BOUNCE        500


#define NUM_ROW     4
#define NUM_COL     4

//KEYBOARD
// ROW 1 ROW 2 ROW 3 ROW 4 // COL1 COL2 COL3 COL4

// columns are inputs

#define COL_1   GPIO_NUM_4 	// GPIO_NUM_25
#define COL_2   GPIO_NUM_0 	// GPIO_NUM_26
#define COL_3   GPIO_NUM_2 	// GPIO_NUM_27
#define COL_4   GPIO_NUM_15 // GPIO_NUM_14

//rows are output

#define ROW_1    GPIO_NUM_18
#define ROW_2    GPIO_NUM_5
#define ROW_3    GPIO_NUM_17
#define ROW_4    GPIO_NUM_16


 gpio_num_t rows[NUM_ROW] = { ROW_1, ROW_2, ROW_3, ROW_4 };
 gpio_num_t cols[NUM_COL] = { COL_1, COL_2, COL_3, COL_4 };




#define GPIO_INPUT_COLUMNS  ((1ULL<<COL_1) | (1ULL<<COL_2) | (1ULL<<COL_3) | (1ULL<<COL_4))
#define GPIO_OUTPUT_ROWS    ((1ULL<<ROW_1) | (1ULL<<ROW_2) | (1ULL<<ROW_3) | (1ULL<<ROW_4))




// Keypad layout: [row][col]
char const KEYS[NUM_ROW][NUM_COL] = {
						  { '1', '2' , '3' ,'A' },
						  { '4', '5' , '6' ,'B' },
						  { '7', '8' , '9' ,'C' },
						  { '*', '0' , '#' ,'D' },
                         };



#define KEYPAD_SEND         '#'
#define KEYPAD_CLEAR        'C'
#define KEYPAD_BUFFER_LEN   30


static cb_get_key  _callback_key= NULL;
static cb_get_buffer  _callback_buffer = NULL;
static cb_clear     _callback_clear = NULL;


static int _index = 0;
static char _buffer[KEYPAD_BUFFER_LEN+1]={0};







static void _columns_config(void) {
	gpio_config_t col_config;
	col_config.pin_bit_mask = GPIO_INPUT_COLUMNS;
	col_config.intr_type = GPIO_INTR_DISABLE; // interrupt type: falling and rising edge
	col_config.mode = GPIO_MODE_INPUT;
	col_config.pull_up_en = GPIO_PULLUP_ENABLE;
	col_config.pull_down_en = GPIO_PULLUP_DISABLE;
	gpio_config(&col_config);
}

static void _rows_config(void) {
	gpio_config_t row_config;
	row_config.pin_bit_mask = GPIO_OUTPUT_ROWS;
	row_config.intr_type = GPIO_INTR_DISABLE;   // interrupt disable
	row_config.mode = GPIO_MODE_OUTPUT;
	row_config.pull_up_en = GPIO_PULLUP_DISABLE;
	row_config.pull_down_en = GPIO_PULLUP_DISABLE;
	gpio_config(&row_config);
}




void keypad_task(void* params){



    for(;;){
        for (uint8_t row = 0; row < NUM_ROW; row++)
        {
            gpio_set_level(rows[row], 0);
            for (uint8_t col = 0; col < NUM_COL; col++) 
            {
                if (!gpio_get_level(cols[col]))   // Detecto el nivel bajo en cols[col]
                {			
                     vTaskDelay(DELAY_BOUNCE/portTICK_PERIOD_MS);
                    if( gpio_get_level(cols[col])){
                    
                        char k = KEYS[row][col];

                        switch (k)
                        {
                        case KEYPAD_SEND:
                            _buffer[_index]= 0; // cierro el cstring
                            if(_callback_buffer)_callback_buffer(_buffer);
                            _index = 0;
                            break;
                        case KEYPAD_CLEAR:
                            _index = 0;
                            if(_callback_clear)_callback_clear();
                            break;
                        default:
                            if (_index < KEYPAD_BUFFER_LEN){
                              if(_callback_key)_callback_key(k);
                                _buffer[_index]= k;
                                _index ++;
                                }
                            else {
                                _buffer[KEYPAD_BUFFER_LEN]= 0; // cierro el cstring
                                if(_callback_buffer)_callback_buffer(_buffer);
                                _index = 0;
                                }      
                            break;
                        }  //end switch
                    }// end if        
                }// end if
            }// end for
            gpio_set_level(rows[row], 1);
        }
        vTaskDelay(50/portTICK_PERIOD_MS);

    }
}





void keypad_init( cb_get_key cb_key_pressed,
                  cb_get_buffer cb_get_buffer,
                  cb_clear    fclear){

                
    _callback_key = cb_key_pressed;
    _callback_buffer = cb_get_buffer;
    _callback_clear = fclear;
    //Configuro pines
	_columns_config();	
	_rows_config();
	for(int i = 0 ; i < NUM_ROW; i++) gpio_set_level(rows[i],1);


    xTaskCreate(keypad_task,"keypad_task",2*2000,NULL, uxTaskPriorityGet(NULL)-1,NULL);


}


