#ifndef _video_data_h
#define _video_data_h

#include <stdint.h>

#define PPS 32

/* All possible field block colors in RGBa values. Ordered by: Phase => Color */
extern const uint8_t display_colors[3][9][4];

#endif
