/*
 * map.h
 *
 * Created: 1/7/2016 7:32:37 PM
 *  Author: Mihaela Stoycheva
 */ 

#ifndef MAP_H_
#define MAP_H_

#include <stdlib.h>
#include "blocked_priority_queue.h"

typedef struct {
	int id;
	BlockedPriorityQueue* queue;
} MapEntry;

typedef struct {
	int currentSize;
	int maximumSize;
	MapEntry **data;
} Map;

MapEntry* createMapEntry(int id, BlockedPriorityQueue* queue);
Map *createMap(unsigned int size);
void destroyMap(Map *map);
void addEntry(Map *map, MapEntry *element);
BlockedPriorityQueue* getEntry(Map* map, int id);
void deleteEntry(Map* map, int id);

#endif /* MAP_H_ */