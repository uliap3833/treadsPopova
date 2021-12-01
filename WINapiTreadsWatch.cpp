#include <windows.h>
#include <stdio.h>
#include <conio.h>
#define BUFF "                                           "
CONSOLE_SCREEN_BUFFER_INFO cbsi;


DWORD WINAPI Delay(LPVOID);
DWORD WINAPI Button(LPVOID);
HANDLE RealWatch[2];
HANDLE Timer[2];
HANDLE Sec[2];
int x = 0, a = 1, b = 1;
int timeC[4] = { 10, 30, 30, 6 };
int posX = 0, posY = 8 ;

void main()
{
	system("chcp 1251>null");
	printf("Это часы.\nПри нажатии на букву Space часы будут поставленны на паузу.\nПри нажатии на Q программа завершит свою работу.\nПри нажатии на S можно настроить время.\nНа R секундомер.\nНа T таймер.\nДля начала работы нажмите Enter.\n");
	int c = _getch();// Q-113(233) ENTER-13  W-119(246) E-101(243) R-114(234) T-116(229)  space = 32 q = 113(233) s=115 T=116 R 114 enter = 13
	Sleep(1000);
	RealWatch[1] = CreateThread(NULL, 0, Button, timeC, 0, 0);
	RealWatch[0] = CreateThread(NULL, 0, Delay, timeC, 0, 0);
	WaitForMultipleObjects(2, RealWatch, TRUE, INFINITE);
}

COORD GetCursorP()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &cbsi);
	return cbsi.dwCursorPosition;
}

void InputChange(int a, int* hms)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (a)
	{
	case 1:
		printf("введите часы: ");
		posY = GetCursorP().Y;
		posX = GetCursorP().X;
		scanf_s("%d", hms);
		break;
	case 2:
		printf("введите минуты: ");
		posY = GetCursorP().Y;
		posX = GetCursorP().X;
		scanf_s("%d", hms);
		break;		
	case 3:
		printf("введите секунды: ");
		posY = GetCursorP().Y;
		posX = GetCursorP().X;
		scanf_s("%d", hms);
		break;
	case 4:
		posY = 8;
		posX = 0;
		SetConsoleCursorPosition(hConsole, (COORD) { posX, posY });
		printf("Пауза");
		break;
	case 0:
		posY = 8;
		posX = 0;		
		SetConsoleCursorPosition(hConsole, (COORD) { posX, posY });
		printf("%s\n%s\n%s\n%s\n%s\n%s\n", BUFF, BUFF, BUFF, BUFF, BUFF, BUFF);
		break;
	}
}

void AmountSec(int* h, int* m, int* s, int sec)
{
	*h = sec / 3600;
	if (*h < 1)
	{
		*h = 0;
	}
	*m = (sec / 60) - ((*h) * 60);
	if (*m < 1)
	{
		*m = 0;
	}
	*s = sec - (*m * 60 + *h * 3600);
	if (*s < 1)
	{
		*s = 0;
	}

}

DWORD WINAPI Button(LPVOID param)
{
	int* p = (int*)param;
	int x = 0;	
	if (p[3] == 7)
	{
		while (1)
		{
			int timeS[4] = { 0,0,0,8 };
			x = _getch();
			switch (x)
			{
			case 32:
				if (a == 0)
				{
					a = 1;
					ResumeThread(RealWatch[0]);
					InputChange(0, NULL);
				}
				else if (a == 1)
				{
					a = 0;
					SuspendThread(RealWatch[0]);
					InputChange(4, NULL);
				}
				break;
			case 113:
				TerminateThread(RealWatch[0], 0);
				ExitThread(0);
				break;
			case 101:
			case 243:
				InputChange(1, &timeS[0]);
				InputChange(2, &timeS[1]);
				InputChange(3, &timeS[2]);
				timeC[0] = timeS[0];
				timeC[1] = timeS[1];
				timeC[2] = timeS[2];
				InputChange(0, NULL);
				break;
			case 114:
				if (b == 1)
				{
					Timer[0] = CreateThread(NULL, 0, Delay, timeS, 0, 0);
					Timer[1] = CreateThread(NULL, 0, Button, timeS, 0, 0);
					WaitForMultipleObjects(2, Timer, TRUE, INFINITE);
				}				
				break;
			case 116:
				InputChange(1, &timeS[0]);
				InputChange(2, &timeS[1]);
				InputChange(3, &timeS[2]);
				InputChange(0, NULL);
				int sec = timeS[0] * 60 * 60 + timeS[1] * 60 + timeS[2];
				while (1)
				{
					AmountSec(&timeS[0], &timeS[1], &timeS[2], sec);
					HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
					COORD position = { 0, timeS[3] }, position1;
					SetConsoleCursorPosition(hConsole, position);
					printf(BUFF);
					SetConsoleCursorPosition(hConsole, position);
					printf("%02d %02d %02d\n", timeS[0], timeS[1], timeS[2]);
					if (sec > 0)
					{
						sec--;
					}
					Sleep(1000);
					if (timeS[0] == 0 && timeS[1] == 0 && timeS[2] == 0)
					{
						break;
					}
				}
				InputChange(0, NULL);
				break;

			case 13:
				break;
			}
		}
	}
	else if (p[3] == 8)
	{
		while (1)
		{
			x = _getch();
			switch (x)
			{
			case 114:
				if (b == 0)
				{
					b = -1;
					TerminateThread(Timer[0], 0);
				}
				else if (b == -1)
				{
					InputChange(0, NULL);
					TerminateThread(Timer[1], 0);
					b = 1;
				}
				break;
				case 13:
				break;
			}
		}

	}
}
DWORD WINAPI Delay(LPVOID param)
{
	int* p = (int*)param;
	while (1)
	{	
		if (p[2] < 60)
		{
			p[2]++;
		}
		if (p[1] < 60 && p[2] == 60)
		{
			p[2] = 0;
			p[1]++;
		}
		if (p[1] == 60)
		{
			p[0]++;
			p[1] = 0;
		}
		if (p[0]==24)
		{
			p[0] = 0;
			p[1] = 0;
			p[2] = 0;
		}
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD position = { 0,p[3] }, position1;
		SetConsoleCursorPosition(hConsole, position);
		printf(BUFF);
		SetConsoleCursorPosition(hConsole, position);
		printf("%02d %02d %02d\n", p[0], p[1], p[2]);

		position1 = (COORD){ posX, posY };
		SetConsoleCursorPosition(hConsole, position1);
		Sleep(1000);
		if (p[3]==8)
		{			
			b = 0;
		}				
	}
	ExitThread(0);

}



