#include "video_data.h"

const uint8_t display_colors[3][9][4] = {
	{ // dropping
		{ 0xff, 0x00, 0x00, 0xff },                                                                
		{ 0xff, 0x77, 0x00, 0xff },                                                                
		{ 0x00, 0x00, 0xff, 0xff },                                                                
		{ 0xff, 0xff, 0x00, 0xff },                                                                
		{ 0xff, 0x00, 0xff, 0xff },                                                                
		{ 0x00, 0xff, 0xff, 0xff },                                                                
		{ 0x00, 0xff, 0x00, 0xff },                                                                
		{ 0x00, 0x00, 0x00, 0xff },                                                                
		{ 0xff, 0xff, 0xff, 0xff }
	}, { // locking
		{ 0xff, 0x77, 0x77, 0xff },                                                                                                
		{ 0xff, 0xbb, 0x77, 0xff },                                                                                                
		{ 0x77, 0x77, 0xff, 0xff },                                                                                                
		{ 0xff, 0xff, 0x77, 0xff },                                                                                                
		{ 0xff, 0x77, 0xff, 0xff },                                                                                                
		{ 0x77, 0xff, 0xff, 0xff },                                                                                                
		{ 0x77, 0xff, 0x77, 0xff },                                                                                                
		{ 0x00, 0x00, 0x00, 0xff },                                                                                                
		{ 0xff, 0xff, 0xff, 0xff }
	}, { // locked
		{ 0xbb, 0x00, 0x00, 0xff },                                                                                                
		{ 0xdd, 0x77, 0x00, 0xff },                                                                                                
		{ 0x00, 0x00, 0xbb, 0xff },                                                                                                
		{ 0xdd, 0xdd, 0x00, 0xff },                                                                                                
		{ 0xdd, 0x00, 0xdd, 0xff },                                                                                                
		{ 0x00, 0xdd, 0xdd, 0xff },                                                                                                
		{ 0x00, 0xdd, 0x00, 0xff },                                                                                                
		{ 0x00, 0x00, 0x00, 0xff },                                                                                                
		{ 0xff, 0xff, 0xff, 0xff }
	}
};
