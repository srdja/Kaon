/*
 * collision.c
 *
 *  Created on: Oct 7, 2014
 *      Author: srdja
 */

#include <math.h>
#include <stdio.h>

#include "collision.h"

bool collision_AABB(Mesh *m1, Mesh *m2)
{
    // XXX ASSUMES BOTH MESHES ARE OF THE SAME SHAPE + THAT THEY ARE SQUARES

    float w = 118;//m1->bbox[4] - m1->bbox[0]; // FIXME WRONG
    float h = 118;//width;

    float m1_x = m1->bbox_transformed[0];
    float m1_y = m1->bbox_transformed[1];

    float m2_x = m2->bbox_transformed[0];
    float m2_y = m2->bbox_transformed[1];

   // fprintf(stdout, "width = %.1f height = %.1f\n", m2->bbox_transformed[0], m2->bbox_transformed[1]);
    //fprintf(stdout, "x1=%.1f x2=%.1f, y1=%.1f y2=%.1f\n", m1_x, m2_x, m1_y, m2_y);

  //  if (m1_x < m2_x + width && m1_x + width < m2_x)
  //      return false;

 //   if ()

    if (m1_x + w < m2_x)
        return false;

    if (m1_x > m2_x + w)
        return false;

    if (m1_y + h < m2_y)
        return false;

    if (m1_y > m2_y + h)
        return false;



    return true;
}
