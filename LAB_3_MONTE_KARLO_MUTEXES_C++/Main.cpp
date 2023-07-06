#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <thread>
#include <time.h>
#include <mutex>
#include "Timer.h"

using namespace std;

void print_menu();
void launch_menu();
void calc_square();
void handle_thread(float _x1, float _x2, int _accuracy, float& _square, mutex &_door);
float square(float _x1, float _x2, int _accuracy);
float func(float _x);

int main()
{
    launch_menu();

    return EXIT_SUCCESS;
}

void print_menu()
{
    cout << "Menu:" << endl;
    cout << "1. Calculate square." << endl;
    cout << "2. Exit." << endl;
}

void launch_menu()
{
    bool _isExit = false;
    int _selector = 0;
    int _menu_items_count = 2;

    while (!_isExit &&
        (_selector <= 0 ||
            _selector > _menu_items_count))
    {
        print_menu();

        cout << "Menu item: ";
        try
        {
            cin >> _selector;
        }
        catch (exception ex) {}
        cout << endl;

        switch (_selector)
        {
            case 1:
                calc_square();
                break;
            case 2:
                _isExit = true;
                break;
        }

        _selector = 0;

        cout << endl;
    }
}

void calc_square()
{
    srand(time(NULL));

    int _accuracy = 0;
    int _threads_count = 0;
    float _x1 = 0.0f;
    float _x2 = 0.0f;
    float _square = 0.0f;
    string _func = "f(x) = x * x * x * exp(sin(x))";

    cout << "Calculate square by Monte-Carlo method..." << endl;
    cout << _func << endl;
    cout << "Enter interval [A;B] or [x1;x2]" << endl;
    cout << "x1: ";
    cin >> _x1;
    cout << "x2: ";
    cin >> _x2;
    cout << "Enter accuracy: ";
    cin >> _accuracy;
    cout << "Enter threads count: ";
    cin >> _threads_count;

    float _distance = _x2 - _x1;
    float _thread_distance = _distance / _threads_count;
    Timer _timer;
    mutex _door;
    vector<thread> _threads(_threads_count);

    _timer.start();

    for (size_t i = 0; i < _threads_count; i++)
    {
        _threads[i] = thread(handle_thread, _x1 + _thread_distance * i, _x1 + _thread_distance * (i + 1), _accuracy / _threads_count, ref(_square), ref(_door));
    }

    for (size_t i = 0; i < _threads_count; i++)
    {
        _threads[i].join();
    }

    _timer.stop();

    cout << "Square of function " << _func << " on intreval [" << _x1 << ";" << _x2 << "] = " << _square << endl;
    cout << _threads_count << " completed work in " << _timer.get_microseconds_delta() << " microseconds!" << endl;
}

void handle_thread(float _x1, float _x2, int _accuracy, float &_square, mutex &_door)
{
    _door.lock();
    _square += square(_x1, _x2, _accuracy);
    _door.unlock();
}

float square(float _x1, float _x2, int _accuracy)
{
    float _square = 0.0f;

    for (size_t i = 0; i < _accuracy; i++)
    {
        _square += fabs(func((float)rand() / RAND_MAX * (_x2 - _x1) + _x1) * (fabs(_x2 - _x1)));
    }

    _square /= _accuracy;

    return _square;
}

float func(float _x)
{
    return _x * _x * _x * exp(sin(_x));
}