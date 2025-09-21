#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>
#include "Carnage.h"
#include "Semaforo.h"
#include "Oficial_Transito.h"
#include <string.h>   // Para strtok
#include <time.h>     // Para time()
#include <unistd.h>

#define MAX_LINE_LENGTH 100 // Longitud máxima de una línea en el archivo

void recuperarDatos(int*, double*, double*, double* , double*,
    int* , int* , int* , int* , int* , int* );
void carnage();
void semaforo();
void oficialTransito();

int main() {
    int opc = 1;

    do{
        printf("Escoja una de las siguientes opciones:\n");
        printf("1. Modo Carnage.\n");
        printf("2. Semaforo.\n");
        printf("3. Oficiales de transito.\n");
        printf("4. Salir.\n");
        scanf("%d", &opc);

        switch(opc){
            case 1: carnage(); break;
            case 2: semaforo(); break;
            case 3: oficialTransito(); break;
            case 4: break;
            default: printf("Opcion no valida.\n \n"); break;
        }
    } while (opc!=4);
    return 0;
}

void recuperarDatos(int* longitudPuente, double* mediaTiempoOE, double* mediaTiempoEO, 
                    double* velocidadPromedioOE, double* velocidadPromedioEO,
                    int* K1, int* K2, int* duracionSemaforoOE, int* duracionSemaforoEO, 
                    int* rangoInferior, int* rangoSuperior) {

    FILE *file = fopen("Datos.txt", "r");
    if (!file) { perror("Error al abrir archivo"); exit(1); }

    char linea[100];

    // 1. Longitud del puente
    fgets(linea, sizeof(linea), file);
    *longitudPuente = atoi(linea);

    // 2. Media de tiempos
    fgets(linea, sizeof(linea), file);
    sscanf(linea, "%lf,%lf", mediaTiempoOE, mediaTiempoEO);

    // 3. Velocidades promedio
    fgets(linea, sizeof(linea), file);
    sscanf(linea, "%lf,%lf", velocidadPromedioOE, velocidadPromedioEO);

    // 4. K1
    fgets(linea, sizeof(linea), file);
    *K1 = atoi(linea);

    // 5. K2
    fgets(linea, sizeof(linea), file);
    *K2 = atoi(linea);

    // 6. Duración semáforos
    fgets(linea, sizeof(linea), file);
    sscanf(linea, "%d,%d", duracionSemaforoOE, duracionSemaforoEO);

    // 7. Rango inferior y superior
    fgets(linea, sizeof(linea), file);
    sscanf(linea, "%d,%d", rangoInferior, rangoSuperior);

    fclose(file);
}

void imprimir_informacion(int longitudPuente, double mediaTiempoOE, double mediaTiempoEO, 
                    double velocidadPromedioOE, double velocidadPromedioEO,
                    int K1, int K2, int duracionSemaforoOE, int duracionSemaforoEO, 
                    int rangoInferior, int rangoSuperior) {
    printf("Longitud del puente: %d\n", longitudPuente);
    printf("Media de tiempo entre llegadas de hilos (Oeste a Este): %.2f\n", mediaTiempoOE);
    printf("Media de tiempo entre llegadas de hilos (Este a Oeste): %.2f\n", mediaTiempoEO);
    printf("Velocidad promedio de los automoviles (Oeste a Este): %.2f\n", velocidadPromedioOE);
    printf("Velocidad promedio de los automoviles (Este a Oeste): %.2f\n", velocidadPromedioEO);
    printf("Valor para K1: %d\n", K1);
    printf("Valor para K2: %d\n", K2);
    printf("Tiempo de duracion de la luz verde de un semaforo (Oeste a Este): %d\n", duracionSemaforoOE);
    printf("Tiempo de duracion de la luz verde de un semaforo (Este a Oeste): %d\n", duracionSemaforoEO);
    printf("Rango inferior de velocidades de un grupo de carros: %d\n", rangoInferior);
    printf("Rango superior de velocidades de un grupo de carros: %d\n", rangoSuperior);
}


