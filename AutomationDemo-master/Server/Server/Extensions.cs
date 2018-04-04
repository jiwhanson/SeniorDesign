using System;
using System.Net.WebSockets;
using System.ServiceModel.Channels;
using System.Text;

namespace Automation
{
    public static class Extensions
    {
        public static byte[] GetBytes<T>(this T data, int maxJsonLength = 10240000, int recursionLimit = 100)
        {
            var serializer = new System.Web.Script.Serialization.JavaScriptSerializer
            {
                MaxJsonLength = maxJsonLength,
                RecursionLimit = recursionLimit,
            };
            return Encoding.UTF8.GetBytes(serializer.Serialize(data));
        }

        public static Message CreateMessage(this byte[] message, WebSocketMessageType messageType = WebSocketMessageType.Text)
        {
            var result = ByteStreamMessage.CreateMessage(new ArraySegment<byte>(message));
            result.Properties["WebSocketMessageProperty"] = new WebSocketMessageProperty
            {
                MessageType = messageType
            };
            return result;
        }
    }
}