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

      private static TcpListener tcpListener;

      public static void Start(int maxPlayers, int port)
      {
         MaxPlayers = maxPlayers;  //For clarity.
         Port = port;              //For clarity.

         tcpListener = new TcpListener(IPAddress.Any, port);
         tcpListener.Start();
         tcpListener.BeginAcceptTcpClient(new AsyncCallback(TCPConnectCallback), null);

         Console.WriteLine($"Server started on port {port}");

      }

      private static void TCPConnectCallback(IAsyncResult result)
      {
         TcpClient client = tcpListener.EndAcceptTcpClient(result);
         tcpListener.BeginAcceptTcpClient(new AsyncCallback(TCPConnectCallback), null);
         Console.WriteLine($"Incoming connection from {client.Client.RemoteEndPoint}...");
       }


   }
}