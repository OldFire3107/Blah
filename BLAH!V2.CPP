/***IMPORTANT NOTICE BY THE DEVELOPER****
�                                       �
� This game is still under alpha stages �
� and the final product may not be the  �
� same.                                 �
�                                       �
� Blah! v2.0 Alpha Build                �
�                                       �
� Duplication of this is strictly       �
� prohibited without the permission     �
� of its creator Agni Purani.           �
�                                       �
� All Rights Resevered!                 �
� Copyright 2018                        �
�                                       �
****************************************/


///////All Header Files here///////////

#include <fstream.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

///////All Global Variables/////////

int GameOver = 0;

/////All User Defined Data Types////////

enum PType{};

struct Pos
{
	int x;
	int y;
}Apple, *Zombies;

struct PowerUps
{
	int Type;
	Pos PowerPos;
};

enum Type{CLASSIC = 0, UNLIMITED, BACK};

class HS   //FOR high score mangement
{
public:
	HS() {}
	~HS() {}

	int CalRankPos();

	void StoreScore(long, char[]);

protected:
	long Score;
	char Name[17];
};

class Game:public HS  //This class defines how the game will be(Easy...) ie Game Instance
{
public:
	int GetSpawnRate() const { return SpawnRate; }
	int GetColor1() const { return Color1; }
	int GetColor2() const { return Color2; }
	int GetScore() const { return Score; }

	void Draw();
	void PutName();
	void Movement();
	void Update();
	void Reset();
	void SaveScore();
	void ShareData(HS &h);

	Game() 
	{ 
		Color1 = 6;
		Color2 = 3; 
		SpawnRate = 5;
	}

	~Game() {}

private:
	int SpawnRate;
	int Color1;
	int Color2;
	long TempScore;
	long ZombieCount;
	int pos;

	Type GameType;
	Pos PlayerPos;

	void ZombieAI();

}BlahGameManger;

enum Movement{STOP = 0, Left, Right, Up, Down, Kill, Use} Move = STOP; //For movement of character

