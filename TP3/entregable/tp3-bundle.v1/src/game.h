/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "tss.h"


typedef enum direccion_e { ARR = 0x4, ABA = 0x7, DER = 0xA, IZQ = 0xD} direccion;

#define MAX_CANT_PIRATAS_VIVOS           8

#define JUGADOR_A                         0
#define JUGADOR_B                         1

#define MAPA_ANCHO                       80
#define MAPA_ALTO                        44


struct jugador_t;
//CAMBIAR POR UNSIGNED INT LOS x e y?
typedef struct str_posicion {
    unsigned char x;
    unsigned char y;
} __attribute__((__packed__)) posicion;

typedef struct pirata_t
{
    posicion posicion;
    uint index;
    //struct jugador_t *jugador;
    tss* tss;
    unsigned int centro;
    int vivo;
    unsigned char clock;
    unsigned char jugador;
    unsigned char indice; //ESTO PARA EL CLOCK
    unsigned char tipo; // 0 explorador 1 minero
    posicion dest;
    //unsigned char vivos[8];
    // id unica, posicion, tipo, reloj
} pirata_t;


typedef struct jugador_t
{
    //uint index;
    //pirata_t piratas[MAX_CANT_PIRATAS_VIVOS];
    //FALTARIA UN ARRAY DE BOOLS PARA LOS VIVOS?
    unsigned int puntaje;
    posicion puerto;
    unsigned char m_pendientes;
    //unsigned int visitadas[3520];
    unsigned int ult_indice_vis;        //ultimo indice visitado del arreglo de visitadas
    unsigned char color;
    unsigned char colorLetra;
    unsigned char vivos;
    // coordenadas puerto, posiciones exploradas, mineros pendientes, etc
} jugador_t;


unsigned int visitadasA[3520];
unsigned int visitadasB[3520];

typedef struct cola_lifo{
	uint arreglo[8][2];
	uint ult;
} colaLifo;

colaLifo pendientesA;
colaLifo pendientesB;

pirata_t piratasA[8];	// piratas del primer jugador
pirata_t piratasB[8];	// piratas del segundo jugador

unsigned int tiempo_sin_juego;

unsigned char llamadasminero[8][2];

jugador_t jugadores[2]; 

unsigned char debug;	//0 -> desactivado, 1-> en espera, 2-> mostrando

extern jugador_t jugadorA, jugadorB;

// ~ auxiliares dadas ~
uint game_xy2lineal();
pirata_t* id_pirata2pirata(uint id);

// ~ auxiliares sugeridas o requeridas (segun disponga enunciado) ~
void game_pirata_inicializar(pirata_t *pirata, jugador_t *jugador, uint index, uint id);
void game_pirata_erigir(pirata_t *pirata, jugador_t *j, uint tipo);
void game_pirata_habilitar_posicion(jugador_t *j, pirata_t *pirata, int x, int y);
void game_pirata_exploto(uint id);

//void game_jugador_inicializar(jugador_t *j);
void game_jugador_lanzar_pirata(unsigned char, unsigned char, unsigned int, unsigned int);
pirata_t* game_jugador_erigir_pirata(jugador_t *j, uint tipo);
void game_jugador_anotar_punto(jugador_t *j);
void game_explorar_posicion(jugador_t *jugador, int x, int y);
unsigned int dame_dir(unsigned int * visitada);
uint game_valor_tesoro(uint x, uint y);
void game_calcular_posiciones_vistas(int *vistas_x, int *vistas_y, int x, int y);
pirata_t* game_pirata_en_posicion(uint x, uint y);
unsigned char revisar_mapeadas_mineros(unsigned int virtualDst, unsigned int *visitadas, unsigned int tamanio);
unsigned char revisar_mapeadas_piratas(unsigned int virtualDst, unsigned int *visitadas, unsigned int tamanio);
uint game_syscall_pirata_posicion(int idx);
void game_syscall_pirata_mover(direccion key);
uint game_syscall_manejar(uint syscall, int param1);
void game_tick(uint id_pirata);
void game_terminar_si_es_hora();
void game_atender_teclado(unsigned char tecla);
void inic_game();
void game_ver_si_termina();
void game_syscall_cavar();        //por ahora es void
void game_matar_pirata();
void game_matar_pirata_interrupt();
void mapear_a_todos(unsigned int virtualDst);
unsigned int mi_codigo(unsigned char tipo);
void cambiar_tarea_ya(unsigned short idle);
#endif  /* !__GAME_H__ */
