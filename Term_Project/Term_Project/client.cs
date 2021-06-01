using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Numerics;

namespace Term_ProjectServer
{
   class Client
   {
      public static int dataBufferSize = 4096;

      public int id;
      public Player player;
      public TCP tcp;

      public Client(int _clientId)
      {
         id = _clientId;
         tcp = new TCP(id);
      }
      public class TCP
      {
         public TcpClient socket;

         private readonly int id;
         private NetworkStream stream;
         private Packet receivedData;
         private byte[] receiveBuffer;

         public TCP(int _id)
         {
            id = _id;
         }

         public void Connect(TcpClient _socket)
         {
            socket = _socket;
            socket.ReceiveBufferSize = dataBufferSize;
            socket.SendBufferSize = dataBufferSize;

            stream = socket.GetStream();

           
            receiveBuffer = new byte[dataBufferSize];
            receivedData = new Packet();
            stream.BeginRead(receiveBuffer, 0, dataBufferSize, ReceiveCallBack, null);

            ServerSend.Welcome(id, "You have connected to the game server.");
         }

         public void SendData(Packet packet)
         {
            //Not sure what errors can happen, but tutorial says to try/catch.
            try
            {
               if (socket != null)
               {
                  stream.BeginWrite(packet.ToArray(), 0, packet.Length(), null, null);
               }
            }
            catch (Exception e) { Console.WriteLine("Error occured while sending data to a player through TCP."); }
         }

         private void ReceiveCallBack(IAsyncResult _result)
         {
            try
            {
               int _byteLength = stream.EndRead(_result);
               if (_byteLength <= 0)
               {
                  Server.clients[id].Disconnect();
                  return;
               }

               byte[] _data = new byte[_byteLength];
               Array.Copy(receiveBuffer, _data, _byteLength);

               receivedData.Reset(HandleData(_data));

               //TODO: Handle //Not sure this is needed, should test it.
               stream.BeginRead(receiveBuffer, 0, dataBufferSize, ReceiveCallBack, null);
            }
            catch (Exception _ex)
            {
               Console.WriteLine($"Error receiving TCP data: { _ex}");
               Server.clients[id].Disconnect();

            }
         }

         private bool HandleData(byte[] data)
         {
            int packetSize = 0;

            receivedData.SetBytes(data);
            if (receivedData.UnreadLength() >= 4) //4 is the next byte of data.
            {
               packetSize = receivedData.ReadInt();
               if(packetSize <= 0) { return true; }
            }

            while(packetSize > 0 && packetSize <= receivedData.UnreadLength())
            {
               byte[] packetData = receivedData.ReadBytes(packetSize);
               ThreadManager.ExecuteOnMainThread(() =>
               {
                  using (Packet packet = new Packet(packetData))
                  {
                     int packetID = packet.ReadInt();
                     server.packetHandlers[packetID](id, packet);
                  }
               });

               //Retry test above.
               packetSize = 0;
                           receivedData.SetBytes(data);
               if (receivedData.UnreadLength() >= 4) //4 is the next byte of data.
               {
                  packetSize = receivedData.ReadInt();
                  if(packetSize <= 0) { return true; }
               }
            }
            if(packetSize <= 1)
            {
               return true;
            }

            //Catch anything bad.
            return false;
         }

         public void Disconnect()
         {
            socket.Close();
            receivedData = null;
            receiveBuffer = null;
            socket = null;
         }
      }

      public void SendIntoGame(string player_name)
      {
         player = new Player(id, player_name, new Vector3(0, 0, 0));
            foreach (Client client in Server.clients.Values)
            {
                if(client.player != null)
                {
                    if(client.id == null)
                    {
                        Console.WriteLine("Error! Unidentified Client!");
                        return;
                    }
                    else
                    {
                        ServerSend.SpawnPlayer(id, client.player);
                    }
                }
            }
      }

      //This is out here because, if we had implemented UDP, we would need to call that as well, but we did not.
      private void Disconnect()
      {
         Console.WriteLine("A client has disconnected");
         player = null;
         tcp.Disconnect();
      }
   }
}
