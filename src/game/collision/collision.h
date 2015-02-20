/*
 * collision.h
 *
 *  Created on: Oct 7, 2014
 *      Author: srdja
 */

#ifndef COLLISION_H_
#define COLLISION_H_

#include <stdbool.h>
#include "../../client/graphics/mesh.h"

bool collision_AABB(Mesh *m1, Mesh *m2);

#endif /* COLLISION_H_ */
