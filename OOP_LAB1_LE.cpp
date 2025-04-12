#include <Windows.h>
#include <iostream>
#include "IDK.h"

#define KEY_DOWN(vk) (GetAsyncKeyState(vk) & 0x8000)

using namespace std;

int PPM = 40; //Pixels Per Move
bool EXIT_TASK = false; //to break infinite loops

void ClearScreen() {
	HWND hWnd = FindWindowA("ConsoleWindowClass", NULL);
	HDC hdc = GetDC(hWnd);
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc,hBrush);
	Rectangle(hdc, 0, 0, 1920, 1080);
	DeleteObject(hBrush);
}

void OperateWith(Fish* fish) {
    while (!EXIT_TASK) {
        cout << "�������� �������� ��� ������� �����: \n"
            << "1 - �������� �� ������\n"
            << "2 - ������\n"
            << "3 - ����������\n"
            << "������ ����� - ��������� �� �������\n";
        int choice;
        cin >> choice;
        system("cls");
        switch (choice) {
            case 1: { fish->Show();    break; }
            case 2: { fish->Hide();    break; }
            case 3: { fish->drag(PPM); break; }
            default: {
                EXIT_TASK = true;
                cout << "���������...\n";
                break;
            }
        }
    }
}

int main() {
    setlocale(LC_ALL, "RUSSIAN");
    HWND hWnd = FindWindowA("ConsoleWindowClass", NULL);
    HDC hdc = GetDC(hWnd);
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hBrush);
    Rectangle(hdc, 0, 0, 1920, 1080);
    int x = 0, y = 0;

    while (1)
    {
        EXIT_TASK = false;
        cout << "�������� ��������: \n"
            << "1 - ������� ������� ����\n"
            << "2 - ������� ���� � ������\n"
            << "3 - ������� ���� � ����� ������� \n"
            << "4 - ������� ���� - ��� \n"
            << "5 - �������� ����� \n";
        int choice;
        cin >> choice;
        switch (choice) {
        case 1: {
            cout << "������� ���������� (x,y): \n";
            cin >> x >> y;
            Fish AFish(x, y);
            OperateWith(&AFish);
            /*while (!EXIT_TASK) {
                cout << "�������� �������� ��� ������� �����: \n"
                    << "1 - �������� �� ������\n"
                    << "2 - ������\n"
                    << "3 - ����������\n"
                    << "������ ����� - ��������� �� �������\n";
                cin >> choice;
                system("cls");
                switch (choice) {
                case 1: { AFish.Show();    break; }
                case 2: { AFish.Hide();    break; }
                case 3: { AFish.drag(PPM); break; }
                default: {
                    EXIT_TASK = true;
                    cout << "���������...\n";
                    break;
                }
                }
            }*/
            break;
        }

        case 2: {
            cout << "������� ���������� (x,y): \n";
            cin >> x >> y;
            HatFish BFish(x, y);
            OperateWith(&BFish);
            /*while (!EXIT_TASK) {
                cout << "�������� �������� ��� ����� - ���������: \n"
                    << "1 - �������� �� ������\n"
                    << "2 - ������\n"
                    << "3 - ����������\n"
                    << "4 - ������������ �����\n"
                    << "5 - ��������� � ������� �����\n"
                    << "������ ����� - ��������� �� �������\n";
                cin >> choice;
                system("cls");
                switch (choice) {
                case 1: { BFish.Show();    break; }
                case 2: { BFish.Hide();    break; }
                case 3: { BFish.drag(PPM); break; }
                default: {
                    EXIT_TASK = true;
                    cout << "���������...\n";
                    break;
                }
                }
            }*/
            break;
        }
        case 3: {
            cout << "������� ���������� (x,y): \n";
            cin >> x >> y;
            MutantFish CFish(x, y);
            OperateWith(&CFish);
            /*while (!EXIT_TASK) {
                cout << "�������� �������� ��� �����-�����: \n"
                    << "1 - �������� �� ������\n"
                    << "2 - ������\n"
                    << "3 - ����������\n"
                    << "4 - ����� - �����\n"
                    << "������ ����� - ��������� �� �������\n";
                cin >> choice;
                system("cls");
                switch (choice) {
                case 1: { CFish.Show();    break; }
                case 2: { CFish.Hide();    break; }
                case 3: { CFish.drag(PPM); break; }
                default: {
                    EXIT_TASK = true;
                    cout << "���������...\n";
                    break;
                }
                }
            }*/
            break;
        }
        case 4: {
            cout << "������� ���������� (x,y): \n";
            cin >> x >> y;
            CircleFish DFish(x, y);
            OperateWith(&DFish);
            /*while (!EXIT_TASK) {
                cout << "�������� �������� ��� �����-�����: \n"
                    << "1 - �������� �� ������\n"
                    << "2 - ������\n"
                    << "3 - ����������\n"
                    << "4 - ����� - �����\n"
                    << "������ ����� - ��������� �� �������\n";
                cin >> choice;
                system("cls");
                switch (choice) {
                case 1: { DFish.Show();    break; }
                case 2: { DFish.Hide();    break; }
                case 3: { DFish.drag(PPM); break; }
                default: {
                    EXIT_TASK = true;
                    cout << "���������...\n";
                    break;
                }
                }
            }*/
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