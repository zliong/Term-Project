using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Term_ProjectServer
{
   class ServerSend
   {
      public static void Welcome(int toClient, string message)
      {
         using (Packet packet = new Packet((int)ServerPackets.welcome))
         {
            packet.Write(message);
            packet.Write(toClient);
            SendTCPData(toClient, packet);
         }
      }

      private static void SendTCPData(int toClient, Packet packet)
      {
         packet.WriteLength();
         server.clients[toClient].tcp.SendData(packet);
      }

      private static void SendTCPDataToAllClients(Packet packet) //One spicy function name.
      {
         packet.WriteLength();
         for (int i = 0; i <= server.MaxPlayers; i++)
         {
            server.clients[i].tcp.SendData(packet);
         }
      }

      //Unsure currently how this is needed, hopefully further guide will explain it.
      public static void SendTCPDataToAllClientsExcept(int client, Packet packet) //Part 2: return of the spicy function names.
      {
         packet.WriteLength();
         for (int i = 0; i <= server.MaxPlayers; i++)
         {
            if(server.clients[i].id != client)
            {
               server.clients[i].tcp.SendData(packet);
            }
         }
      }

   }
}
