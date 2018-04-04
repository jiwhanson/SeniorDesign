using System;
using System.IO;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.Threading.Tasks;

namespace Automation
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.Single, ConcurrencyMode = ConcurrencyMode.Multiple, UseSynchronizationContext = true, IncludeExceptionDetailInFaults = true)]
    public class AutomationService : IAutomationService, IDisposable
    {
        private int productId = 1;

        public async Task GetProductInfo(Message message)
        {
            if (message.IsEmpty)
            {
                return;
            }

            Client client;

            try
            {
                var body = message.GetBody<byte[]>();
                using (var stream = new MemoryStream(body))
                {
                    var json = new System.Runtime.Serialization.Json.DataContractJsonSerializer(typeof(Client));
                    client = (Client)json.ReadObject(stream);
                }

                Console.WriteLine($"{DateTimeOffset.Now} -> Client says: {client.Message}");

                var productInfo = new ProductInfo
                {
                    Id = "Testing",
                    Customer = "Tenaris",
                };

                var callback = OperationContext.Current.GetCallbackChannel<IAutomationServiceCallback>();

                await callback.Receive(productInfo.GetBytes().CreateMessage());
            }
            catch (Exception error)
            {
                Console.WriteLine($"{error.Message}");
            }
        }

        #region IDisposable
        private bool disposedValue = false;

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                disposedValue = true;
            }
        }

        public void Dispose()
        {
            Dispose(true);
        }
        #endregion
    }
}