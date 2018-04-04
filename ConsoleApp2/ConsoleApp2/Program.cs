using System;
using System.ServiceModel;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ImageServer
{
    class Program
    {
        static void Main(string[] args)
        {
            var host = new ServiceHost(typeof(SendImages));
            host.Open();
            Console.WriteLine("Hi =) Press any key to stop the Server!");
            Console.ReadKey();
            host.Close();
        }
    }
}
