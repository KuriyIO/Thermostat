#ifndef __MENU_H__
#define __MENU_H__

void menu_upd(void);
char blinkCharTime(char symbol, uint8_t position);
char blinkCharMode(char symbol, uint8_t position);
char blinkChar(char symbol);
void modeUpdStr(char* str, uint8_t i, char nav);
void modeUpBtn(uint8_t i);
void modeDownBtn(uint8_t i);
void modeSelBtn(void);

extern uint8_t page;

enum menu_pages {
    home,
    sets_mode,
    sets_time,
    sets_led,
    mode1,
    mode2,
    mode3,
    mode4,
    time,
    led
};

typedef struct {
    unsigned sth: 4;
    unsigned stl: 4;
    unsigned eth: 4;
    unsigned etl: 4;
    unsigned th: 4;
    unsigned tl: 4;
    unsigned hyst: 4;
} modes;

typedef struct {
    unsigned led_t: 2;
} ledTime;

enum pos_mode {
    pos_sth, pos_stl,
    pos_eth = 3, pos_etl,
    pos_th = 7, pos_tl,
    pos_hyst = 13,
    pos_nav = 15
};

enum pos_time {
    pos_hh, pos_hl,
    pos_mh = 3, pos_ml,
    pos_hm = 6
};

enum led_time {
    off,
    sec30,
    min1,
    on
};

#pragma location=0x1000
extern modes mode[4];
extern uint8_t led_t;

#endif
