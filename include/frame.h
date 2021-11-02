#ifndef FRAME_H_
#define FAME_H_

#define SD_C                 0xAAU  //Start Delimiter for fixed length telegram
#define ED_C                 0x55U  //End Delimiter

enum flags_bits {CTRL_F=0,
                 ACK_F=1,
                 RTS_F=2,
                 CTS_F=3,
                 ERROR_F=4};

struct frame {
        uint8_t SD;
        uint8_t flags;
        struct telegram PDU;
        uint8_t FCS;
        uint8_t ED;
};


