/*
    Date: Jan 30, 2023
    Author: Arash Roshanineshat
*/

/*
    VDIF_BITMASK takes two inputs and generates the 
    bitmask for the required field. It assumes a 32-bit
    word.
    n: MSB offset (index from 0)
    c: mask count
    example:
    VDIF_BITMASK(2, 3) will generate
    0b0011_1000_0000_0000_0000_0000_0000_0000 -> 0x3800_0000
*/
#define VDIF_BITMASK(n, c) (((1<<c)-1) << (32-n-c))

struct vdif_frame {
    uint32_t index; // location in VDIF file
    uint8_t invalid; //1bit
    uint8_t legacy; //1bit
    uint32_t seconds_from_epoch; //30bits
    uint16_t ref_epoch; // 6bits
    uint32_t data_frame_within_second; // 24bits
    uint8_t vdif_version; // 3 bits
    uint8_t log2_channels; // 5bits
    uint32_t data_frame_length; // 24bits
    uint8_t complex_real; // 1bit
    uint8_t bits_per_sample; // 5bits
    uint16_t thread_id; //10 bits
    uint16_t station_id; // 16 bits
    uint8_t edv; // 8bits
    uint32_t eud0; //24bits
    uint32_t eud1; //31bits
    uint32_t eud2; //32bits
    uint32_t eud3;// 32 buts
};

static inline uint8_t vdif_get_invalid(void *buf){ 
    uint32_t bitmask = VDIF_BITMASK(0,1);
    uint8_t invalid_bit = (((uint32_t*)buf)[0] & bitmask) >> (32-1);
    return invalid_bit;
}


static inline uint32_t vdif_get_data_frame_length(void *buf){ 
    uint32_t bitmask = VDIF_BITMASK(7,24);
    uint32_t data_frame_length = (((uint32_t*)buf)[0] & bitmask) >> (32 - 24);
    return data_frame_length;
}
