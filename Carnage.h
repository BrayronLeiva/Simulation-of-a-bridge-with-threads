#ifndef PROYECTO_CARNAGE_H
#define PROYECTO_CARNAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define LARGO_PUENTE 5

extern int largoPuente;

extern int velocidadOesteaEste;
extern int velocidadEsteaOeste;

extern pthread_mutex_t mutexes[LARGO_PUENTE];
extern pthread_mutex_t mutex; //mutex para manejar acceso de puente
extern pthread_mutex_t mutex2; //mutex para manejar acceso de puente
extern pthread_cond_t cond_cruzar; //condición para que los automóviles esperen a cruzar el puente
extern int carrosEnPuente;
extern int carrosOaE;
extern int carrosEaO;

struct Automovil {
    int id;
    char sentido; // o 'hacia oeste', e hacia 'este'
};


void* comportamiento_automovil(void*);

void cargaDatos(int , double , double , double , double ,
    int , int , int , int , int , int );

#endif //PROYECTO_CARNAGE_H
