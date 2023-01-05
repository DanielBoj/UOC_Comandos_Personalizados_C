#pragma once // Nos aseguramos de que la librería solo pueda incluirse una vez en la compilación.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef MAX_ADPT_NAME
#define MAX_ADPT_NAME 255 // Longitud máxima del nombre del adaptador
#endif

/**
 * @brief Estructura TAD para almacenar la información que nos interesa de un
 * adaptador.
 *
 */
typedef struct tAdptr {
    char* nombre;
    int key;
    struct tAdptr* next;
} adptr;

/**
 * @brief Estructura de datos tipo Linked List para almacenar TAD de tipod adptr
 * con la información de los adaptadores de red.
 *
 */
typedef struct tListaAdptr {
    adptr* head;
    int numElem;
} listaAdptr;

/**
 * @brief initListaAdptr: Inicializa una Linked List de TAD adptr con la
 * información de los adaptadores de res. Realiza la reserva de memoria
 * e inicializa head a NULL.
 *
 * @param lista listaAdptr Parámetro de entrada/salida con los TAD tipo adptr.
 */
void initListaAdptr(listaAdptr* lista);

/**
 * @brief appendAdptr: Permite añadir un nuevo adaptador a la lista. Por
 * defecto, como último elemento.
 *
 * @param lista listaAdtr Linked List de TAD tipo adptr.
 * @param nombreAdptr String Nombre del adaptador para asignar al nuevo nodo.
 *  Parámetro de entrada.
 */
void appendAdptr(listaAdptr* lista, const char* nombreAdptr);

/**
 * @brief buscarAptrPorKey: Actúa como un índice de posición en un Array.
 * Devuelve un adapatardor localizándolo por su posición en la lista, las
 * posiciones relativas se guardan en el campo key del TAD.
 *
 * @param head adptr Nodo inicial de la lista.
 * @param key int Posicíon en la lista.
 * @return adptr* TAD tipo adptr que contiene la información de un adaptador
 *  de red.
 */
adptr* buscarAdptrPorKey(adptr* head, const int key);

/**
 * @brief clearListAdpr: Libera la memoria de una lista de adaptadores,
 * borrándola de manera efectiva. Además, iguale los punteros a NULL para
 * que no haya punteros que apunten a posiciones erróneas.
 *
 * @param lista listaAdptr Linked List de TDA tipo adptr.
 * @param head adptr Puntero a HEAD de la Linked List.
 */
void clearListaAdptr(listaAdptr* lista, adptr* head);

/**
 * @brief printListaAdptrs(): Imprime por pantalla los TDA tipo adptr que forma los
 * nodos de una LInked List listaAdptr. Es decir, imprime una lista de todos los
 * adaptadores que hemso guardado.
 *
 * @param head adptr Nod head de la Linked List.
 */
void printListaAdptrs(adptr* head);