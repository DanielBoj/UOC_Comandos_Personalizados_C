#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Menu.h"

/**
* @brief driverMain(): Función driver principal. Delega en Menu.c.
* Ejecuta la función menuDriver() de Menu.c.
*/
void driverMain();

int main(int argc, char const* argv[]) {
    driverMain();

    return 0;
}

void driverMain() {
    menuDriver();
}