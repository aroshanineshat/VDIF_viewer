/*
    Date: Jan 31, 2023
    Author: Arash Roshanineshat
*/

#include "vdif_cli.h"

int vdif_cli::process_header(int idx, void* hdr_p){
        vdif_frame new_vdif;
        new_vdif.index = idx;
        // word 0
        new_vdif.invalid = vdif_get_invalid((uint32_t*)hdr_p);
        new_vdif.legacy = vdif_get_legacy((uint32_t*)hdr_p);
        new_vdif.seconds_from_epoch = vdif_get_seconds_from_epoch((uint32_t*)hdr_p);
        
        // word 1
        new_vdif.ref_epoch = vdif_get_ref_epoch((uint32_t*)hdr_p+1);
        new_vdif.data_frame_within_second = vdif_get_data_frame_within_second((uint32_t*)hdr_p+1);
        // word 2
        new_vdif.vdif_version = vdif_get_vdif_version((uint32_t*)hdr_p + 2);
        new_vdif.log2_channels = vdif_get_log2_channels((uint32_t*)hdr_p + 2);
        new_vdif.data_frame_length = vdif_get_data_frame_length((uint32_t*)hdr_p + 2);

        // word 3
        new_vdif.complex_real = vdif_get_complex_real((uint32_t*)hdr_p + 3);
        new_vdif.bits_per_sample = vdif_get_bits_per_sample((uint32_t*)hdr_p + 3);
        new_vdif.thread_id = vdif_get_thread_id((uint32_t*)hdr_p + 3);
        vdif_get_station_id((uint32_t*)hdr_p + 3, new_vdif.station_id);
        
        // word 4
        new_vdif.edv = vdif_get_edv((uint32_t*)hdr_p + 4);
        new_vdif.eud0 = vdif_get_eud0((uint32_t*)hdr_p + 4);
        // word 5
        new_vdif.eud1 = vdif_get_eudx((uint32_t*)hdr_p + 5);
        // word 6
        new_vdif.eud2 = vdif_get_eudx((uint32_t*)hdr_p + 6);
        // word 7
        new_vdif.eud3 = vdif_get_eudx((uint32_t*)hdr_p + 7);

        // 
        
        int payload_size = new_vdif.data_frame_length * 8 - 32; // vdif header has 32 bytes
        
        //new_vdif.data_buf = (char*)malloc (sizeof(char)*payload_size);
        //memcpy(new_vdif.data_buf, (char*)hdr_p + 32, payload_size); // copy the payload
        //
        this->vdif_headers.push_back(new_vdif);
        return payload_size;
}

void vdif_cli::save_packet_payload(){
    std::ofstream payload_file("payload.txt");
    uint32_t tmp_word;
    for (auto&i: this->vdif_headers){
        int word32bit_count = ((i.data_frame_length * 8) - 32) / 4;
        for (int j=0; j<word32bit_count ; j++){
            tmp_word = *(((uint32_t *)this->vdif_payloads[i.index]) + j); // take the 32 bit word out
            for (int k = 0; k < 16;k ++){
                uint8_t sample;
                sample = (tmp_word >> (k*2)) & 0x3;
                payload_file << +sample << ",";
            }
        }
        std::cout << "Stored " << i.index << "/" << this->vdif_frame_count() << " samples" << std::endl;
        if (i.index == 100){
            return;
        }
    }
}

void vdif_cli::clear_frames(){
    if (this->vdif_payloads.size() > 0){
        for(auto& i: this->vdif_payloads) {
            if (i){
                delete [] i; // delete each data frame 
            }
        }
    }
    this->vdif_payloads.clear();
    this->vdif_headers.clear();
}

