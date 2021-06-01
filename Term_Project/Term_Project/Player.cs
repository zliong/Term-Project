using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Numerics;

namespace Term_ProjectServer
{
    class Player
    {
        public int player_id;
        public string username;

        private float moveSpeed = 5f / Constants.TICKS_PER_SEC;
        public Vector3 player_pos;
        public Quaternion player_rotation;
        private bool[] inputs;

        //default constructor
        public Player(int id, string user, Vector3 spawnLoc)
        {
            player_id = id;
            username = user;
            player_pos = spawnLoc;
            player_rotation = Quaternion.Identity;

            inputs = new bool[4];
        }

        public void Update()
        {
            Vector2 inputDirection = Vector2.Zero;
            if(inputs[0])
            {
                inputDirection.Y += 1;
            }
            if (inputs[1])
            {
                inputDirection.Y -= 1;
            }
            if (inputs[2])
            {
                inputDirection.X += 1;
            }
            if (inputs[3])
            {
                inputDirection.X -= 1;
            }

            //Move(inputDirection);
        }

      
        public void Move(Vector2 input)
        {
        Vector3 forward = Vector3.Transform(new Vector3(0, 0, 1), player_rotation);
        Vector3 right = Vector3.Normalize(Vector3.Cross(forward, new Vector3(0, 1, 0)));

        Vector3 moveDirection = right * input.X + forward * input.Y;
        player_pos += moveDirection * moveSpeed;

            ServerSend.PlayerPosition(this);
            ServerSend.PlayerRotation(this);
        }

        /// <summary>Updates the player input with newly received input.</summary>
        /// <param name="_inputs">The new key inputs.</param>
        /// <param name="_rotation">The new rotation.</param>
        public void SetInput(bool[] _inputs, Quaternion _rotation)
        {
            inputs = _inputs;
            player_rotation = _rotation;
        }
    }
}
