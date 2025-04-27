/**************************************************************
*                      ����������������                       *
* ----------------------------------------------------------  *
* Project Type: Win64 Console Application                     *
* Project Name: OOP_LAB1_LE                                   *
* File Name: OOP_LAB1_LE.cpp				                  *
* Programmers:���� �.�., ������� 4, ������ 209�               *
* Modified -                                                  *
* Created: 23.03.25                                           *
* Last Revision: 12.04.24                                     *
* Comment: ������������� ��������� ��� ��� �������� ��������  *
* ���                                                         *
***************************************************************/

#include <Windows.h>
#include <iostream>
#include "IDK.h"

#define KEY_DOWN(vk) (GetAsyncKeyState(vk) & 0x8000)

using namespace std;


int main() {
    setlocale(LC_ALL, "RUSSIAN");
    ClearScreen();
    int x = 0, y = 0;
    while (1)
    {
        
        cout << "�������� ��������: \n"
            << "1 - ������� ������� ����\n"
            << "2 - ������� ���� � ������\n"
            << "3 - ������� ���� � ����� ������� \n"
            << "4 - ������� ���� - ������� \n"
            << "5 - ������������ \n"
            << "6 - �������� ����� \n";
        int choice;
        cin >> choice;
        switch (choice) {
        case 1: {
            cout << "������� ���������� (x,y): \n";
            cin >> x >> y;
            Fish AFish(x, y);
            OperateWith(&AFish);
            
            break;
        }

        case 2: {
            cout << "������� ���������� (x,y): \n";
            cin >> x >> y;
            HatFish BFish(x, y);
            OperateWith(&BFish);
            
            break;
        }
        case 3: {
            cout << "������� ���������� (x,y): \n";
            cin >> x >> y;
            MutantFish CFish(x, y);
            OperateWith(&CFish);
            
            break;
        }
        case 4: {
            cout << "������� ���������� (x,y): \n";
            cin >> x >> y;
            SquareFish DFish(x, y);
            OperateWith(&DFish);
            
            break;
        }
        case 5: {
            demonstrate_collisions();
            break;
        }
        case 6: {
            ClearScreen();
            break;
        }
        default:
            break;
        }
    }
    return 0;
}