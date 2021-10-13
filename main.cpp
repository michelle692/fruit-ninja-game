//=================================================================
// The main program file.
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

// External libs
#include <stdlib.h>

// Project includes
#include "globals.h"
#include "hardware.h"
#include "compost_pile_public.h"
#include "fruit_public.h"
#include "player_public.h"
#include "doubly_linked_list.h"

//For sound components
////AnalogOut DACout(p18);
//PwmOut pwmout(p26);
////PwmOut speaker(p25);

////wave_player waver(&DACout);
//SDFileSystem sd(p5, p6, p7, p8, "sd"); // mosi, miso, sck, cs 
DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);

// ===User implementations start===
int fruit_contact(void); // iterate through fruits and see if any collided
void compost_pile_update(void);

void playSound(char * wav);
void playNotes(void);
void set_random_seed(Timer);
void clear_screen(void);
void game_over(void);

int row = 0;
char *pF = "\033[0E%s\n";
int running = 1;
int score = 0;
int lives = 3;
int counter = 0;
int strikes = 7;

int main()
{
    GameInputs inputs; 
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    pc.printf(pF, "Program Starting");

    // ===User implementations start===
    // Game state variables
    
    // Timer to measure game update speed (secondarily used to generate random seed)
    Timer t;
    int dt; // delta time
    set_random_seed(t);
 
    //pwmout.period(1.0/4000.0);
    //playSound("/sd/wavfiles/BUZZER.wav");//test the sound convert to thread
    //wait(0.1);
    //playNotes();
    
    //initialize functions
    compost_pile_init();
    fruit_init();
    player_init();
           
    pc.printf(pF, "Initialization complete");
    while(1)
    {
        t.start();
        uLCD.locate(0, 0);
        uLCD.color(0x00FF00);
        uLCD.printf("PRESS ANY BUTTON \nTO START");
        inputs = read_inputs();
        if(inputs.b1 == 0 || inputs.b2 == 0 || inputs.b3 == 0)
        {
            break;
        }
        t.stop();
        dt = t.read_ms();
        
        if (dt < 100) wait_ms(100 - dt);
        
    }
    clear_screen();
    while(running)
    {
        t.start();

        // Draw fruits first
        //pc.printf("fruit_generator\n");
        fruit_generator();
        
        //Put score on screen
        // NOTE: Text is 8 pixels tall
        uLCD.locate(0, 0);
        uLCD.color(0xFF0000);
        uLCD.printf("%s %d", "SC:", score);
        
        uLCD.locate(50, 0);
        uLCD.color(0xFFFF00);
        uLCD.printf("%s %d", "LI:", lives);
        
        uLCD.locate(80, 0);
        uLCD.color(0xFFFFFF);
        uLCD.printf("%s %d", "ST:", strikes);

        //pc.printf("player_fruit_draw\n");
        //player_draw(0x0000FF);
        //player_knife_draw();
        
        pc.printf(pF,"Reading inputs");
        inputs = read_inputs();
        //This will pause the entire game when button 1 is pressed
        if (!inputs.b1) 
        {
            bool flag = true;
            clear_screen();
            while(1) 
            {
                t.start();
                uLCD.locate(0, 0);
                uLCD.color(0x00FF00);
                uLCD.printf("Game Paused");
                
                uLCD.locate(77, 2);
                uLCD.color(0xFFFFFF);
                uLCD.printf("%s %d", "Score:", score);
                uLCD.locate(77,4);
                uLCD.color(0xFFFFFF);
                uLCD.printf("%s %d", "Strikes:", strikes);
                uLCD.locate(77, 6);
                uLCD.color(0xFFFFFF);
                uLCD.printf("%s %d", "Lives:", lives);
                uLCD.locate(77, 8);
                uLCD.color(0xFFFFFF);
                uLCD.printf("Basic Mode");
                uLCD.locate(77, 10);
                
                
                inputs = read_inputs();
                if (inputs.b1 == 1)
                {
                    flag = false;
                }
                if (!inputs.b1 && !flag)
                {
                    break;
                }
                t.stop();
                dt = t.read_ms();
                
                if (dt < 100) wait_ms(100 - dt);
            }
            clear_screen();
        }
        pc.printf("\033[0E%s\n", " ");
    
        //pc.printf("player motions\n");
        if (inputs.ay >= PLAYER_MOVE_DOWN_GREATER) player_moveUp();            
        if (inputs.ay <= PLAYER_MOVE_UP_LESSTHAN) player_moveDown();                
        if (!inputs.b3) player_throw();
        player_draw(0x0000FF);
        player_knife_draw();
        PLAYER p = player_get_info();
        int before = p.player_has_knife;
        counter = p.counter;
        player_knife_return();
        if (before == 0 && p.player_has_knife == 1)
        {
            if (counter > 1)
            {
                score+=counter;
            }
        } 
        
        //pc.printf("compost_pile_update\n");
        draw_compost();
        compost_pile_update();
        fruit_contact();
        //pc.printf("fruit_contact\n");
        //int is_bomb = fruit_contact();
        
        // TODO: Check end of the game        
        //pc.printf("check endgame\n");
        if(get_compost_tallest_height() > SIZE_Y/2) 
        {
            game_over();
        }
        // TODO: Check if game won
        // Compute update time
        t.stop();
        dt = t.read_ms();
        
        if (dt < 100) wait_ms(100 - dt);
    }
    pc.printf("out of main loop\n");
    destroyList(get_fruit_list());
    
    return 0;
    // ===User implementations end===
}

