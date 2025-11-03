# Simulation Of A Bridge With Threads

This project implements a vehicle traffic simulation on a bridge, exploring different flow control mechanisms. The simulation represents the bridge as an array of mutexes, where each mutex controls access to a specific bridge segment. It uses synchronization mechanisms like `pthread_cond_wait` and `pthread_cond_broadcast` to manage vehicle concurrency.

# Skills
![C](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)
![pthreads](https://img.shields.io/badge/Multithreading-p--threads-FF6F00?style=for-the-badge)
![Threads](https://img.shields.io/badge/Concept-Threads-9C27B0?style=for-the-badge)
![Deadlocks](https://img.shields.io/badge/Concept-Deadlocks-F44336?style=for-the-badge)
![Operating Systems](https://img.shields.io/badge/Topic-Operating%20Systems-3F51B5?style=for-the-badge&logo=linux&logoColor=white)


## Bridge Operation

The bridge is divided into segments, each protected by an individual mutex. Vehicle access logic works as follows:

* **Opposite-direction vehicles:** When a vehicle tries to enter a segment occupied by an opposite-direction vehicle, its thread waits using `pthread_cond_wait`. The thread remains suspended until the segment is freed and a general notification (`pthread_cond_broadcast`) indicates the path may be clear for reevaluation.
* **Same-direction vehicles:** Vehicles moving in the same direction can occupy multiple bridge segments simultaneously. The wait operation (`pthread_cond_wait`) only activates if the next specific mutex array position is already taken by another same-direction vehicle, enabling efficient one-direction traffic flow.

## Project Structure

The project contains these main modules, each implementing different traffic control strategies, plus configuration and build files:

### 1. `Carnage.h` and `Carnage.c` (Controlled Collision Model)

* **Description:** Implements a traffic control model designed for high-concurrency scenarios with minimal directional prioritization. Serves as a baseline for testing bridge system robustness.
* **Key Features:**
    * Controls vehicle movement (`comportamiento_automovil`) through bridge segments
    * Uses `pthread_mutex_t` and `pthread_cond_t` for access synchronization
    * Defines parameters like bridge length (`LARGO_PUENTE`) and vehicle speeds
    * Tracks vehicles on bridge and per direction (`carrosEnPuente`, `carrosOaE`, `carrosEaO`)
* **Structures:** `struct Automovil` (ID, direction)
* **Functions:** `cargaDatos()`, `comportamiento_automovil()`

### 2. `Semaforo.h` and `Semaforo.c` (Traffic Light Model)

* **Description:** Implements a traffic light system that alternates traffic direction at predefined intervals.
* **Key Features:**
    * Defines duration for each traffic direction (`duracionOesteEste`, `duracionEsteOeste`)
    * Uses multiple mutexes and condition variables for direction switching synchronization
    * Monitors vehicles per direction (`carrosEnPuenteSemaforo`, `carrosOaESemaforo`, `carrosEaOSemaforo`)
* **Structures:** `struct AutomovilSemaforo` (ID, direction), `struct Semaforo` (status, direction)
* **Functions:** `cargarDatosSemaforo()`, `comportamiento_semaforo()`, `comportamiento_automovil_semaforo()`

### 3. `Oficial_Transito.h` and `Oficial_Transito.c` (Traffic Officer Model)

* **Description:** Implements a traffic control system where officers dynamically direct vehicle flow based on crossing/queued vehicles.
* **Key Features:**
    * Defines bridge length and vehicle speeds
    * Introduces `k1` and `k2` parameters controlling direction-switching thresholds
    * Uses multiple mutexes/condition variables for vehicle-officer synchronization
    * Manages current traffic direction (`sentidoOfi`) and vehicle counters (`carrosEnPuenteOfi`, `carrosOaEOfi`, etc.)
* **Structures:** `struct AutomovilOfi` (ID, direction), `struct Oficial` (status, direction, ID)
* **Functions:** `cargarDatosTrafico()`, `comportamiento_oficial_Oeste()`, `comportamiento_oficial_Este()`, `comportamiento_automovil_oficiales()`

### Configuration and Build Files

* **`Makefile`:** Compilation rules using `g++` with warning flags (`-Wall`, `-Werror`), C++17 standard (`-std=c++17`), and address sanitizer (`-fsanitize=address`) for debugging
* **`CMakeLists.txt`:** CMake build script setting C17 standard and configuring `proyecto` executable compilation
* **`Prueba.txt`:** Input parameter file for simulation configuration
