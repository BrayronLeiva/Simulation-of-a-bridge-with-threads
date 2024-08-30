#include "Oficial_Transito.h"

#include "Carnage.h"


int largoPuenteOficiales;

int velocidadOesteaEsteOficiales;
int velocidadEsteaOsteOficiales;


int k1;
int k2;

pthread_mutex_t mutexesOfi[LARGO_PUENTE];
pthread_mutex_t mutexOfi; //mutex para manejar acceso de puente
pthread_mutex_t mutex2Ofi; //mutex para manejar acceso de puente
pthread_mutex_t mutex3Ofi; //mutex para manejar acceso de puente
pthread_cond_t cond_cruzarOfi = PTHREAD_COND_INITIALIZER; //condición para que los automóviles esperen a cruzar el puente
pthread_cond_t cond_esperarOfi = PTHREAD_COND_INITIALIZER; //condición para que los automóviles esperen a cruzar el puente
pthread_cond_t cond_esperar2Ofi = PTHREAD_COND_INITIALIZER; //condición para que los automóviles esperen a cruzar el puente
int carrosEnPuenteOfi = 0;
int carrosOaEOfi = 0;
int carrosEaOOfi = 0;

char sentidoOfi = 'o'; // '0' de este a oeste 'e' de oeste hacia este
int turnoOfi = 0;
int pasados_oeste_este = 0;
int pasados_este_oeste = 0;

int pasados_oeste_esteOfi = 0;
int pasados_este_oesteOfi = 0;

int esperando_oeste_esteOfi = 0;
int esperando_este_oesteOfi = 0;

//----------------------------------

void cargarDatosTrafico(int longitudPuente, double mediaTiempoOE, double mediaTiempoEO, double velocidadPromedioOE, double velocidadPromedioEO,
    int K1p, int K2p, int duracionSemaforoOE, int duracionSemaforoEO, int rangoInferior, int rangoSuperior) {
    largoPuenteOficiales = longitudPuente;
    velocidadOesteaEsteOficiales = velocidadPromedioOE;
    velocidadEsteaOsteOficiales = velocidadPromedioEO;
    k1 = K1p;
    k2 = K2p;



    //printf(k1, k2);
}

void* comportamiento_oficial_Oeste(void* arg) {
    struct Oficial *oficial = arg;

    while (1) {

        pthread_mutex_lock(&mutexOfi);
        pthread_cond_wait(&cond_esperarOfi, &mutexOfi);
        pasados_oeste_esteOfi++;

        if(pasados_oeste_esteOfi>=k1 || esperando_oeste_esteOfi==0 && esperando_este_oesteOfi>0){
            printf("\n=========================CAMBIO DE TURNO =========================\n");
            pasados_oeste_esteOfi = 0;
            sentidoOfi = 'e';
        }

        pthread_cond_broadcast(&cond_cruzarOfi); // Permitir que todos los automóviles esperando cambien de sentido

        pthread_mutex_unlock(&mutexOfi);
    }

}

void* comportamiento_oficial_Este(void* arg) {
    struct Oficial *oficial = arg;

    while (1) {

        pthread_mutex_lock(&mutexOfi);
        pthread_cond_wait(&cond_esperar2Ofi, &mutexOfi);
        pasados_este_oesteOfi++;

        if(pasados_este_oesteOfi>=k2 || esperando_este_oesteOfi==0 && esperando_oeste_esteOfi>0){
            printf("\n =========================CAMBIO DE TURNO =========================\n");
            pasados_este_oesteOfi = 0;
            sentidoOfi = 'o';
        }
        //si ya pasaron los carros pero hay en el sentido contrario aun el puente
        //duermase y hasta que no hayan carros despierte para hacer el cambio
        //considerar que el otro codigo la condicion para ese signal tiene que considerar
        //pasados_este_oeste>=CANTIDAD_ESTE_OESTE por que hay momentos que tal vez hayan 0 pero no hayan pasado el maximo
        //esto de arriba no sirvio pero voy a mandar 2 ideas al cel de mio yo de whatsapp

        //pero pensandolo, ese signal solo afectaria si me dormi por eso o no?

        pthread_cond_broadcast(&cond_cruzarOfi); // Permitir que todos los automóviles esperando cambien de sentido

        pthread_mutex_unlock(&mutexOfi);
    }

}


