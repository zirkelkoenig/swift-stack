/*
Copyright (C) 2017 Marvin Kipping

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; version 2 only.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <stdlib.h>
#include "static_data.h"
#include "dbg.h"

const int x_rotations[7][4][4] = {
	{ // red
		{ 2, 1, 0, -1 },
		{ -2, -1, 0, 1 },
		{ 2, 1, 0, -1 },
		{ -2, -1, 0, 1 }
	}, { // orange
		{ 1, 1, -1, -1 },
		{ -1, 0, 2, 1, },
		{ 1, 1, -1, -1 },
		{ -1, -2, 0, 1 }
	}, { // blue
		{ -2, 1, 0, -1 },
		{ 0, 0, 1, -1 },
		{ 1, 0, -1, 2 },
		{ 1, -1, 0, 0 }
	}, { // yellow
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	}, { // pink
		{ 1, -1, 0, -2 },
		{ -1, 1, 0, 2 },
		{ 1, -1, 0, -2 },
		{ -1, 1, 0, 2 }
	}, { // turquoise
		{ 0, 0, 0, -1 },
		{ -1, 1, 1, 0 },
		{ 1, 0, 0, 0 },
		{ 0, -1, -1, 1 }
	}, { // green
		{ 0, -1, 2, 1 },
		{ 0, 1, -2, -1 },
		{ 0, -1, 2, 1 },
		{ 0, 1, -2, -1 }
	}
};

const int y_rotations[7][4][4] = {
	{ // red
		{ -2, -1, 0, 1 },
		{ 2, 1, 0, -1 },
		{ -2, -1, 0, 1 },
		{ 2, 1, 0, -1 }
	}, { // orange
		{ 0, 0, 1, 1 },
		{ 0, -1, -2, -1 },
		{ 0, 0, 1, 1 },
		{ 0, 1, 0, -1 }
	}, { // blue
		{ 0, -1, 0, 1 },
		{ 0, 0, -1, -1 },
		{ 0, 1, 2, 1 },
		{ 0, 0, -1, -1 }
	}, { // yellow
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	}, { // pink
		{ 0, 1, 0, 1 },
		{ 0, -1, 0, -1 },
		{ 0, 1, 0, 1 },
		{ 0, -1, 0, -1 }
	}, { // turquoise
		{ 0, 0, 0, 1 },
		{ 0, -1, -1, -1 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 0, -1 }
	}, { // green
		{ 0, 1, 0, 1 },
		{ 0, -1, 0, -1 },
		{ 0, 1, 0, 1 },
		{ 0, -1, 0, -1 }
	}
};

const Piece init_pieces[7] = {
	{ RED, NORTH, { 3, 4, 5, 6 }, { 19, 19, 19, 19 } },
	{ ORANGE, NORTH, { 3, 3, 4, 5 }, { 18, 19, 19, 19 } },
	{ BLUE, NORTH, { 5, 3, 4, 5 }, { 18, 19, 19, 19 } },
	{ YELLOW, NORTH, { 4, 5, 4, 5 }, { 18, 18, 19, 19 } },
	{ PINK, NORTH, { 3, 4, 4, 5 }, { 18, 18, 19, 19 } },
	{ TURQUOISE, NORTH, { 4, 3, 4, 5 }, { 18, 19, 19, 19 } },
	{ GREEN, NORTH, { 4, 5, 3, 4 }, { 18, 18, 19, 19 } }
};

const Timing level_timings[] = {
	{ 4, 25, 25, 14, 30, 40 },
	{ 6, 25, 25, 14, 30, 40 },
	{ 8, 25, 25, 14, 30, 40 },
	{ 10, 25, 25, 14, 30, 40 },
	{ 12, 25, 25, 14, 30, 40 },
	{ 16, 25, 25, 14, 30, 40 },
	{ 32, 25, 25, 14, 30, 40 },
	{ 48, 25, 25, 14, 30, 40 },
	{ 64, 25, 25, 14, 30, 40 },
	{ 80, 25, 25, 14, 30, 40 },
	{ 96, 25, 25, 14, 30, 40 },
	{ 112, 25, 25, 14, 30, 40 },
	{ 128, 25, 25, 14, 30, 40 },
	{ 144, 25, 25, 14, 30, 40 },
	{ 4, 25, 25, 14, 30, 40 },
	{ 32, 25, 25, 14, 30, 40 },
	{ 64, 25, 25, 14, 30, 40 },
	{ 96, 25, 25, 14, 30, 40 },
	{ 128, 25, 25, 14, 30, 40 },
	{ 160, 25, 25, 14, 30, 40 },
	{ 192, 25, 25, 14, 30, 40 },
	{ 224, 25, 25, 14, 30, 40 },
	{ 256, 25, 25, 14, 30, 40 },
	{ 512, 25, 25, 14, 30, 40 },
	{ 768, 25, 25, 14, 30, 40 },
	{ 1024, 25, 25, 14, 30, 40 },
	{ 1280, 25, 25, 14, 30, 40 },
	{ 1024, 25, 25, 14, 30, 40 },
	{ 768, 25, 25, 14, 30, 40 },
	{ 5120, 25, 25, 8, 30, 25 },
	{ 5120, 25, 16, 8, 30, 16 },
	{ 5120, 16, 12, 8, 30, 12 },
	{ 5120, 12, 6, 8, 30, 6 },
	{ 5120, 12, 6, 6, 30, 6 },
	{ 5120, 12, 6, 6, 17, 6 }
};

Timing *get_timings(int level)
{
	check(level >= 0 && level <= 999, "argument \"level\" not between 0 and 999");

	Timing *ret = malloc(sizeof(Timing));
	check_mem(ret);

	switch (level) {
	case 0:
		*ret = level_timings[0];
		break;
	case 30:
		*ret = level_timings[1];
		break;
	case 35:
		*ret = level_timings[2];
		break;
	case 40:
		*ret = level_timings[3];
		break;
	case 50:
		*ret = level_timings[4];
		break;
	case 60:
		*ret = level_timings[5];
		break;
	case 70:
		*ret = level_timings[6];
		break;
	case 80:
		*ret = level_timings[7];
		break;
	case 90:
		*ret = level_timings[8];
		break;
	case 100:
		*ret = level_timings[9];
		break;
	case 120:
		*ret = level_timings[10];
		break;
	case 140:
		*ret = level_timings[11];
		break;
	case 160:
		*ret = level_timings[12];
		break;
	case 170:
		*ret = level_timings[13];
		break;
	case 200:
		*ret = level_timings[14];
		break;
	case 220:
		*ret = level_timings[15];
		break;
	case 230:
		*ret = level_timings[16];
		break;
	case 233:
		*ret = level_timings[17];
		break;
	case 236:
		*ret = level_timings[18];
		break;
	case 239:
		*ret = level_timings[19];
		break;
	case 243:
		*ret = level_timings[20];
		break;
	case 247:
		*ret = level_timings[21];
		break;
	case 251:
		*ret = level_timings[22];
		break;
	case 300:
		*ret = level_timings[23];
		break;
	case 330:
		*ret = level_timings[24];
		break;
	case 360:
		*ret = level_timings[25];
		break;
	case 400:
		*ret = level_timings[26];
		break;
	case 420:
		*ret = level_timings[27];
		break;
	case 450:
		*ret = level_timings[28];
		break;
	case 500:
		*ret = level_timings[29];
		break;
	case 601:
		*ret = level_timings[30];
		break;
	case 701:
		*ret = level_timings[31];
		break;
	case 801:
		*ret = level_timings[32];
		break;
	case 900:
		*ret = level_timings[33];
		break;
	case 901:
		*ret = level_timings[34];
		break;
	default:
		free(ret);
		ret = NULL;
	}

	return ret;

error:
	if (ret) {
		free(ret);
	}
	return NULL;
}
