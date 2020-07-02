#pragma once
#include "ncurses.h"

//various constants allowing easy configuration of the game state

//offset for the in-game level to be in the center of the created window
const int GAME_WINDOW_OFFSET_X = 1;
const int GAME_WINDOW_OFFSET_Y = 1;
//main menu dimensions
const int MENU_HEIGHT = 20;
const int MENU_WIDTH = 51;
//status window dimensions
const int STATUS_WINDOW_HEIGHT = 7;
const int STATUS_WINDOW_WIDTH = 32;

//main menu states
const int MENU_STATE_EXIT = -1;
const int MENU_STATE_MAIN = 0;
const int MENU_STATE_LEVELSELECT = 1;
const int MENU_STATE_RUNGAME = 2;

//keybinds for player 1
const int PLAYER_1_UP = 'w';
const int PLAYER_1_DOWN = 's';
const int PLAYER_1_LEFT = 'a';
const int PLAYER_1_RIGHT = 'd';
const int PLAYER_1_SETBOMB = 'v';
const int PLAYER_1_DETONATE = 'b';

//keybinds for player 2
const int PLAYER_2_UP = KEY_UP;
const int PLAYER_2_DOWN = KEY_DOWN;
const int PLAYER_2_LEFT = KEY_LEFT;
const int PLAYER_2_RIGHT = KEY_RIGHT;
const int PLAYER_2_SETBOMB = '/';
const int PLAYER_2_DETONATE = '.';

//game state constants
const int GAMEMODE_SINGLEPLAYER = 0;
const int GAMEMODE_MULTIPLAYER = 1;
const int GAME_SPEED = 50;      //how much delay(ms) between each game tick
const int AI_MOVEMENT_DELAY = 20; //in game ticks
const int BOMB_EXPLOSION_DELAY = 3; //in seconds
const int INVULNERABILITY_TIMER = 3;//in seconds
const int SCORE_DAMAGE_PENALTY = 30;
const int SCORE_KILL_BONUS = 100;

const int CONTINUE_GAME = 0;
const int PLAYER_1_DEAD = 1;
const int PLAYER_2_DEAD = 2;

//game loop return values
const int GAME_QUIT_BY_PLAYER = -1; //if player pressed the exit button
const int GAME_WIN = 5;             //if player won
const int GAME_CONTINUE = 0;        //if game still goes on
//debugging game state constants
const int GAME_QUIT = 'q';
const char ENABLE_REMOTE = 0;       //debugging variable

//master constant for turning powerups off completely
//as well as the calculations associated with them
const int DISABLE_ALL_POWERUPS = 0;

//chance of spawning a powerup upon breaking a block (in percentages)
const int POWERUP_SPAWN_CHANCE = 25;

//power-ups; change value to 0 to disable
const int POWERUP_DISABLED = 0;
//enabled = 1
const int POWERUP_HP = 1;
const int POWERUP_HP_STRENGTH = 1;
//enabled = 2
const int POWERUP_POW = 2;
const int POWERUP_POW_STRENGTH = 1;
//enabled = 3
const int POWERUP_MAX_BOMB_CNT = 3;
const int POWERUP_MAX_BOMB_CNT_STRENGTH = 1;
//enabled = 4
const int POWERUP_REMOTE = 4;

//display graphics constants
const char DISPLAY_BOMB = '!';
const char DISPLAY_NPC = 'E';
const char DISPLAY_PLAYER_1 = '@';
const char DISPLAY_PLAYER_2 = '&';

const char DISPLAY_TILE_EMPTY = ' ';
const char DISPLAY_TILE_BREAKABLE = '+';
const char DISPLAY_TILE_UNBREAKABLE = '#';

const char DISPLAY_TILE_POWERUP = '?';

const char DISPLAY_EXPLOSION = '%';

const char DISPLAY_PLAYER_SPAWN = 'X';
const char DISPLAY_NPC_SPAWN = 'O';

//attribute key constants
const int ATTR_NONEXISTENT = -1;
//PC keys
const int ATTR_HP = 0;
const int ATTR_POW = 1;
const int ATTR_BOMB_CNT = 2;
const int ATTR_MAX_BOMB_CNT = 3;
const int ATTR_REMOTE = 4;
const int ATTR_SCORE = 5;
//Bomb keys
const int ATTR_TIME_PLACED = 6;
const int ATTR_RADIUS = 7;
const int ATTR_SET_BY = 8;
const int ATTR_LAST_TAKEN_DAMAGE = 9;
//NPC keys
const int ATTR_DIRECTION = 12;
const int DIRECTION_UP = 0;
const int DIRECTION_DOWN = 1;
const int DIRECTION_LEFT = 2;
const int DIRECTION_RIGHT = 3;
//health +/- keys
const int ATTR_ONEUP = 10;
const int ATTR_DAMAGE = 11;
//Explosion contol values
const int ATTR_DETONATE = 20;
const int ATTR_NO_DETONATE = 21;