void carnage(){

    //recuperarDatos(&cantCarros);
    int longitudPuente, K1, K2, duracionSemaforoOE, duracionSemaforoEO, rangoInferior, rangoSuperior;
    double mediaTiempoOE, mediaTiempoEO, velocidadPromedioOE, velocidadPromedioEO;

    recuperarDatos(&longitudPuente, &mediaTiempoOE, &mediaTiempoEO, &velocidadPromedioOE, &velocidadPromedioEO, &K1, &K2, &duracionSemaforoOE, &duracionSemaforoEO, &rangoInferior, &rangoSuperior);

    imprimir_informacion(longitudPuente, mediaTiempoOE, mediaTiempoEO, velocidadPromedioOE, velocidadPromedioEO, K1, K2, duracionSemaforoOE, duracionSemaforoEO, rangoInferior, rangoSuperior);

    cargaDatos(longitudPuente, mediaTiempoOE, mediaTiempoEO, velocidadPromedioOE, velocidadPromedioEO,
    K1, K2, duracionSemaforoOE, duracionSemaforoEO, rangoInferior, rangoSuperior);

    // Inicializa los mutexes
    for (int i = 0; i < LARGO_PUENTE; i++) {
        pthread_mutex_init(&mutexes[i], NULL);
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex2, NULL);


    srand(time(NULL));

    // Bucle infinito para la creación de hilos
    int i = 0;
    while (1) {
        // Calcular el tiempo de espera entre la creación de hilos
        double tiempoEspera = -log(1.0 - ((double)rand() / RAND_MAX)) * mediaTiempoOE;

        // Esperar el tiempo calculado
        unsigned int segundos = (unsigned int)(tiempoEspera + 0.5); 
        sleep(segundos);

        // Crear un nuevo hilo
        pthread_t hilo;
        struct Automovil *automovil = malloc(sizeof(struct Automovil));
        automovil->id = i + 1;

        printf("\nSe creo el hilo %d\n", automovil->id);

        //Asignacion aleatoria de sentido
        if(rand() % 2 == 0) {
            automovil->sentido = 'e';
        }else {
            automovil->sentido = 'o';
        }

        if (pthread_create(&hilo, NULL, comportamiento_automovil, automovil) != 0) {
            perror("Error al crear el hilo");
            exit(EXIT_FAILURE);
        }
        
        //El hilo se libera automaticamente al terminar
        pthread_detach(hilo);


        i++;
    }


    // Destruye los mutexes
    for (int i = 0; i < LARGO_PUENTE; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }
}

