using System;

namespace Term_ProjectServer
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.Title = "Game Server";

            Server.Start(50, 25568);

            Console.ReadKey();
        }
    }
}
