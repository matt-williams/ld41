//this example shows non-looped vertical scroll in the most primitive form
//it just puts random tiles on each new row, metatiles and tile attributes aren't used
//it would be impractical for a real game, but it shows the basics
//horizontal mirroring (gives 32x60 nametable layout) is used to hide the changing row

#include "neslib.h"


static unsigned char update_list[3+32+1];


//these macro are needed to simplify defining update list constants

#define NTADR(x,y)	((NAMETABLE_A|((y)<<5)|x))

#define MSB(x)		(((x)>>8))
#define LSB(x)		(((x)&0xff))

#define SCORE_X 16
 #define SCORE_Y 16
#define POWER_X 16
#define POWER_Y 26

const unsigned char bg_pal[] = {0x19, 0x09, 0x07, 0x29, 0x19, 0x27, 0x11, 0x21, 0x19, 0x09, 0x07, 0x29, 0x19, 0x09, 0x07, 0x29};
const unsigned char spr_pal[16]={ 0x19,0x00,0x10,0x16,0x19,0x00,0x10,0x11,0x19,0x0f,0x30,0x09,0x19,0x09,0x19,0x29 };


unsigned char frameno = 0;

signed char interp(signed char x) {
  unsigned char abs_x;
  unsigned char abs_interp_x;
  if (x >= 0) {
    abs_x = x;
  } else {
    abs_x = -x;
  }
  abs_interp_x = (abs_x >> 4);
  if (frameno & 1) {
    abs_interp_x += (abs_x >> 3) & 1;
  } else if (frameno & 2) {
    abs_interp_x += (abs_x >> 2) & 1;
  } else if (frameno & 4) {
    abs_interp_x += (abs_x >> 1) & 1;
  } else if (frameno & 8) {
    abs_interp_x += abs_x & 1;
  }
  if (x >= 0) {
    return abs_interp_x;
  } else {
    return -abs_interp_x;
  }
}


