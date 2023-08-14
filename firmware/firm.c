#include <stdint.h>
#include "../pl110.h"



void firmware_start(){


    for(int i = 0; i < 480;i++){

            pixel_data dat={i,i-2,i-5};    


        for(int j = 0 ;j < 640;j++){

            pl110_write_data(dat,i,j);

        }
    }
   
    pl110_forceUpdate(&one_and_only_dev);


    while(1){
        
    }
   

}