void vdif_cli::process_stream(std::istream* i_stream){
    this->clear_frames(); 
    this->i_stream = i_stream;
    this->i_stream->seekg(0, std::ios::beg);

    // parsing the standard 32-byte VDIF header
    int payload_length = 0;
    int vdif_index = 0;
    void *header_buf = malloc(sizeof(char) * 32);
    do{
        this->i_stream->read((char*)header_buf, 32);
        payload_length = this->process_header(vdif_index, header_buf);
        char * payload_buf = (char *) malloc(sizeof(char) * payload_length);
        this->i_stream->read((char*) payload_buf, payload_length); //copy the payload to the new memory
        //add the payload buffer into the vector
        this->vdif_payloads.push_back(payload_buf);
        vdif_index++;
        this->i_stream->peek();
    } while (!this->i_stream->eof()) ;
    delete [] header_buf;
    std::cout << "Loaded " << this->vdif_frame_count() << " VDIF frames" << std::endl;
    this->analize_packet_numbers();
    this->save_packet_payload();
}

void vdif_cli::analize_packet_numbers(){
    // going through each packet number and print it. 
    // if there is any incompetencies, it will print it. 

    uint64_t frame_number = 0;
    int frame_skipped = 0;

    int printing_c = 0;
    std::cout << "-------" << std::endl;
    for (auto&i: this->vdif_headers){
        uint64_t tmp=0;
        tmp = i.eud3;
        tmp = (tmp<<32) + (i.eud2 & 0x00000000FFFFFFFF);
        if (tmp - frame_number != 1 && frame_number != 0){
            frame_skipped++;
            std::cout << "At " << i.index << " skipped from " << frame_number << " to " << tmp << std::endl;
        }
        if (printing_c > 0) {
            std::cout << std::hex << i.eud3 << std::endl;
            printing_c --;
        }
        if (frame_number == 0) {
            std::cout << "Packet Number started from: " << tmp << std::endl;
        }
        if (i.index == this->vdif_headers.size()-1){
            std::cout << "Packet Number ended with: " << tmp << std::endl;
        }
        frame_number = tmp;
    }
    std::cout << "Total Frame Skipped :" << frame_skipped << std::endl;
    std::cout << "-------" << std::endl;
}

int vdif_cli::vdif_frame_count(void) const{
    /*
        Returns the size of the vdif_thread
    */
    return this->vdif_headers.size();
}

int vdif_cli::get_vdif_frame(int idx, vdif_frame& frame) const{
    if (idx > this->vdif_frame_count()-1){
        std::cout << "Vdif frame out of range" << std::endl;
        return -1; // error
    }
    frame = this->vdif_headers[idx];
    return 0;
}

int vdif_cli::print_vdif_frame (int idx){
    vdif_frame c_frame; 
    if (this->get_vdif_frame(idx, c_frame)<0){
        return -1;
    }
    std::cout << "Frame:" << idx <<"\t=============================================" << std::endl;
    std::cout << "0:\tI:" <<  +c_frame.invalid << "\tL:" << +c_frame.legacy << "\t\tSeconds From Epoch:" << c_frame.seconds_from_epoch << std::endl;
    std::cout << "1:\t" <<  "Ref Epoch:" << +c_frame.ref_epoch << "\t\tData Frame Withing Second:" << +c_frame.data_frame_within_second << std::endl;
    std::cout << "2:\tV:" << +c_frame.vdif_version << "\tlog2(ch):" << +c_frame.log2_channels << "\t\tData Frame Length:" << c_frame.data_frame_length << std::endl;
    std::cout << "3:\tC:" << +c_frame.complex_real << "\tbit/s:" << +c_frame.bits_per_sample << "\t   Thread ID:"  << c_frame.thread_id << "\t Station ID:" << c_frame.station_id << std::endl;
    std::cout << "4:\tEDV:" << +c_frame.edv << "\t\t EUD0:" << +c_frame.eud0<< std::endl;
    std::cout << "5:\t" << "EUD2:\t" << +c_frame.eud1<< std::endl;
    std::cout << "6:\t" << "Packet Number Low:\t" << +c_frame.eud2<< std::endl;
    std::cout << "7:\t" << "Packet Number High:\t" << +c_frame.eud3<< std::endl;
    return 0;
}

vdif_cli::vdif_cli(){
    
}

vdif_cli::~vdif_cli(){
    this->clear_frames(); // clear the memory and collect garbage
}