// ===User implementations start===

/* This function iterate through the fruit list, checking if anyone one of them is contacting 
*  with the player's knife. Don't forget to check if the "fruit" is the bomb, since we 
*  generated bombs using the fruit_generator. 
*/ 
int fruit_contact(void) {
    PLAYER p = player_get_info();
    boundingBox k = p.knife_boundingBox;
    //0, player.y, player.height, player.y + player.height
    boundingBox t;
    t.topLeft.x = 0;
    t.topLeft.y = p.y;
    t.bottomRight.x = p.height;
    t.bottomRight.y = p.y + p.height;
    LLNode *currFruit = getHead(get_fruit_list());
    while(currFruit) 
    {
        FRUIT *fruit = static_cast < FRUIT * > (getData(currFruit));
        boundingBox f = fruit->box;
        if (k.bottomRight.x >= f.topLeft.x && k.bottomRight.x <= f.bottomRight.x)
        {
            if((k.topLeft.y >= f.topLeft.y && k.topLeft.y <= f.bottomRight.y) || (k.bottomRight.y >= f.topLeft.y && k.bottomRight.y <= f.bottomRight.y))
            {
                if(fruit->type == 0) 
                {
                    LLNode *temp = currFruit;
                    draw_nothing(f);
                    deleteNode(get_fruit_list(), temp);
                    lives--;
                    if (lives == 0)
                    {
                        game_over();
                    }
                }
                else
                {
                    p.counter+=1;
                    LLNode *temp = currFruit;
                    draw_nothing(f);
                    deleteNode(get_fruit_list(), temp);
                    score++;
                    //p.player_has_knife = 1;
                }
            }
        }
        if (t.bottomRight.x >= f.topLeft.x && t.bottomRight.x <= f.bottomRight.x)
        {
            if((t.topLeft.y >= f.topLeft.y && t.topLeft.y <= f.bottomRight.y) || (t.bottomRight.y >= f.topLeft.y && t.bottomRight.y <= f.bottomRight.y))
            {
                if(fruit->type == 0) 
                {
                    LLNode *temp = currFruit;
                    draw_nothing(f);
                    deleteNode(get_fruit_list(), temp);
                    lives--;
                    if (lives == 0) 
                    {
                        game_over();
                    }
                }
                else
                {
                    LLNode *temp = currFruit;
                    draw_nothing(f);
                    deleteNode(get_fruit_list(), temp);
                    lives--;
                    if (lives == 0)
                    {
                        game_over();
                    }
                }
            }
        }
        currFruit = getNext(currFruit);  
    }
    return 0;
}

/** Call compost_pile_update() repeatedly in your game-loop. ex: main()
    This function iterate through the fruit list. Add fruits that reach the bottom of the screen
    to the compost pile.
    @return  Number of remaining cities. You might end the game when all cities are demolished.
*/

