using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;
using System.Threading.Tasks

namespace ImageServer
{
    [ServiceContract(CallbackContract = typeof(ISendImagesCallback))]
    interface ISendImages
    {
        [OperationContract(IsOneWay = true, Action = "*")]
        [WebInvoke(Method = "POST"
            , BodyStyle = WebMessageBodyStyle.Wrapped
            , RequestFormat = WebMessageFormat.Json
            , ResponseFormat = WebMessageFormat.Json
            , UriTemplate = "*")]
        Task GetImages;
    }
}
