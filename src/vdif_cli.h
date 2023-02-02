/*
    Date: Jan 31, 2023
    Author: Arash Roshanineshat
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <istream>
#include "vdif_parser.h"

class vdif_cli{
    private:
        std::istream *i_stream;
        std::vector<vdif_frame> vdif_headers;
        std::vector<char*> vdif_payloads;

        int process_header(int idx, void* hdr_p);
        void clear_frames();

        void analize_packet_numbers();
        void save_packet_payload();
        
    public:
        void process_stream(std::istream* i_stream);
        /* returns the number of vdif frames in the file*/
        int vdif_frame_count(void) const;
        /* get the vdif_frame using its index*/
        int get_vdif_frame(int, vdif_frame&) const;
        /* print vdif frame using the index*/
        int print_vdif_frame (int);
        /* initializing the class. it accepts the buffer
        and the buffer size*/
        vdif_cli();
        ~vdif_cli();
};
