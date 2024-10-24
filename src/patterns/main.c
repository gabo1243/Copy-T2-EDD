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

typedef struct node
{
  int index;
  struct node* next;
  struct node* last;
} Node;

Node* create_node(int index)
{
  Node* new_node = malloc(sizeof(Node));
  new_node->index = index;
  new_node->next = NULL;
  new_node->last = new_node;
  return new_node;
}


int get_height(int n_nodes)
{
  int num = n_nodes + 1;
  return (int) ceil((log2(num))) - 1;
}

// long get_hash(int size, char string[size +1] , int n_mod, long* pows_array)
// {
//   int position = 0;
//   long sum = 0;
//   for (int i = 0; i<size; i++)
//   {
//     int int_char = (int) string[i];
//     sum += int_char* pow_manual(2, position, pows_array);
//     position++;
//   }
//   return sum%n_mod;
// }


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

long get_hash(int size, char string[size +1] , int n_mod)
{
  // PARA EL HASH ME BASÉ EN https://www.chegg.com/homework-help/questions-and-answers/write-simple-hashing-function-uses-addition-multiplication-find-pair-strings-return-hash-v-q104558866
  // AUNQUE LO CAMBIÉ LIGERAMENTE
  int position = 0;
  long sum = 0;
  for (int i = 0; i<size; i++)
  {
    long long_char = (long) string[i];
    // sum += long_char* position;
    sum = (sum +((position) + 1)* long_char) % n_mod;
    position++;
  }
  return sum;
}

void put_hash_in_array(long sum, int n_mod, int sub_tree_root_index, Node** array)
{
  // HASHEO E INDEXO
  long hash = sum;
  // long hash = sum%n_mod;
  Node* node = create_node(sub_tree_root_index);
  // printf("%s: SUM: %d\n", array[hash]== NULL ? "NULL": " NOT NULL", sum);
  if (array[hash] == NULL)
  {
    array[hash] = node;
  }
  else
  {
    Node* actual_node = array[hash];
    // printf("%s\n", actual_node== NULL ? " NOT NULL": "NULL");
    
    // while(actual_node->next!= NULL)
    // {
    //   actual_node = actual_node->next;
    // }
    // if (actual_node->index == sub_tree_root_index)
    // {
    //   free(node);
    //   return;
    // }
    Node* last_node = actual_node->last;
    if (last_node->index == sub_tree_root_index)
    {
      free(node);
      return;
    }
    last_node->next = node;
    actual_node->last = node;


    // actual_node->next = node;
  }
}

bool is_sub_tree(int n_nodes, char tree[n_nodes + 1], int sub_tree_n_nodes, int sub_tree_root_index, char* pattern_tree)
{
  // SUB ARBOL PARTIENDO DEL NODO QUE ESTA EN sub_tree_root_index CON ALTURA sub_tree_heigth
  int sub_tree_root_height= get_height(sub_tree_n_nodes) + 1;
  int count = 0;
  for (int sub_tree_height = 0; sub_tree_height < sub_tree_root_height; sub_tree_height++)
  {
    // ALTURA RELATIVA sub_tree_height
    int nodes_in_height = pow(2, sub_tree_height);
    int offset = nodes_in_height * (sub_tree_root_index + 1) - 1;
    for (int sub_tree_node_index = offset; sub_tree_node_index < offset + nodes_in_height; sub_tree_node_index++)
    {
      // NODO
      // printf("N_NODES: %d, N_NODES_SUB_TREE: %d\n", n_nodes, sub_tree_n_nodes);
      // printf("COUNT: %d, INDEX: %d\n", count, sub_tree_node_index);
      if (sub_tree_node_index < n_nodes && tree[sub_tree_node_index] != pattern_tree[count])
      {
        return false;
      }
      count++;
    }
  }
  return true;
  
}



