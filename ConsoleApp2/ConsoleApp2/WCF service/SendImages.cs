using System;
using System.IO;
using System.ServiceModel.Channels;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace ImageServer
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.Single, ConcurrencyMode = ConcurrencyMode.Multiple, UseSynchronizationContext = true, IncludeExceptionDetailInFaults = true)]
    public class SendImages : ISendImages, IDisposable
    {

        public async Task GetImages(Message message)
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
                    Id = "23344444",
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
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "SendImages" in both code and config file together.
  /*  public class SendImages : ISendImages
    {
        public Stream GetImages()
        {
            FileStream fs4 = File.OpenRead(@"C:\TestFolder\TestFolder1.zip");
            WebOperationContext.Current.OutgoingResponse.ContentType = "application/zip";
            return fs4;

        }
    }
    */

