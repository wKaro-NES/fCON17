#ifndef _FCON17-MANIFEST-DATA-H_
#define _FCON17-MANIFEST-DATA-H_

#include "neslib.h"


// ---------------------------------------------------------------------
// 8x8 metasprite Cheep-cheep
// ---------------------------------------------------------------------
 
#define METASPRITE_CHR_OFFSET  0x0
#define METASPRITE_SIZE (52 * 4 + 1)

const byte metasprite[]={
        0x10,0x00,0x01,0x00, 		//x,y,tile,attributes
        0x18,0x00,0x02,0x00, 
        0x20,0x00,0x03,0x00, 
        0x28,0x00,0x04,0x00, 

        0x08,0x08,0x05,0x00, 
        0x10,0x08,0x29,0x02,
        0x18,0x08,0x2A,0x02,
        0x20,0x08,0x2B,0x02,
        0x28,0x08,0x06,0x00, 
        0x30,0x08,0x07,0x00, 
        0x38,0x08,0x08,0x00, 

        0x08,0x10,0x09,0x00, 
        0x10,0x10,0x0E,0x01,
        0x18,0x10,0x0F,0x01,
        0x20,0x10,0x0A,0x00, 
        0x28,0x10,0x0B,0x00, 
        0x30,0x10,0x0C,0x00, 
        0x38,0x10,0x0D,0x00, 
 
        0x00,0x18,0x10,0x00, 
        0x08,0x18,0x11,0x00, 
        0x10,0x18,0x16,0x01,
        0x18,0x18,0x17,0x01,
        0x20,0x18,0x12,0x00, 
        0x28,0x18,0x13,0x00, 
        0x30,0x18,0x14,0x00, 
        0x38,0x18,0x15,0x00, 

        0x00,0x20,0x2C,0x02,
        0x08,0x20,0x2D,0x02,
        0x10,0x20,0x18,0x00, 
        0x18,0x20,0x1B,0x01,
        0x20,0x20,0x1C,0x01,
        0x28,0x20,0x19,0x00,
        0x30,0x20,0x2E,0x03,
        0x38,0x20,0x1A,0x00,
  
        0x00,0x28,0x1D,0x00, 
        0x08,0x28,0x2F,0x02,
        0x10,0x28,0x1E,0x00, 
        0x18,0x28,0x1F,0x00, 
        0x20,0x28,0x30,0x03,
        0x28,0x28,0x31,0x03,
        0x30,0x28,0x32,0x03,
        0x38,0x28,0x20,0x00, 

        0x08,0x30,0x21,0x00, 
        0x10,0x30,0x22,0x00, 
        0x18,0x30,0x23,0x00, 
        0x20,0x30,0x24,0x00, 
        0x28,0x30,0x33,0x03,
        0x30,0x30,0x34,0x03,
        0x38,0x30,0x25,0x00, 

        0x18,0x38,0x26,0x00, 
        0x20,0x38,0x27,0x00, 
        0x28,0x38,0x28,0x00, 
    
        0x80};				//mark end of oam definition

// ---------------------------------------------------------------------
// FamiCON 17 (aka fCON17) logo and text info
// ---------------------------------------------------------------------

#define NAMETABLE_CHR_LOGO_OFFSET 0x35
#define NAMETABLE_CHR_LETTERS_OFFSET 0xBC
#define LETTERS_SIZE 0x44
#define LOGO_X 6
#define LOGO_WIDTH 20

typedef enum line_type {
  LOGO,
  TEXT,
  TEXT_CURSOR,
  COLOR,
  SHOT
} line_type;

typedef struct line {
  line_type type;
  const byte* data;
} line;

#define COLOR_SCREEN 0x21

// initial colors
// 1	text shadow	0x0F
// 2	unused		0x10
// 3 	text		0x30
  //{COLOR, "\x0F\x10\x30"},		//1st (default)
  //{COLOR, "\x0F\x10\x31"},		//2nd
  //{COLOR, "\x0F\x10\x21"},		//3rd
  //{COLOR, "\x21\x10\x21"},		//4th (last)



