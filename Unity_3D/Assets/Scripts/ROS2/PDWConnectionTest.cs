
using UnityEngine;
using Unity.Robotics.ROSTCPConnector;
using RosMessageTypes.Std;

public class PDWConnectionTest : MonoBehaviour
{
    private int messageCount = 0;

    void Start()
    {
        ROSConnection ros = ROSConnection.GetOrCreateInstance();

        ros.Subscribe<Float32MultiArrayMsg>(
            "/pdw/state",
            OnPDWMessage
        );

        Debug.Log("[PDW ROS2] Abonnement actif !");
    }

    void OnPDWMessage(Float32MultiArrayMsg msg)
    {
        if (msg.data == null || msg.data.Length < 4)
            return;

        messageCount++;

        if (messageCount == 1 || messageCount % 50 == 0)
        {
            Debug.Log(
                $"[PDW ROS2] Distance: {msg.data[1]:F2} m | " +
                $"Warning: {msg.data[2]:F0} | " +
                $"Buzzer: {msg.data[3]:F0}"
            );
        }
    }
}
