using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;

namespace LAB_4_SOLAE_LLT_SEMAPHORS_CS
{
    public class Program
    {
        private static Semaphore Semaphore = new Semaphore(1, 1);

        private static void Main(string[] args)
        {
            LaunchMenu();
        }

        private static void LaunchMenu()
        {
            var _isExit = false;
            var _selector = 0;
            var _menu_items_count = 2;

            while (!_isExit &&
                   (_selector <= 0 ||
                    _selector > _menu_items_count))
            {
                Console.WriteLine("Menu:");
                Console.WriteLine("1. Generate solaes files.");
                Console.WriteLine("2. Solve solaes from files.");
                Console.WriteLine("3. Exit.");
                Console.Write("Menu item: ");

                try
                {
                    _selector = int.Parse(Console.ReadLine());
                }
                catch (Exception ex) { }

                Console.WriteLine();

                switch (_selector)
                {
                    case 1:
                        GenerateSolaes();
                        break;
                    case 2:
                        SolveSolaes();
                        break;
                    case 3:
                        _isExit = true;
                        break;
                }

                _selector = 0;

                Console.WriteLine();
            }
        }

        private static void GenerateSolaes()
        {
            var _path = "./Resources/";
            Console.WriteLine("Generate solaes files...");
            Console.Write("Enter solaes count: ");
            var _filesCount = int.Parse(Console.ReadLine());
            Console.Write("Enter solaes size: ");
            var _size = int.Parse(Console.ReadLine());

            for (int i = 0; i < _filesCount; i++)
            {
                FileManager.WriteFile(new Solae(_size, i + 1).ToString(), $"{_path}Solae_{i + 1}.txt", true);
            }
        }

        private static void SolveSolaes()
        {
            var _path = "./Resources/";
            Console.WriteLine("Solve solaes...");
            Console.Write("Enter solaes count: ");
            var _solaesCount = int.Parse(Console.ReadLine());
            var _solaes = new List<Solae>(_solaesCount);

            for (int i = 0; i < _solaesCount; i++)
            {
                _solaes.Add(new Solae(FileManager.ReadFile($"{_path}Solae_{i + 1}.txt"), i + 1));
            }
            
            Console.Write("Enter threads count (must be > then solaes count): ");
            var _threadsCount = int.Parse(Console.ReadLine());
            var _solaesCountToThread = _solaesCount / _threadsCount;
            var _threads = new List<Thread>(_threadsCount);
            var _timer = new Stopwatch();

            _timer.Start();

            for (int i = 0; i < _threadsCount; i++)
            {
                _threads.Add(new Thread(new ParameterizedThreadStart(HandleThread)));

                if (i < _threadsCount - 1)
                {
                    _threads[i].Start(_solaes.GetRange(i * _solaesCountToThread, _solaesCountToThread));
                }
                else
                {
                    var _remains = _solaesCount % _threadsCount;
                    _threads[i].Start(_solaes.GetRange(i * _solaesCountToThread, _remains == 0 ? _solaesCountToThread : _solaesCountToThread + _remains));
                }
            }

            for (int i = 0; i < _threadsCount; i++)
            {
                _threads[i].Join();
            }

            _timer.Stop();

            Console.WriteLine($"Work is done in {_timer.ElapsedMilliseconds} ms!");
        }

        private static void HandleThread(object _solaesObj)
        {
            var _path = "./Resources/";
            var _solaes = _solaesObj as List<Solae>;
            var _decisions = new List<string>();

            for (int i = 0; i < _solaes.Count; i++)
            {
                _decisions.Add(_solaes[i].ToStringWithDecision());
            }

            for (int i = 0; i < _decisions.Count; i++)
            {
                FileManager.WriteFile(_decisions[i], $"{_path}Decision_{_solaes[i].Index}.txt", true);
            }

            Semaphore.WaitOne();
            for (int i = 0; i < _decisions.Count; i++)
            {
                Console.WriteLine(_decisions[i]);

                if (i == _solaes.Count - 1)
                {
                    Console.WriteLine($"Thread solved {_solaes.Count} solaes!");
                }

            }
            Semaphore.Release();
        }
    }
}
