using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Net.Http;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace ComPortMonitor
{
    class Program
    {
        static async Task Main(string[] args)
        {
            if (args.Length < 1)
            {
                Console.WriteLine("Usage: ComPortMonitor <COM Port> [REST API URL]");
                return;
            }

            string comPort = args[0];
            string apiUrl = args.Length > 1 ? args[1] : null;

            using SerialPort port = new SerialPort(comPort, 9600);
            port.DataReceived += async (sender, e) => await DataReceivedHandler(sender, e, apiUrl);
            port.Open();

            Console.WriteLine($"Monitoring {comPort}...");
            Console.ReadLine();
        }

        private static async Task DataReceivedHandler(object sender, SerialDataReceivedEventArgs e, string apiUrl)
        {
            SerialPort port = (SerialPort)sender;
            string data = port.ReadLine();
            Console.WriteLine($"Data Received: {data}");

            if (data.StartsWith("T:") && data.Contains(";H:"))
            {
                string[] parts = data.Split(';');
                string temperature = parts[0].Split(':')[1];
                string humidity = parts[1].Split(':')[1];

                var sensorDataList = new List<SensorData>
                {
                    new SensorData { Type = SensorType.Temperature, Value = temperature },
                    new SensorData { Type = SensorType.Humidity, Value = humidity }
                };

                string json = JsonSerializer.Serialize(sensorDataList);
                Console.WriteLine($"JSON Payload: {json}");

                if (!string.IsNullOrEmpty(apiUrl))
                {
                    using HttpClient client = new HttpClient();
                    StringContent content = new StringContent(json, Encoding.UTF8, "application/json");
                    HttpResponseMessage response = await client.PostAsync(apiUrl, content);

                    string responseString = await response.Content.ReadAsStringAsync();
                    Console.WriteLine($"Response: {responseString}");
                }
            }
        }
    }

    public enum SensorType
    {
        Temperature,
        Humidity,
        WaterFlow // You can add more as needed
    }

    public class SensorData
    {
        public SensorType Type { get; set; }
        public string Value { get; set; }
    }
}
