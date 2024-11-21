#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

#define WIDTH 30
#define HEIGHT 15
#define EMPTY ' '
#define WALL '#'
#define SNAKE 'O'
#define FOOD '*'


typedef struct 
{
    int x;
    int y;
} Position;

typedef struct 
{
    Position segments[WIDTH * HEIGHT];
    int length;
    char direction;
} Snake;


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

void initialize_game(char map[HEIGHT][WIDTH], Snake *snake, Position *food) 
{
    for (int i = 0; i < HEIGHT; i++) 
    {
        for (int j = 0; j < WIDTH; j++) 
        {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) 
            {
                map[i][j] = WALL;
            } 
            else 
            {
                map[i][j] = EMPTY;
            }
        }
    }

    snake->length = 1;
    snake->segments[0].x = HEIGHT / 2;
    snake->segments[0].y = WIDTH / 2;
    snake->direction = 'd';

    map[snake->segments[0].x][snake->segments[0].y] = SNAKE;

    food->x = rand() % (HEIGHT - 2) + 1;
    food->y = rand() % (WIDTH - 2) + 1;
    map[food->x][food->y] = FOOD;
}

void print_map(char map[HEIGHT][WIDTH]) 
{
    for (int i = 0; i < HEIGHT; i++) 
    {
        for (int j = 0; j < WIDTH; j++) 
        {
            printf("%c", map[i][j]);
        }

        printf("\n");
    }
}

int move_snake(char map[HEIGHT][WIDTH], Snake *snake, Position *food) 
{
    Position next = snake->segments[0];

    switch (snake->direction) 
    {
        case 'w': next.x--; break;
        case 's': next.x++; break;
        case 'a': next.y--; break;
        case 'd': next.y++; break;
    }

    if (map[next.x][next.y] == WALL || map[next.x][next.y] == SNAKE) 
    {
        return 0; // break the game
    }

    if (map[next.x][next.y] == FOOD) 
    { 
        snake->length++;
        food->x = rand() % (HEIGHT - 2) + 1;
        food->y = rand() % (WIDTH - 2) + 1;
        map[food->x][food->y] = FOOD;
    } 
    else 
    {
        Position tail = snake->segments[snake->length - 1];
        map[tail.x][tail.y] = EMPTY;
    }

    for (int i = snake->length - 1; i > 0; i--) 
    {
        snake->segments[i] = snake->segments[i - 1];
    }

    snake->segments[0] = next;
    map[next.x][next.y] = SNAKE;

    return 1;
}

int main() 
{
    char map[HEIGHT][WIDTH];
    Snake snake;
    Position food;
    char input;

    srand(time(NULL));
    initialize_game(map, &snake, &food);

    enable_raw_mode();

    printf("Snake! Control: W/A/S/D. Press Q to leave.\n");

    while (1) 
    {
        system("clear");
        print_map(map);

        usleep(150000); // delay 150 ms

        if (read(STDIN_FILENO, &input, 1) > 0) 
        {
            if (input == 'q') break;

            if (input == 'w' || input == 'a' || input == 's' || input == 'd') 
            {
                snake.direction = input;
            }
        }

        if (!move_snake(map, &snake, &food)) 
        {
            printf("Game over.\n");
            break;
        }
    }

    disable_raw_mode();
    
    return 0;
}