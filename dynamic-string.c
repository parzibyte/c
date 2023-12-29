#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Cadena
{
    size_t longitud;
    char *datos;
};
void iniciarCadena(struct Cadena *cadena)
{
    cadena->longitud = 0;
    cadena->datos = malloc(1);
    cadena->datos[cadena->longitud] = '\0';
}
void concatenar(struct Cadena *cadena, char *otraCadena)
{
    size_t longitudDeOtraCadena = strlen(otraCadena);
    size_t nuevaLongitud = cadena->longitud + longitudDeOtraCadena;
    size_t nuevaLongitudConByteNulo = nuevaLongitud + 1;
    size_t longitudDeOtraCadenaIncluyendoByteNulo = longitudDeOtraCadena + 1;
    char *cadenaConNuevaLongitud = realloc(cadena->datos, nuevaLongitudConByteNulo);
    if (cadenaConNuevaLongitud == NULL)
    {
        printf("Sin memoria suficiente para concatenar la nueva cadena");
        return;
    }
    cadena->datos = cadenaConNuevaLongitud;
    memcpy((char *)(cadena->datos + cadena->longitud), otraCadena, longitudDeOtraCadenaIncluyendoByteNulo);
    cadena->longitud = nuevaLongitud;
}
int main()
{
    struct Cadena *miCadena = (struct Cadena *)malloc(sizeof(struct Cadena));
    if (miCadena == NULL)
    {
        printf("Sin memoria suficiente para alojar el struct de cadena");
        return EXIT_FAILURE;
    }
    iniciarCadena(miCadena);
    printf("La cadena inicial es: '%s'\n", miCadena->datos);
    concatenar(miCadena, "Hola mundo ");
    concatenar(miCadena, "parzibyte.me ");
    printf("La cadena es: '%s'\n", miCadena->datos);
    free(miCadena->datos);
    free(miCadena);
    return 0;
}