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

int PPM = 40; //Pixels Per Move
bool EXIT_TASK = false; //to break infinite loops

int main() {
    setlocale(LC_ALL, "RUSSIAN");
    ClearScreen();
    int x = 0, y = 0;
    while (1)
    {
        EXIT_TASK = false;
        cout << "Выберите действие: \n"
            << "1 - Создать базовую рыбу\n"
            << "2 - Создать рыбу с шляпой\n"
            << "3 - Создать рыбу с двумя глазами \n"
            << "4 - Создать рыбу - шар \n"
            << "5 - Очистить экран \n";
        int choice;
        cin >> choice;
        switch (choice) {
        case 1: {
            cout << "Введите координаты (x,y): \n";
            cin >> x >> y;
            Fish AFish(x, y);
            //OperateWith(&AFish);
            while (!EXIT_TASK) {
                cout << "Выберите действие над базовой рыбой: \n"
                    << "1 - Показать на экране\n"
                    << "2 - Скрыть\n"
                    << "3 - Перемещать\n"
                    << "Другая цифра - Вернуться на главную\n";
                cin >> choice;
                system("cls");
                switch (choice) {
                case 1: { AFish.Show();    break; }
                case 2: { AFish.Hide();    break; }
                case 3: { AFish.drag(PPM); break; }
                default: {
                    EXIT_TASK = true;
                    cout << "Возращаем...\n";
                    break;
                }
                }
            }
            break;
        }

        case 2: {
            cout << "Введите координаты (x,y): \n";
            cin >> x >> y;
            HatFish BFish(x, y);
            //OperateWith(&BFish);
            while (!EXIT_TASK) {
                cout << "Выберите действие над рыбой - патриотом: \n"
                    << "1 - Показать на экране\n"
                    << "2 - Скрыть\n"
                    << "3 - Перемещать\n"
                    << "Другая цифра - Вернуться на главную\n";
                cin >> choice;
                system("cls");
                switch (choice) {
                case 1: { BFish.Show();    break; }
                case 2: { BFish.Hide();    break; }
                case 3: { BFish.drag(PPM); break; }
                default: {
                    EXIT_TASK = true;
                    cout << "Возращаем...\n";
                    break;
                }
                }
            }
            break;
        }
        case 3: {
            cout << "Введите координаты (x,y): \n";
            cin >> x >> y;
            MutantFish CFish(x, y);
            //OperateWith(&CFish);
            while (!EXIT_TASK) {
                cout << "Выберите действие над диско-рыбой: \n"
                    << "1 - Показать на экране\n"
                    << "2 - Скрыть\n"
                    << "3 - Перемещать\n"
                    << "Другая цифра - Вернуться на главную\n";
                cin >> choice;
                system("cls");
                switch (choice) {
                case 1: { CFish.Show();    break; }
                case 2: { CFish.Hide();    break; }
                case 3: { CFish.drag(PPM); break; }
                default: {
                    EXIT_TASK = true;
                    cout << "Возращаем...\n";
                    break;
                }
                }
            }
            break;
        }
        case 4: {
            cout << "Введите координаты (x,y): \n";
            cin >> x >> y;
            CircleFish DFish(x, y);
            //OperateWith(&DFish);
            while (!EXIT_TASK) {
                cout << "Выберите действие над диско-рыбой: \n"
                    << "1 - Показать на экране\n"
                    << "2 - Скрыть\n"
                    << "3 - Перемещать\n"
                    << "Другая цифра - Вернуться на главную\n";
                cin >> choice;
                system("cls");
                switch (choice) {
                case 1: { DFish.Show();    break; }
                case 2: { DFish.Hide();    break; }
                case 3: { DFish.drag(PPM); break; }
                default: {
                    EXIT_TASK = true;
                    cout << "Возращаем...\n";
                    break;
                }
                }
            }
            break;
        }
        case 5: {
            ClearScreen();
            break;
        }
        default:
            break;
        }
    }
    return 0;
}