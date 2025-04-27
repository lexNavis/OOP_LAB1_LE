/**************************************************************
*                      Программирование                       *
* ----------------------------------------------------------  *
* Project Type: Win64 Console Application                     *
* Project Name: OOP_LAB1_LE                                   *
* File Name: OOP_LAB1_LE.cpp				                  *
* Programmers:Шеин Д.К., Бригада 4, Группа 209Б               *
* Modified -                                                  *
* Created: 23.03.25                                           *
* Last Revision: 12.04.24                                     *
* Comment: Использование принципов ООП для создании иерархии  *
* рыб                                                         *
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
        
        cout << "Выберите действие: \n"
            << "1 - Создать базовую рыбу\n"
            << "2 - Создать рыбу с шляпой\n"
            << "3 - Создать рыбу с двумя глазами \n"
            << "4 - Создать рыбу - квадрат \n"
            << "5 - Столкновения \n"
            << "6 - Очистить экран \n";
        int choice;
        cin >> choice;
        switch (choice) {
        case 1: {
            cout << "Введите координаты (x,y): \n";
            cin >> x >> y;
            Fish AFish(x, y);
            OperateWith(&AFish);
            
            break;
        }

        case 2: {
            cout << "Введите координаты (x,y): \n";
            cin >> x >> y;
            HatFish BFish(x, y);
            OperateWith(&BFish);
            
            break;
        }
        case 3: {
            cout << "Введите координаты (x,y): \n";
            cin >> x >> y;
            MutantFish CFish(x, y);
            OperateWith(&CFish);
            
            break;
        }
        case 4: {
            cout << "Введите координаты (x,y): \n";
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