using System;

namespace Automation
{
    public class ProductInfo
    {
        public string Id { get; set; }

        public string Customer { get; set; }

        public DateTime Timestamp { get; set; } = DateTime.Now;
    }
}