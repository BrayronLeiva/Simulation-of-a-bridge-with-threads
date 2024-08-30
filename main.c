#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include "Carnage.h"
#include "Semaforo.h"
#include "Oficial_Transito.h"



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

void recuperarDatos(int* longitudPuente, double* mediaTiempoOE, double* mediaTiempoEO, double* velocidadPromedioOE, double* velocidadPromedioEO,
    int* K1, int* K2, int* duracionSemaforoOE, int* duracionSemaforoEO, int* rangoInferior, int* rangoSuperior) {
    FILE *file;
    char linea[MAX_LINE_LENGTH];
    char *token;

    file = fopen("../Prueba.txt", "r");

    if (file == NULL) { // Si el archivo posee errores no leera el archivo
        perror("Error al abrir el archivo");
        return;
    }

    int parametro_actual = 0; // variable para controlar qué parámetro se está leyendo

    // Lee el archivo línea por línea
    while (fgets(linea, MAX_LINE_LENGTH, file) != NULL && parametro_actual < 10) {
        // Dividir la línea en tokens usando el delimitador ","
        token = strtok(linea, ",");
        while (token != NULL) {
            // Convertir el token actual a su tipo correspondiente y almacenarlo en la variable adecuada
            switch (parametro_actual) {
                case 0:
                    *longitudPuente = atoi(token);
                    break;
                case 1:
                    *mediaTiempoOE = atof(token);
                    break;
                case 2:
                    *mediaTiempoEO = atof(token);
                    break;
                case 3:
                    *velocidadPromedioOE = atof(token);
                    break;
                case 4:
                    *velocidadPromedioEO = atof(token);
                    break;
                case 5:
                    *K1 = atoi(token);
                    break;
                case 6:
                    *K2 = atoi(token);
                    break;
                case 7:
                    *duracionSemaforoOE = atoi(token);
                    break;
                case 8:
                    *duracionSemaforoEO = atoi(token);
                    break;
                case 9:
                    *rangoInferior = atoi(token);
                    break;
                case 10:
                    *rangoSuperior = atoi(token);
                    break;
                default:
                    break;
            }
            // Obtener el siguiente token
            token = strtok(NULL, ",");
            parametro_actual++;
        }
    }

    // Cierra el archivo
    fclose(file);
}

