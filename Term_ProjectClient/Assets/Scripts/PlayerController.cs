using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour
{
    private void FixedUpdate()  //awaits the input of users and sends to server on a constant tick
    {
        SendInputToServer();    
    }

    private void SendInputToServer()
    {
        bool[] inputs = new bool[]  //the controls for player movement
        {
            Input.GetKey(KeyCode.W),
            Input.GetKey(KeyCode.S),
            Input.GetKey(KeyCode.A),
            Input.GetKey(KeyCode.D),
        };

        ClientSend.PlayerMovement(inputs);
    }

}
