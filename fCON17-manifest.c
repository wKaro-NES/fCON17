/*

 "fCON17 manifest", demo coded by Wojciech "wKaro" Karolik 
  More info about
    event: Famicon 17, https://contrabanda.eu/o-zlocie-famicon/
    tools: https://8bitworkshop.com/ and Steven Hugg's book
    hints:
           full visual cursor effects require PAL TV system,
           PAD controller is supported on port 1
           ZAPPER (light gun) is supported on port 2

*/

// NES_MIRRORING = 0; # 0 horizontal, 1 vertical, 8 four screen
#define NES_MIRRORING 0

#include <stdlib.h>
#include <string.h>

// include NESLIB header
#include "neslib.h"

// include CC65 NES Header (PPU)
#include <nes.h>

// link the pattern table into CHR ROM
//#link "fCON17-manifest-chr.s"

// VRAM update buffer
#include "vrambuf.h"
//#link "vrambuf.c"

// setup Famitone library
//#link "famitone2.s"
void __fastcall__ famitone_update(void);

//#link "music_aftertherain.s"
extern char after_the_rain_music_data[];

//#link "fCON17-manifest-sfx.s"
extern char sfx_sounds[];

// zapper library
#include "zaplib.h"
//#link "zaplib.s"

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// fCON17 data structures and content
#include "fCON17-manifest-data.h"

// initial colors

