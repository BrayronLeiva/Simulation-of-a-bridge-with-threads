cmake_minimum_required(VERSION 3.23)
project(proyecto C)

set(CMAKE_C_STANDARD 17)

add_executable(proyecto main.c
        Semaforo.c
        Semaforo.h
        Carnage.h
        Carnage.c
        Oficial_Transito.h
        Oficial_Transito.c
)
