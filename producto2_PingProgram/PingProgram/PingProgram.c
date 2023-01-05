#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Menu.h"

/**
* @brief driverMain(): Funci�n driver principal. Delega en Menu.c.
* Ejecuta la funci�n menuDriver() de Menu.c.
*/
void driverMain();

int main(int argc, char const* argv[]) {
    driverMain();

    return 0;
}

void driverMain() {
    menuDriver();
}