// FUNCIÓN DADA EN ANEXO DEL ENUNCIADO
void hash_tree(int n_nodes, char tree[n_nodes + 1], int n_mod, Node** array)
{
  int tree_height = get_height(n_nodes);
  for (int sub_tree_root_index = 0; sub_tree_root_index < n_nodes; sub_tree_root_index++)
  {
    // SUB ARBOL PARTIENDO DEL NODO QUE ESTA EN sub_tree_root_index CON ALTURA sub_tree_heigth
    int sub_tree_root_height = tree_height - get_height(sub_tree_root_index + 1) + 1;
    // printf("\nNODO: %d: %c\n", sub_tree_root_index, tree[sub_tree_root_index]);
    int position = 0;
    long sum = 0;
    for (int sub_tree_height = 0; sub_tree_height < sub_tree_root_height; sub_tree_height++)
    {
      // printf("ALTURA %d: ", sub_tree_height);
      // ALTURA RELATIVA sub_tree_height
      int nodes_in_height = pow(2, sub_tree_height);
      int offset = nodes_in_height * (sub_tree_root_index + 1) - 1;
      for (int sub_tree_node_index = offset; sub_tree_node_index < offset + nodes_in_height; sub_tree_node_index++)
      {
        // NODO
        // printf("%c ", tree[sub_tree_node_index]);
        long long_char = (long) tree[sub_tree_node_index];
        // sum += (int) int_node * pow(2, position);
        // sum +=  long_char * position;
        sum = (sum +((position) + 1)* long_char) % n_mod;
        position++;
      }
      put_hash_in_array(sum, n_mod, sub_tree_root_index, array);
    }
  }
  
}

int main(int argc, char **argv) {
  check_arguments(argc, argv);

  FILE *input_file = fopen(argv[1], "r");
  FILE *output_file = fopen(argv[2], "w");

  
  int N_NODOS;
  fscanf(input_file, "%d\n", &N_NODOS);

  char arbol[N_NODOS + 1];
  fscanf(input_file, "%s\n", arbol);
  // printf("Arbol %s\n", arbol);

  int N_CONSULTAS;
  fscanf(input_file, "%d\n", &N_CONSULTAS);

  // long* pows_array = calloc(50, sizeof(long));

  int N_MOD = 111121;

  Node** array = calloc(N_MOD, sizeof(Node*));
  // printf("ANTES\n");
  hash_tree(N_NODOS, arbol, N_MOD, array);

  /* TODO: Construcción de las estructuras */
  // printf("HASHEADO\n");
  /* Eventos */
  for (int i = 0; i < N_CONSULTAS; i++) {
    int N_NODOS_CONSULTA;
    fscanf(input_file, "%d\n", &N_NODOS_CONSULTA);
    char consulta[N_NODOS_CONSULTA + 1];
    fscanf(input_file, "%s\n", consulta);
    // printf("Consulta %s\n", consulta);
    int consult_hash = get_hash(N_NODOS_CONSULTA, consulta, N_MOD);
    // printf("HASH: %d\n", consult_hash);
    Node* node = array[consult_hash];
    
    if (node == NULL)
    {
      fprintf(output_file, "%d\n", -1);
    }
    else
    {
      int count = 0;
      while (node != NULL)
      {
        if (is_sub_tree(N_NODOS, arbol, N_NODOS_CONSULTA, node->index, consulta))
        {
          fprintf(output_file, "%s%d", count==0?"":" ",node->index);
          count++;
        }
        node = node->next;
      }
      fprintf(output_file, "%s\n", count==0 ? "-1": "");   
    }
  }
  // char pat[4] = "GIG";
  // printf("%s\n",is_sub_tree(N_NODOS, arbol, 3, 2, pat)? "TRUE":"FALSE" );

  /* Cerra archivos */
  fclose(input_file);
  fclose(output_file);

  /* TODO: Liberar memoria */

  for (int i = 0; i<N_MOD; i++)
  {
    Node* node = array[i];
    while (node != NULL)
    {
      Node* aux = node;
      node = node->next;
      free(aux);
    }
    free(node);
  }

  free(array);
  // free(pows_array);


  // printf("%f", log2(262143));

  // char* abecedario = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  // for (int i =0; i<26; i++)
  // {
  //   printf("VALOR %c CON INT %d\n", abecedario[i], abecedario[i]);
  // }
  // int n = 3;
  
  // printf("ALTURA CON %d NODOS: %d\n", n, get_height(n));


  return 0;
}
