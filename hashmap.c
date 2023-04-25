#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  
  long hash_value = hash(key, map->capacity);
  node* curr_node = *(map->buckets + hash_value);
  Node* prev_node = NULL;

  while(curr_node != NULL){
    if(strcmp(curr_node->pair->key,key)== 0){
      curr_node->pair->value = value;
      return;
    }
    prev_node = curr_node;
    curr_node = curr_node->next;
    
  }

  Pair* new_pair = createPair(key,value);
  Node* new_node = createNode(new_pair);
  if (prev_node==NULL){
    *(map->buckets + hash_value) = new_node;
  }else{
    prev_node->next = new_node;
  }
  map->size++;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
  
  HashMap* new = (HashMap*) calloc(1, sizeof(HashMap));

  new->buckets = (Pair**) calloc(capacity, sizeof(Pair));

  new->current = -1;
  new->size = 0;
  new->capacity = capacity;

  return new;

  
}

void eraseMap(HashMap * map,  char * key) {    


}

Pair * searchMap(HashMap * map,  char * key) {   


    return NULL;
}

Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
