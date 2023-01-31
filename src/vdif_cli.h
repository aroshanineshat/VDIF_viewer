/*
    Date: Jan 31, 2023
    Author: Arash Roshanineshat
*/

#include <iostream>
#include <vector>
#include "vdif_parser.h"

class vdif_cli{
    private:
        std::vector<vdif_frame> vdif_thread;
        
    public:
        /* returns the number of vdif frames in the file*/
        int vdif_frame_count(void) const;
        /* get the vdif_frame using its index*/
        int get_vdif_frame(int, vdif_frame&) const;
        /* print vdif frame using the index*/
        int print_vdif_frame (int);
        /* initializing the class. it accepts the buffer
        and the buffer size*/
        vdif_cli(void*, int);
        ~vdif_cli();
};