void semaforo(){
    printf("\nSE EJECUTO SEMAFORO\n");

    int longitudPuente, K1, K2, duracionSemaforoOE, duracionSemaforoEO, rangoInferior, rangoSuperior;
    double mediaTiempoOE, mediaTiempoEO, velocidadPromedioOE, velocidadPromedioEO;

    recuperarDatos(&longitudPuente, &mediaTiempoOE, &mediaTiempoEO, &velocidadPromedioOE, &velocidadPromedioEO,
        &K1, &K2, &duracionSemaforoOE, &duracionSemaforoEO, &rangoInferior, &rangoSuperior);
    
    imprimir_informacion(longitudPuente, mediaTiempoOE, mediaTiempoEO, velocidadPromedioOE, velocidadPromedioEO, K1, K2, duracionSemaforoOE, duracionSemaforoEO, rangoInferior, rangoSuperior);


    cargarDatosSemaforo(longitudPuente, mediaTiempoOE, mediaTiempoEO, velocidadPromedioOE, velocidadPromedioEO,
    K1, K2, duracionSemaforoOE, duracionSemaforoEO, rangoInferior, rangoSuperior);
    // Inicializa los mutexes
    for (int i = 0; i < LARGO_PUENTE; i++) {
        pthread_mutex_init(&mutexesSemaforo[i], NULL);
    }

    pthread_mutex_init(&mutexSemaforo, NULL);
    pthread_mutex_init(&mutex2Semaforo, NULL);
    pthread_mutex_init(&mutex3Semaforo, NULL);

    pthread_t semaforoOeste;
    pthread_t semaforoEste;

    struct Semaforo *semaforoE = malloc(sizeof(semaforoE));
    semaforoE->sentidoS = 'e';


    struct Semaforo *semaforoO = malloc(sizeof(semaforoO));
    semaforoO->sentidoS = 'o';

    pthread_create(&semaforoOeste, NULL, comportamiento_semaforo, semaforoO);
    pthread_create(&semaforoEste, NULL, comportamiento_semaforo, semaforoE);

    srand(time(NULL));

    // Bucle infinito para la creación de hilos
    int e = 0;
    while (1) {
        // Calcular el tiempo de espera entre la creación de hilos
        double tiempoEspera = -log(1.0 - ((double)rand() / RAND_MAX)) * mediaTiempoOE;

        // Esperar el tiempo calculado
        unsigned int segundos = (unsigned int)(tiempoEspera + 0.5);
        sleep(segundos);

        // Crear un nuevo hilo
        pthread_t hilo;
        struct AutomovilSemaforo *automovil = malloc(sizeof(struct AutomovilSemaforo));
        automovil->id = e + 1;

        if(rand() % 2 == 0) {
            automovil->sentido = 'e';
        }else {
            automovil->sentido = 'o';
        }
        if (pthread_create(&hilo, NULL, comportamiento_automovil_semaforo, automovil) != 0) {
            perror("Error al crear el hilo");
            exit(EXIT_FAILURE);
        }

        pthread_detach(hilo);
        

        e++;
    }


    pthread_join(semaforoOeste, NULL);
    pthread_join(semaforoEste, NULL);

    free(semaforoE);
    free(semaforoO);

    // Destruye los mutexes
    for (int i = 0; i < LARGO_PUENTE; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }
}
void oficialTransito(){

    printf("\nSE EJECUTO OFICIAL DE TRANSITO\n");

    int longitudPuente, K1, K2, duracionSemaforoOE, duracionSemaforoEO, rangoInferior, rangoSuperior;
    double mediaTiempoOE, mediaTiempoEO, velocidadPromedioOE, velocidadPromedioEO;

    recuperarDatos(&longitudPuente, &mediaTiempoOE, &mediaTiempoEO, &velocidadPromedioOE, &velocidadPromedioEO, &K1, &K2,
        &duracionSemaforoOE, &duracionSemaforoEO, &rangoInferior, &rangoSuperior);

    
    imprimir_informacion(longitudPuente, mediaTiempoOE, mediaTiempoEO, velocidadPromedioOE, velocidadPromedioEO, K1, K2, duracionSemaforoOE, duracionSemaforoEO, rangoInferior, rangoSuperior);

    cargarDatosTrafico(longitudPuente, mediaTiempoOE, mediaTiempoEO, velocidadPromedioOE, velocidadPromedioEO,
    K1, K2, duracionSemaforoOE, duracionSemaforoEO, rangoInferior, rangoSuperior);

    // Inicializa los mutexes
    for (int i = 0; i < LARGO_PUENTE; i++) {
        pthread_mutex_init(&mutexesOfi[i], NULL);
    }

    pthread_mutex_init(&mutexOfi, NULL);
    pthread_mutex_init(&mutex2Ofi, NULL);
    pthread_mutex_init(&mutex3Ofi, NULL);

    pthread_t oficialOeste;
    pthread_t oficialEste;

    struct Oficial *oficialE = malloc(sizeof(oficialE));
    oficialE->sentidoS = 'e';
    oficialE->id = 1;

    struct Oficial *oficialO = malloc(sizeof(oficialO));
    oficialO->id = 0;
    oficialO->sentidoS = 'o';

    pthread_create(&oficialOeste, NULL, comportamiento_oficial_Oeste, oficialO);
    pthread_create(&oficialEste, NULL, comportamiento_oficial_Este, oficialE);



    int tipo;
    int j = 0;
    while (1) {
        
        // Calcular el tiempo de espera entre la creación de hilos
        srand(time(NULL));
        double tiempoEspera = -log(1.0 - ((double)rand() / RAND_MAX)) * mediaTiempoOE;

        // Esperar el tiempo calculado
        unsigned int segundos = (unsigned int)(tiempoEspera + 0.5);
        sleep(segundos);

        // Crear un nuevo hilo
        pthread_t hilo;
        struct AutomovilOfi *automovil = malloc(sizeof(struct AutomovilOfi));
        automovil->id = j + 1;

        srand(time(NULL));
        
        tipo = rand() % 2;
        if(tipo==1) {
            automovil->sentido='o';
        }else {
            automovil->sentido='e';
        }

        if (pthread_create(&hilo, NULL, comportamiento_automovil_oficiales, automovil) != 0) {
            perror("Error al crear el hilo");
            exit(EXIT_FAILURE);
        }

        pthread_detach(hilo);
        

        j++;
    }



    pthread_join(oficialOeste, NULL);
    pthread_join(oficialEste, NULL);
    
    free(oficialE);
    free(oficialO);

    // Destruye los mutexes
    for (int i = 0; i < LARGO_PUENTE; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }

}