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

        public Vector3 player_pos;
        public Quaternion player_rotation;

        //default constructor
        public Player(int id, string user, Vector3 spawnLoc)
        {
            player_id = id;
            username = user;
            player_pos = spawnLoc;
            player_rotation = Quaternion.Identity;
        }
    }
}
