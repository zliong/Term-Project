using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;
public class ClientHandle : MonoBehaviour
{
    public static void Welcome(Packet _packet)
    {
        string _msg = _packet.ReadString();
        int _myId = _packet.ReadInt();

        Debug.Log($"Message from server: {_msg}");
        Client.instance.myId = _myId;
        ClientSend.WelcomeReceived();
    }

    public static void SpawnPlayer(Packet _packet)
    {
        int id = _packet.ReadInt();
        string username = _packet.ReadString();
        Vector3 pos = _packet.ReadVector3();
        Quaternion rotation = _packet.ReadQuaternion();

        GameManager.instance.SpawnPlayer(id, username, pos, rotation);
    }

    public static void PlayerPosition(Packet _packet)
    {
        int id = _packet.ReadInt();
        Vector3 pos = _packet.ReadVector3();

        GameManager.players[id].transform.position = pos;
    }

    public static void PlayerRotation(Packet _packet)
    {
        int id = _packet.ReadInt();
        Quaternion rotation = _packet.ReadQuaternion();

        GameManager.players[id].transform.rotation = rotation;
    }
}
