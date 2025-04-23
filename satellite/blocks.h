#ifndef __UTIL_H__
#define __UTIL_H__

typedef enum {
  Small = 0, // 3x3x3
  Big = 1,   // 6x6x6
} BlockSize;

typedef enum {
  Green = 0,
  Red = 1,
  Blue = 2,
  Black = 3,
} BlockColor;

typedef struct {
  BlockSize size;
  BlockColor color;
} BlockInformation;

#endif

