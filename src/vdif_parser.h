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
#include <stdio.h>
#include <string.h>

#define VDIF_BITMASK(n, c) (((1<<c)-1) << n)

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
    char station_id[3]; // 16 bits
    uint8_t edv; // 8bits
    uint32_t eud0; //24bits
    uint32_t eud1; //31bits
    uint32_t eud2; //32bits
    uint32_t eud3;// 32 buts
};

static inline uint8_t vdif_get_invalid(void *buf){ 
    uint32_t bitmask = VDIF_BITMASK(31,1);
    uint8_t r = (((uint32_t*)buf)[0] & bitmask) >> (31);
    return r;
}

static inline uint8_t vdif_get_legacy(void *buf){ 
    uint32_t bitmask = VDIF_BITMASK(30,1);
    uint8_t r = (((uint32_t*)buf)[0] & bitmask) >> (30);
    return r;
}

static inline uint32_t vdif_get_seconds_from_epoch(void *buf){ 
    uint32_t bitmask = VDIF_BITMASK(0,30);
    uint32_t r = (((uint32_t*)buf)[0] & bitmask) >> (0);
    return r;
}

static inline uint16_t vdif_get_ref_epoch(void *buf){ 
    uint32_t bitmask = VDIF_BITMASK(24,6);
    uint16_t r = (((uint32_t*)buf)[0] & bitmask) >> (24);
    return r;
}

static inline uint32_t vdif_get_data_frame_within_second(void *buf){ 
    uint32_t bitmask = VDIF_BITMASK(0,24);
    uint32_t r = (((uint32_t*)buf)[0] & bitmask) >> (0);
    return r;
}

static inline uint8_t vdif_get_vdif_version(void *buf){ 
    uint32_t bitmask = VDIF_BITMASK(29,3);
    uint8_t r = (((uint32_t*)buf)[0] & bitmask) >> (29);
    return r;
}

static inline uint8_t vdif_get_log2_channels(void *buf){ 
    uint32_t bitmask = VDIF_BITMASK(24,5);
    uint8_t r = (((uint32_t*)buf)[0] & bitmask) >> (24);
    return r;
}

static inline uint32_t vdif_get_data_frame_length(void *buf){ 
    uint32_t bitmask = VDIF_BITMASK(0,24);
    uint32_t r = (((uint32_t*)buf)[0] & bitmask) >> (0);
    return r;
}

static inline uint8_t vdif_get_complex_real(void *buf){ 
    uint32_t bitmask = VDIF_BITMASK(31,1);
    uint8_t r = (((uint32_t*)buf)[0] & bitmask) >> (31);
    return r;
}

static inline uint8_t vdif_get_bits_per_sample(void *buf){ 
    uint32_t bitmask = VDIF_BITMASK(26,5);
    uint8_t r = (((uint32_t*)buf)[0] & bitmask) >> (26);
    return r;
}

static inline uint16_t vdif_get_thread_id(void *buf){ 
    uint32_t bitmask = VDIF_BITMASK(16,10);
    uint16_t r = (((uint32_t*)buf)[0] & bitmask) >> (16);
    return r;
}

static inline void vdif_get_station_id(void *buf, char *stationid){ 
    uint32_t bitmask = VDIF_BITMASK(0,16);
    uint16_t r = (((uint32_t*)buf)[0] & bitmask) >> (0);
    r = (r>>8) | (r<<8);
    memcpy (stationid, &r, 2);
    stationid[2] = '\0';
}

static inline uint8_t vdif_get_edv(void *buf){ 
    uint32_t bitmask = VDIF_BITMASK(24,8);
    uint8_t r = (((uint32_t*)buf)[0] & bitmask) >> (24);
    return r;
}

static inline uint32_t vdif_get_eud0(void *buf){ 
    uint32_t bitmask = VDIF_BITMASK(0,24);
    uint32_t r = (((uint32_t*)buf)[0] & bitmask) >> (0);
    return r;
}

static inline uint32_t vdif_get_eudx(void *buf){ 
    uint32_t bitmask = 0xFFFFFFFF;
    uint32_t r = (((uint32_t*)buf)[0] & bitmask) >> (0);
    return r;
}
