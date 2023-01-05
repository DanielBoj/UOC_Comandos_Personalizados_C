#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "LinkedList.h"

/**
 * @brief initListaAdptr: Inicializa una Linked List de TAD adptr con la
 * información de los adaptadores de res. Realiza la reserva de memoria
 * e inicializa head a NULL.
 *
 * @param lista listaAdptr Parámetro de entrada/salida con los TAD tipo adptr.
 */
void initListaAdptr(listaAdptr* lista) {
    lista->head = (adptr*)malloc(sizeof(adptr));
    lista->head = NULL;
    lista->numElem = 0;
}

/**
 * @brief appendAdptr: Permite añadir un nuevo adaptador a la lista. Por
 * defecto, como último elemento.
 *
 * @param lista listaAdtr Linked List de TAD tipo adptr.
 * @param nombreAdptr String Nombre del adaptador para asignar al nuevo nodo.
 *  Parámetro de entrada.
 */
void appendAdptr(listaAdptr* lista, const char* nombreAdptr) {
    adptr* nuevoAdptr = (adptr*)malloc(sizeof(adptr));
    adptr* actualAdptr = lista->head;

    /* Construimos el nodo */
    nuevoAdptr->nombre = (char*)malloc(MAX_ADPT_NAME);
    if (nuevoAdptr->nombre != NULL) {
        strncpy(nuevoAdptr->nombre, nombreAdptr, MAX_ADPT_NAME);
    }
            
    nuevoAdptr->key = lista->numElem;
    nuevoAdptr->next = NULL;

    if (!actualAdptr) {
        lista->head = nuevoAdptr;
        lista->numElem = lista->numElem + 1;
        // printf("%s\n", lista->head->nombre);
        return;
    }

    while (actualAdptr->next) {
        actualAdptr = actualAdptr->next;
    }

    actualAdptr->next = nuevoAdptr;
    // printf("%s\n", nuevoAdptr->nombre);
    lista->numElem = lista->numElem + 1;

    return;
}

/**
 * @brief buscarAptrPorKey: Actúa como un índice de posición en un Array.
 * Devuelve un adapatardor localizándolo por su posición en la lista, las
 * posiciones relativas se guardan en el campo key del TAD.
 * El recorrido de la lista se realiza mediante un bucle while que finaliza
 * cuando el apuntador a un nodo siguiente devuelve NULL.
 *
 * @param head adptr Nodo inicial de la lista.
 * @param key int Posicíon en la lista.
 * @return adptr* TAD tipo adptr que contiene la información de un adaptador
 *  de red.
 */
adptr* buscarAdptrPorKey(adptr* head, const int key) {
    adptr* actual = head;

    if (!actual) {
        puts("La lista esta vacia.");
        return NULL;
    }

    /* Iteramos por la lista mientras el valor de nodo->next no devuelva nulo. */
    while (actual) {
        if (actual->key == key) {

            return actual;
        }

        actual = actual->next;
    }

    return NULL;
}

/**
 * @brief clearListAdpr: Libera la memoria de una lista de adaptadores,
 * borrándola de manera efectiva. Además, iguale los punteros a NULL para
 * que no haya punteros que apunten a posiciones erróneas.
 *
 * @param lista listaAdptr Linked List de TDA tipo adptr.
 * @param head adptr Puntero a HEAD de la Linked List.
 */
void clearListaAdptr(listaAdptr* lista, adptr* head) {
    if (head == NULL) {
        lista->numElem = 0;
        return;
    }

    clearListaAdptr(lista, head->next);
    free(head->nombre);
    head->nombre = NULL;
    free(head);
    head = NULL;
}

/**
 * @brief printListaAdptrs(): Imprime por pantalla los TDA tipo adptr que forma los
 * nodos de una LInked List listaAdptr. Es decir, imprime una lista de todos los
 * adaptadores que hemso guardado.
 *
 * @param head adptr Nod head de la Linked List.
 */
void printListaAdptrs(adptr* head) {

    adptr* actual = head;

    while (actual) {
        printf("#%d: %s\n", actual->key + 1, actual->nombre);
        actual = actual->next;
    }
}