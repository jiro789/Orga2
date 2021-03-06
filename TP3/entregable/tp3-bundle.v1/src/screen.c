/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "screen.h"
#include "sched.h"
#include "mmu.h"


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
    if (c != 'q'){
        p[fila][columna].c = c;
    }
    p[fila][columna].a = color;
}

void screen_pintar_botin(unsigned char jug,uint fila, uint columna)
{ 
    p[fila][columna].c = 'O';
    if(jug == 0){
    p[fila][columna].a = C_FG_BLACK | C_BG_GREEN;
    }else{
    p[fila][columna].a = C_FG_BLACK | C_BG_CYAN;
}
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
    videoCache = obtener_pagina_libre();


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

    screen_inicializar_reloj_pirata();
}

int long_string(const char* s){
	int i = 0;
	while(s[i] != 0){
		i++;
	}
	return i;
}

void imprime_nombre_grupo(){
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
	char* nombre_grupo = " Circus / Family";
	int len = long_string(nombre_grupo);
	int x = VIDEO_COLS-len;
	int y = 0;
	int i = 0;
	for(i = 0; i<len; i++){
		p[y][x+i].a = (unsigned char) C_FG_LIGHT_CYAN;
		p[y][x+i].c = (unsigned char) nombre_grupo[i];
	}
}


unsigned char imprime_tecla(unsigned char n, unsigned char prev){
	if(n >= 0x80 && prev+0x80 != n){ //cuando presiono
		return 0;
	}
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
	int len;
	int x;
	int y = 0;
	int i = 0;
	if(n>=0x80){
		n = n-0x80; //recuperar el make
		char* mensajeSol = "Usted ha soltado la tecla: ";
		len = long_string(mensajeSol)+6;
		x = VIDEO_COLS-len;
		for(i = 0; i<len; i++){
			p[y][x+i].a = (unsigned char) C_FG_LIGHT_CYAN;
			p[y][x+i].c = (unsigned char) mensajeSol[i];
		}
	}else{
		char* mensajeA = "Usted ha presionado la tecla: ";
		len = long_string(mensajeA)+6;
		x = VIDEO_COLS-len;
		for(i = 0; i<len; i++){
			p[y][x+i].a = (unsigned char) C_FG_LIGHT_CYAN;
			p[y][x+i].c = (unsigned char) mensajeA[i];
		}
	}
	
	
	//~ int len = long_string(mensaje)+1;
	switch ( n ) {
 
                case 0x2a:  //Lshift
                        p[y][VIDEO_COLS-6].a = (unsigned char) C_FG_LIGHT_CYAN;
                        p[y][VIDEO_COLS-6].c = (unsigned char) 'L';
                        p[y][VIDEO_COLS-5].a = (unsigned char) C_FG_LIGHT_CYAN;
                        p[y][VIDEO_COLS-5].c = (unsigned char) 'S';
                        p[y][VIDEO_COLS-4].a = (unsigned char) C_FG_LIGHT_CYAN;
                        p[y][VIDEO_COLS-4].c = (unsigned char) 'H';
                        p[y][VIDEO_COLS-3].a = (unsigned char) C_FG_LIGHT_CYAN;
                        p[y][VIDEO_COLS-3].c = (unsigned char) 'I';
                        p[y][VIDEO_COLS-2].a = (unsigned char) C_FG_LIGHT_CYAN;
                        p[y][VIDEO_COLS-2].c = (unsigned char) 'F';
                        p[y][VIDEO_COLS-1].a = (unsigned char) C_FG_LIGHT_CYAN;
                        p[y][VIDEO_COLS-1].c = (unsigned char) 'T';
                break;
                case 0x36:  //Rshift
                        p[y][VIDEO_COLS-6].a = (unsigned char) C_FG_LIGHT_CYAN;
                        p[y][VIDEO_COLS-6].c = (unsigned char) 'R';
                        p[y][VIDEO_COLS-5].a = (unsigned char) C_FG_LIGHT_CYAN;
                        p[y][VIDEO_COLS-5].c = (unsigned char) 'S';
                        p[y][VIDEO_COLS-4].a = (unsigned char) C_FG_LIGHT_CYAN;
                        p[y][VIDEO_COLS-4].c = (unsigned char) 'H';
                        p[y][VIDEO_COLS-3].a = (unsigned char) C_FG_LIGHT_CYAN;
                        p[y][VIDEO_COLS-3].c = (unsigned char) 'I';
                        p[y][VIDEO_COLS-2].a = (unsigned char) C_FG_LIGHT_CYAN;
                        p[y][VIDEO_COLS-2].c = (unsigned char) 'F';
                        p[y][VIDEO_COLS-1].a = (unsigned char) C_FG_LIGHT_CYAN;
                        p[y][VIDEO_COLS-1].c = (unsigned char) 'T';
                break;
		//~ case
	}
	
	return 1;
}