void* comportamiento_automovil_oficiales(void* arg) {
    //int thread_id = ((int)arg);
    struct AutomovilOfi *automovil = arg;


    pthread_mutex_lock(&mutexOfi);

    //|| automovil->estado=='o'&& carrosOaE>1 && carrosEaO==0 || automovil->estado =='e'&& carrosEaO>1 && carrosOaE==0
    while ( (automovil->sentido == 'o' && (sentidoOfi=='e' || carrosEaOOfi>=1) ) || (automovil->sentido == 'e' && (sentidoOfi=='o' || carrosOaEOfi>=1) )) {
        // Esperar a que el puente esté disponible o haya espacio en el sentido adecuado
        //printf("Carros en puente %d", carrosEnPuente);
        if(automovil->sentido=='o'){esperando_oeste_esteOfi++;} //viene del oeste va hacia el este
        if(automovil->sentido=='e'){esperando_este_oesteOfi++;} //viene del este va hacia el oeste
        pthread_cond_wait(&cond_cruzarOfi, &mutexOfi);
    }

    carrosEnPuenteOfi++;
    if(automovil->sentido=='o') {
        carrosOaEOfi++;
        esperando_oeste_esteOfi--;
    } //viene del oeste va hacia el este
    if(automovil->sentido=='e') {
        carrosEaOOfi++;
        esperando_este_oesteOfi--;
    } //viene del este va hacia el oeste

    if(automovil->sentido=='o') {
        pthread_cond_signal(&cond_esperarOfi);
    }
    if(automovil->sentido=='e') {
        pthread_cond_signal(&cond_esperar2Ofi);
    }

    pthread_mutex_unlock(&mutexOfi);


    if(automovil->sentido=='o') {
        // Recorre el arreglo de mutexes
        for (int i = 0; i < LARGO_PUENTE; i++) {
            // Bloquea el mutex actual
            pthread_mutex_lock(&mutexesOfi[i]);
            printf("Hilo %d pasando por el mutex %d\n", automovil->id, i, "\n");

            // Espera un momento simulando el paso por el carril
            usleep(velocidadOesteaEsteOficiales);
            // Desbloquea el mutex anterior
            if (i > 0) {
                pthread_mutex_unlock(&mutexesOfi[i - 1]);
            }
        }

        // Desbloquea el último mutex
        pthread_mutex_unlock(&mutexesOfi[LARGO_PUENTE - 1]);
    }
    else {

        // Recorre el arreglo de mutexes desde el final hacia el inicio
        for (int i = LARGO_PUENTE - 1; i >= 0; i--) {
            // Bloquea el mutex actual
            pthread_mutex_lock(&mutexesOfi[i]);
            printf("Hilo %d pasando por el mutex %d\n", automovil->id, i, "\n");
            // Espera un momento simulando el paso por el carril
            usleep(velocidadEsteaOsteOficiales);
            // Desbloquea el mutex siguiente
            if (i < LARGO_PUENTE - 1) {
                pthread_mutex_unlock(&mutexesOfi[i + 1]);
            }
        }

        // Desbloquea el primer mutex
        pthread_mutex_unlock(&mutexesOfi[0]);
    }




    printf("2: carros en puente: %d, ID del carro: %d\n \n", carrosEnPuenteOfi, automovil->id );
    printf("2: carros en puente de este a oeste: %d , ID del carro: %d\n\n", carrosEaOOfi, automovil->id );
    printf("2: carros en puente de oeste a este: %d, ID del carro: %d\n\n", carrosOaEOfi, automovil->id );

    pthread_mutex_lock(&mutex2Ofi);//el problema sera usar otro mutex
    carrosEnPuenteOfi--;
    if(automovil->sentido=='o'){carrosOaEOfi--;}
    if(automovil->sentido=='e'){carrosEaOOfi--;}
    pthread_cond_broadcast(&cond_cruzarOfi); // Permitir que todos los automóviles esperando cambien de sentido

    //pthread_cond_signal(&cond_cruzar); // Señalar a otros automóviles que puedan cruzar
    pthread_mutex_unlock(&mutex2Ofi);

    return NULL;
}