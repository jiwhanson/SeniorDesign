using System;
using System.ServiceModel;

namespace Automation
{
    class Program
    {
        /// <summary>
        /// run this command into Command / PowerShell Console
        /// with Admin rights before run the Program
        ///     netsh http add urlacl url=http://+:8023/AutomationService user=LocalAdminUserName
        /// </summary>
        static void Main(string[] args)
        {
            var host = new ServiceHost(typeof(AutomationService));
            host.Open();
            Console.WriteLine("Hi =) Press any key to stop the Server!");
            Console.ReadKey();
            host.Close();
        }
    }
}