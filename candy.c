#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

void draw(int candyX, int candyY, int boxX, int boxY, int score, int lifeline);
void clearScreen();
void gotoxy(int x, int y);
int kbhit();
void sound(int frequency, int duration);
void gameOver();

int main() {
    int candyX = 10, candyY = 2;
    int boxX = 10, boxY = 20;
    int score = 0, lifeline = 3;
    int d = 0;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    timeout(0);

    srand(time(NULL));

    while (1) {
        clearScreen();
        draw(candyX, candyY, boxX, boxY, score, lifeline);

        if (kbhit()) {
            int ch = getch();
            switch (ch) {
                case 'a': if (boxX > 1) boxX--; break;
                case 'd': if (boxX < 70) boxX++; break;
                case 'x': endwin(); exit(0);
            }
        }

        candyY++;
        if (candyY == 20) {
            d = abs(candyX - boxX);
            if (d >= 0 && d <= 5) {
                score++;
            } else {
                lifeline--;
                sound(300, 100);
            }

            if (lifeline == 0) {
                gameOver();
            }

            candyY = 2;
            candyX = rand() % 70 + 1;
        }

        usleep(200000);
    }

    endwin();
    return 0;
}

void draw(int candyX, int candyY, int boxX, int boxY, int score, int lifeline) {

    gotoxy(1, 0);
    printw("Score = %d ", score);
    gotoxy(20, 0);
    printw("Lifeline = %d ", lifeline);
    gotoxy(40, 0);
    printw("A LEFT D RIGHT ");
    gotoxy(60, 0);
    printw("X-EXIT");


    for (int i = 0; i < 20; i++) {
        gotoxy(1, i + 2);
        printw("|");
        gotoxy(80, i + 2);
        printw("|");
    }
    for (int i = 0; i < 80; i++) {
        gotoxy(i + 1, 2);
        printw("-");
        gotoxy(i + 1, 22);
        printw("-");
    }
    gotoxy(candyX, candyY);
    printw("0");

    gotoxy(boxX, boxY);
    printw("#         #");
    gotoxy(boxX, boxY + 1);
    printw("###########");
}

void clearScreen() {
    clear();
}

void gotoxy(int x, int y) {
    move(y - 1, x - 1);
}

int kbhit() {
    int ch = getch();
    if (ch != ERR) {
        ungetch(ch);
        return 1;
    }
    return 0;
}

void sound(int frequency, int duration) {
    char cmd[256];
    sprintf(cmd, "beep -f %d -l %d", frequency, duration);
    system(cmd);
}

void gameOver() {
    clearScreen();
    gotoxy(35, 13);
    printw("GAME OVER");
    refresh();
    sleep(1);
    sound(300, 5000);
    endwin();
    exit(0);
}