struct HighScore
{
	char name[17];
	unsigned int score;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                 All Function Prototype                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void Greet();
void Game(Type);

//Game functions
void SubGame();
void Draw();
void Input();
void Process();
void GameOverScreen(int);

//Menu fuctions
void MasterMenu();
int Menu();
Type SubMenu();
void SettingMenu();
void HelpMenu();
void HighScoreMenu(Type);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                            MAIN                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void main()
{
	randomize();
	clrscr();

	Greet();
	MasterMenu();

	textcolor(CYAN);
	gotoxy(30, 12);
	cprintf("Thanks for playing!");
	sleep(3);
	delete Zombies;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                      All Function Definetion                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void Greet()
{
	_setcursortype(_NOCURSOR);

	BlahGameManger.PutName();
}

void Game(Type GameType)
{
	clrscr();

	BlahGameManger.Reset();
	enum CauseOfDeath{Zombie = 0, Time} Game = Zombie;

	switch(GameType)
	{
	case CLASSIC:
		time_t Start;
		time_t Stop;

		Start = time(NULL);

		while(GameOver != 1)
		{
			SubGame();
			Stop = time(NULL);
			if(difftime(Stop, Start) > 120)
			{
				Game = Time;
				GameOver = 1;
			}
		}

		break;

	case UNLIMITED:
		while(GameOver != 1)
		{
			SubGame();
		}
		break;

	case BACK:
		return;

	default:
		textcolor(RED);
		gotoxy(22, 12);
		cprintf("ERROR 2: Game could not be intiated.");
	}

	GameOverScreen(Game);
}

//Game functions---------------------------------------------------------

void SubGame()
{
	Input();
	Process();
	Draw();
	delay(100);
}

void Draw()
{
	clrscr();

	textcolor(BlahGameManger.GetColor1());
	for(int i = 1; i <= 24; i++)
	{
		gotoxy(1, i);
		cprintf("�");
	}
	for(i = 1; i <= 24; i++)
	{
		gotoxy(80, i);
		cprintf("�");
	}
	for(i = 1; i <= 80; i++)
	{
		gotoxy(i, 1);
		cprintf("�");
	}
	for(i = 1; i <= 80; i++)
	{
		gotoxy(i, 24);
		cprintf("�");
	}

	BlahGameManger.Draw();

	textcolor(BlahGameManger.GetColor2());
	gotoxy(18, 25);
	cprintf("Your Score:%u", BlahGameManger.GetScore());
}

void Input()
{
	if(kbhit())
	{
		switch(getch())
		{
		case 72:
			Move = Up;
			break;

		case 80:
			Move = Down;
			break;

		case 75:
			Move = Left;
			break;

		case 77:
			Move = Right;
			break;

		case 'k':
			Move = Kill;
			break;

		case 'x':
			Move = STOP;
			break;

		case ' ':
			Move = Use;
			break;
		}
	}
}

void Process()
{
	BlahGameManger.Movement();

	BlahGameManger.Update();
}

void GameOverScreen(int Reason)
{
	clrscr();

	textcolor(RED);
	gotoxy(35, 10);
	cprintf("GAME OVER!");

	gotoxy(24, 11);

	switch(Reason)
	{
	case 0:
		cprintf("You Died! Better Luck Next time!");
		break;

	case 1:
		cprintf("So Sorry, You Ran Out of Time!!!");
		break;

	default:
		cprintf("ERROR 3: Cause of Death unknown!");
	}

	textcolor(BlahGameManger.GetColor2());
	gotoxy(40 - (3 + log10(BlahGameManger.GetScore() + 10)), 12);
	cprintf("SCORE: %u", BlahGameManger.GetScore());

	gotoxy(21, 14);
	cprintf("Would you like to save your score: (y/n)");

	char Choice;

	for( ; ; )
	{
		Choice = getch();
		if(Choice == 'n' || Choice == 'N')
		{
			break;
		}
		else if(Choice == 'y' || Choice == 'Y')
		{
			BlahGameManger.SaveScore();
			break;
		}
	}
}

//Menu functions----------------------------------------------------------
void MasterMenu()
{
	int GameExit = 0;

	do
	{
		switch(Menu())
		{
		case 1:
			Game(SubMenu());
			break;

		case 2:
			HelpMenu();
			break;

		case 3:
			HighScoreMenu(SubMenu());
			break;

		case 5:
			GameExit = 1;
			break;

		default:
			textcolor(RED);
			gotoxy(29, 12);
			cprintf("ERROR 1: Menu failure");
			sleep(3);
		}
	}while(GameExit == 0);

	clrscr();
}

int Menu()
{
	int Selection = 4;
	char c;

	for( ; ; )
	{
		clrscr();
		textcolor(CYAN);

		gotoxy(38, 5);
		cprintf("PLAY");
		gotoxy(38, 9);
		cprintf("HELP");
		gotoxy(35, 13);
		cprintf("HIGH SCORE");
		gotoxy(36, 17);
		cprintf("SETTINGS");
		gotoxy(38, 21);
		cprintf("EXIT");

		textcolor(6 + BLINK);
		gotoxy(33, Selection);
		cprintf("������������Ŀ");
		gotoxy(33, Selection + 1);
		cprintf("�");
		gotoxy(46, Selection + 1);
		cprintf("�");
		gotoxy(33, Selection + 2);
		cprintf("��������������");

		if(kbhit())
		{
			c = getch();

			if(c == 72)
				Selection -= 4;
			if(c == 80)
				Selection += 4;

			if(c == 13)
			{
				sound(500);
				delay(50);
				sound(700);
				delay(300);
				sound(450);
				delay(50);
				nosound();
				break;
			}

			if(Selection > 20)
				Selection = 4;
			if(Selection < 4)
				Selection = 20;
		}
	delay(15);
	}

	return (Selection/4);
}

Type SubMenu()
{
	Reselect:

	int Selection = 5;
	char c;

	for( ; ; )
	{
		clrscr();
		textcolor(BlahGameManger.GetColor2());

		gotoxy(37, 6);
		cprintf("CLASSIC!");
		gotoxy(36, 12);
		cprintf("UNLIMITED!");
		gotoxy(39, 18);
		cprintf("BACK");

		textcolor(BlahGameManger.GetColor1() + BLINK);
		gotoxy(34, Selection);
		cprintf("������������Ŀ");
		gotoxy(34, Selection + 1);
		cprintf("�");
		gotoxy(47, Selection + 1);
		cprintf("�");
		gotoxy(34, Selection + 2);
		cprintf("��������������");

		if(kbhit())
		{
			c = getch();

			if(c == 72)
				Selection -= 6;
			if(c == 80)
				Selection += 6;

			if(c == 13)
			{
				sound(500);
				delay(50);
				sound(700);
				delay(300);
				sound(450);
				delay(50);
				nosound();
				break;
			}

			if(Selection > 20)
				Selection = 5;
			if(Selection < 4)
				Selection = 17;
		}
	delay(15);
	}

	if(Selection == 5)
		return CLASSIC;
	else if(Selection == 11)
		return UNLIMITED;
	else if(Selection == 17)
		return BACK;
	else
		goto Reselect;
}

void SettingMenu()
{
	int Selection = 4;
	char c;

	for( ; ; )
	{
		clrscr();
		textcolor(BlahGameManger.GetColor2());

		gotoxy(38, 5);
		cprintf("DESIGN");
		gotoxy(38, 9);
		cprintf("DIFFCLULTY");

		textcolor(6 + BLINK);
		gotoxy(33, Selection);
		cprintf("������������Ŀ");
		gotoxy(33, Selection + 1);
		cprintf("�");
		gotoxy(46, Selection + 1);
		cprintf("�");
		gotoxy(33, Selection + 2);
		cprintf("��������������");

		if(kbhit())
		{
			c = getch();

			if(c == 72)
				Selection -= 4;
			if(c == 80)
				Selection += 4;

			if(c == 13)
			{
				sound(500);
				delay(50);
				sound(700);
				delay(300);
				sound(450);
				delay(50);
				nosound();
				break;
			}

			if(Selection > 20)
				Selection = 4;
			if(Selection < 4)
				Selection = 20;
		}
	delay(15);
	}
}

void HelpMenu()
{
	char exit;
	clrscr();
	textcolor(0);
	textbackground(15);
	for(int i = 0; i <= 80*25; i++)
		cprintf(" ");

	gotoxy(1, 2);
	cout << " Welcome to Blah! Your aim is to collect as many apples( ) and survive from zo- "
			" mbies( ) using arrow keys on your keyboard. Once you press a key you will keep "
			" moving in that direction. If you would like to stop your character you have to "
			" press your 'x' key on your keyboard. Your character can cross the screen ie.   "
			" move from one edge to another but zombies can't. Zombies can move diagonally   "
			" but you can. If you collect a power up you can use it by pressing the space    "
			" bar.";

	gotoxy(25, 15);
	textcolor(RED + BLINK);
	cprintf("\t\t\tPress return to continue\t\t\t");

	do
	{
		exit = getch();
	}while(exit != 13);

	textbackground(0);
}

void HighScoreMenu(Type GameType)
{

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                        Class Member functions                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void HS::StoreScore(long s, char n[])
{
	Score = s;
	strcpy(Name, n);
}

void Game::ShareData(HS &h)
{
	h.StoreScore(Score, Name);
}

void Game::SaveScore()
{
	HS Writter, Arranger;
	ShareData(Writter);


}

void Game::PutName()
{
	char Namebuff[18], t;
	int count = 0, f = 0;

	Namebuff[0] = '\0';

	do
	{
		clrscr();
		textcolor(6);
		gotoxy(14, 10);
		cprintf("Welcome to Blah! Please enter your name(max 16 char):");

		textcolor(CYAN);
		gotoxy(30, 12);
		cprintf("������������������Ŀ");
		gotoxy(30, 13);
		cprintf("�");
		gotoxy(49, 13);
		cprintf("�");
		gotoxy(30, 14);
		cprintf("��������������������");
		 
		gotoxy(40 - (count + 1) / 2, 13);
		textcolor(WHITE);
		cprintf("%s", Namebuff);

		if (count <= 16)
		{
			t = getch();

			if (t == 8 && count != 0)
			{
				if (f != 1)
				{
					count--;
				}
				else
				{
					f = 0;
				}
				Namebuff[count] = '\0';
			}
			else if (t == 8 || t == 13)
			{
				;
			}
			else if (f == 1)
			{
				count++;
			}
			else 
			{
				Namebuff[count++] = t;
				Namebuff[count] = '\0';
			}
		}
		else
		{
			gotoxy(11 ,24);
			textcolor(RED);
			cprintf("Name cannot exceed more than 16 characters. Press any key.");
			textcolor(WHITE);
			count--;
			f = 1;
			getch();
		}
		
	}while(t != 13 || count == 0);

	strcpy(Name, Namebuff);
}

void Game::Movement()
{
	switch(Move)
	{
	case Up:
		PlayerPos.y--;
		break;

	case Down:
		PlayerPos.y++;
		break;

	case Left:
		PlayerPos.x--;
		break;

	case Right:
		PlayerPos.x++;
		break;

	case STOP:
		break;

	case Kill:
		GameOver = 1;
		break;

	case Use:
		break;

	}

	if(PlayerPos.x > 79)
		PlayerPos.x = 2;
	if(PlayerPos.x < 2)
		PlayerPos.x = 79;
	if(PlayerPos.y > 23)
		PlayerPos.y = 2;
	if(PlayerPos.y < 2)
		PlayerPos.y = 23;
}

void Game::Update()
{
	TempScore = Score;


	if(PlayerPos.x == Apple.x && PlayerPos.y == Apple.y)
	{
		Score++;
		Apple.x = random(77) + 2;
		Apple.y = random(21) + 2;
	}

	if(Score % 5 == 0)
	{
		if(TempScore + 1 == Score)
		{
			ZombieCount++;
			Zombies[ZombieCount].x = random(77) + 2;
			Zombies[ZombieCount].y = random(21) + 2;
		}
	}

	ZombieAI();
}

void Game::Reset()
{
	PlayerPos.x = 40;
	PlayerPos.y = 12;
	Apple.x = random(77) + 2;
	Apple.y = random(21) + 2;
	Zombies[0].x = 2;
	Zombies[0].y = 2;
	ZombieCount = 0;
	GameOver = 0;
	Move = STOP;
	Score = 0;
}

void Game::Draw()
{
	textcolor(WHITE);
	gotoxy(PlayerPos.x, PlayerPos.y);
	cprintf("");

	textcolor(RED);
	gotoxy(Apple.x, Apple.y);
	cprintf("A");

	textcolor(GREEN);
	for(long i = 0; i < Score / SpawnRate + 1; i++)
	{
		gotoxy(Zombies[i].x, Zombies[i].y);
		cprintf("Z");
	}
}

void Game::ZombieAI()
{
	for(long i = 0; i < Score / SpawnRate + 1; i++)
	{
		if(Zombies[i].x < PlayerPos.x)
			Zombies[i].x++;
		if(Zombies[i].y < PlayerPos.y)
			Zombies[i].y++;
		if(Zombies[i].x > PlayerPos.x)
			Zombies[i].x--;
		if(Zombies[i].y > PlayerPos.y)
			Zombies[i].y--;

		if(Zombies[i].x == PlayerPos.x && Zombies[i].y == PlayerPos.y)
		{
			GameOver = 1;
		}
	}
}

/*************************************
�                                    �
� The end of program.                �
� Thanks for seeing.                 �
� Contact: agni.purani@gmail.com     �
�                                    �
� Files used in this program:        �
�	� HighScoreT.txt                �
�	� UserSettings.txt              �
�                                    �
*************************************/