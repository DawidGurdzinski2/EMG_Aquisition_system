#ifndef __IMAGES_H
#define __IMAGES_H

#include <stdint.h>

typedef struct IMG
{
  const uint16_t *array;
  uint8_t Width;
  uint8_t Height;

}IMG;

extern IMG gerwazy;



#endif /* __IMAGES_H */
