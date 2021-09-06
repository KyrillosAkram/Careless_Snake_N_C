/*
 ============================================================================
 Name        : my_game_snake.c
 Author      : Kyrillos Akram
 Version     :
 Copyright   : void
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

//ToDo: solve tail algorithm issue
//ToDo: less hardcoded
//ToDo: customizable level by user

#define speed_easy 250
#define ground_row 20
#define ground_col 20
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <Windows.h>
//#include <cursor.h>
#include <conio.h>
//#define console_clear() printf("\e[1;1H\e[2J");
//#define console_clear() clrscr();
#define console_clear() system("cls");
#define msleep(x) Sleep(x);
#else
#include <unistd.h>
#define console_clear() printf("\e[1;1H\e[2J");
//#define console_clear() system("clear")
#define msleep(x) usleep(x*1000);
#endif

typedef struct{
	char row;
	char col;
}part;

void render_screen(void);
char snake_action(char kb_last_pressed,char ground[ground_row][ground_col]);
void ground_init(void);
void find_new_tail_and_set();
void dollar_rand_set();
unsigned char check_uturn() ;

char ground[ground_row][ground_col];
part head={10,10},tail={10,7};
char kb_last_pressed='d',head_direction='>';
unsigned short score=0;
char _[]="------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------";

int main(void) {
	srand(time(0));
	ground_init();
	ground[10][10]='>';
	ground[10][ 9]=ground[10][ 8]=ground[10][ 7]='O';
	dollar_rand_set();
	for(;;){
		printf("Score:%hu\n",score);
		msleep(speed_easy);
        if(kbhit())kb_last_pressed=getch();
		if(kb_last_pressed=='q')break;
		if(snake_action(kb_last_pressed,ground)){
			puts("Game Over !!!\a");
			break;
		}
		render_screen();
	}

	return EXIT_SUCCESS;
}


void render_screen(void){
	console_clear();
	//printf("%c",'+');
	//for(unsigned char i=0;i<ground_row;i++)printf("%c",'-');
	//printf("%c\n",'+');
	printf("+%.*s+\n",ground_col,_);
	for(unsigned char i=0;i<ground_row;i++){
		printf("|");
		for(unsigned char j=0;j<ground_row;j++){
			printf("%c",ground[i][j]);
		}
		printf("|\n");
	}
	/*
	printf("%c",'+');
	for(unsigned char i=0;i<ground_row;i++)printf("%c",'-');
	printf("%c\n",'+');
	*/
	printf("+%.*s+\n",ground_col,_);
	return;
}

char snake_action(char kb_last_pressed,char ground[ground_row][ground_col]){
	char crash=0;
	switch(kb_last_pressed){
		case 'w':
		case 'W':
			head_direction='^';
			if( ground[head.row-1][head.col]==' '&&head.row-1>-1){
				ground[head.row--][head.col] ='O';
				ground[head.row  ][head.col] =head_direction;
				ground[tail.row  ][tail.col] =' ';
				find_new_tail_and_set();
			}else if(ground[head.row-1][head.col]=='$'){
				ground[head.row][head.col] ='O';
				ground[--head.row][head.col] =head_direction;
				score++;
				dollar_rand_set();
			}else{
				crash=1;
			}
			break;

		case 's':
		case 'S':
			head_direction='v';
			if( ground[head.row+1][head.col]==' '&&head.row+1<ground_row){
				ground[head.row++][head.col] ='O';
				ground[head.row  ][head.col] =head_direction;
				ground[tail.row  ][tail.col] =' ';
				find_new_tail_and_set();
			}else if(ground[head.row+1][head.col]=='$'){
				ground[head.row][head.col] ='O';
				ground[++head.row][head.col] =head_direction;
				score++;
				dollar_rand_set();
			}else{
				crash=1;
			}
			break;


		case 'd':
		case 'D':
			head_direction='>';
			if( ground[head.row][head.col+1]==' '&&head.col+1<ground_col){
				ground[head.row][head.col++] ='O';
				ground[head.row][head.col  ] =head_direction;
				ground[tail.row][tail.col  ] =' ';
				find_new_tail_and_set();
			}else if(ground[head.row][head.col+1]=='$'){
				ground[head.row][head.col] ='O';
				ground[head.row][++head.col] =head_direction;
				score++;
				dollar_rand_set();
			}else{
				crash=1;
			}
			break;

		case 'a':
		case 'A':
			head_direction='<';
			if( ground[head.row][head.col-1]==' '&&head.col-1>-1){
				ground[head.row][head.col--] ='O';
				ground[head.row][head.col  ] =head_direction;
				ground[tail.row][tail.col  ] =' ';
				find_new_tail_and_set();
			}else if(ground[head.row][head.col-1]=='$'){
				ground[head.row][head.col] ='O';
				ground[head.row][--head.col] =head_direction;
				score++;
				dollar_rand_set();
			}else{
				crash=1;
			}
			break;


	}

	return crash;
}

void ground_init(void){
	for(unsigned char i=0;i<ground_row;i++){
		for(unsigned char j=0;j<ground_col;j++)ground[i][j]=' ';
	}
	return;
}

void find_new_tail_and_set(){
	unsigned char surronding_nodes=0;

	if(ground[tail.row][tail.col+1]=='O'){
		tail.col++;
	}else if(ground[tail.row][tail.col-1]=='O'){
		tail.col--;
	}else if(ground[tail.row+1][tail.col]=='O'){
		tail.row++;
	}else{ if(ground[tail.row-1][tail.col]=='O'){
		tail.row--;
	}else if(ground[tail.row-1][tail.col-1]=='O'){
		tail.row--;
		tail.col--;
	}else if(ground[tail.row+1][tail.col+1]=='O'){
		tail.row++;
		tail.col++;
	}else if(ground[tail.row+1][tail.col-1]=='O'){
		tail.row++;
		tail.col--;
	}else{
		tail.row--;
		tail.col++;
	}
	return;
}

void dollar_rand_set(){
	unsigned char x,y;
	do{
		x=rand()%ground_row;
		y=rand()%ground_col;
	}while(ground[x][y]=='O');
	ground[x][y]='$';
	return;
}

unsigned char check_uturn(){

	return 0;
}
