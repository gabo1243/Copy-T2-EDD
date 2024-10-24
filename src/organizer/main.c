#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PACKAGE_SIZE 3

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


typedef struct position
{
  int row;
  int col;
} Position;

typedef struct package
{
  int height;
  int width;
  int min_width;
  int min_height;
  int size;
  char id;
  Position* positions;

} Package;


bool can_put_package(char** compartment, int row, int col, Package package, int n_rows, int n_cols)
{
  for (int pos = 0; pos < package.size; pos++)
  {
    if (row + package.positions[pos].row < n_rows && col+ package.positions[pos].col < n_cols)
    {
      // POSICION VALIDA SEGUN RANGOS
      if (compartment[row + package.positions[pos].row][col+ package.positions[pos].col] != '1')
      {
        // HAY ALGUNA POSICION NO DISPONIBLE DESDE LA ORIGEN
        return false;
      }
    }
    else
    {
      // PASA RANGOS; ENTONCES NO SE PUEDE PONER
      return false;
    }
  }
  // NO HAY CONFLICTOS
  return true;
}

void put_package(char** compartment, int row, int col, Package package)
{
  // SE ASUME QUE SE PUEDEN PONER TODAS LAS POSICIONES YA QUE ANTES SE COMPROBÓ
  for (int pos = 0; pos < package.size; pos++)
  {
    int new_row = row + package.positions[pos].row;
    int new_col = col+ package.positions[pos].col;;
    compartment[new_row][new_col] = package.id;
  }
}

char** get_compartment_copy(char** compartment, int n_rows, int n_cols)
{
  char** compartment_copy= calloc(n_rows, sizeof(int*));
  for (int i = 0; i < n_rows; i++) {
    compartment_copy[i] = calloc(n_cols, sizeof(int));
    for (int j = 0; j < n_cols; j++) {
      compartment_copy[i][j] = compartment[i][j];
    }
  }
  return compartment_copy;
}

void free_compartment(char** compartment, int n_rows)
{
  for (int i = 0; i<n_rows; i++)
  {
    free(compartment[i]);
  }
  free(compartment);
}

bool get_solution(Package* packages_available,int actual_package_int, int total_packages, char** compartment, int n_rows, int n_cols, FILE* output_file)
{
  if (actual_package_int >= total_packages)
  {
    for(int i = 0; i<n_rows; i++)
    {
      for(int j = 0; j<n_cols; j++)
      {
        if (j == n_cols - 1)
        {
          fprintf(output_file, "%c", compartment[i][j]);
        }
        else
        {
          fprintf(output_file, "%c ", compartment[i][j]);
        }
      }
      fprintf(output_file, "\n");
    }
    return true;
  }
  char** compartment_copy = get_compartment_copy(compartment, n_rows, n_cols);

  int pack_int = actual_package_int;
  // VEO CADA FILA
  for (int compartment_row= 0; compartment_row < n_rows; compartment_row++)
  {
    // VEO CADA COLUMNA
    for (int compartment_col= 0; compartment_col < n_cols; compartment_col++)
    {
      // FALTA REVISAR SI EL PAQUETE CABE
      if (can_put_package(compartment, compartment_row, compartment_col, packages_available[pack_int], n_rows, n_cols))
      {
        // EL PAQUETE CABE, ENTONCES SIGO CON EL RESTO DE PAQUETES
        // PONGO EL PAQUETE
        put_package(compartment_copy, compartment_row, compartment_col, packages_available[pack_int]);
        if(get_solution(packages_available, pack_int +1, total_packages, compartment_copy, n_rows, n_cols, output_file))
        {
          free_compartment(compartment_copy, n_rows);
          return true;
        }
        free_compartment(compartment_copy, n_rows);
        compartment_copy = get_compartment_copy(compartment, n_rows, n_cols);
      }
    }
  }

  free_compartment(compartment_copy, n_rows);
  return false;
}