/*void recuperarDatos(int* cantCarros){
    FILE *file;
    char linea[MAX_LINE_LENGTH];
    file = fopen("../Prueba.txt", "r");

    if (file == NULL) { // Si el archivo posee errores no leera el archivo
        perror("Error al abrir el archivo");
        return;
    }

    // Lee el archivo línea por línea
    while (fgets(linea, MAX_LINE_LENGTH, file) != NULL) {
        // Convertir la línea leida a un numero entero
        *cantCarros = atoi(linea);
    }

    // Cierra el archivo
    fclose(file);
}*/
void carnage(){

    //recuperarDatos(&cantCarros);
    int longitudPuente, K1, K2, duracionSemaforoOE, duracionSemaforoEO, rangoInferior, rangoSuperior;
    double mediaTiempoOE, mediaTiempoEO, velocidadPromedioOE, velocidadPromedioEO;

    recuperarDatos(&longitudPuente, &mediaTiempoOE, &mediaTiempoEO, &velocidadPromedioOE, &velocidadPromedioEO, &K1, &K2, &duracionSemaforoOE, &duracionSemaforoEO, &rangoInferior, &rangoSuperior);

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

    cargaDatos(longitudPuente, mediaTiempoOE, mediaTiempoEO, velocidadPromedioOE, velocidadPromedioEO,
    K1, K2, duracionSemaforoOE, duracionSemaforoEO, rangoInferior, rangoSuperior);

    // Inicializa los mutexes
    for (int i = 0; i < LARGO_PUENTE; i++) {
        pthread_mutex_init(&mutexes[i], NULL);
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex2, NULL);

    // Crea los hilos
    //for (int i = 0; i < cantCarros; i++) {
        //thread_ids[i] = i + 1;
        //struct Automovil *automovil = malloc(sizeof(automovil));
        //automovil->id = i + 1;
        //if(rand() % 2 == 0) {
            //automovil->sentido = 'e';
        //}else {
            //automovil->sentido = 'o';
        //}
        //pthread_create(&threads[i], NULL, comportamiento_automovil, automovil);
    //}

    srand(time(NULL));

    // Bucle infinito para la creación de hilos
    int i = 0;
    while (1) {
        // Calcular el tiempo de espera entre la creación de hilos
        double tiempoEspera = -log(1.0 - ((double)rand() / RAND_MAX)) * mediaTiempoOE;

        // Esperar el tiempo calculado
        usleep((useconds_t)tiempoEspera * 1000000);

        // Crear un nuevo hilo
        pthread_t hilo;
        struct Automovil *automovil = malloc(sizeof(automovil));
        automovil->id = i + 1;
        if(rand() % 2 == 0) {
        automovil->sentido = 'e';
        }else {
            automovil->sentido = 'o';
        }
        if (pthread_create(&hilo, NULL, comportamiento_automovil, automovil) != 0) {
            perror("Error al crear el hilo");
            exit(EXIT_FAILURE);
        }
        i++;
    }


    // Destruye los mutexes
    for (int i = 0; i < LARGO_PUENTE; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }
}
void semaforo(){
    printf("\nSE EJECUTO OFICIAL DE TRANSITO\n");

    int longitudPuente, K1, K2, duracionSemaforoOE, duracionSemaforoEO, rangoInferior, rangoSuperior;
    double mediaTiempoOE, mediaTiempoEO, velocidadPromedioOE, velocidadPromedioEO;

    recuperarDatos(&longitudPuente, &mediaTiempoOE, &mediaTiempoEO, &velocidadPromedioOE, &velocidadPromedioEO,
        &K1, &K2, &duracionSemaforoOE, &duracionSemaforoEO, &rangoInferior, &rangoSuperior);

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
    //semaforoE->sentidoS = 'e';

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
        usleep((useconds_t)tiempoEspera * 1000000);

        // Crear un nuevo hilo
        pthread_t hilo;
        struct AutomovilSemaforo *automovil = malloc(sizeof(automovil));
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
        e++;
    }


    pthread_join(semaforoOeste, NULL);
    pthread_join(semaforoEste, NULL);
    // Destruye los mutexes
    for (int i = 0; i < LARGO_PUENTE; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }
}
void oficialTransito(){
    //pthread_t threads[NUM_VEHICULOS];
    //int thread_ids[NUM_VEHICULOS];

    printf("\nSE EJECUTO OFICIAL DE TRANSITO\n");

    int longitudPuente, K1, K2, duracionSemaforoOE, duracionSemaforoEO, rangoInferior, rangoSuperior;
    double mediaTiempoOE, mediaTiempoEO, velocidadPromedioOE, velocidadPromedioEO;

    recuperarDatos(&longitudPuente, &mediaTiempoOE, &mediaTiempoEO, &velocidadPromedioOE, &velocidadPromedioEO, &K1, &K2,
        &duracionSemaforoOE, &duracionSemaforoEO, &rangoInferior, &rangoSuperior);

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
    //semaforoE->sentidoS = 'e';

    struct Oficial *oficialO = malloc(sizeof(oficialO));
    oficialO->id = 0;
    oficialO->sentidoS = 'o';

    pthread_create(&oficialOeste, NULL, comportamiento_oficial_Oeste, oficialO);
    pthread_create(&oficialEste, NULL, comportamiento_oficial_Este, oficialE);



    int tipo;
    int j = 0;
    while (1) {
        // Calcular el tiempo de espera entre la creación de hilos
        // Semilla para la generación de números aleatorios
        srand(time(NULL));
        double tiempoEspera = -log(1.0 - ((double)rand() / RAND_MAX)) * mediaTiempoOE;

        // Esperar el tiempo calculado
        usleep((useconds_t)tiempoEspera * 1000000);

        // Crear un nuevo hilo
        pthread_t hilo;
        struct AutomovilOfi *automovil = malloc(sizeof(automovil));
        automovil->id = j + 1;

        srand(time(NULL));
        // Generar un número aleatorio entre 0 y 1 y asignarlo a 'tipo'
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

        j++;
    }



    pthread_join(oficialOeste, NULL);
    pthread_join(oficialEste, NULL);

    // Destruye los mutexes
    for (int i = 0; i < LARGO_PUENTE; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }

}