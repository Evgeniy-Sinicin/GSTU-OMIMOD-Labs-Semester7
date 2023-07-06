#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "Point.h"
#include "Trapezoid.h"
#include "FileManager.h"
#include "StringManager.h""
#include "Timer.h"

using namespace std;

void print_menu();
void launch_menu();
void generate_points_file();
void write_to_file();
void read_from_file();
void calc_trapezoids();
void remove_trapezoids_file();
void handle_thread(int _thread_id, vector<bool>& _flags, vector<Point> _points);
int random_range(int _min, int _max);
vector<Trapezoid> find_trapezoids(vector<Point> _points);

const char* POINTS_PATH = "points.txt";
const char* TRAPEZOIDS_PATH = "trapezoids.txt";

int main()
{
    launch_menu();

    return EXIT_SUCCESS;
}

void print_menu()
{
    cout << "Menu:" << endl;
    cout << "1. Generate points file." << endl;
    cout << "2. Calculate trapezoids." << endl;
    cout << "3. Write to file." << endl;
    cout << "4. Read from file." << endl;
    cout << "5. Remove trapezoids file." << endl;
    cout << "6. Exit." << endl;
}

void launch_menu()
{
    bool _isExit = false;
    int _selector = 0;
    int _menu_items_count = 6;

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
        catch (exception ex) { }
        cout << endl;

        switch (_selector)
        {
            case 1:
                generate_points_file();
                break;
            case 2:
                calc_trapezoids();
                break;
            case 3:
                write_to_file();
                break;
            case 4:
                read_from_file();
                break;
            case 5:
                remove_trapezoids_file();
                break;
            case 6:
                _isExit = true;
                break;
        }

        _selector = 0;

        cout << endl;
    }
}

void remove_trapezoids_file()
{
    FileManager::writeToFile("", TRAPEZOIDS_PATH, true);
}

void generate_points_file()
{
    int _pointsCount;
    int _startDiapasonX = -100;
    int _startDiapasonY = -100;
    int _endDiapasonX = 100;
    int _endDiapasonY = 100;

    cout << "Generate points file... " << endl;

    /*
    cout << "Start diapason x:";
    cin >> _startDiapasonX;
    cout << "Start diapason y:";
    cin >> _startDiapasonY;
    cout << "End diapason x:";
    cin >> _endDiapasonX;
    cout << "End diapason y:";
    cin >> _endDiapasonY;
    */

    cout << "Enter points count: ";
    cin >> _pointsCount;

    string _data = "";
    //string _meta = std::to_string(_pointsCount) + "\n";
    vector<Point> _points(_pointsCount);

    for (size_t i = 0; i < _pointsCount; i++)
    {
        int _x = random_range(_startDiapasonX, _endDiapasonX + 1);
        int _y = random_range(_startDiapasonY, _endDiapasonY + 1);
        Point &_point = _points[i];

        _point.set(_x, _y);
        _data += _point.to_string();

        if (i < _pointsCount - 1)
        {
             _data += "\n";
        }

        cout << "Point " << _points[i].to_string() << " generated!" << endl;
    }

    FileManager::writeToFile(_data, POINTS_PATH, true);
}

void write_to_file()
{
    string _path;
    string _message;

    cout << "Write to file..." << endl;

    cout << "Path/File name: ";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(cin, _path);

    cout << "Press enter and input data... ";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(cin, _message);

    FileManager::writeToFile(_message, _path);

    cout << "File " << _path << " created!" << endl;
}

void read_from_file()
{
    string _path;
    string _data;

    cout << "Read from file..." << endl;
    cout << "Path/File name: ";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(cin, _path);

    _data = FileManager::readFromFile(_path);

    cout << "File data:" << endl << _data << endl;
}

void calc_trapezoids()
{
    cout << "Calculate trapezoids..." << endl;

    // Read points file
    string _data = FileManager::readFromFile(POINTS_PATH);
    vector<string> _pointLines = StringManager::split(_data, "\n");
    vector<Point> _points;

    for (size_t i = 0; i < _pointLines.size(); i++)
    {
        string _line = _pointLines[i];
        vector<string> _values = StringManager::split(_pointLines[i], " ");
        Point _point(stoi(_values[0]), stoi(_values[1]));
        _points.push_back(_point);

        //cout << "Point " << i + 1 << ". " << _point.to_string() << endl;
    }

    cout << "Total points count: " << _points.size() << endl;

    // Choose streams count
    int _threads_count = 0;

    while (_threads_count <= 0)
    {
        cout << "Enter threads count: ";
        cin >> _threads_count;
    }

    int _points_count_to_thread = _points.size() / _threads_count;
    cout << "Points count to thread: " << _points_count_to_thread << endl;

    vector<vector<Point>> _thread_point_vectors(_threads_count);

    for (size_t i = 0; i < _thread_point_vectors.size(); i++)
    {
        _thread_point_vectors[i] = vector<Point>(_points_count_to_thread);
        
        //cout << "Vector " << i + 1 << ":" << endl;

        for (size_t j = 0; j < _points_count_to_thread; j++)
        {
            _thread_point_vectors[i][j] = _points[i * _points_count_to_thread + j];

            //cout << j + 1 << ". " + _points[i * _points_count_to_stream + j].to_string() << endl;
        }
    }

    // Find trapezoids (in own stream)
    Timer _timer;
    vector<bool> _flags(_threads_count);
    vector<thread> _threads(_threads_count);

    _timer.start();

    for (int i = 0; i < _threads_count; i++)
    {
        _threads[i] = thread(handle_thread, i, std::ref(_flags), _thread_point_vectors[i]);
    }

    for (size_t i = 0; i < _threads_count; i++)
    {
        _threads[i].join();
    }

    _timer.stop();

    cout << "Work is done in " << _timer.get_microseconds_delta() << " microseconds!" << endl;
}

