using System.ServiceModel;
using System.ServiceModel.Channels;
using System.Threading.Tasks;

namespace Automation
{
    [ServiceContract]
    interface IAutomationServiceCallback
    {
        [OperationContract(IsOneWay = true, Action = "*")]
        Task Receive(Message message);
    }
}