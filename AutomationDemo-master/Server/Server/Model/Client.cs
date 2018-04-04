using System.Runtime.Serialization;

namespace Automation
{
    [DataContract()]
    public class Client : IExtensibleDataObject
    {
        [DataMember(Name = "message", IsRequired = true)]
        public string Message { get; set; }

        internal IAutomationServiceCallback Callback { get; set; }

        public ExtensionDataObject ExtensionData { get; set; }
    }
}