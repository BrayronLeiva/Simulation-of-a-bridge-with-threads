#ifndef PROYECTO_SEMAFORO_H
#define PROYECTO_SEMAFORO_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <unistd.h>   // Para usleep
#include <string.h>   // Si usas strtok
#include <math.h>
#include <stdbool.h>


#define LARGO_PUENTE 5

extern int duracionOesteEste;
extern int duracionEsteOeste;

extern int largoPuenteSemaforo;

extern int velocidadOesteaEsteSemaforo;
extern int velocidadEsteaOsteSemaforo;

extern pthread_mutex_t mutexesSemaforo[LARGO_PUENTE];
extern pthread_mutex_t mutexSemaforo; //mutex para manejar acceso de puente
extern pthread_mutex_t mutex2Semaforo; //mutex para manejar acceso de puente
extern pthread_mutex_t mutex3Semaforo; //mutex para manejar acceso de puente
extern pthread_cond_t cond_cruzarSemaforo; //condición para que los automóviles esperen a cruzar el puente
extern pthread_cond_t cond_esperarSemaforo; //condición para que los automóviles esperen a cruzar el puente
//pthread_cond_t cond_cruzar = PTHREAD_COND_INITIALIZER; //condición para que los automóviles esperen a cruzar el puente
extern int carrosEnPuenteSemaforo;
extern int carrosOaESemaforo;
extern int carrosEaOSemaforo;

extern char sentidoSemaforo; // '0' de este a oeste 'e' de oeste hacia este

struct AutomovilSemaforo {
    int id;
    char sentido; // o 'hacia oeste', e hacia 'este'
};

struct Semaforo {
    int estado; //1 verde 0 rojo
    char sentidoS;

};

void* comportamiento_semaforo(void* arg);
void* comportamiento_automovil_semaforo(void* arg);

void cargarDatosSemaforo(int , double , double , double , double ,
    int , int , int , int , int , int );



#endif //PROYECTO_SEMAFORO_H



