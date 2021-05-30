using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;


namespace Term_ProjectServer
{
   class server
   {
      //Local Variables
      public static int MaxPlayers { get; private set; }
      public static int Port { get; private set; }
      public static Dictionary<int, Client> clients = new Dictionary<int, Client>();
      private static TcpListener tcpListener;
      public delegate void PacketHandler(int client, Packet packet);
      public static Dictionary<int, PacketHandler> packetHandlers; //The packethandler version of the clients. 

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

      //According to tutorial, this should be in its own class, but integrating it here seemed for efficient with no downside, so I do it here.
      public static void WelcomeRecieved(int client, Packet packet)
      {
         int clientIDCheck = packet.ReadInt();
         string username = packet.ReadString();
         Console.WriteLine($"A client connected successfully takes the name {username}");

         //This tests if somehow the client ID was assigned wrong.
         if (client != clientIDCheck)
         {
            Console.WriteLine($"User {username} wrongly took the ID {clientIDCheck}");
         }
         //Todo: put player into the fight.
      }

      private static void TCPConnectCallback(IAsyncResult result)
      {
         TcpClient client = tcpListener.EndAcceptTcpClient(result);
         tcpListener.BeginAcceptTcpClient(new AsyncCallback(TCPConnectCallback), null);
         Console.WriteLine("Someone is attempting to connect!");

         for(int i = 1; i <= MaxPlayers; i++)
         {
            if(clients[i].tcp.socket == null)
            {
               clients[i].tcp.Connect(client);
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

         packetHandlers = new Dictionary<int, PacketHandler>()
         {
            { (int)ClientPackets.welcomeReceived, server.WelcomeRecieved } //Weird formatting, but needed.
         }; //Initialize it to the values above.

         //For debugging / to make sure program does right things in general.
         Console.WriteLine("Finished setting up the server's data.");
      }
   }
}