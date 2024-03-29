#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"
#include <stdbool.h>

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
 long posicion = hash(key, map->capacity);

  if( *(map->buckets+posicion) == NULL){

    *(map->buckets+posicion) = createPair(key, value);
    
  }else{
    
    for(long i = 0, it = posicion+1; i < map->capacity; i++, it++){
      it = it%map->capacity;
      
      if(*(map->buckets+it) ==  NULL){
          
        *(map->buckets+it) = createPair(key, value);
        
        break;
        
      } else if((*(map->buckets+it))->key == NULL){
          
        *(map->buckets+it) = createPair(key, value);
        
        break;
      }
      
    }
    
  }
  
  map->size++;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)

    long new_capacity = map->capacity*2;
    Pair** new_buckets = (Pair**)calloc(new_capacity, sizeof(Pair*));
    for (long i = 0; i < map->capacity; i++) {
        if (*(map->buckets+i) != NULL && (*(map->buckets+i))->key != NULL) {
            long new_position = hash((*(map->buckets+i))->key, new_capacity);
            if (*(new_buckets+new_position) == NULL) {
                *(new_buckets+new_position) = *(map->buckets+i);
            } else {
                for (long j = 1; j < new_capacity; j++) {
                    long new_it = (new_position+j) % new_capacity;
                    if (*(new_buckets+new_it) == NULL) {
                        *(new_buckets+new_it) = *(map->buckets+i);
                        break;
                    }
                }
            }
        }
    }
    free(map->buckets);
    map->buckets = new_buckets;
    map->capacity = new_capacity;

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

   long pos = hash(key, map->capacity);

  if(is_equal(key, (*(map->buckets+pos))->key)){
    map->current =  pos;
    (*(map->buckets+pos))->key = NULL;
  } else {
    for(long i = 0, it = pos+1; i < map->capacity; i++, it++){
      it = it%map->capacity;

      if(*(map->buckets+it) ==  NULL) continue;
      
      if(is_equal(key, (*(map->buckets+it))->key)){
        map->current = it;
        (*(map->buckets+it))->key = NULL;
      }
    }   
  }

  if(map->size > 0) map->size--; 
  


}

Pair * searchMap(HashMap * map,  char * key) {   
    long posicion = hash(key, map->capacity);
    if (*(map->buckets+posicion) != NULL && (*(map->buckets+posicion))->key != NULL) {
        if (is_equal((*(map->buckets+posicion))->key, key)) {
            map->current = posicion;
            return *(map->buckets+posicion);
        } else {
            for (long i = 1; i < map->capacity; i++) {
                long it = (posicion+i) % map->capacity;
                if (*(map->buckets+it) == NULL || (*(map->buckets+it))->key == NULL) {
                    break;
                } else if (is_equal((*(map->buckets+it))->key, key)) {
                    map->current = it;
                    return *(map->buckets+it);
                }
            }
        }
    }
    return NULL;
}

Pair * firstMap(HashMap * map) {
  for(long it = 0; it < map->capacity; it++){

    if(*(map->buckets+it) ==  NULL) continue;

        if( (*(map->buckets+it))->key != NULL ){
        
        map->current = it;
      
        return *(map->buckets+it);
        }
  }
  return NULL;
}

Pair * nextMap(HashMap * map) {
    if (map->current == -1) {
        return NULL;
    }
      for (long i = map->current+1; i < map->capacity; i++) {
        if (*(map->buckets+i) != NULL && (*(map->buckets+i))->key != NULL) {
            map->current = i;
            return *(map->buckets+i);
          }
     }
    map->current = -1;
    return NULL;
}
