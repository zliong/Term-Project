using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Term_ProjectServer
{
    class ThreadManager
    {
        private static readonly List<Action> executeOnMainThread = new List<Action>();
        private static readonly List<Action> executeCopiedOnMainThread = new List<Action>();
        private static bool actionToExecuteOnMainThread = false;

        public static void ExecuteOnMainThread(Action action)
        {
            if(action == null)
            {
                Console.WriteLine("No action to execute on main thread!");
                return;
            }
            lock (executeOnMainThread)     //lock the thread to prevent another from executing another action
            {
                executeOnMainThread.Add(action);
                actionToExecuteOnMainThread = true;
            }
        }

        public static void UpdateMain()
        {
            if(actionToExecuteOnMainThread == true)
            {
                executeCopiedOnMainThread.Clear();
                lock(executeOnMainThread)
                {
                    executeCopiedOnMainThread.AddRange(executeOnMainThread);
                    executeOnMainThread.Clear();
                    actionToExecuteOnMainThread = false;
                }
                for(int i = 0; i < executeCopiedOnMainThread.Count;i++)
                {
                    executeCopiedOnMainThread[i](); 
                }
            }
        }
    }
}
