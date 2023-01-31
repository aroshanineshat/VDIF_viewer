#include <iostream>
#include <fstream>
#include "vdif_cli.h"

int main (int argc, char* argv[]){
    //TODO: handle input arguments properly
    //TODO: implement libzmq support streaming 

    if (argc < 2){
        std::cout << "select a vdif file to open" << std::endl;
        exit(1);
    }

    /* Loading the file */

    std::ifstream vdif_file (argv[1], std::ios::binary|std::ios::in|std::ios::ate);
    if (!vdif_file) {
        std::cout << "Error opening the vdif file." << std::endl;
        exit(1);
    }
    int file_size = vdif_file.tellg();
    char *buf;
    vdif_file.seekg(0, std::ios::beg);
    buf = new char [file_size];
    vdif_file.read(buf, file_size);
    vdif_file.close();
    // loading the vdif headers
    vdif_cli vdif_thread((void*)buf, file_size);
    std::cout << "done." << std::endl;
    // closing the file.
    delete[] buf;
    
    return 0;
}