/*{pal:"nes",layout:"nes"}*/
const byte palette[32] = { 
  0x00,					// screen color

  0x0F,0x10,0x30,0x00,			// background palette 0
  0x09,0x19,0x29,0x00,			// background palette 1
  0x0A,0x1A,0x2A,0x00,			// background palette 2
  0x0B,0x1B,0x2B,0x00,			// background palette 3

  0x30,0x31,0x0F,0x00,			// sprite palette 0
  0x26,0x16,0x0F,0x00,			// sprite palette 1
  0x27,0x38,0x0F,0x00,			// sprite palette 2
  0x35,0x38,0x0F			// sprite palette 3
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// fCON17 code

// setup PPU and tables
void setup_graphics() {
  // clear sprites
  oam_clear();
  // set palette colors
  pal_all(palette);
  
  // clear vram buffer
  vrambuf_clear();
  // set NMI handler
  set_vram_update(updbuf);
  
  // turn on PPU
  ppu_on_all();
}

// ---------------------------------------------------------------------
// color effects

byte flash_color = 0x00;

void update_flash(bool full_color) {
  pal_col(2, flash_color);		// background palette 0 color 2
  flash_color += 0x10;			// increase bright
  if (flash_color > 0x40) {
    flash_color = flash_color & 0x0F;
    if (full_color) {
      if (++flash_color > 0x0C)		// next color
        flash_color = 0x01;
    }
  }
}

// ----------------------------------------

#define VIRTUAL_BRIGHT_STAGES 10
const byte virtual_bright[VIRTUAL_BRIGHT_STAGES] = { 4,5,6,7,8,8,7,6,5,4 };
byte bright_stage = 0;

bool update_bright() {
  pal_bg_bright(virtual_bright[bright_stage++]);
  if (bright_stage == VIRTUAL_BRIGHT_STAGES) {
    bright_stage = 0;
    pal_col(0, COLOR_SCREEN);
  }
  
  return !bright_stage;
}

// ----------------------------------------

void update_color(const line* line) {
  pal_col(1, line->data[0]);
  pal_col(2, line->data[1]);
  pal_col(3, line->data[2]);
}

// ---------------------------------------------------------------------
// screen

bool skip_wait = FALSE;	

void wait_or_skip_frame() {
    if (skip_wait)
      skip_wait = FALSE;
    else
      ppu_wait_frame();  
}

// ----------------------------------------

byte line_buf[32];

void update_screen(const line* line, byte col, byte row, byte cursor, bool both) {
  byte i;
  const byte* text_line;
  memset(line_buf,' ' - 0x20 + NAMETABLE_CHR_LETTERS_OFFSET,32); 
  if (line) {
    text_line =  line->data;
    if (line->type == LOGO) {
      for (i = 0; i < LOGO_WIDTH; i++) {
        line_buf[i + LOGO_X] = text_line[i] + NAMETABLE_CHR_LOGO_OFFSET;
      }
    }
    else {
      word size = strlen(text_line);
      if (size > 32)
        size = 32;
      col = (32 - size) / 2;      	//center
      for (i = 0; i < size; i++) {
        line_buf[i + col] = text_line[i];
        if (text_line[i] >= 'a' && text_line[i] <= 'z')
          line_buf[i + col] = text_line[i] - 0x20;	//toupper()

        line_buf[i + col] = line_buf[i + col] - 0x20 + NAMETABLE_CHR_LETTERS_OFFSET;

        if (i == cursor) {
          line_buf[i + col + 1] = 0x61 - 0x20 + NAMETABLE_CHR_LETTERS_OFFSET;
          break;
        }      
      }          
    }
  }

  if (row < 30) {
    vrambuf_put(NTADR_A(0,row), &line_buf[0], 32);
    if (both)
      vrambuf_put(NTADR_C(0,row), &line_buf[0], 32);
  }
  else if (row < 60) {
    vrambuf_put(NTADR_C(0,row-30), &line_buf[0], 32);
    if (both)
      vrambuf_put(NTADR_A(0,row-30), &line_buf[0], 32);
  }

  vrambuf_flush();
  
  skip_wait = TRUE;		//flushing buffer occurs in NMI handler
}

// ----------------------------------------

word text_idx = 0;		// text index in table
word text_idx_last = 0;

// helper functions for handling dynamic text index

void prepare_idx() {
  if (screen[text_idx].type == TEXT && screen[text_idx].data == 0x00) {
    text_idx_last = text_idx;
    text_idx = 0;		//end of text
  }
}

const line* prepare_line() {
  while (TRUE) {
    prepare_idx();
    if (screen[text_idx].type != COLOR)
      break;
    update_color(&screen[text_idx]);
    text_idx++;
  };
  
  return &screen[text_idx];
}

// ---------------------------------------------------------------------
// audio

void setup_audio() {
  famitone_init(after_the_rain_music_data);
  sfx_init(sfx_sounds);

  // set music callback function for NMI
  nmi_set_callback(famitone_update);
}

void update_music() {
  music_play(0);
}

byte sound_interval = 0;
void update_sound() {
  if (!sound_interval--) {
    sfx_play(0,3);
    sound_interval = 2;
  }
}

void update_sound_gun() {
  sfx_play(1,3);  
}

// ---------------------------------------------------------------------
// zapper

const byte palette_sprite_front[16] = { 
  0x00,0x30,0x31,0x0F,
  // red
  0x00,0x26,0x16,0x0F,
  0x00,0x27,0x38,0x0F,
  0x00,0x35,0x38,0x0F
};

const byte palette_sprite_behind[16] = { 
  0x00, 0x30,0x31,0x0F,
  // gray
  0x00,0x10,0x00,0x0F,
  0x00,0x27,0x38,0x0F,
  0x00,0x35,0x38,0x0F
};

const byte palette_sprite_target[16] = { 
  0x00,0x30,0x30,0x30,
  0x00,0x30,0x30,0x30,
  0x00,0x30,0x30,0x30,
  0x00,0x30,0x30,0x30
};

const byte palette_sprite_hit[16] = { 
  0x00,0x30,0x31,0x0F,
  // blue
  0x00,0x22,0x12,0x0F,
  0x00,0x27,0x38,0x0F,
  0x00,0x35,0x38,0x0F
};

typedef enum sprite_state {
  INIT,
  FRONT,
  HIT,
  BEHIND
} sprite_state;

sprite_state sprite = INIT;

void update_state(sprite_state state) {
  sprite = state;
  switch (state) {
    default:
    case INIT:
    case FRONT:
      pal_spr(palette_sprite_front);
      break;
    case (BEHIND):
      pal_spr(palette_sprite_behind);
      break;
    case (HIT):
      pal_spr(palette_sprite_hit);      
      break;
  }
}

bool zapper_ready = FALSE;
byte zapper = 0;

void check_zapper() {
  zapper_ready = !zapper;		// prevents autofire
  zapper = zap_shoot(1); 		// controller 2

  if(zapper && zapper_ready) {
    update_sound_gun();

    //change sprite palette to white & screen color to true black
    pal_col(0, 0x00);
    pal_bg_bright(0);
    pal_spr(palette_sprite_target);
    
    //turn off background
    //eq ppu_mask(MASK_SPR + MASK_EDGE_SPR + MASK_EDGE_BG);
    ppu_on_spr();
    
    //wait for next frame, turn on background
    ppu_wait_nmi();
    
    //eq ppu_mask(MASK_SPR + MASK_BG + MASK_EDGE_SPR + MASK_EDGE_BG);
    ppu_on_all();

    //check button
    if (zap_read(1)) {
      update_state(HIT);
    }
    else{
      update_state(FRONT);
    }
    
    //restore colors
    pal_col(0, COLOR_SCREEN);
    pal_bg_bright(4);
  }
}

// sprites

void prepare_metasprite(const byte* src, byte* dst, word size, bool oam_flip_h, bool oam_behind) {
  word i;
  for (i = 0; i < size - 1; i += 4){
    *(dst+i+1) = *(src+i+1);
    *(dst+i+2) = *(src+i+2) + METASPRITE_CHR_OFFSET;
    if (oam_flip_h) {
        *(dst+i) = 0x38 - *(src+i);
      	*(dst+i+3) = *(src+i+3) | OAM_FLIP_H;
    }
    else {
        *(dst+i) = *(src+i);
        *(dst+i+3) = *(src+i+3);
    }
    if (oam_behind) {
      *(dst+i+3) = *(dst+i+3) | OAM_BEHIND;
    }
    else {
      *(dst+i+3) = *(dst+i+3) & ~OAM_BEHIND;
    }
  }
  *(dst+i) = *(src+i);			//end of definition, should be always 0x80
}

// ----------------------------------------

byte sprite_x = 50 , sprite_y = 50;	// sprite x/y positions
sbyte sprite_dx = 1, sprite_dy = 2;	// sprite x/y deltas per frame (signed)
byte oam_id;				// sprite ID

byte metasprite_draw[METASPRITE_SIZE];	// current metasprite buffer (orientation, priority etc.)

#define SPRITE_X_MAX (256 - 72)
#define SPRITE_X_MIN (8)
void update_metasprite(byte pad) {
  oam_id = 0;
  oam_id = oam_meta_spr(sprite_x, sprite_y, oam_id, metasprite_draw);

  //manual move disruption
  if ( sprite_x > SPRITE_X_MIN && sprite_x < SPRITE_X_MAX) {
    if (pad & PAD_LEFT)		// decelerate
      sprite_x -= sprite_dx;
    if (pad & PAD_RIGHT)	// accelerate 
      sprite_x += sprite_dx;
  }

  sprite_x += sprite_dx;
  sprite_y += (rand() & 3) - sprite_dy;

  // now it is time to use light gun to kill the fish
  if (sprite == FRONT) {
    check_zapper(); 
  }
  
  //check boundaries
  if (sprite_y < 8) {
    sprite_dy = -1;
    prepare_metasprite(metasprite, metasprite_draw, METASPRITE_SIZE, sprite_dx < 0, FALSE);
    update_state(FRONT);
  }

  if (sprite_y > (240 - 72)) {
    sprite_dy = 2;
    prepare_metasprite(metasprite, metasprite_draw, METASPRITE_SIZE, sprite_dx < 0, TRUE);
    update_state(BEHIND);
  }

  if (sprite == HIT || sprite_x > SPRITE_X_MAX || sprite_x < SPRITE_X_MIN) {
    sprite_dx = -sprite_dx;
    prepare_metasprite(metasprite, metasprite_draw, METASPRITE_SIZE, sprite_dx < 0, sprite_dy > 0);
  }

  if (oam_id!=0) 
    oam_hide_rest(oam_id);
}


// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// main loop

#define IDLE_COUNTER 3*50
#define IDLE_FRAME_SKIP 0x01
#define FLASH_INITIAL_FRAME_SKIP 100
#define FLASH_FULL_INITIAL_FRAME_SKIP 300
#define SPRITE_INITIAL_FRAME_SKIP 200
#define CURSOR_OUT_OF_SCREEN 32

void main() {
  bool enable_flash = FALSE;
  bool enable_flash_full = FALSE;
  bool enable_bright = TRUE;
  bool enable_sprite = FALSE;
  bool enable_cursor = TRUE;

  byte pad;  

  word frame;				// frame counter
  word idle = 0;			// inactivity timer
  byte idle_skip = IDLE_FRAME_SKIP;	// slow down automatic scroll
  
  byte text_x = 0, text_y = 0;		// text x/y positions

  byte cursor = CURSOR_OUT_OF_SCREEN;	// current cursor posiion
  
  bool scrolling = FALSE;		// scroll screen ?
  word scroll_y = 0;			// scroll position
  word scroll_pause = 0;
  
  bool clear_next_line = FALSE;
  
  // draw sprite
  prepare_metasprite(metasprite, metasprite_draw, METASPRITE_SIZE, sprite_dx < 0, TRUE);
  
  // initialize PPU
  setup_graphics();

  // ...background color
  update_bright();

  // initialize audio system
  setup_audio();
    
  // loop forever
  while (1) {
    frame++;
    
    // read controller
    pad = pad_poll(0);
    
    // fade in/out
    if (enable_bright) {
      if ((frame & 0x03) == 0)
      	enable_bright = !update_bright();
    }
    
    // flashing
    if (enable_flash) {
      if ((frame & 0x03) == 0)
      	update_flash(enable_flash_full);
      if (!enable_flash_full) {
        if (frame > FLASH_FULL_INITIAL_FRAME_SKIP) {
          enable_flash_full = TRUE;
          //last line isn't always visible on NTSC due to overscan
          //so disable cursor effect when scrolling
	  enable_cursor = !ppu_system(); 	//0 for PAL, not 0 for NTSC
        }
      }
    }
    else {
      if (frame > FLASH_INITIAL_FRAME_SKIP)
        enable_flash = TRUE;
    }
    
    // draw and move metasprite
    if (enable_sprite) {
      	update_metasprite(pad);
    }
    else {
      if (frame > SPRITE_INITIAL_FRAME_SKIP) {
        enable_sprite = TRUE;
        update_music();
      }
    }
      
    // inactivity counter
    if (idle)
      idle--;
    else
      idle_skip = IDLE_FRAME_SKIP;

    // update screen
    if ((frame & idle_skip) == 0) {
      // needs more lines then scroll...
      if (scrolling) {
	// prepare next (hidden) line
        if (clear_next_line) {
          const line* next_line = prepare_line();
          if (enable_cursor && next_line->type == TEXT_CURSOR)
            update_screen(NULL, text_x, text_y, 32, FALSE);		//clear
          else
            update_screen(next_line, text_x, text_y, 32, FALSE);	//show next text
          clear_next_line = FALSE;
        }
	// manual speed overrride ?
        if (pad & PAD_UP) {
          idle = scroll_pause = IDLE_COUNTER; 	// reset & pause
          idle_skip = 0x00;
        }
        else if (pad & PAD_DOWN || idle == 0) {
          if (pad & PAD_DOWN) {
            idle = IDLE_COUNTER;		// reset
            idle_skip = 0x00;
          }
        }
	// do scrolling
        if (scroll_pause) {
          scroll_pause--;
        }
        else {
          scroll_y++;
          if (scroll_y >= 240)
            scroll_y = 0;
          if (scroll_y % 8 == 0)
            scrolling = FALSE;
          
          scroll(0, scroll_y);
        }
      }
      
      // print line...
      if (!scrolling) {
        line_type type = prepare_line()->type;
        
        if (enable_cursor && text_idx_last) 	//disable cursor effects after 1st pass 
          enable_cursor = FALSE;		// (faster, smoother scroll and don't mix sfx with music anymore)
        					
        if ((!enable_cursor) || (type == TEXT_CURSOR) && (pad & PAD_DOWN)) {
          type = TEXT;
          cursor = CURSOR_OUT_OF_SCREEN;
        }
        switch (type) {
          case TEXT:
            prepare_idx();
          case LOGO:
            update_screen(&screen[text_idx], text_x, text_y, 32, TRUE);
            clear_next_line = TRUE;
            text_idx++;
            text_y++;
            break;
          case TEXT_CURSOR:
            update_sound();
            if (cursor == CURSOR_OUT_OF_SCREEN) {
              cursor = 0;
              scrolling = FALSE;
            }
            else 
              cursor++;
            update_screen(&screen[text_idx], text_x, text_y, cursor, TRUE);
            if (cursor > strlen(screen[text_idx].data)) {
              //next line
              clear_next_line = TRUE;
              cursor = CURSOR_OUT_OF_SCREEN;
              text_idx++;
              text_y++;
            }
            break;
        }

        if (text_y > 29 && cursor == CURSOR_OUT_OF_SCREEN)
          scrolling = TRUE;
        if (text_y > 59)
          text_y = 30;
      }
    }
    
    // wait for next frame
    wait_or_skip_frame();
  }
}
