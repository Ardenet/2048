/*
 * File Name	: 2048
 * Description	: 2048小游戏
 * Author		: Ardenet
 * Date			: 2021.3.15
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

//游戏目标
#define MAX 199999
//屏幕大小
#define COLS 36
#define LINES 20

//
int E = 4;
//
enum color
{
	BLACK = 0,
	BLUE,
	GREEN,
	INDIGO,
	RED,
	PURPLE,
	YELLOW,
	WHITE,
	GREY,
	BRIGHT_BLUE,
	BRIGHT_GREEN,
	BRIGHT_INDIGO,
	BRIGHT_RED,
	BRIGHT_PUPLE,
	BRIGHT_YELLOW,
	BRIGHT_WHITE
};

//
typedef struct list
{
	int data;
	struct list *next;
} LIST;

typedef struct newnum
{
	int line;
	int col;
} NEWNUM;

//
void HideCursor(void)
{
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

//
void MoveCursor(int x, int y)
{
	COORD coord = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//
VOID SetColor(UINT uFore, UINT uBack)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, uFore + uBack * 0x10);
}

//
int CompareArray(int *s, int *t)
{
	for (int i = 0; i < E * E; i++)
	{
		if (s[i] != t[i])
			return 1;
	}
	return 0;
}

//
void CopyArray(int *s, int *t)
{
	for (int i = 0; i < E * E; i++)
		t[i] = s[i];
}

//
int BlankBlock(int *s, int a, int b)
{
	if (s[a * E + b] == 0)
		return 1;
	return 0;
}

//
void Print(int *s)
{
	int i, j, k;
	for (i = 0; i < E; i++)
	{
		//
		MoveCursor(E == 4 ? COLS / 2 - 10 : COLS / 2 - 13, 2 + 2 * i);
		for (k = 0; k < E; k++)
			printf("-----");
		MoveCursor(E == 4 ? COLS / 2 - 11 : COLS / 2 - 14, 3 + 2 * i);
		printf("|");
		for (j = 0; j < E; j++)
		{
			if (s[i * E + j] != 0)
				printf("%4d|", s[i * E + j]);
			else
				printf("    |");
		}
	}
	MoveCursor(E == 4 ? COLS / 2 - 10 : COLS / 2 - 13, 2 + 2 * i);
	for (i = 0; i < E; i++)
		printf("-----");
	MoveCursor(COLS / 2 - 10, LINES - 5);
	printf("W,S,A,D/上,下,左,右");
	MoveCursor(COLS / 2 - 6, LINES - 3);
	printf("<ESC>退出");
}

//
int Blank(int *s)
{
	int result = 0;
	for (int i = 0; i < E * E; i++)
	{
		if (s[i] == 0)
		{
			result += 1;
		}
	}
	return result;
}

//
int ScanHit(int *s)
{
	int result = 0, i = 0;
	char key;
	FILE *fp = NULL;
	key = _getch();
	switch (key)
	{
	case 27:
		system("cls");
		MoveCursor(COLS / 2 - 4, LINES / 2);
		printf("正在退出...");
		Sleep(500);
		if (s != NULL)
		{
			if (fopen_s(&fp, "saved", "w") != 0)
			{
				system("cls");
				MoveCursor(COLS / 2 - 4, LINES / 2);
				printf("存档保存失败...");
				Sleep(500);
				exit(1);
			}
			else
			{
				fprintf(fp, "%d ", E);
				for (i = 0; i < E * E; i++)
				{
					fprintf(fp, "%d ", s[i]);
				}
				if (fclose(fp))
					exit(1);
			}
		}
		exit(0);
	case 13:
		result = 9;
		break;
	case 'w':
		result = 1;
		break;
	case 's':
		result = 2;
		break;
	case 'a':
		result = 3;
		break;
	case 'd':
		result = 4;
		break;
	case -32:
		key = _getch();
		switch (key)
		{
		case 72:
			result = 1;
			break;
		case 80:
			result = 2;
			break;
		case 75:
			result = 3;
			break;
		case 77:
			result = 4;
		}
	}
	return result;
}

//
int Max(int *s)
{
	int result = s[0];
	for (int i = 1; i < E * E; i++)
	{
		if (s[i] >= result)
			result = s[i];
	}
	return result;
}

//
int SameNum(int *s)
{
	int i, j, result = 0;
	for (i = 0; i <= (E - 1); i++)
	{
		for (j = 1; j <= (E - 1); j++)
		{
			if ((s[i * E + j] == s[i * E + (j - 1)]) || (s[(j - 1) * E + i] == s[j * E + i]))
				result++;
		}
	}
	return result;
}

//
void Move(int *s, char fa, char am)
{
	int i, j, t, x, m = (am == 'a') ? 1 : (E - 2);
	for (i = 0; i < E; i++) //
	{
		for (t = 0; t < E; t++) //
		{
			for (j = m; (am == 'a') ? (j < E) : (j >= 0); j = (am == 'a') ? (j + 1) : (j - 1)) //
			{
				x = (am == 'a') ? (j - 1) : (j + 1);
				if (fa == 'a')
				{
					if (s[i * E + x] == 0 && s[i * E + j] != 0)
					{
						s[i * E + x] = s[i * E + j];
						s[i * E + j] = 0;
					}
				}
				else
				{
					if (s[x * E + i] == 0 && s[j * E + i] != 0)
					{
						s[x * E + i] = s[j * E + i];
						s[j * E + i] = 0;
					}
				}
			}
		}
	}
}

void Con(int *s, char fa, char am) //
{
	int i, j, x, m = (am == 'a') ? 1 : (E - 2);
	for (i = 0; i < E; i++) //
	{
		for (j = m; (am == 'a') ? (j < E) : (j >= 0); j = (am == 'a') ? (j + 1) : (j - 1)) //
		{
			x = (am == 'a') ? (j - 1) : (j + 1);
			if (fa == 'a')
			{
				if (s[i * E + x] == s[i * E + j])
				{
					s[i * E + x] *= 2;
					s[i * E + j] = 0;
				}
			}
			else
			{
				if (s[x * E + i] == s[j * E + i])
				{
					s[x * E + i] *= 2;
					s[j * E + i] = 0;
				}
			}
		}
	}
}

//
NEWNUM RandomNumBlock(int *s)
{
	int find;
	NEWNUM result = {2, 2};
	find = rand() % E;
	result.line = rand() % E;
	result.col = rand() % E;
	if (BlankBlock(s, result.line, result.col))
		s[result.line * E + result.col] = (find == result.line ? 4 : 2);
	else
		result = RandomNumBlock(s);
	return result;
}

void AllMove(int der, int *s) //
{
	char fa = '\0', am = '\0';
	switch (der)
	{
	case 1:
		fa = 'f';
		am = 'a';
		break;
	case 2:
		fa = 'f';
		am = 'm';
		break;
	case 3:
		fa = 'a';
		am = 'a';
		break;
	case 4:
		fa = 'a';
		am = 'm';
	}
	Move(s, fa, am);
	Con(s, fa, am);
	Move(s, fa, am);
}

int main(void)
{
	int der = 0, result = 0, i;
	int *table = NULL, *temp = NULL;
	LPRECT rect = (RECT *)calloc(1, sizeof(RECT)); //
	LPCWSTR title = L"2048";				   //
	LIST *head = (LIST *)malloc(sizeof(LIST));	   //
	NEWNUM newnum = {0, 0};
	FILE *fp = NULL;
	char str[64], frist[10][34] = {"******   ******   **  **   ******",
								   "******   ******   **  **   ******",
								   "    **   **  **   **  **   **  **",
								   "    **   **  **   **  **   **  **",
								   "******   **  **   ******   ******",
								   "******   **  **   ******   ******",
								   "**       **  **       **   **  **",
								   "**       **  **       **   **  **",
								   "******   ******       **   ******",
								   "******   ******       **   ******"};
	HideCursor();
	srand(time(NULL));
	sprintf_s(str, 64, "mode con cols=%d lines=%d", COLS, LINES); //
	system(str);												  //
	GetWindowRect(GetConsoleWindow(), rect);					  //
	//
	MoveWindow(GetConsoleWindow(), 700, 280, rect->right - rect->left, rect->bottom - rect->top, 0);
F:
	SetConsoleTitleW(title), SetColor(BRIGHT_INDIGO, BLACK);
	for (i = 0; i < 10; i++) //
	{
		MoveCursor(COLS / 2 - 17, 2 + i);
		puts(frist[i]);
	}
	SetColor(WHITE, BLACK);
	head->data = LINES - 5, head->next = (LIST *)malloc(sizeof(LIST)); //
	(head->next)->data = LINES - 3;
	head->next->next = (LIST *)malloc(sizeof(LIST));
	head->next->next->data = LINES - 1, head->next->next->next = head;
	MoveCursor(COLS / 2 - 3, LINES - 3);
	printf("5 x 5");
	MoveCursor(COLS / 2 - 3, LINES - 1);
	printf("读取存档");
	while (1)
	{ //
		MoveCursor(COLS / 2 - 5, head->data);
		printf(">");
		MoveCursor(COLS / 2 - 3, head->data);
		SetColor(BRIGHT_WHITE, GREY);
		switch (head->data)
		{
		case LINES - 5:
			printf("4 x 4");
			break;
		case LINES - 3:
			printf("5 x 5");
			break;
		case LINES - 1:
			printf("读取存档");
			break;
		}
		SetColor(WHITE, BLACK);
		result = ScanHit(table);
		if (result == 9)
			break;
		else if (result == 1 || result == 2)
		{
			for (i = 0; i < 2; i++)
			{
				MoveCursor(COLS / 2 - 5, head->data);
				printf(" ");
				MoveCursor(COLS / 2 - 3, head->data);
				switch (head->data)
				{
				case LINES - 5:
					printf("4 x 4");
					break;
				case LINES - 3:
					printf("5 x 5");
					break;
				case LINES - 1:
					printf("读取存档");
					break;
				}
			}
			if (result == 1)
				head = head->next->next;
			else
				head = head->next;
		}
	}
	system("cls");
	if (head->data == LINES - 1) //
	{
		if (fopen_s(&fp, "saved", "r") != 0)
		{
			MoveCursor(COLS / 2 - 7, LINES / 2);
			printf("暂无存档");
			Sleep(500);
			system("cls");
			goto F;
		}
	}
	else
		E = head->data == 15 ? 4 : 5;
	table = (int *)calloc(E * E, sizeof(int)), temp = (int *)calloc(E * E, sizeof(int));
	if (table == NULL)
	{
		MoveCursor(COLS / 2 - 4, LINES / 2);
		printf("生成失败");
		Sleep(500);
		exit(1);
	}
	if (head->data == LINES - 1)
	{
		fscanf_s(fp, "%d", &E);
		for (i = 0; i < E * E; i++)
			fscanf_s(fp, "%d", &table[i]);
		if (fclose(fp))
			exit(1);
		Print(table);
	}
	while (1)
	{
		if ((SameNum(table) == 0 && Blank(table) == 0) || Max(table) == MAX)
			break;
		if (Blank(table) != E * E)
		{
			der = ScanHit(table);	//
			CopyArray(table, temp); //
			if (der != 0)			//
			{
				AllMove(der, table);		   //
				if (CompareArray(table, temp)) //
					system("cls");
			}
		}
		if (CompareArray(table, temp) || Blank(table) == E * E)
		{
			Print(table);					//
			newnum = RandomNumBlock(table); //
			SetColor(BRIGHT_RED, BLACK);
			MoveCursor(E == 4 ? COLS / 2 - 10 + 5 * newnum.col : COLS / 2 - 13 + 5 * newnum.col, 3 + 2 * newnum.line);
			printf("%4d", table[newnum.line * E + newnum.col]);
			SetColor(WHITE, BLACK);
		}
	}
	if (Blank(table) == 0 && SameNum(table) == 0) //
	{
		MoveCursor(COLS / 2 - 11, LINES / 2 + 5);
		printf("You Failed!!!");
		remove("saved");
	}
	else if (Max(table) == MAX)
	{
		MoveCursor(COLS / 2 - 5, LINES / 2 + 5);
		printf("YOU WIN!!!!");
		remove("saved");
	}
	free(head), free(table), free(temp);
	_getch();
	return 0;
}
