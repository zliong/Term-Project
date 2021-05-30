using System.Collections;
using System.Collections.Generic;
using UnityEngine;


//A test program to see if packets are being sent 
public class ClientSend : MonoBehaviour
{
    private static void SendTCPData(Packet packet)
    {
        packet.WriteLength();
        Client.instance.tcp_.SendData(packet);
    }

    #region Packets
    public static void WelcomeReceived()
    {
        //ensures the proper use of IAysncDisposable Objects
        using (Packet packet = new Packet((int)ClientPackets.welcomeReceived))
        {
            packet.Write(Client.instance.myId);
            packet.Write(UIManager.instance.username_field.text);

            SendTCPData(packet);
        }
    }
    public static void PlayerMovement(bool[] inputs)
    {
        using (Packet _packet = new Packet((int)ClientPackets.playerMovement))
        {
            _packet.Write(inputs.Length);
            foreach (bool _input in inputs)
            {
                _packet.Write(_input);
            }
            _packet.Write(GameManager.players[Client.instance.myId].transform.rotation);

            SendTCPData(_packet);
        }
    }
    #endregion
}
