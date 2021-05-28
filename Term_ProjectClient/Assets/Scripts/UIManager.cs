using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIManager : MonoBehaviour
{
    public static UIManager instance;
    public GameObject startMenu;
    public InputField username_field;

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
    
    public void ConnectToServer()
    {
        startMenu.SetActive(false);
        username_field.interactable = false;
        client.instance.ConnectToServer();
    }
}
