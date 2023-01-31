/*
    
*/

#include "vdif_cli.h"

vdif_cli::vdif_cli(void *buf, int buf_size){
    this->vdif_thread.clear();
    
    // parsing the standard 32-byte VDIF header
    int p = 0;
    int vdif_index = 0;
    int uint32_count = buf_size/sizeof(uint32_t);
    std::cout << "32 bit word count " << uint32_count << std::endl;
    while (p < (buf_size/sizeof(uint32_t))-1){
        vdif_frame new_vdif;
        new_vdif.index = vdif_index;
        // word 0
        new_vdif.invalid = vdif_get_invalid((uint32_t*)buf+p);
        // word 2
        new_vdif.data_frame_length = vdif_get_data_frame_length((uint32_t*)buf + p + 2);
        
        std::cout << new_vdif.invalid << " " << new_vdif.data_frame_length << std::endl;
        break;
        vdif_index ++;
    }
}

vdif_cli::~vdif_cli(){
}


