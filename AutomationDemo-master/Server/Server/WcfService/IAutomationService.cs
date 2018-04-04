using System.ServiceModel;
using System.ServiceModel.Channels;
using System.ServiceModel.Web;
using System.Threading.Tasks;

namespace Automation
{
    [ServiceContract(CallbackContract = typeof(IAutomationServiceCallback))]
    interface IAutomationService
    {
        [OperationContract(IsOneWay = true, Action = "*")]
        [WebInvoke(Method = "POST"
            , BodyStyle = WebMessageBodyStyle.Wrapped
            , RequestFormat = WebMessageFormat.Json
            , ResponseFormat = WebMessageFormat.Json
            , UriTemplate = "*")]
        Task GetProductInfo(Message message);
    }
}