void main(void)
{
	static unsigned char i,row;
	static int y;
	static unsigned int adr;
        static unsigned char spr;
        static unsigned char mx[16];
        static unsigned char mvx[16];
        static unsigned char my[16];
        static unsigned char mvy[16];
        static unsigned char sx = 128;
        static signed char svx = 16;
        static unsigned char sy = 12;
        static char svy = 0;
unsigned char sx2;
unsigned char score = 0;
unsigned char power = 0;
unsigned char cx = 128;
unsigned char cy = 192;
unsigned char btns;

	pal_bg(bg_pal);
        pal_bg_bright(3);
        pal_spr(spr_pal);
        pal_spr_bright(4);
        oam_size(64);
	
	y=0;
	
	update_list[0]=0x20|NT_UPD_HORZ;//horizontal update sequence, dummy address
	update_list[1]=0x00;
	update_list[2]=32;//update sequence is 32 tiles wide
	update_list[3+32]=NT_UPD_EOF;
	
	ppu_on_all();//enable rendering
	

memfill(my, 255, 16);

	while(1)
	{
		scroll(0,y);//scroll value will be applied on the next nmi

  sx2 = sx;
  svx += interp(8 - (sx >> 4));
  sx += interp(svx);
  if ((sx2 > 32) && (sx <= 32)) {
    svx = -svx;
  } else if ((sx2 < 224) && (sx >= 224)) {
    svx = -svx;
  }

  svy += interp(2 - (sy >> 4));
  sy += interp(svy);
  
  if (!(frameno & 63)) {
    score += 1;
  }

  if (!(frameno&15)) {
    for (i = 0; i < 16; i++) {
      if (my[i] >= 255) {
        mx[i] = sx - 8 + ((y & 16) >> 1);
        mvx[i] = svx;
        my[i] = sy;
        mvy[i] = svy + 8;
        break;
      }
    }
  }
        spr = 0;

        spr = oam_spr(SCORE_X     , SCORE_Y, 0x30, 2, spr);
        spr = oam_spr(SCORE_X +  8, SCORE_Y, 0x31, 2, spr);
        spr = oam_spr(SCORE_X + 16, SCORE_Y, 0x32, 2, spr);
        if (score >= 100) {
          spr = oam_spr(SCORE_X + 25, SCORE_Y, 0x36 + score / 100, 2, spr);
        }
        if (score >= 10) {
          spr = oam_spr(SCORE_X + 34, SCORE_Y, 0x36 + (score / 10) % 10, 2, spr);
        }
        spr = oam_spr(SCORE_X + 43, SCORE_Y, 0x36 + score % 10, 2, spr);
        spr = oam_spr(POWER_X     , POWER_Y, 0x33, 2, spr);
        spr = oam_spr(POWER_X +  8, POWER_Y, 0x34, 2, spr);
        spr = oam_spr(POWER_X + 16, POWER_Y, 0x35, 2, spr);
        if (power >= 100) {
          spr = oam_spr(POWER_X + 25, POWER_Y, 0x36 + power / 100, 2, spr);
        }
        if (power >= 10) {
          spr = oam_spr(POWER_X + 34, POWER_Y, 0x36 + (power / 10) % 10, 2, spr);
        }
        spr = oam_spr(POWER_X + 43, POWER_Y, 0x36 + power % 10, 2, spr);

        spr = oam_spr(cx - 5, cy - 5, 0x20, 2, spr);
        spr = oam_spr(cx + 5, cy - 5, 0x21, 2, spr);
        spr = oam_spr(cx - 5, cy + 5, 0x22, 2, spr);
        spr = oam_spr(cx + 5, cy + 5, 0x23, 2, spr);

        spr = oam_spr(sx - 8, sy, 0x0e, 0, spr);
        spr = oam_spr(sx, sy, 0x0f, 0, spr);
  for (i = 0; i < 16; i++) {
    if (my[i] < 255) {
      spr = oam_spr(mx[i], my[i], 0x0d, 0, spr);
      mx[i] += interp(mvx[i]);
      mvy[i] += interp(2);
      my[i] += interp(mvy[i]);;
      if ((mx[i] - 1 > 248) || (my[i] < 8)) {
        my[i] = 255;
      }
    }
  }
  oam_hide_rest(spr);


		if(!(y&7))//put new row every 8 pixels
		{
			row=(y>>3)+30;//get row number for update, it is shifted just above the visible part of the screen
			
			if(row>=60) row-=60;//keep the row number within the limits
			
			adr=row<<5;//convert Y from pixels to tiles (row=y/8), then to nametable offset (offset=row*32)
			
			if(adr<960) adr=adr|NAMETABLE_A; else adr=(adr-960)|NAMETABLE_C;//add base address of one of two nametables
			
			update_list[0]=MSB(adr)|NT_UPD_HORZ;//set the address
			update_list[1]=LSB(adr);
			
			for(i=0;i<32;++i) {
                            if ((rand8() & 15) == 0) {
                                update_list[3+i]=10 + (rand8() & 1);//(rand8()&31);//fill row buffer with random tiles
                            } else {
update_list[3+i]=0;
                            }
                        }
	                set_vram_update(update_list);
		} else {
	                set_vram_update(NULL);
                }

    btns = pad_poll(0);
    if ((cx > 5) && (btns & PAD_LEFT)) {
      --cx;
    }
    if ((cx < 243) && (btns & PAD_RIGHT)) {
      ++cx;
    }
    if (cy > 12) {
      --cy;
    }
    if ((cy > 12) && (btns & PAD_UP)) {
      --cy;
    }
    if ((cy < 218) && (btns & PAD_DOWN)) {
      cy = cy + 3;
      if (cy > 218) {
        cy = 218;
      }
    }
                

		++y;//next pixel
		
		if(y>=240*2) y=0;//keep Y within the total height of two nametables

		ppu_wait_nmi();
++frameno;
	}
}