const line screen[] = {
  {COLOR, "\x0F\x10\x30"},		//1st (default)
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {LOGO, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00"},
  {LOGO, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x03\x04\x05\x06\x00"},
  {LOGO, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x07\x08\x09\x0a\x0b\x0c\x0d"},
  {LOGO, "\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x00\x1a\x1b\x1c\x1d\x1e\x1f\x00"},
  {LOGO, "\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33"},
  {LOGO, "\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x39\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46"},
  {LOGO, "\x47\x48\x49\x37\x38\x39\x3d\x4a\x4b\x39\x3d\x3e\x4c\x4d\x4e\x4f\x50\x51\x52\x53"},
  {LOGO, "\x54\x55\x56\x37\x38\x39\x3d\x57\x58\x59\x3d\x3e\x5a\x5b\x5c\x5d\x5e\x5f\x60\x00"},
  {LOGO, "\x61\x55\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x48\x6b\x6c\x6d\x6e\x6f\x70\x71\x00"},
  {LOGO, "\x72\x73\x74\x55\x55\x55\x55\x75\x76\x77\x78\x55\x55\x79\x7a\x7b\x7c\x7d\x7e\x00"},
  {LOGO, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x80\x81\x82\x83\x00\x84\x00"},
  {LOGO, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x85\x86\x00\x00\x00\x00"},
  {TEXT_CURSOR,"Konsole 8-bit"},
  {TEXT_CURSOR,"Famicom @ NES @ Pegasus"},
  {TEXT,""},
  {TEXT_CURSOR,"11-16 lipca 2025"},
  {TEXT_CURSOR,"Cieksyn, gm. Nasielsk"},
  {TEXT_CURSOR,"remiza OSP"},
  {TEXT_CURSOR,"<60 km od Warszawy>"},
  {TEXT,""},
  {TEXT_CURSOR,"https://contrabanda.eu/"},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,"Harmonogram i atrakcje:"},
  {TEXT,""},
  {TEXT,"\x5B\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5D"},
  {TEXT_CURSOR,"\x5F PI$TEK 11 lipca \x5F"},
  {TEXT,"\x5E\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x60"},
  {TEXT,""},
  {TEXT,"22:00"},
  {TEXT,"Otwarcie-rozwarcie"},
  {TEXT,"fCON17"},
  {TEXT,""},
  {TEXT,"23:00"},
  {TEXT,"Folkowe piosenki"},
  {TEXT,"dla folkowych ludzi"},
  {TEXT,"- PCV edition"},
  {TEXT,"<Gargi>"},
  {TEXT,""},
  {TEXT,"\x5B\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5D"},
  {TEXT_CURSOR,"\x5F SOBOTA 12 lipca \x5F"},
  {TEXT,"\x5E\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x60"},
  {TEXT,""},
  {TEXT,"12:00"},
  {TEXT,"Trzecie Mistrzostwa"},
  {TEXT,"Szwole;er*w Pancernych"},
  {TEXT,"im. porucznika Zdzis(awa"},
  {TEXT,"Ziemskiego"},
  {TEXT,"<Mcin>"},
  {TEXT,""},
  {TEXT,"14:00"},
  {TEXT,"Kocham pivo"},
  {TEXT,"<Pieseu>"},
  {TEXT,""},
  {TEXT,"15:50"},
  {TEXT,"Pami$tkowe zdj'cie"},
  {TEXT,"<daf>"},
  {TEXT,""},
  {TEXT,"16:00"},
  {TEXT,"Micro Machines Challenge"},
  {TEXT,"#7 - New Chapter"},
  {TEXT,"<OsA>"},
  {TEXT,""},
  {TEXT,"19:00"},
  {TEXT,"Super Mario"},
  {TEXT,"Czterdziestolatek,"},
  {TEXT,"mini-turniej-ho(d"},
  {TEXT,"<daf>"},
  {TEXT,""},
  {TEXT,"20:00"},
  {TEXT,"Freestyle Compo 2025"},
  {TEXT,"<Duobix>"},
  {TEXT,""},
  {TEXT,"22:00"},
  {TEXT,"Relacja z FamiCON 16"},
  {TEXT,"<Krisuroku>"},
  {TEXT,""},
  {TEXT,"24:00"},
  {TEXT,"Festiwal Chiptu)sk*w pod"},
  {TEXT,"szyldem \"bo dawno nie by(o\""},
  {TEXT,"<Chiyoko>"},
  {TEXT,""},
  {TEXT,"\x5B\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5D"},
  {TEXT_CURSOR,"\x5F NIEDZIELA 13 lipca \x5F"},
  {TEXT,"\x5E\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x60"},
  {TEXT,""},
  {TEXT,"11:00"},
  {TEXT,"Baseball - o co w tym"},
  {TEXT,"chodzi? - warsztaty"},
  {TEXT,"<OsA>"},
  {TEXT,""},
  {TEXT,"12:00"},
  {TEXT,"Andruty #2 - Jebnij se"},
  {TEXT,"t' pi(k' ch(opie"},
  {TEXT,"<OsA>"},
  {TEXT,""},
  {TEXT,"14:00"},
  {TEXT,"ContraBanda World"},
  {TEXT,"Championships 2025"},
  {TEXT,"<Martin>"},
  {TEXT,""},
  {TEXT,"16:00"},
  {TEXT,"NESpodziewanka - zlot"},
  {TEXT,"F17 edition"},
  {TEXT,"<Duobix>"},
  {TEXT,""},
  {TEXT,"18:00"},
  {TEXT,"Pixeloza 17.0"},
  {TEXT,"<daf&MWK>"},
  {TEXT,""},
  {TEXT,"20:00"},
  {TEXT,"Wielka Gra: konkurs"},
  {TEXT,"wiedzy o grach"},
  {TEXT,"<daf>"},
  {TEXT,""},
  {TEXT,"22:00"},
  {TEXT,"FamiCON Cinema Vezdival:"},
  {TEXT,"II Letni Przegl$d Film*w"},
  {TEXT,"Prawie Niezale;nych"},
  {TEXT,"<OsA>"},
  {TEXT,""},
  {TEXT,"\x5B\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5D"},
  {TEXT_CURSOR,"\x5F PONIEDZIA(EK 14 lipca \x5F"},
  {TEXT,"\x5E\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x60"},
  {TEXT,""},
  {TEXT,"12:00"},
  {TEXT,"Bombermania"},
  {TEXT,"<Preki>"},
  {TEXT,""},
  {TEXT,"14:00"},
  {TEXT,"Baseball - Cieksyn"},
  {TEXT,"Series 2025"},
  {TEXT,"<OsA>"},
  {TEXT,""},
  {TEXT,"18:00"},
  {TEXT,"Ojciec Mateusz:"},
  {TEXT,"Kryminalne Zagadki"},
  {TEXT,"Sandomierza"},
  {TEXT,"<Pieseu>"},
  {TEXT,""},
  {TEXT,"20:00"},
  {TEXT,"FamiCON Cinema Vezdival:"},
  {TEXT,"Srogi Dokumentsk"},
  {TEXT,"<Mcin>"},
  {TEXT,""},
  {TEXT,"\x5B\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5D"},
  {TEXT_CURSOR,"\x5F WTOREK 15 lipca \x5F"},
  {TEXT,"\x5E\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x60"},
  {TEXT,""},
  {TEXT,"Chillout przy grach"},
  {TEXT,""},
  {TEXT,"\x5B\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5D"},
  {TEXT_CURSOR,"\x5F +RODA 16 lipca \x5F"},
  {TEXT,"\x5E\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x60"},
  {TEXT,""},
  {TEXT,"Pakowanie, sprz$tanie,"},
  {TEXT,"po;egnanie"},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,"\x5B\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5D"},  
  {TEXT,"\x5F @   fCON17 Manifest  @ \x5F"},
  {TEXT,"\x5F                        \x5F"},
  {TEXT,"\x5F Inspiracja:            \x5F"},
  {TEXT,"\x5F                        \x5F"},
  {TEXT_CURSOR,"\x5F  grafika     <daf>     \x5F"},
  {TEXT_CURSOR,"\x5F  technologia <Mcin>    \x5F"},
  {TEXT_CURSOR,"\x5F              <Verteks> \x5F"},
  {TEXT_CURSOR,"\x5F  pistolet    <nesdoug> \x5F"},
  {TEXT,"\x5F                        \x5F"},
  {TEXT,"\x5F Realizacja:            \x5F"},
  {TEXT,"\x5F                        \x5F"},
  {TEXT_CURSOR,"\x5F  kod         <wKaro>   \x5F"},
  {TEXT_CURSOR,"\x5F  testy       <Mcin>    \x5F"},
  {TEXT,"\x5F  muzyka                \x5F"},
  {TEXT,"\x5F      \"After the rain\"  \x5F"},
  {TEXT_CURSOR,"\x5F      <8bitworkshop.com>\x5F"},
  {TEXT,"\x5F                        \x5F"},
  {TEXT,"\x5F Konfiguracja:          \x5F"},
  {TEXT,"\x5F                        \x5F"},
  {TEXT,"\x5F  port 1 Game pad       \x5F"},
  {TEXT,"\x5F  port 2 Zapper         \x5F"},
  {TEXT,"\x5E\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x5C\x60"},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {COLOR, "\x0F\x10\x31"},		//2nd  
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {COLOR, "\x0F\x10\x21"},		//3rd
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {SHOT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT,""},
  {TEXT, 0}	//mark end of screen definition
};

#endif	//fCON17-manifest-data.h