void mostrar_clock(unsigned int indice) {
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;

	pirata_t pirata;

	if (jugadorJugando == 0){
		pirata = piratasA[indice];
	}else{
		pirata = piratasB[indice];
	}
	
	int y = VIDEO_FILS-2;
	int x = (2*pirata.indice);
	if (pirata.jugador == 0){
		x =  2 + x;
	}else{
		x = 57 + x;
	}
	
	p[y][x].a = (unsigned char) C_FG_LIGHT_GREY | (unsigned char) C_BG_BLACK;

    if(!pirata.vivo){
		p[y][x].c = 'X';
	}else{
    
		switch (pirata.clock) {
			case 0:
				p[y][x].c = '-';
				break;
			case 1:
				p[y][x].c = '\\';
				break;
			case 2:
				p[y][x].c = '|';
				break;
			case 3:
				p[y][x].c = '/';
				break;
		}
	}

	if (jugadorJugando == 0){
		piratasA[indice].clock = (pirata.clock + 1) % 4;;
	}else{
		piratasB[indice].clock = (pirata.clock + 1) % 4;;
	}
}
unsigned char corregirPosicion(jugador_t *j, pirata_t *pirata){
    // miro si es el jugador 1 o el dos a traves del puerto
    if (j->puerto.x==1){
        return pirata->posicion.y + 1;
    }else {
        return pirata->posicion.y + 1;
    }
}

void screen_pintar_pirata(jugador_t *j, pirata_t *pirata){
    unsigned char y = corregirPosicion(j,pirata);
    unsigned char x = pirata->posicion.x;
    unsigned char color = screen_color_jugador(j);
    unsigned char colorFondo= j->color;
    unsigned char caracter = screen_caracter_pirata(pirata->tipo);
   
    screen_pintar_rect('q', colorFondo, y - 1, x - 1, 3, 3); // pinte un rectangulo con el color del jugador de 3x3
    screen_pintar(caracter,(C_FG_WHITE | color), y, x); // ahi puse el caracter en el medio
    
}

void screen_borrar_pirata(jugador_t *j, pirata_t *pirata){
     unsigned int y= corregirPosicion(j,pirata);
     unsigned int x = pirata->posicion.x;

     unsigned char caracter = screen_caracter_pirata(pirata->tipo);
     screen_pintar(caracter, j->color, y, x); // ahi pinte el background del mismo color
}

unsigned char screen_color_jugador(jugador_t *j){
    
    return j->colorLetra;
}

unsigned char screen_caracter_pirata(unsigned int tipo){
   
    if(tipo == 0) {
        return 'E';
    }else{
        return 'M';
    }
}
void screen_pintar_rect(unsigned char c, unsigned char color, int fila, int columna, int alto, int ancho){
    int i = 0;
    for(i = 0; i<alto; i++){
        screen_pintar_linea_h(c,color,(fila + i), columna, ancho);
    }
}
void screen_pintar_linea_h(unsigned char c, unsigned char color, int fila, int columna, int ancho){
    int i = 0;
    for(i = 0; i<ancho; i++){
        screen_pintar(c,color,fila, (columna + i));
    }
}
void screen_pintar_linea_v(unsigned char c, unsigned char color, int fila, int columna, int alto){
    int i = 0;
    for(i = 0; i<alto; i++){
        screen_pintar(c,color,(fila + i), columna);
    }
}

void screen_pintar_puntajes(){
    unsigned int puntajeJ1 = jugadores[0].puntaje;
    unsigned int puntajeJ2 = jugadores[1].puntaje;
    print_dec(puntajeJ1,3,36,47,C_FG_WHITE | C_BG_RED);
    print_dec(puntajeJ2,3,41,47,C_FG_WHITE | C_BG_BLUE);
}
 

void screen_stop_game_show_winner(jugador_t *j){
    unsigned int inicioX = VIDEO_COLS/2 - 15;
    unsigned int inicioY = VIDEO_FILS/2 - 10 ;
    screen_pintar_rect(' ', C_BG_BLACK, inicioY-1, inicioX-1, 22,32);
    screen_pintar_rect(' ', j->colorLetra , inicioY, inicioX, 20 , 30); // rectangulo de 20x30 con el color del jugador 
    print("Has ganado", inicioX+10, inicioY+5,C_FG_WHITE | j->colorLetra);
    print_dec((unsigned int)(j->puntaje),3 , inicioX + 13, inicioY + 10,  C_FG_WHITE | j->colorLetra); // escribo el puntaje calculando 3 pixeles para el mismo 
}  

