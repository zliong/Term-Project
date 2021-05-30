using System;

namespace Term_Project
{
    class Program
    {
        static void Main(string[] args)
        {
         Term_ProjectServer.server.Start(8, 1337);

            Console.WriteLine("Hello World!");
        }
    }
}
