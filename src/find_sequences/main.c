#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static bool check_arguments(int argc, char **argv) {
  if (argc != 3) {
    printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
    printf("Donde:\n");
    printf("\tINPUT es la ruta del archivo de input\n");
    printf("\tOUTPUT es la ruta del archivo de output\n");
    exit(1);
  }
  return true;
}

long pow_manual(int number, int times, long* pows_array)
{
  if (pows_array[times] != 0)
  {
    return pows_array[times];
  }
  // printf("PRIMER: %d\n", times);
  long result = 1;
  for (int i = 1; i<=times; i++)
  {
    result *= (long) number;
    // printf("%ld\n", result);
  }
  pows_array[times] = result;
  return result;
}


long hash_pattern(char* pattern, int size, long* pows_array)
{
  long hvalue = 0;
  int m = size -1;
  for (int j = 0; j < size; j++)
  {
    hvalue+=  (int)(pattern[j]) * pow_manual(3, m, pows_array);
    // printf("LETRA: %c, NUM: %f, M: %d,  MULT: %f\n",pattern[j], get_number(pattern[j]),m,pow_manual(5, m) );
    m -= 1;
  }
  return hvalue;
}


long* hash(char* sequence, int size,int search_size, long* pows_array)
{
  long* H = (long*)calloc(size, sizeof(long));

  for (int i = 0; i <= size - search_size; i++)
  {
    if (i==0)
    {
      H[i] = hash_pattern(sequence, search_size, pows_array);
    }

    else
    {
      // int div = pow(5, search_size - 1);
      // int hvalue = (H[i-1] % div ) *10 +get_number(sequence[i + search_size -1]);
      long hvalue = (H[i-1]- (int)(sequence[i-1])*pow_manual(3, search_size - 1, pows_array))*3 +(int)(sequence[i + search_size -1]);

      H[i] = hvalue;
    }
  }
  return H;
  
}


int search(long* H, int sequence_size, char* pattern, int pattern_size, long* pows_array)
{
  long hashed_pattern = hash_pattern(pattern, pattern_size, pows_array);
  
  // printf("PATTERN: %f\n", hashed_pattern);
  for (int i = 0; i <= sequence_size - pattern_size; i++)
  {
    // printf("EN %d: %d\n", i,  H[i]);
    if (H[i] == hashed_pattern)
    {
      return i;
    }
  }
  return -1;

}


int main(int argc, char **argv) {
  check_arguments(argc, argv);

  FILE *input_file = fopen(argv[1], "r");
  FILE *output_file = fopen(argv[2], "w");

  int TAMANO_DOCUMENTO;
  fscanf(input_file, "%d", &TAMANO_DOCUMENTO);

  // Leer el documento
  char documento[TAMANO_DOCUMENTO + 1];  // +1 para el carácter nulo
  fscanf(input_file, "%s", documento);

  int largo_cadena, cantidad_consultas;
  fscanf(input_file, "%d", &largo_cadena);
  fscanf(input_file, "%d", &cantidad_consultas);


  // cantidad_consultas = 0;
  long* pows_array = calloc(largo_cadena, sizeof(long)); 

  long* H = hash(documento, TAMANO_DOCUMENTO, largo_cadena, pows_array); 
  // Leer las cadenas de consulta y buscarlas en el documento
  for (int q = 0; q < cantidad_consultas; q++) {
    char cadena[largo_cadena + 1];  // +1 para el carácter nulo
    fscanf(input_file, "%s", cadena);
    // Implementa aca tu tarea
    int result = search(H, TAMANO_DOCUMENTO, cadena, largo_cadena, pows_array);

    fprintf(output_file, "Consulta %d\n", q);
    // fprintf(output_file,"%d: %s\n", q, cadena);
    if (result != -1)
    {
      fprintf(output_file,"%d\n", result);
    }
    
  }

  free(H);
  free(pows_array);


  /* Cerrar archivos */
  fclose(input_file);
  fclose(output_file);

  // char* sequence = "GTCACAAC";

  // char* pattern = "AAC";

  // printf("POS: %d\n", search(sequence, 8, pattern, 3));


  return 0;
}
