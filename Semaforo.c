#include "Semaforo.h"

#include "Carnage.h"


int duracionOesteEste;
int duracionEsteOeste;
int largoPuenteSemaforo;

int velocidadOesteaEsteSemaforo;
int velocidadEsteaOsteSemaforo;

// Definición de las variables
pthread_mutex_t mutexesSemaforo[LARGO_PUENTE];
pthread_mutex_t mutexSemaforo; //mutex para manejar acceso de puente
pthread_mutex_t mutex2Semaforo; //mutex para manejar acceso de puente
pthread_mutex_t mutex3Semaforo; //mutex para manejar acceso de puente
pthread_cond_t cond_cruzarSemaforo = PTHREAD_COND_INITIALIZER; //condición para que los automóviles esperen a cruzar el puente
pthread_cond_t cond_esperarSemaforo = PTHREAD_COND_INITIALIZER; //condición para que los automóviles esperen a cruzar el puente
//pthread_cond_t cond_cruzar = PTHREAD_COND_INITIALIZER; //condición para que los automóviles esperen a cruzar el puente
int carrosEnPuenteSemaforo = 0;
int carrosOaESemaforo = 0;
int carrosEaOSemaforo = 0;

char sentidoSemaforo = 'o'; // '0' de este a oeste 'e' de oeste hacia este


void cargarDatosSemaforo(int longitudPuente, double mediaTiempoOE, double mediaTiempoEO, double velocidadPromedioOE, double velocidadPromedioEO,
    int K1, int K2, int duracionSemaforoOE, int duracionSemaforoEO, int rangoInferior, int rangoSuperior) {
    largoPuenteSemaforo = longitudPuente;
    velocidadOesteaEsteSemaforo = velocidadPromedioOE;
    velocidadEsteaOsteSemaforo = velocidadPromedioEO;
    duracionOesteEste = duracionSemaforoOE;
    duracionEsteOeste = duracionSemaforoEO;

    printf("la velocidad promedio de oeste a este es: %d\n", velocidadOesteaEsteSemaforo);
    printf("la velocidad promedio de oeste a este es: %d\n", velocidadEsteaOsteSemaforo);
    printf("la duracion del semaforo de oeste a este es%d\n:", duracionSemaforoOE);
    printf("la duracion del semaforo de este a oeste es%d\n:", duracionSemaforoEO);


}


void* comportamiento_semaforo(void* arg) {
    struct Semaforo *semafor = arg;

    while (1) {

        pthread_mutex_lock(&mutex3Semaforo);
        if (semafor->sentidoS=='e') {
            unsigned int segundos = (unsigned int)(duracionEsteOeste + 0.5);
            sleep(segundos);
        }else {
            unsigned int segundos = (unsigned int)(duracionOesteEste + 0.5);
            sleep(segundos);
        }
        sentidoSemaforo = (sentidoSemaforo == 'o') ? 'e' : 'o'; // Cambiar el sentido del tráfico
        printf("\n =========================CAMBIO DE SENTIDO DE TRAFICO =========================Nuevo sentido: %c\n", sentidoSemaforo);

        pthread_cond_broadcast(&cond_cruzarSemaforo); // Permitir que todos los automóviles esperando cambien de sentido

        pthread_mutex_unlock(&mutex3Semaforo);
    }

}


void* comportamiento_automovil_semaforo(void* arg) {
    //int thread_id = *((int*)arg);
    struct AutomovilSemaforo *automovil = arg;


    pthread_mutex_lock(&mutexSemaforo);

    //|| automovil->estado=='o'&& carrosOaE>1 && carrosEaO==0 || automovil->estado =='e'&& carrosEaO>1 && carrosOaE==0
    while ( (automovil->sentido == 'o' && (sentidoSemaforo=='e' || carrosEaOSemaforo>=1) ) || (automovil->sentido == 'e' && (sentidoSemaforo=='o' || carrosOaESemaforo>=1) )) {
        // Esperar a que el puente esté disponible o haya espacio en el sentido adecuado
        //printf("Carros en puente %d", carrosEnPuente);
        pthread_cond_wait(&cond_cruzarSemaforo, &mutexSemaforo);
    }

    carrosEnPuenteSemaforo++;
    if(automovil->sentido=='o'){carrosOaESemaforo++;} //viene del oeste va hacia el este
    if(automovil->sentido=='e'){carrosEaOSemaforo++;} //viene del este va hacia el oeste
    pthread_mutex_unlock(&mutexSemaforo);


    if(automovil->sentido=='o') {
        // Recorre el arreglo de mutexes
        for (int i = 0; i < LARGO_PUENTE; i++) {
            // Bloquea el mutex actual
            pthread_mutex_lock(&mutexesSemaforo[i]);
            printf("Hilo %d pasando por el mutex %d\n", automovil->id, i, "\n");

            // Espera un momento simulando el paso por el carril;
            unsigned int segundos = (unsigned int)(velocidadOesteaEsteSemaforo + 0.5);
            sleep(segundos);

            // Desbloquea el mutex anterior
            if (i > 0) {
                pthread_mutex_unlock(&mutexesSemaforo[i - 1]);
            }
        }

        // Desbloquea el último mutex
        pthread_mutex_unlock(&mutexesSemaforo[LARGO_PUENTE - 1]);
    }
    else {

        // Recorre el arreglo de mutexes desde el final hacia el inicio
        for (int i = LARGO_PUENTE - 1; i >= 0; i--) {
            // Bloquea el mutex actual
            pthread_mutex_lock(&mutexesSemaforo[i]);
            printf("Hilo %d pasando por el mutex %d\n", automovil->id, i, "\n");
            // Espera un momento simulando el paso por el carril
            unsigned int segundos = (unsigned int)(velocidadEsteaOsteSemaforo + 0.5);
            sleep(segundos);
            // Desbloquea el mutex siguiente
            if (i < LARGO_PUENTE - 1) {
                pthread_mutex_unlock(&mutexesSemaforo[i + 1]);
            }
        }

        // Desbloquea el primer mutex
        pthread_mutex_unlock(&mutexesSemaforo[0]);
    }




    printf("2: carros en puente: %d, ID del carro: %d\n \n", carrosEnPuenteSemaforo, automovil->id );
    printf("2: carros en puente de este a oeste: %d , ID del carro: %d\n\n", carrosEaOSemaforo, automovil->id );
    printf("2: carros en puente de oeste a este: %d, ID del carro: %d\n\n", carrosOaESemaforo, automovil->id );

    pthread_mutex_lock(&mutex2Semaforo);
    carrosEnPuenteSemaforo--;
    if(automovil->sentido=='o'){carrosOaESemaforo--;}
    if(automovil->sentido=='e'){carrosEaOSemaforo--;}
    //pthread_cond_signal(&cond_cruzar); // Señalar a otros automóviles que puedan cruzar
    pthread_mutex_unlock(&mutex2Semaforo);

    return NULL;
}