using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.Net.Sockets;

public class client : MonoBehaviour
{
    public static client instance;
    public static int bufsize = 4096;

    public string ip = "127.0.0.1";
    public int port = 25568;
    public TCP tcp_;

    private void Awake()
    {
        if(instance = null)
        {
            instance = this;
        }
        else if(instance != this)
        {
            Debug.log("Client/Instance already exists, destroying object!");
            Destroy(this);
        }
    }
}
