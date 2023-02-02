#include <iostream>
#include <fstream>
#include <string>
#include "vdif_cli.h"

//function prototypes
void print_usage(void);
int process_vdif_file(const char* file_path);

//VDIF container
vdif_cli* vdif_thread;


int process_vdif_file(const char* file_path){
    std::cout << "openning " << file_path << std::endl;
    std::ifstream vdif_stream (file_path, std::ios::binary|std::ios::in|std::ios::ate);
    if (!vdif_stream) {
        std::cout << "Error opening the vdif file." << std::endl;
        return -1;
    }

    vdif_thread = new vdif_cli();
    vdif_thread->process_stream(&vdif_stream);
}


void print_usage(void) {
    std::cout << "vdif_viewer [options]" << std::endl;
    std::cout << "\t\t-f [path to vdif file]" << std::endl;
    std::cout << "\t\t\t vdif_viewer -f /home/user/example.vdif" << std::endl;
}

int main (int argc, char* argv[]){
    //TODO: handle input arguments properly
    if (argc < 2){
        print_usage();
        exit(1);
    }

    int status;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-f") {
           status = process_vdif_file(argv[++i]); 
        }
    }
    if (vdif_thread) {
        delete vdif_thread;
    }
    return status;
}
