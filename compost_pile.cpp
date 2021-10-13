//=================================================================
// The file is for module "compost pile"
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

#include "compost_pile_private.h"

COMPOST compost_record[NUM_PILE];
int tallest_pile_height;


// See the comments in compost_pile_public.h
void compost_pile_init() {
    for (int i = 0; i < NUM_PILE; i++)
    {
        compost_record[i].x = i*PILE_WIDTH;
        compost_record[i].y = 128;
        compost_record[i].width = PILE_WIDTH;
        compost_record[i].height = 0;
        tallest_pile_height = 64;
    }
}

COMPOST compost_get_info(int index){
    return compost_record[index];
}

void compost_add(int index) {
    compost_record[index].height = compost_record[index].height + 11;
}

void draw_compost(void){
    for (int i = 0; i < NUM_PILE; i++)
    {
        uLCD.filled_rectangle(compost_record[i].x, compost_record[i].y - compost_record[i].height, compost_record[i].x + compost_record[i].width, compost_record[i].y, 0x00FF00);
    }
}

int get_compost_tallest_height() {
    int max = compost_record[0].height;
    for (int i = 0; i < NUM_PILE; i++)
    {
        if (compost_record[i].height > max) 
        {
            max = compost_record[i].height;
        }
    }
    return max;
}

int get_compost_height(int index) {
    return compost_record[index].height;
}