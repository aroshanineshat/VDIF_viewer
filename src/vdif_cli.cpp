/*
    
*/

#include "vdif_cli.h"

vdif_cli::vdif_cli(void *buf, int buf_size){
    this->vdif_thread.clear();
    
    // parsing the standard 32-byte VDIF header
    int p = 0;
    int vdif_index = 0;
    int uint32_count = buf_size/sizeof(uint32_t);
    while (p < uint32_count-1){
        vdif_frame new_vdif;
        new_vdif.index = vdif_index;
        // word 0
        new_vdif.invalid = vdif_get_invalid((uint32_t*)buf+p);
        new_vdif.legacy = vdif_get_legacy((uint32_t*)buf+p);
        new_vdif.seconds_from_epoch = vdif_get_seconds_from_epoch((uint32_t*)buf+p);
        
        // word 1
        new_vdif.ref_epoch = vdif_get_ref_epoch((uint32_t*)buf+p+1);
        new_vdif.data_frame_within_second = vdif_get_data_frame_within_second((uint32_t*)buf+p+1);
        // word 2
        new_vdif.vdif_version = vdif_get_vdif_version((uint32_t*)buf + p + 2);
        new_vdif.log2_channels = vdif_get_log2_channels((uint32_t*)buf + p + 2);
        new_vdif.data_frame_length = vdif_get_data_frame_length((uint32_t*)buf + p + 2);
        
        // word 3
        new_vdif.complex_real = vdif_get_complex_real((uint32_t*)buf + p + 3);
        new_vdif.bits_per_sample = vdif_get_bits_per_sample((uint32_t*)buf + p + 3);
        new_vdif.thread_id = vdif_get_thread_id((uint32_t*)buf + p + 3);
        vdif_get_station_id((uint32_t*)buf + p + 3, new_vdif.station_id);
        
        // word 4
        new_vdif.edv = vdif_get_edv((uint32_t*)buf + p + 4);
        new_vdif.eud0 = vdif_get_eud0((uint32_t*)buf + p + 4);
        // word 5
        new_vdif.eud1 = vdif_get_eudx((uint32_t*)buf + p + 5);
        // word 6
        new_vdif.eud1 = vdif_get_eudx((uint32_t*)buf + p + 6);
        // word 7
        new_vdif.eud1 = vdif_get_eudx((uint32_t*)buf + p + 7);

        this->vdif_thread.push_back(new_vdif);
        p = p+ new_vdif.data_frame_length * 2;
        vdif_index ++;
    }
}

int vdif_cli::vdif_frame_count(void) const{
    /*
        Returns the size of the vdif_thread
    */
    return this->vdif_thread.size();
}

int vdif_cli::get_vdif_frame(int idx, vdif_frame& frame) const{
    if (idx > this->vdif_frame_count()-1){
        std::cout << "Vdif frame out of range" << std::endl;
        return -1; // error
    }
    frame = this->vdif_thread[idx];
    return 0;
}

int vdif_cli::print_vdif_frame (int idx){
    vdif_frame c_frame; 
    if (this->get_vdif_frame(idx, c_frame)<0){
        return -1;
    }
    std::cout << "Frame:" << idx <<"\t===============================" << std::endl;
    std::cout << "0:\t" <<  +c_frame.invalid << "\t" << +c_frame.legacy << "\t\t" << c_frame.seconds_from_epoch << std::endl;
    std::cout << "1:\t" <<  "\t" << +c_frame.ref_epoch << "\t\t" << +c_frame.data_frame_within_second << std::endl;
    std::cout << "2:\t" << +c_frame.vdif_version << "\t" << +c_frame.log2_channels << "\t\t" << c_frame.data_frame_length << std::endl;
    std::cout << "3:\t" << +c_frame.complex_real << "\t" << +c_frame.bits_per_sample << "\t"  << c_frame.thread_id << "\t" << c_frame.station_id << std::endl;
    std::cout << "4:\t" << +c_frame.edv << "\t    " << +c_frame.eud0<< std::endl;
    std::cout << "5:\t" << "\t" << +c_frame.eud1<< std::endl;
    std::cout << "6:\t" << "\t" << +c_frame.eud2<< std::endl;
    std::cout << "7:\t" << "\t" << +c_frame.eud3<< std::endl;
    return 0;


}

vdif_cli::~vdif_cli(){
}


