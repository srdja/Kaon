/*
 * map.h
 *
 *  Created on: Nov 13, 2014
 *      Author: srdja
 */

#ifndef MAP_H_
#define MAP_H_

#include <hashtable.h>

typedef struct map_info_s {
    int tiles_width;
    int tiles_height;

    /* Used to figure out object locations */
  //  int tile_w_pix;
 //   int tile_h_pix;


  //  int w;
  //  int h;

  //  int x;
  //  int y;


    HashTable *tile_info;
    int *data;

 //   Vector *tilelayers;
 //   Vector *objectlayers;
} MapInfo;

MapInfo *load_map();

#endif /* MAP_H_ */
