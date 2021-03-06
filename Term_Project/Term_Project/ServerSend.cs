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
         Server.clients[toClient].tcp.SendData(packet);
      }

      private static void SendTCPDataToAllClients(Packet packet) //One spicy function name.
      {
         packet.WriteLength();
         for (int i = 0; i <= Server.MaxPlayers; i++)
         {
            Server.clients[i].tcp.SendData(packet);
         }
      }
      public static void SpawnPlayer(int _toClient, Player _player)
      {
          using (Packet _packet = new Packet((int)ServerPackets.spawnPlayer))
          {
              _packet.Write(_player.player_id);
              _packet.Write(_player.username);
              _packet.Write(_player.player_pos);
              _packet.Write(_player.player_rotation);
              SendTCPData(_toClient, _packet);
          }
      }
      
         /// <summary>Sends a player's updated position to all clients.</summary>
        /// <param name="_player">The player whose position to update.</param>
        public static void PlayerPosition(Player _player)
        {
            using (Packet _packet = new Packet((int)ServerPackets.playerPos))
            {
                _packet.Write(_player.player_id);
                _packet.Write(_player.player_pos);

                SendTCPDataToAllClients(_packet);
            }
        }

        /// <summary>Sends a player's updated rotation to all clients except to himself (to avoid overwriting the local player's rotation).</summary>
        /// <param name="_player">The player whose rotation to update.</param>
        public static void PlayerRotation(Player _player)
        {
            using (Packet _packet = new Packet((int)ServerPackets.playerRotation))
            {
                _packet.Write(_player.player_id);
                _packet.Write(_player.player_rotation);

                SendTCPDataToAllClientsExcept(_player.player_id, _packet);
            }
        }
        //Unsure currently how this is needed, hopefully further guide will explain it.
      public static void SendTCPDataToAllClientsExcept(int client, Packet packet) //Part 2: return of the spicy function names.
      {
         packet.WriteLength();
         for (int i = 0; i <= Server.MaxPlayers; i++)
         {
            if(Server.clients[i].id != client)
            {
               Server.clients[i].tcp.SendData(packet);
            }
         }
      }

   }
}
