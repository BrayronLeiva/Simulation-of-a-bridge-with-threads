#ifndef PROYECTO_OFICIAL_TRANSITO_H
#define PROYECTO_OFICIAL_TRANSITO_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>   // Si usas strtok
#include <math.h>

#define LARGO_PUENTE 5
#define NUM_VEHICULOS 10
#define CANTIDAD_OESTE_ESTE 3
#define CANTIDAD_ESTE_OESTE 2


extern int largoPuenteOficiales;

extern int velocidadOesteaEsteOficiales;
extern int velocidadEsteaOsteOficiales;

extern int k1;
extern int k2;


extern pthread_mutex_t mutexesOfi[LARGO_PUENTE];
extern pthread_mutex_t mutexOfi; //mutex para manejar acceso de puente
extern pthread_mutex_t mutex2Ofi; //mutex para manejar acceso de puente
extern pthread_mutex_t mutex3Ofi; //mutex para manejar acceso de puente
extern pthread_cond_t cond_cruzarOfi; //condición para que los automóviles esperen a cruzar el puente
extern pthread_cond_t cond_esperarOfi; //condición para que los automóviles esperen a cruzar el puente
extern pthread_cond_t cond_esperar2Ofi ; //condición para que los automóviles esperen a cruzar el puente
extern int carrosEnPuenteOfi;
extern int carrosOaEOfi;
extern int carrosEaOOfi;
extern char sentidoOfi; // '0' de este a oeste 'e' de oeste hacia este
extern int turnoOfi;
extern int pasados_oeste_esteOfi;
extern int pasados_este_oesteOfi;

extern int esperando_oeste_esteOfi;
extern int esperando_este_oesteOfi;

struct AutomovilOfi {
    int id;
    char sentido; // o 'hacia oeste', e hacia 'este'
};

struct Oficial {
    int estado; //1 verde 0 rojo
    char sentidoS;
    int id;

};

void* comportamiento_automovil_oficiales(void*);

void* comportamiento_oficial_Oeste(void* arg);

void* comportamiento_oficial_Este(void* arg);

void cargarDatosTrafico(int , double , double , double , double ,
    int , int , int , int , int , int );

#endif //PROYECTO_OFICIAL_TRANSITO_H





