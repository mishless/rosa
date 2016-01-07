/*
 * map.c
 *
 * Created: 1/7/2016 7:35:54 PM
 *  Author: Mihaela Stoycheva
 */ 

#include "map.h"

MapEntry* createMapEntry(int id, BlockedPriorityQueue* queue)
{
	MapEntry* entry = malloc(sizeof(MapEntry));
	entry->id = id;
	entry->queue = queue;
	return entry;
}

Map* createMap(unsigned int size)
{
	Map* map = malloc(sizeof(Map));
	map->data = malloc(size * sizeof(MapEntry));
	map->currentSize = 0;
	map->maximumSize = size;
	return map;
}

void destroyMap(Map *map)
{
	int i;
	for (i=0; i<map->currentSize; i++)
	{
		free(map->data[i]);
	}
	free(map->data);
	free(map);
}

void addEntry(Map *map, MapEntry *element)
{
	if (map->currentSize < map->maximumSize)
	{
		map->data[map->currentSize] = element;
		map->currentSize++;
	}
}

BlockedPriorityQueue* getBlockedPriorityQueue(Map* map, int id)
{
	int i;
	for (i=0; i<map->currentSize; i++)
	{
		if (map->data[i]->id == id)
		{
			return map->data[i]->queue;
		}
	}
	return NULL;
}

void deleteEntry(Map* map, int id)
{
	int i;
	for (i=0; i<map->currentSize; i++)
	{
		if (map->data[i]->id == id)
		{
			free(map->data[i]);
			map->data[i] = map->data[map->currentSize-1];
			map->currentSize--;
		}
	}
}