/// <summary>
/// Return value in range [min; max)
/// </summary>
int random_range(int _min, int _max)
{
    return _min + rand() % (_max - _min);
}

vector<Trapezoid> find_trapezoids(vector<Point> _points)
{
    vector<Trapezoid> _trapezoids;

    for (size_t i = 0; i < _points.size(); i++)
    {
        Point _a;
        Point _b;
        Point _c;
        Point _d;

        // Get A point
        _a = _points[i];
        //cout << "A: " << _a.to_string() << endl;

        // Get B point
        bool isFound = false;

        for (size_t j = i + 1; j < _points.size() && !isFound; j++)
        {
            _b = _points[j];

            if (_b.get_x() > _a.get_x() &&
                _b.get_y() > _a.get_y())
            {
                isFound = true;
            }
        }

        for (size_t j = 0; j < i && !isFound; j++)
        {
            _b = _points[j];

            if (_b.get_x() > _a.get_x() &&
                _b.get_y() > _a.get_y())
            {
                isFound = true;
            }
        }

        if (isFound)
        {
            //cout << "B: " << _b.to_string() << endl;
        }
        else
        {
            //cout << "B: Not found!" << endl;
            continue;
        }

        isFound = false;

        // Get C point
        for (size_t j = i + 1; j < _points.size() && !isFound; j++)
        {
            _c = _points[j];

            if (_c.get_x() > _b.get_x() &&
                _c.get_y() == _b.get_y())
            {
                isFound = true;
            }
        }

        for (size_t j = 0; j < i && !isFound; j++)
        {
            _c = _points[j];

            if (_c.get_x() > _b.get_x() &&
                _c.get_y() == _b.get_y())
            {
                isFound = true;
            }
        }

        if (isFound)
        {
            //cout << "C: " << _c.to_string() << endl;
        }
        else
        {
            //cout << "C: Not found!" << endl;
            continue;
        }

        isFound = false;

        // Get D point
        for (size_t j = i + 1; j < _points.size() && !isFound; j++)
        {
            _d = _points[j];

            if (_d.get_x() > _c.get_x() &&
                _d.get_y() < _c.get_y())
            {
                isFound = true;
            }
        }

        for (size_t j = 0; j < i && !isFound; j++)
        {
            _d = _points[j];

            if (_d.get_x() > _c.get_x() &&
                _d.get_y() < _c.get_y())
            {
                isFound = true;
            }
        }

        if (!isFound)
        {
            //cout << "D: Not found!" << endl;
            continue;
        }

        //cout << "D: " << _d.to_string() << endl;

        /*cout << "Trapezoid generated!" << endl;
        cout << "A: " << _a.to_string() << endl;
        cout << "B: " << _b.to_string() << endl;
        cout << "C: " << _c.to_string() << endl;
        cout << "D: " << _d.to_string() << endl;*/

        Trapezoid _trapeziod(_a, _b, _c, _d);
        _trapezoids.push_back(_trapeziod);
    }

    return _trapezoids;
}

void handle_thread(int _thread_id, vector<bool>& _flags, vector<Point> _points)
{
    // Do routine work
    vector<Trapezoid> _trapezoids = find_trapezoids(_points);

    // Synchronization
    bool _canEnter = false;

    while (!_canEnter)
    {
        for (size_t i = 0; i < _flags.size(); i++)
        {
            if (_flags[i] == true)
            {
                break;
            }

            if (i == _flags.size() - 1)
            {
                _canEnter = true;
            }
        }
    }

    _flags[_thread_id] = true;

    // Start work in critical section
    string _result = "Thread #" + std::to_string(_thread_id + 1) + "\n";

    //cout << "Thread #" << _thread_id + 1 << ":" << endl;
    //cout << std::to_string(_trapezoids.size()) << " trapezoids found!" << endl;

    for (size_t j = 0; j < _trapezoids.size(); j++)
    {
        //cout << j + 1 << ". " << _trapezoids[j].to_string() << endl;
        _result += std::to_string(j + 1) + ". " + _trapezoids[j].to_string() + "\n";
    }

    cout << _result << endl;

    FileManager::writeToFile(_result, TRAPEZOIDS_PATH);
    // End work in critical section

    _flags[_thread_id] = false;
}