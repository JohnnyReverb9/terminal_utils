#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#define MAP_SIZE 10
#define PLAYER '@'
#define EXIT 'X'
#define EMPTY '.'
#define WALL '#'


typedef struct 
{
    int x;
    int y;
} Position;

// exclude standart input/output
void enable_raw_mode() 
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void disable_raw_mode() 
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void generate_map(char map[MAP_SIZE][MAP_SIZE], Position *player, Position *exit) 
{
    for (int i = 0; i < MAP_SIZE; i++) 
    {
        for (int j = 0; j < MAP_SIZE; j++) 
        {
            map[i][j] = (rand() % 5 == 0) ? WALL : EMPTY; // 20% wall chance
        }
    }

    // player place
    player->x = 0;
    player->y = 0;
    exit->x = MAP_SIZE - 1;
    exit->y = MAP_SIZE - 1;
    map[player->x][player->y] = PLAYER;
    map[exit->x][exit->y] = EXIT;
}

void print_map(char map[MAP_SIZE][MAP_SIZE]) 
{

    for (int i = 0; i < MAP_SIZE; i++) 
    {
        for (int j = 0; j < MAP_SIZE; j++) 
        {
            printf("%c ", map[i][j]);
        }

        printf("\n");
    }
}

int move_player(char map[MAP_SIZE][MAP_SIZE], Position *player, char direction) 
{
    int new_x = player->x;
    int new_y = player->y;

    switch (direction) 
    {
        case 'w': new_x--; break; // up
        case 's': new_x++; break; // down
        case 'a': new_y--; break; // left
        case 'd': new_y++; break; // right
        default: return 0; // invalid input
    }

    if (new_x >= 0 && new_x < MAP_SIZE && new_y >= 0 && new_y < MAP_SIZE && map[new_x][new_y] != WALL) 
    {
        map[player->x][player->y] = EMPTY;
        player->x = new_x;
        player->y = new_y;
        map[player->x][player->y] = PLAYER;
        return 1;
    }

    return 0; // can`t move
}

int main() 
{
    char map[MAP_SIZE][MAP_SIZE];
    Position player, exit;
    char input;

    srand(time(NULL));
    generate_map(map, &player, &exit);

    printf("Reach 'X'.\n");
    enable_raw_mode();

    while (1) 
    {
        system("clear");
        print_map(map);

        if (player.x == exit.x && player.y == exit.y) 
        {
            printf("WIN!\n");
            break;
        }

        printf("Your move (w/a/s/d): ");
        input = getchar();

        if (!move_player(map, &player, input)) 
        {
            printf("Wrong move!\n");
        }
    }

    disable_raw_mode();
    
    return 0;
}