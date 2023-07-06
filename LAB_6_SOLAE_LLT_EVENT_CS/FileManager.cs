using System;
using System.IO;

namespace LAB_6_SOLAE_LLT_EVENT_CS
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
