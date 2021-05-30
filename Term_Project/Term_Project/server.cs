using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;


namespace Term_ProjectServer
{
   class server
   {
      public static int MaxPlayers { get; private set; }

      public static int Port { get; private set; }
      public static Dictionary<int, Client> clients = new Dictionary<int, Client>();
      private static TcpListener tcpListener;

      public static void Start(int maxPlayers, int port)
      {
         MaxPlayers = maxPlayers;  //For clarity.
         Port = port;              //For clarity.

         Console.WriteLine("Revving up the server!");

         InitalizeServerData();
         tcpListener = new TcpListener(IPAddress.Any, port);
         tcpListener.Start();
         tcpListener.BeginAcceptTcpClient(new AsyncCallback(TCPConnectCallback), null);

         Console.WriteLine($"Server started on port {port}!");
      }

      private static void TCPConnectCallback(IAsyncResult result)
      {
         TcpClient client = tcpListener.EndAcceptTcpClient(result);
         tcpListener.BeginAcceptTcpClient(new AsyncCallback(TCPConnectCallback), null);
         Console.WriteLine("Someone is attempting to connect!");

         for(int i = 1; i <= MaxPlayers; i++)
         {
            if(clients[i].tcp.Connected == false) //Change from guide for perhaps more efficiency. If something doesn't work check here, may need to be .tcp.socket.
            {
               clients[i].tcp.Connect(_client);
               return;
            }
         }
         Console.WriteLine("Somebody failed to connect: server is full.");
      }

      private static void InitalizeServerData()
      {
         for(int i = 1; i <= MaxPlayers; i++)
         {
            clients.Add(i, new Client(i));
         }
      }
   }
}