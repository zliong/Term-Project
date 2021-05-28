using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.Net.Sockets;
using System;

public class client : MonoBehaviour
{
    public static client instance;
    public static int bufsize = 4096;

    public string ip = "127.0.0.1";
    public int port = 25568;
    public int id = 0;
    public TCP tcp_;
    public class TCP
    {
        public TcpClient sock;

        private NetworkStream stream;
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
            if(sock.Connected == false)
            {
                return;
            }
            stream = sock.GetStream();
            stream.BeginRead(receiveBuf, 0, bufsize, ReceiveCallBack, null);
        }

        private void ReceiveCallBack(IAsyncResult result)
        {
            try
            {
                int byte_length = stream.EndRead(result);
                if (byte_length <= 0)
                {
                    //disconnect 
                    return;
                }
                byte[] data = new byte[byte_length];
                Array.Copy(receiveBuf, data, byte_length);  //copy receiveBuf to data, should be same length

                //handle data
                stream.BeginRead(receiveBuf, 0, bufsize, ReceiveCallBack, null);
            }
            catch
            {
                //disonnect client
            }
            
        }
    }

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
    public void ConnectToServer()
    {
        tcp_.Connect();
    }
}

