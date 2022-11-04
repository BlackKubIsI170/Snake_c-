#include <iostream>
#include <ncurses.h>


struct tsnake{
	int n = 10, k = 300;
	int delta[2][2] = {{-1, 0}, {-1, 0}};
	int xy[300][2];
	char head = '#';
	char body = 'o';
};
struct tfood{
	int n = 5;
	int xy[100][2];
	char apple = 'e';
};
struct tfield{
	int X;
	int Y;
	char side = '@';
	char exit = 'q';
};

void game(tfield field, tsnake snake, tfood food);
void settings();
void draw_snake(tfield field, tsnake snake);
void move_snake(tfield &field, tsnake &snake, tfood &food, char ch);
void draw_food(tfood food);
void end(tfield field);


int main(){
	initscr();
	settings();

	tfield field;
	getmaxyx(curscr, field.Y, field.X);
	tsnake snake;
	// позиция змеи в начале игры
	for (int i = 0;i < snake.k;i++){
        snake.xy[i][0] = field.X / 2 + i;
        snake.xy[i][1] = field.Y / 2;
    }
	//
	tfood food;
	// заполняем кормушку
	for (int i = 0;i < food.n;i++){
        food.xy[i][0] = rand() % field.X;
        food.xy[i][1] = rand() % field.Y;
    }
	//

	game(field, snake, food);

    end(field);
	endwin();
	return 0;
}


void settings(){
	start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    keypad(stdscr, TRUE);
    halfdelay(2);
}

void game(tfield field, tsnake snake, tfood food){
	char ch = ' ';
	do{
		bkgd(COLOR_PAIR(1));
        getmaxyx(curscr, field.Y, field.X);
        draw_snake(field, snake);
        draw_food(food);
        refresh();
        for (int i = 0;i < snake.n;i++){
            for (int g = 0;g < snake.n;g++){
                if (i != g && snake.xy[i][0] == snake.xy[g][0] &&
                              snake.xy[i][1] == snake.xy[g][1]){
                    beep();
                    end(field);
                    return;
                }
            }
        }
        move_snake(field, snake, food, ch);
        ch = getch();
	} while(field.exit != ch);
}

void draw_snake(tfield field, tsnake snake){
	box(stdscr, field.side, field.side);
    mvaddch(snake.xy[0][1], snake.xy[0][0], snake.head);
    mvaddch(snake.xy[1][1], snake.xy[1][0], snake.body);
    for (int i = 2;i < snake.n;i++){
        mvaddch(snake.xy[i][1], snake.xy[i][0], snake.body);
    }
}

void draw_food(tfood food){
	for (int i = 0;i < food.n;i++){
        mvaddch(food.xy[i][1], food.xy[i][0], food.apple);
    }
}

void move_snake(tfield &field, tsnake &snake, tfood &food, char ch){
    snake.delta[1][1] = snake.delta[0][1];
    snake.delta[1][0] = snake.delta[0][0];
    if (ch == 2){
        snake.delta[0][1] = 1;
        snake.delta[0][0] = 0;
    }
    else if (ch == 3){
        snake.delta[0][1] = -1;
        snake.delta[0][0] = 0;
    }
    else if (ch == 4){
        snake.delta[0][0] = -1;
        snake.delta[0][1] = 0;
    }
    else if (ch == 5){
        snake.delta[0][0] = 1;
        snake.delta[0][1] = 0;
    }
    mvaddch(snake.xy[snake.n - 1][1], snake.xy[snake.n - 1][0], ' ');
    for (int i = snake.n - 1;i >= 0;i--){
        for (int g = 0;g < food.n;g++){
            if (snake.xy[i][0] == food.xy[g][0] && snake.xy[i][1] == food.xy[g][1]){
                snake.n++;
                food.xy[g][0] = rand() % field.X;
                food.xy[g][1] = rand() % field.Y;
            }
        }
    }
    for (int i = snake.n - 1;i >= 0;i--){
        if (i == 0){
            snake.xy[i][0] += snake.delta[i][0];
            snake.xy[i][1] += snake.delta[i][1];
        }
        else{
            snake.xy[i][0] = snake.xy[i - 1][0];
            snake.xy[i][1] = snake.xy[i - 1][1];
        }
        if (snake.xy[i][0] >= field.X) snake.xy[i][0] = 0;
        else if (snake.xy[i][0] <= 0) snake.xy[i][0] = field.X;
        if (snake.xy[i][1] >= field.Y) snake.xy[i][1] = 0;
        else if (snake.xy[i][1] <= 0) snake.xy[i][1] = field.Y;
    }
}

void end(tfield field){
	clear();
    mvprintw(field.Y / 2, field.X / 2 - 3, "The END");
    refresh();
    char ch = ' ';
    while (field.exit != ch) ch = getch();
}