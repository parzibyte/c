#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

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

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
	// Esta función podría ser invocada varias veces por cURL. https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
	size_t realsize = size * nmemb;
	concatenar((struct Cadena *)userp, (char *)contents);
	concatenar((struct Cadena *)userp, "");
	return realsize;
}

int main(void)
{
	CURL *curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if (!curl)
	{
		printf("No se pudo inicializar CURL\n");
		return 0;
	}
	struct Cadena *miCadena = (struct Cadena *)malloc(sizeof(struct Cadena));
	if (miCadena == NULL)
	{
		printf("Sin memoria suficiente para alojar el struct de cadena");
		return EXIT_FAILURE;
	}
	iniciarCadena(miCadena);
	char *url = "https://jsonplaceholder.typicode.com/posts";
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)miCadena);
	res = curl_easy_perform(curl);
	if (res == CURLE_OK)
	{
		printf("La respuesta es: %s", miCadena->datos);
	}
	else
	{
		fprintf(stderr, "Error en la petición: %s\n", curl_easy_strerror(res));
	}
	free(miCadena->datos);
	free(miCadena);
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return 0;
}