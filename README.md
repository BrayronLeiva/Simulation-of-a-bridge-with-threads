# SimulationOfABridgeWithThreads
# Simulación de Tráfico Vehicular en un Puente

Este proyecto implementa una simulación de tráfico vehicular en un puente, explorando diferentes mecanismos de control de flujo. La base de la simulación es la representación del puente como un arreglo de mutexes, donde cada mutex gestiona el acceso a un segmento específico del puente. Se emplean mecanismos de sincronización como `pthread_cond_wait` y `pthread_cond_broadcast` para gestionar la concurrencia entre los vehículos.

## Funcionamiento del Puente

El puente está segmentado y cada segmento es protegido por un mutex individual. La lógica de acceso para los vehículos se define de la siguiente manera:

* **Vehículos en sentido contrario:** Si un vehículo intenta ocupar un segmento que ya está ocupado por un vehículo que se mueve en sentido opuesto, el hilo del vehículo que espera se suspende utilizando `pthread_cond_wait`. Este hilo permanecerá en espera hasta que el segmento sea liberado y se reciba una notificación general mediante `pthread_cond_broadcast`, indicando que el camino podría estar despejado para reevaluar su avance.
* **Vehículos en el mismo sentido:** Los vehículos que se desplazan en la misma dirección pueden ocupar múltiples segmentos del puente simultáneamente. La operación de espera (`pthread_cond_wait`) solo se activa si la siguiente posición específica en el arreglo de mutexes que el vehículo intenta ocupar ya se encuentra tomada por otro vehículo en la misma dirección. Esto permite un flujo continuo y eficiente de vehículos en una única dirección.

## Estructura del Proyecto

El proyecto se compone de los siguientes módulos principales, cada uno implementando una estrategia de control de tráfico distinta, además de los archivos de configuración y construcción:

### 1. `Carnage.h` y `Carnage.c` (Modelo de Colisión Controlada)

* **Descripción:** Implementa un modelo de control de tráfico diseñado para simular escenarios de alta concurrencia con una mínima priorización direccional. Sirve como un entorno para evaluar la robustez fundamental del sistema del puente.
* **Funcionalidad Clave:**
    * Controla el movimiento de los automóviles (`comportamiento_automovil`) a través de los segmentos del puente.
    * Utiliza `pthread_mutex_t` y `pthread_cond_t` para la sincronización de acceso.
    * Define parámetros como la longitud del puente (`LARGO_PUENTE`) y las velocidades vehiculares.
    * Mantiene registros de la cantidad de vehículos en el puente y en cada dirección (`carrosEnPuente`, `carrosOaE`, `carrosEaO`).
* **Estructuras:** `struct Automovil` (ID, sentido).
* **Funciones:** `cargaDatos()`, `comportamiento_automovil()`.

### 2. `Semaforo.h` y `Semaforo.c` (Modelo de Semáforo)

* **Descripción:** Implementa un sistema de control de tráfico para el puente basado en un semáforo, el cual alterna el sentido del flujo vehicular en intervalos de tiempo predefinidos.
* **Funcionalidad Clave:**
    * Define las duraciones para el tráfico en cada sentido (`duracionOesteEste`, `duracionEsteOeste`).
    * Emplea múltiples mutexes y variables de condición para la sincronización del cambio de sentido y el acceso.
    * Monitorea la cantidad de vehículos en el puente por dirección (`carrosEnPuenteSemaforo`, `carrosOaESemaforo`, `carrosEaOSemaforo`).
* **Estructuras:** `struct AutomovilSemaforo` (ID, sentido), `struct Semaforo` (estado, sentido).
* **Funciones:** `cargarDatosSemaforo()`, `comportamiento_semaforo()`, `comportamiento_automovil_semaforo()`.

### 3. `Oficial_Transito.h` y `Oficial_Transito.c` (Modelo de Oficial de Tránsito)

* **Descripción:** Describe un sistema de control de tráfico donde oficiales de tránsito dirigen el flujo vehicular. Este modelo incorpora una lógica de turnos más dinámica, influenciada por la cantidad de vehículos que han cruzado o que están en espera.
* **Funcionalidad Clave:**
    * Define la longitud del puente y las velocidades de los vehículos.
    * Introduce los parámetros `k1` y `k2`, que controlan los umbrales para el cambio de turno.
    * Utiliza múltiples mutexes y variables de condición para la sincronización entre vehículos y oficiales.
    * Controla el sentido actual del tráfico (`sentidoOfi`) y los contadores de vehículos (`carrosEnPuenteOfi`, `carrosOaEOfi`, `carrosEaOOfi`, `esperando_oeste_esteOfi`, `esperando_este_oesteOfi`).
* **Estructuras:** `struct AutomovilOfi` (ID, sentido), `struct Oficial` (estado, sentido, ID).
* **Funciones:** `cargarDatosTrafico()`, `comportamiento_oficial_Oeste()`, `comportamiento_oficial_Este()`, `comportamiento_automovil_oficiales()`.

### Archivos de Configuración y Construcción

* **`Makefile`:** Archivo que define las reglas de compilación del proyecto. Utiliza `g++` con flags para advertencias (`-Wall`, `-Werror`), el estándar C++17 (`-std=c++17`), y la herramienta `address sanitizer` (`-fsanitize=address`) para depuración.
* **`CMakeLists.txt`:** Script de construcción para CMake. Establece el estándar C17 y configura la compilación del ejecutable `proyecto` incluyendo todos los archivos fuente y de cabecera relevantes.
* **`Prueba.txt`:** Archivo de texto que contiene los parámetros numéricos de entrada utilizados para configurar las diferentes simulaciones del proyecto.

---
