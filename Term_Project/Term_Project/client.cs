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
                  server.clients[id].Disconnect();
                  return;
               }

               byte[] _data = new byte[_byteLength];
               Array.Copy(receiveBuffer, _data, _byteLength);

               //TODO: Handle
               stream.BeginRead(receiveBuffer, 0, dataBufferSize, ReceiveCallBack, null);
            }
            catch (Exception _ex)
            {
               Console.WriteLine($"Error receiving TCP data: { _ex}");
               server.clients[id].Disconnect();

            }
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