void screen_inicializar_reloj_pirata(){
    unsigned int i;
    unsigned int columna1 = 4;
    unsigned int columna2 = 59;
    for(i=1;i<9;i++){
        print_dec(i,1,columna1,46,C_FG_WHITE);
        print_dec(i,1,columna2,46,C_FG_WHITE);
        screen_pintar('-',C_FG_WHITE, 48, columna1 );
        screen_pintar('-',C_FG_WHITE, 48, columna2 );
        columna1 += 2;
        columna2 += 2;
    }
}  
void screen_copiar_pantalla(){
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    ca (*q)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) videoCache;
    int y, x;
    for(y=8; y<44; y++){
        for(x=25; x<55; x++){
            q[y][x].c = p[y][x].c;
            q[y][x].a = p[y][x].a;
        }
    }
}
void screen_restaurar_pantalla(){
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    ca (*q)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) videoCache;
    int y, x;
    for(y=8; y<44; y++){
        for(x=25; x<55; x++){
            p[y][x].c = q[y][x].c;
            p[y][x].a = q[y][x].a;
        }
    }
}

void screen_muestra_error(char *error, unsigned int ebp, unsigned int edi, unsigned int esi
                            , unsigned int edx, unsigned int ecx, unsigned int ebx, unsigned int eax
                            , unsigned int ds, unsigned int es, unsigned int fs, unsigned int gs
                            , unsigned int errorCode, unsigned int eip, unsigned int cs
			    , unsigned int eflags, unsigned int *esp, unsigned int ss){
    unsigned int inicioC = 25;
    unsigned int inicioF = 8;
    unsigned int alto = 36;
    unsigned int ancho = 30;
    breakpoint();
   
        screen_copiar_pantalla();
        screen_pintar_rect(' ',C_BG_BLACK,inicioF, inicioC, alto , ancho); // pinto el rectangulo externo en negro
        screen_pintar_rect(' ',C_BG_LIGHT_GREY,inicioF+1, inicioC+1, alto -2 , ancho -2); // pinto el rectangulo gris
        screen_pintar_linea_h(' ',C_BG_RED,inicioF+1,inicioC+1,ancho-2);
	print(error, inicioC+1, inicioF+1,C_FG_BLACK|C_BG_RED);
        
        unsigned int columna1= inicioC + 3;
        unsigned int columna2= inicioC + 16;
        unsigned int fila = inicioF + 3;

        unsigned int cr0 = rcr0();
        unsigned int cr2 = rcr2();
        unsigned int cr3 = rcr3();
        unsigned int cr4 = rcr4();
        
        //void print_hex(uint numero, int size, uint x, uint y, unsigned short attr)

        print( "eax", columna1, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(eax,8,columna1 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        print( "cr0", columna2, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(cr0,8,columna2 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        fila +=2;
        print( "ebx", columna1, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(ebx,8,columna1 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        print( "cr2", columna2, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(cr2,8,columna2 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        fila +=2;
        print( "ecx", columna1, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(ecx,8,columna1 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        print( "cr3", columna2, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(cr3,8,columna2 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        fila +=2;
        print( "edx", columna1, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(edx,8,columna1 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        print( "cr4", columna2, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(cr4,8,columna2 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        fila +=2;
        print( "esi", columna1, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(esi,8,columna1 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        fila +=2;
        print( "edi", columna1, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(edi,8,columna1 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        fila +=2;
        print( "ebp", columna1, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(ebp,8,columna1 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        fila +=2;
        print( "esp", columna1, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex((unsigned int)esp,8,columna1 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        fila +=2;
        print( "eip", columna1, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(eip,8,columna1 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        fila +=1;
        print( "stack", columna2, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        fila +=1;
        print( "cs", columna1, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(cs,4,columna1 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        fila +=2;
        print( "ds", columna1, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        
        // aca printear las cosas de stack
        print_hex(esp[0],8,columna2 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        print_hex(esp[1],8,columna2 + 4, fila + 1, C_FG_WHITE|C_BG_LIGHT_GREY );
        print_hex(esp[2],8,columna2 + 4, fila + 2, C_FG_WHITE|C_BG_LIGHT_GREY );
        print_hex(esp[3],8,columna2 + 4, fila + 3, C_FG_WHITE|C_BG_LIGHT_GREY );

        print_hex(ds,4,columna1 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        fila +=2;
        print( "es", columna1, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(es,4,columna1 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        fila +=2;
        print( "fs", columna1, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(fs,4,columna1 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        fila +=2;
        print( "gs", columna1, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(gs,4,columna1 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        fila +=2;
        print( "ss", columna1, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(ss,4,columna1 + 4, fila, C_FG_WHITE|C_BG_LIGHT_GREY );
        fila +=2;
        print( "eflags", columna1, fila, C_FG_BLACK|C_BG_LIGHT_GREY);
        print_hex(eflags,8,columna1 + 6, fila, C_FG_WHITE|C_BG_LIGHT_GREY );

    
}