void compost_pile_update(void){
    LLNode *currFruit = getHead(get_fruit_list());
    while(currFruit) 
    {
        FRUIT *f = static_cast < FRUIT * > (getData(currFruit));
        boundingBox box = f->box;
        //STRIKES
        if (box.topLeft.x >= 128)
        {
            if(f->type != 0)
            {
                strikes--;
                if (strikes == 0)
                {
                    game_over();
                }
            }
        } else if (box.bottomRight.x <= 0)
        {
            if(f->type != 0)
            {
                strikes--;
                if (strikes == 0)
                {
                    game_over();
                }
            }
        }
        int index = (int) (((box.topLeft.x)/12.0) + 0.5);
        if(index >= 11)
            index = 10;
        COMPOST c = compost_get_info(index);
        int h = c.y - c.height;
        COMPOST c2 = compost_get_info(index+1);
        int h2 = c2.y - c2.height;
        // pc.printf("\033[0E tl.y=%d br.y=%d c.h=%d i=%d\n", box.topLeft.y, box.bottomRight.y, h, index);
        if(box.bottomRight.y >= h)
        {
            LLNode *temp = currFruit;
            currFruit = getNext(currFruit);
            draw_nothing(box);
            deleteNode(get_fruit_list(), temp);
            if(f->type != 0)
            {
                compost_add(index);
            }
        }
        else if(box.bottomRight.y >= h2)
        {
            LLNode *temp = currFruit;
            currFruit = getNext(currFruit);
            draw_nothing(box);
            deleteNode(get_fruit_list(), temp);
            if(f->type != 0)
            {
                compost_add(index+1);
            }
        }
        else
            currFruit = getNext(currFruit);
    }
}

//fcn to play a wav
void playSound(char* wav)
{
    //open wav file
    FILE *wave_file;
    wave_file=fopen(wav,"r");
    
    if(wave_file != NULL) 
    {
        printf("File opened successfully\n");

        //play wav file
        printf("Sound playing...\n");
        waver.play(wave_file);
    
        //close wav file
        printf("Sound stopped...\n");
        fclose(wave_file);
        return;
    }
    
    printf("Could not open file for reading - %s\n", wav);
        return;
}

/* Wanna hear some annoying sounds?*/
void playNotes(void)
{
    int i;
// generate a 500Hz tone using PWM hardware output
    speaker.period(1.0/500.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(3);
    speaker=0.0; // turn off audio
    wait(2);
// generate a short 150Hz tone using PWM hardware output
// something like this can be used for a button click effect for feedback
    for (i=0; i<10; i++) {
        speaker.period(1.0/150.0); // 500hz period
        speaker =0.25; //25% duty cycle - mid range volume
        wait(.02);
        speaker=0.0; // turn off audio
        wait(0.5);
    }

// sweep up in frequency by changing the PWM period
    for (i=0; i<8000; i=i+100) {
        speaker.period(1.0/float(i));
        speaker=0.25;
        wait(.1);
    }
    wait(2);

// two tone police siren effect -  two periods or two frequencies
// increase volume - by changing the PWM duty cycle
    for (i=0; i<26; i=i+2) {
        speaker.period(1.0/969.0);
        speaker = float(i)/50.0;
        wait(.5);
        speaker.period(1.0/800.0);
        wait(.5);
    }
    // decrease volume
    for (i=25; i>=0; i=i-2) {
        speaker.period(1.0/969.0);
        speaker = float(i)/50.0;
        wait(.5);
        speaker.period(1.0/800.0);
        wait(.5);
    }
    speaker =0.0;
    wait(2);
}

void set_random_seed(Timer t) {
    GameInputs inputs; 
    t.start();
    uLCD.printf("Push any button to start.\n");
    while(1){
      inputs = read_inputs();
      if (inputs.b1 || inputs.b2 || inputs.b3) break;
      }
    uLCD.cls();
    t.stop();
    int seed = t.read_ms();    
    srand(seed);
    }
void clear_screen(void) {
    boundingBox box;
    box.topLeft.x = 0;
    box.topLeft.y = 0;
    box.bottomRight.x = SIZE_X;
    box.bottomRight.y = SIZE_Y;
    draw_nothing(box);
}
void game_over(void) {
    clear_screen();
    uLCD.locate(0, 0);
    uLCD.color(0xFF0000);
    uLCD.printf("GAME OVER");
    uLCD.locate(75, 2);
    uLCD.color(0xFFFFFF);
    uLCD.printf("%s %d", "Combos:", counter);
    uLCD.locate(75, 4);
    uLCD.color(0xFFFFFF);
    uLCD.printf("%s %d", "Final Score:", score);
    running = 0;
}
// ===User implementations end===