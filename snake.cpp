#include <iostream>
#include <ncurses.h>
#include <unistd.h>

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

void Setup()
{
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);

    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void Draw()
{
    clear();

    for (int i = 0; i < width + 2; i++)
        mvprintw(0, i, "+");
    mvprintw(1, 0, "+");

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                mvprintw(i + 1, j, "+");
            if (i == y && j == x)
                mvprintw(i + 1, j + 1, "*");
            else if (i == fruitY && j == fruitX)
                mvprintw(i + 1, j + 1, "?");
            else
            {
                bool printTail = false;
                for (int k = 0; k < nTail; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        mvprintw(i + 1, j + 1, "*");
                        printTail = true;
                    }
                }
                if (!printTail)
                    mvprintw(i + 1, j + 1, " ");
            }

            if (j == width - 1)
                mvprintw(i + 1, j + 1, "+");
        }
    }

    mvprintw(height + 1, 0, "+");
    for (int i = 0; i < width + 2; i++)
        mvprintw(height + 1, i, "+");

    mvprintw(height + 3, 0, "Score: %d", score) ;

    refresh();
}

void Input()
{
    keypad(stdscr, TRUE);
    halfdelay(1);

    int c = getch();
    switch (c)
    {
        case KEY_LEFT:
        case 'a':
        case 'A':
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case KEY_RIGHT:
        case 'd':
        case 'D':
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case KEY_UP:
        case 'w':
        case 'W':
            if (dir != DOWN)
                dir = UP;
            break;
        case KEY_DOWN:
        case 's':
        case 'S':
            if (dir != UP)
                dir = DOWN;
            break;
        case 'x':
        case 'X':
            gameOver = true;
            break;
    }
}

void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir)
    {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }



    for (int i = 0; i < nTail; i++)
    {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    }

    if (x > width || x < 0 || y > height || y < 0)
        {
			gameOver = true;
		}
    if (x == fruitX && y == fruitY)
    {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}


int main()
{
    Setup();
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
        usleep(100000); // Sleep for 100 milliseconds (adjust as needed)
    }
    endwin();
    return 0;
}
