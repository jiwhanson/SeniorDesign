using System.ServiceModel;
using System.ServiceModel.Channels;
using System.Threading.Tasks;

namespace ImageServer
{
    [ServiceContract]
    interface ISendImagesCallback
    {
        [OperationContract(IsOneWay = true, Action = "*")]
        Task Receive(Message message);
    }
}