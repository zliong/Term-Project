using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.Net.Sockets;
using System;

public class Client : MonoBehaviour
{
    public static Client instance;
    public static int bufsize = 4096;

    public string ip = "127.0.0.1";
    public int port = 25568;
    public int myId = 0; //Changed this to match Thread manager 
    public TCP tcp_;

    private bool isConnected = false;
    private delegate void PacketHandler(Packet _packet);
    private static Dictionary<int, PacketHandler> packetHandlers;

    private void Awake()
    {
        if (instance = null)
        {
            instance = this;
        }
        else if (instance != this)
        {
            Debug.Log("Client/Instance already exists, destroying object!");
            Destroy(this);
        }
    }
    private void Start()
    {
        tcp_ = new TCP();
    }

    private void OnApplicationQuit()
    {
        Disconnect();
    }

    public void ConnectToServer()
    {
        InitializeClientData();

        isConnected = true;
        tcp_.Connect();
    }


    public class TCP
    {
        public TcpClient sock;

        private NetworkStream stream;
        private Packet receivedData;
        private byte[] receiveBuf;

        public void Connect()
        {
            sock = new TcpClient
            {
                ReceiveBufferSize = bufsize,
                SendBufferSize = bufsize
            };
            receiveBuf = new byte[bufsize]; //intialize the data buffer for client to receive
            sock.BeginConnect(instance.ip, instance.port, ConnectCallBack, sock);   //connect the client to the ip 

        }
        private void ConnectCallBack(IAsyncResult result)
        {
            sock.EndConnect(result);   //ends any pending connections to the server

            //check if socket has successfully connected
            if (sock.Connected == false)
            {
                return;
            }
            stream = sock.GetStream();

            receivedData = new Packet();

            stream.BeginRead(receiveBuf, 0, bufsize, ReceiveCallBack, null);
        }

        public void SendData(PacketHandler _packet)
        {
            try
            {
                if(Socket != null)
                {
                    stream.BeginWrite(_packet.ToArray(), 0, _packet.Length(), null, null);
                }
            }
            catch (Exception _ex)
            {
                Debug.Log($"Error sending data to server via TCP: { _ex}");

            }
        }

        private void ReceiveCallBack(IAsyncResult result)
        {
            try
            {
                int byte_length = stream.EndRead(result);
                if (byte_length <= 0)
                {
                    instance.Disconnect();
                    return;
                }
                byte[] data = new byte[byte_length];
                Array.Copy(receiveBuf, data, byte_length);  //copy receiveBuf to data, should be same length

                receivedData.Reset(HandleData(data));

                //handle data
                stream.BeginRead(receiveBuf, 0, bufsize, ReceiveCallBack, null);
            }
            catch
            {
                Disconnect();
            }

        }
        private bool HandleData(byte[] data)
        {
            int _packetLength = 0;

            receivedData.SetBytes(data);

            if (receivedData.UnreadLength() >= 4)
            {
                _packetLength = receivedData.ReadInt();
                if (_packetLength <= 0)
                {
                    return true;
                }
            }

            while (_packetLength > 0 && _packetLength <= receivedData.UnreadLength())
            {
                byte[] _packetBytes = receivedData.(_packetLength);// If there is a problem below talk to carson
                ThreadManager.ExecuteOnMainThread(() =>
                {
                    using (Packet _packet = new Packet(_packetBytes))
                    {
                        int _packetId = _packet.ReadInt();
                        packetHandlers[_packetId](_packet);

                    }
                });


                _packetLength = 0;

                if (receivedData.UnreadLength() >= 4)
                {
                    _packetLength = receivedData.ReadInt();
                    if (_packetLength <= 0)
                    {
                        return true;
                    }
                }
            }

            if (_packetLength <= 1)
            {
                return true;
            }

            return false;
        }


        private void Disconnect()
        {
            instance.Disconnect();

            stream = null;
            receivedData = null;
            receivedBuffer = null;
            socket = null;
        } 
    }

    

    private void InitializeClientData(){
        packetHandlers = new Dictionary<int, PacketHandler>()
        {
            {(int)ServerPackets.welcome, ClientHandle.Welcome }
        };
        Debug.Log("Initialize packets.");
    }

    private void Disconnect()
    {
        if (isConnected)
        {
            isConnected = false;
            tcp_.socket.Close();
            //udp socket close will go here if change is made

            Debug.Log("Disconnected from  server.");
        }
    }
}