int main(int argc, char **argv) {
  check_arguments(argc, argv);

  FILE *input_file = fopen(argv[1], "r");
  FILE *output_file = fopen(argv[2], "w");

  int N_FILAS, N_COLUMNAS;
  fscanf(input_file, "%d", &N_FILAS);
  fscanf(input_file, "%d", &N_COLUMNAS);

  char** compartment= calloc(N_FILAS, sizeof(int*));

  // Para leer la matriz de compartimientos
  for (int i = 0; i < N_FILAS; i++) {
    compartment[i] = calloc(N_COLUMNAS, sizeof(int));
    for (int j = 0; j < N_COLUMNAS; j++) {
      int value;
      fscanf(input_file, "%d", &value);
      compartment[i][j] = '0'+ value;
      // if (compartment[i][j]=='1')
      //   printf("%c\n", compartment[i][j] );
      // printf("%c\n", compartment[i][j]=="1");
      
    }
  }

  int N_PAQUETES;
  fscanf(input_file, "%d", &N_PAQUETES);

  // int packages_array[N_PAQUETES][PACKAGE_SIZE][PACKAGE_SIZE];
  Package* packages = calloc(N_PAQUETES, sizeof(Package));

  for (int i = 0; i < N_PAQUETES; i++) {
    char package_buffer_id[2];
    fscanf(input_file, "%s\n", package_buffer_id);
    char package_id = package_buffer_id[0];

    // int package[PACKAGE_SIZE][PACKAGE_SIZE];
    packages[i].id = package_id;
    packages[i].positions = calloc(PACKAGE_SIZE*PACKAGE_SIZE, sizeof(Position));
    int pos_count = 0;
    for (int j = 0; j < PACKAGE_SIZE; j++) {
      for (int k = 0; k < PACKAGE_SIZE; k++) {
        // j = fila, k = columna
        int value;
        // fscanf(input_file, "%d", &packages_array[i][j][k]);
        fscanf(input_file, "%d", &value);
        if (value != 0)
        {
          packages[i].positions[pos_count].row = j;
          packages[i].positions[pos_count].col = k;
          pos_count ++;
        }
      }
    }
    packages[i].size = pos_count;

    // printf("Paquete %c\n", package_id);
    // for (int j = 0; j < PACKAGE_SIZE; j++) {
    //   for (int k = 0; k < PACKAGE_SIZE; k++) {
    //     printf("%d\n", package[j][k]);
    //   }
    // }
  }


  //----------------------COMIENZA PROCESO PRE BACKTRACKING------------------------------


  for (int i = 0; i<N_PAQUETES; i++)
  {
    // printf("ID: %c\n", packages[i].id);
    // printf("OCUPA: %d\n", packages[i].size);

    // AJUSTO EL PACKETE PARA QUE QUEDE EN 0 (POSICIONES RELATIVAS)
    int max_width = 0;
    int min_width = PACKAGE_SIZE;
    int max_height = 0;
    int min_height = PACKAGE_SIZE;
    for (int pos = 0; pos < packages[i].size; pos++)
    {
      // printf("POS FILA: %d\n", packages[i].positions[pos].row);
      // printf("POS COLUMNA: %d\n", packages[i].positions[pos].col);
      if (packages[i].positions[pos].col > max_width)
      {
        max_width = packages[i].positions[pos].col;
      }
      if (packages[i].positions[pos].col < min_width)
      {
        min_width = packages[i].positions[pos].col;
      }
      if (packages[i].positions[pos].row > max_height)
      {
        max_height = packages[i].positions[pos].row;
      }
      if (packages[i].positions[pos].row < min_height)
      {
        min_height = packages[i].positions[pos].row;
      }
    }
    packages[i].min_width = min_width;
    packages[i].width = max_width - min_width + 1;
    packages[i].min_height = min_height;
    packages[i].height = max_height - min_height + 1;
    // printf("MAX MIN HEIGHT:(%d, %d)\n",max_height, min_height);

    // HAGO REESCALAMIENTO (MOVER A ESQUINA IZQ)
    for (int pos_ = 0; pos_ < packages[i].size; pos_++)
    {
      // printf("POS ANTES FILA COLUMNA:(%d, %d)\n",packages[i].positions[pos_].row, packages[i].positions[pos_].col);
      if (packages[i].min_width > 0)
        packages[i].positions[pos_].col -= (PACKAGE_SIZE - packages[i].width);
      if (packages[i].min_height > 0)
        packages[i].positions[pos_].row -= (PACKAGE_SIZE - packages[i].height);
      // printf("POS FILA COLUMNA:(%d, %d)\n",packages[i].positions[pos_].row, packages[i].positions[pos_].col);
    }
  }


  //----------------------TERMINA PROCESO PRE BACKTRACKING------------------------------

  // BACKTRACKING EN SÍ
  bool final = get_solution(packages, 0, N_PAQUETES, compartment, N_FILAS, N_COLUMNAS, output_file);
  // printf("%s\n", final ? "TRUE": "FALSE");
  
  /* Cerrar archivos */
  fclose(input_file);
  fclose(output_file);

  /* TODO: Liberar memoria */

  for (int i = 0; i<N_PAQUETES; i++)
  {
    free(packages[i].positions);
  }
  free(packages);

  for (int i = 0; i <N_FILAS; i++)
  {
    free(compartment[i]);
  }
  free(compartment);

  return 0;
}
