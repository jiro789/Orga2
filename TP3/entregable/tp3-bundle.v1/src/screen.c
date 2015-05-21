/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "screen.h"
#include "game.h"


extern jugador_t jugadorA, jugadorB;


static ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;

const char reloj[] = "|/-\\";
#define reloj_size 4


void screen_actualizar_reloj_global()
{
    static uint reloj_global = 0;

    reloj_global = (reloj_global + 1) % reloj_size;

    screen_pintar(reloj[reloj_global], C_BW, 49, 79);
}

void screen_pintar(uchar c, uchar color, uint fila, uint columna)
{
    p[fila][columna].c = c;
    p[fila][columna].a = color;
}

uchar screen_valor_actual(uint fila, uint columna)
{
    return p[fila][columna].c;
}

void print(const char * text, uint x, uint y, unsigned short attr) {
    int i;
    for (i = 0; text[i] != 0; i++)
     {
        screen_pintar(text[i], attr, y, x);

        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(uint numero, int size, uint x, uint y, unsigned short attr) {
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_dec(uint numero, int size, uint x, uint y, unsigned short attr) {
    int i;
    char letras[16] = "0123456789";

    for(i = 0; i < size; i++) {
        int resto  = numero % 10;
        numero = numero / 10;
        p[y][x + size - i - 1].c = letras[resto];
        p[y][x + size - i - 1].a = attr;
    }
}

void inic_video(){
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;


    int x = 0;
    int y = 0;
    //PRIMERA FILA EN NEGRO
    while (x<VIDEO_COLS){
          p[0][x].c = (unsigned char) ' ';
          p[0][x].a = (unsigned char) C_BG_BLACK;
          x++;
    }
    x=0;
    y=1;
    //PANTALLA EN GRIS
    while (y<45){
          p[y][x].c = (unsigned char) ' ';
          p[y][x].a = (unsigned char) C_BG_LIGHT_GREY;
          x++;
          if (x == VIDEO_COLS){
            x = 0;
            y++;
      }
    }
    y = VIDEO_FILS -5;
    x = 0;
    while (y<VIDEO_FILS){
        p[y][x].c = (unsigned char) ' ';
        p[y][x].a = (unsigned char) C_BG_BLACK;
        x++;
        if (x == VIDEO_COLS){
            x= 0;
            y++;
        }
        
    }
    y = VIDEO_FILS - 5;
    x = (VIDEO_COLS/2) - 5;
    while (y<VIDEO_FILS){
        p[y][x].c = (unsigned char) ' ';
        if(x<VIDEO_COLS/2){
            p[y][x].a = (unsigned char) C_BG_RED;
        }
        else{
            p[y][x].a = (unsigned char) C_BG_BLUE;
        }
        x++;
        if(x == VIDEO_COLS/2+5){
            x = VIDEO_COLS/2 -5;
            y++;
        }
    }
    //0s para puntaje jugador 1
    p[47][36].c = (unsigned char) '0';
    p[47][36].a = (unsigned char) C_FG_WHITE;
    p[47][37].c = (unsigned char) '0';
    p[47][37].a = (unsigned char) C_FG_WHITE;
    p[47][38].c = (unsigned char) '0';
    p[47][38].a = (unsigned char) C_FG_WHITE;
    //0s para puntaje jugador 2
    p[47][41].c = (unsigned char) '0';
    p[47][41].a = (unsigned char) C_FG_WHITE;
    p[47][42].c = (unsigned char) '0';
    p[47][42].a = (unsigned char) C_FG_WHITE;
    p[47][43].c = (unsigned char) '0';
    p[47][43].a = (unsigned char) C_FG_WHITE;


}

