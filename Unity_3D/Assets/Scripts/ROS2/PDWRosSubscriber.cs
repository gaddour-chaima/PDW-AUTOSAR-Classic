
using UnityEngine;
using Unity.Robotics.ROSTCPConnector;
using RosMessageTypes.Std;
using TMPro;

[DefaultExecutionOrder(500)]
[RequireComponent(typeof(AudioSource))]
public class PDWRosSubscriber : MonoBehaviour
{
    [Header("Vehicle")]
    public Transform vehicleRoot;
    public float initialDistance = 2.5f;

    [Header("User Interface")]
    public TMP_Text distanceText;
    public TMP_Text warningText;

    [Header("ROS 2")]
    public string rosTopic = "/pdw/state";

    private Vector3 initialPosition;
    private AudioSource buzzerAudio;

    private readonly object dataLock = new object();
    private bool hasNewData = false;

    private float receivedDistance;
    private int receivedWarning;
    private bool receivedBuzzer;

    private bool hasReceivedMessage = false;
    private bool connectionTimedOut = false;
    private float lastMessageTime;

    void Awake()
    {
        buzzerAudio = GetComponent<AudioSource>();

        buzzerAudio.playOnAwake = false;
        buzzerAudio.loop = true;
        buzzerAudio.spatialBlend = 0f;
        buzzerAudio.volume = 0.15f;

        // Create the buzzer's 880 Hz sound.
        // ROS 2 controls when the sound is ON or OFF.
        int sampleRate = 44100;
        int sampleCount = 4410;

        float[] samples = new float[sampleCount];

        for (int i = 0; i < sampleCount; i++)
        {
            float time = (float)i / sampleRate;

            samples[i] = 0.6f * Mathf.Sin(
                2f * Mathf.PI * 880f * time
            );
        }

        AudioClip clip = AudioClip.Create(
            "PDW_ROS_Buzzer",
            sampleCount,
            1,
            sampleRate,
            false
        );

        clip.SetData(samples, 0);
        buzzerAudio.clip = clip;
    }

    void Start()
    {
        if (vehicleRoot == null)
        {
            Debug.LogError(
                "[PDW ROS2] VehicleRoot non assigne !"
            );
            enabled = false;
            return;
        }

        // Store vehicle position after initial calibration.
        initialPosition = vehicleRoot.position;

        ROSConnection.GetOrCreateInstance()
            .Subscribe<Float32MultiArrayMsg>(
                rosTopic,
                OnPDWMessage
            );

        Debug.Log(
            "[PDW ROS2] Controle ROS actif : " + rosTopic
        );
    }

    void OnPDWMessage(Float32MultiArrayMsg msg)
    {
        if (msg.data == null || msg.data.Length < 4)
            return;

        lock (dataLock)
        {
            receivedDistance = msg.data[1];
            receivedWarning =
                Mathf.RoundToInt(msg.data[2]);
            receivedBuzzer = msg.data[3] >= 0.5f;

            hasNewData = true;
        }
    }

    void Update()
    {
        float distance = 0f;
        int warning = 0;
        bool buzzer = false;
        bool updated = false;

        lock (dataLock)
        {
            if (hasNewData)
            {
                distance = receivedDistance;
                warning = receivedWarning;
                buzzer = receivedBuzzer;

                hasNewData = false;
                updated = true;
            }
        }

        if (updated)
        {
            hasReceivedMessage = true;
            connectionTimedOut = false;
            lastMessageTime = Time.realtimeSinceStartup;

            ApplyPDW(distance, warning, buzzer);
        }

        // Stop buzzer if ROS messages stop arriving.
        if (hasReceivedMessage &&
            !connectionTimedOut &&
            Time.realtimeSinceStartup - lastMessageTime > 1f)
        {
            connectionTimedOut = true;
            buzzerAudio.Stop();

            if (warningText != null)
            {
                warningText.text = "ROS 2 : connexion perdue";
                warningText.color = Color.gray;
            }

            Debug.LogWarning(
                "[PDW ROS2] Communication interrompue."
            );
        }
    }

    void ApplyPDW(float distance, int warning, bool buzzer)
    {
        bool invalid =
            warning == 4 ||
            float.IsNaN(distance) ||
            float.IsInfinity(distance) ||
            distance < 0f;

        // Use the same world direction as the CSV playback.
        if (!invalid)
        {
            float displacement = Mathf.Clamp(
                initialDistance - distance,
                0f,
                initialDistance
            );

            vehicleRoot.position =
                initialPosition + Vector3.back * displacement;
        }

        if (distanceText != null)
        {
            distanceText.text = invalid
                ? "Distance : INVALID"
                : $"Distance : {distance:F2} m";
        }

        string message;
        Color color;

        switch (warning)
        {
            case 1:
                message = "WL1 - ATTENTION";
                color = Color.yellow;
                break;

            case 2:
                message = "WL2 - OBSTACLE PROCHE";
                color = new Color(1f, 0.5f, 0f);
                break;

            case 3:
                message = "WL3 - DANGER";
                color = Color.red;
                break;

            case 4:
                message = "WL4 - DEFAUT CAPTEUR";
                color = Color.magenta;
                break;

            default:
                message = "WL0 - AUCUNE ALERTE";
                color = Color.green;
                break;
        }

        if (warningText != null)
        {
            warningText.text = message;
            warningText.color = color;
        }

        // The beep timing comes directly from Simulink.
        bool soundEnabled = buzzer && !invalid;

        if (soundEnabled && !buzzerAudio.isPlaying)
        {
            buzzerAudio.Play();
        }
        else if (!soundEnabled && buzzerAudio.isPlaying)
        {
            buzzerAudio.Stop();
        }
    }

    void OnDisable()
    {
        if (buzzerAudio != null)
        {
            buzzerAudio.Stop();
        }
    }
}
