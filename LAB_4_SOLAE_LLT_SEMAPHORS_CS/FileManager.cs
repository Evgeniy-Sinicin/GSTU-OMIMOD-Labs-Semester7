using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace LAB_4_SOLAE_LLT_SEMAPHORS_CS
{
    public static class FileManager
    {
        public static void WriteFile(string _message, string _path, bool _recreate = false)
        {
            if (File.Exists(_path) &&
                !_recreate)
            {
                File.AppendAllText(_path, _message);
            }
            else
            {
                File.WriteAllText(_path, _message);
            }
        }

        public static string ReadFile(string _path)
        {
            if (File.Exists(_path))
            {
                return File.ReadAllText(_path);
            }

            return String.Empty;
        }
    }
}