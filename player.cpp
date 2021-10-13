//=================================================================
// Implementation for the player module
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================
#include "player_private.h"

PLAYER player; // structure of player

PLAYER player_get_info(void){
    return player;
}

void player_init(void) {    
    player.y = PLAYER_INIT_Y;
    player.height = PLAYER_HEIGHT;
    player.score = 0;
    player.counter = 0;
    player.knife_length = KNIFE_LENGTH;
    player.player_has_knife = 1;
    player.difficulty = 0;
    
    player.knife_boundingBox.topLeft.x = 11;
    player.knife_boundingBox.topLeft.y = player.y - 2;
    player.knife_boundingBox.bottomRight.x = player.knife_length + 11;
    player.knife_boundingBox.bottomRight.y = 0;
}

void player_moveUp(void) {
    if (player.y - player.height >= 0)
    {
        player_draw(0x000000);
        player.y -= player.height;
    }
}

void player_moveDown(void) { 
    if (player.y + player.height <= SIZE_Y  - get_compost_tallest_height())
    {
        player_draw(0x000000);
        player.y += player.height;
    }
}

void player_throw(void) { 
    player.player_has_knife = 0;
    player.counter = 0;
}

void player_knife_draw(void) {
    if(!player.player_has_knife) 
    {
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.knife_boundingBox.topLeft.y, player.knife_boundingBox.bottomRight.x, player.knife_boundingBox.bottomRight.y, 0x000000);
        player.knife_boundingBox.topLeft.x += player.knife_length + player.knife_length;
        player.knife_boundingBox.bottomRight.x += player.knife_length + player.knife_length;
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.knife_boundingBox.topLeft.y, player.knife_boundingBox.bottomRight.x, player.knife_boundingBox.bottomRight.y, 0x0040FF);
    } 
}

void player_knife_return(void) {
    if (player.knife_boundingBox.bottomRight.x >= SIZE_X)
    {
        player.player_has_knife = 1;
    }
}

// ==== player_private.h implementation ====
void player_draw(int color) {
    if (player.player_has_knife)
    {
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.knife_boundingBox.topLeft.y, player.knife_boundingBox.bottomRight.x, player.knife_boundingBox.bottomRight.y, 0x000000);
        player.knife_boundingBox.topLeft.y = player.y;
        player.knife_boundingBox.bottomRight.y = (player.y + 3);
        player.knife_boundingBox.topLeft.x = 11;
        player.knife_boundingBox.bottomRight.x = player.knife_length + 11;
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.knife_boundingBox.topLeft.y, player.knife_boundingBox.bottomRight.x, player.knife_boundingBox.bottomRight.y, 0x0040FF);
    }
    uLCD.filled_rectangle(0, player.y, player.height, player.y + player.height, color);
    
}

void player_score_inc(void) {

}
