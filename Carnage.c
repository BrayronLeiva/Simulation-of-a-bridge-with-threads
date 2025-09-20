#include "Carnage.h"

int largoPuente;

int velocidadOesteaEste;
int velocidadEsteaOeste;

pthread_mutex_t mutexes[LARGO_PUENTE];
pthread_mutex_t mutex; //mutex para manejar acceso de puente
pthread_mutex_t mutex2; //mutex para manejar acceso de puente
pthread_cond_t cond_cruzar; //condición para que los automóviles esperen a cruzar el puente
int carrosEnPuente = 0;
int carrosOaE = 0;
int carrosEaO = 0;

void cargaDatos(int longitudPuente, double mediaTiempoOE, double mediaTiempoEO, double velocidadPromedioOE, double velocidadPromedioEO,
    int K1, int K2, int duracionSemaforoOE, int duracionSemaforoEO, int rangoInferior, int rangoSuperior) {
    largoPuente = longitudPuente;
    velocidadOesteaEste = velocidadPromedioOE;
    velocidadEsteaOeste = velocidadPromedioEO;



}

void* comportamiento_automovil(void* arg) {
    //int thread_id = *((int*)arg);
    struct Automovil *automovil = arg;


    pthread_mutex_lock(&mutex);

    while (carrosEnPuente > 0 && (automovil->sentido == 'o' && carrosOaE == 0 && carrosEaO >= 1) ||
    carrosEnPuente > 0 && (automovil->sentido == 'e' && carrosEaO == 0 && carrosOaE >= 1)) {
        // Esperar a que el puente esté disponible o haya espacio en el sentido adecuado
        //printf("Carros en puente %d", carrosEnPuente);
        pthread_cond_wait(&cond_cruzar, &mutex);
    }

    carrosEnPuente++;
    if(automovil->sentido=='o'){carrosOaE++;} //viene del oeste va hacia el este
    if(automovil->sentido=='e'){carrosEaO++;} //viene del este va hacia el oeste
    pthread_mutex_unlock(&mutex);


    if(automovil->sentido=='o') {
        // Recorre el arreglo de mutexes
        for (int i = 0; i < LARGO_PUENTE; i++) {
            // Bloquea el mutex actual
            pthread_mutex_lock(&mutexes[i]);
            printf("Hilo %d pasando por el mutex %d\n", automovil->id, i, "\n");

            // Espera un momento simulando el paso por el carril
            unsigned int segundos = (unsigned int)(velocidadOesteaEste + 0.5);
            sleep(segundos);

            // Desbloquea el mutex anterior
            if (i > 0) {
                pthread_mutex_unlock(&mutexes[i - 1]);
            }
        }

        // Desbloquea el último mutex
        pthread_mutex_unlock(&mutexes[LARGO_PUENTE - 1]);
    }
    else {

        // Recorre el arreglo de mutexes desde el final hacia el inicio
        for (int i = LARGO_PUENTE - 1; i >= 0; i--) {
            // Bloquea el mutex actual
            pthread_mutex_lock(&mutexes[i]);
            printf("Hilo %d pasando por el mutex %d\n", automovil->id, i, "\n");
            // Espera un momento simulando el paso por el carril
            unsigned int segundos = (unsigned int)(velocidadEsteaOeste + 0.5);
            sleep(segundos);
            // Desbloquea el mutex siguiente
            if (i < LARGO_PUENTE - 1) {
                pthread_mutex_unlock(&mutexes[i + 1]);
            }
        }

        // Desbloquea el primer mutex
        pthread_mutex_unlock(&mutexes[0]);
    }




    printf("2: carros en puente: %d, ID del carro: %d\n \n", carrosEnPuente, automovil->id );
    printf("2: carros en puente de este a oeste: %d , ID del carro: %d\n\n", carrosEaO, automovil->id );
    printf("2: carros en puente de oeste a este: %d, ID del carro: %d\n\n", carrosOaE, automovil->id );

    pthread_mutex_lock(&mutex2);
    carrosEnPuente--;
    if(automovil->sentido=='o'){carrosOaE--;}
    if(automovil->sentido=='e'){carrosEaO--;}
    pthread_cond_signal(&cond_cruzar); // Señalar a otros automóviles que puedan cruzar
    pthread_mutex_unlock(&mutex2);

    return NULL;
}