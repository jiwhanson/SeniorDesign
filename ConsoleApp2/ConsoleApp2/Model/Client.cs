using System.Runtime.Serialization;

namespace ImageServer
{
    [DataContract()]
    public class Client : IExtensibleDataObject
    {
        [DataMember(Name = "message", IsRequired = true)]
        public string Message { get; set; }

        internal ISendImagesCallback Callback { get; set; }

        public ExtensionDataObject ExtensionData { get; set; }
    }
}