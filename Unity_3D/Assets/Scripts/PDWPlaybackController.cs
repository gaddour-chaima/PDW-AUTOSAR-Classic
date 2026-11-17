
using UnityEngine;
using TMPro;
using System;
using System.IO;
using System.Collections.Generic;
using System.Globalization;

public class PDWPlaybackController : MonoBehaviour
{
    [Header("Objets Unity")]
    public Transform vehicleRoot;
    public TMP_Text distanceText;
    public TMP_Text warningText;

    [Header("Simulation PDW")]
    public string csvFileName = "pdw_simulation.csv";
    public bool enableBuzzerSound = true;

    private struct PDWSample
    {
        public float time;
        public float distance;
        public int warning;
        public int buzzer;
    }

    private readonly List<PDWSample> samples =
        new List<PDWSample>();

    private Vector3 initialPosition;
    private float initialDistance;
    private float elapsedTime;
    private int currentIndex;
    private AudioSource buzzerAudio;

    void Start()
    {
        if (vehicleRoot == null ||
            distanceText == null ||
            warningText == null)
        {
            Debug.LogError("PDW : References Unity manquantes.");
            enabled = false;
            return;
        }

        string path = Path.Combine(
            Application.streamingAssetsPath,
            csvFileName
        );

        try
        {
            string[] lines = File.ReadAllLines(path);

            for (int i = 1; i < lines.Length; i++)
            {
                if (string.IsNullOrWhiteSpace(lines[i]))
                    continue;

                string[] c = lines[i].Split(',');

                if (c.Length != 4)
                    throw new Exception("Ligne CSV invalide : " + i);

                PDWSample s = new PDWSample();

                s.time = float.Parse(
                    c[0], CultureInfo.InvariantCulture);

                s.distance = float.Parse(
                    c[1], CultureInfo.InvariantCulture);

                s.warning = Mathf.RoundToInt(float.Parse(
                    c[2], CultureInfo.InvariantCulture));

                s.buzzer = Mathf.RoundToInt(float.Parse(
                    c[3], CultureInfo.InvariantCulture));

                if (samples.Count > 0 &&
                    s.time <= samples[samples.Count - 1].time)
                {
                    throw new Exception(
                        "Temps CSV non croissants."
                    );
                }

                samples.Add(s);
            }

            if (samples.Count < 2)
                throw new Exception("CSV vide ou incomplet.");
        }
        catch (Exception e)
        {
            Debug.LogError("PDW CSV : " + e.Message);
            enabled = false;
            return;
        }

        initialPosition = vehicleRoot.position;
        initialDistance = samples[0].distance;
        elapsedTime = samples[0].time;
        currentIndex = 0;

        if (enableBuzzerSound)
            SetupBuzzer();

        ApplySample(initialDistance,
                    samples[0].warning,
                    samples[0].buzzer);

        Debug.Log("PDW : CSV charge : "
                  + samples.Count + " echantillons.");
    }

    void Update()
    {
        elapsedTime += Time.deltaTime;

        float endTime = samples[samples.Count - 1].time;
        elapsedTime = Mathf.Min(elapsedTime, endTime);

        while (currentIndex < samples.Count - 1 &&
               samples[currentIndex + 1].time <= elapsedTime)
        {
            currentIndex++;
        }

        PDWSample a = samples[currentIndex];
        float distance = a.distance;

        // Interpolation visuelle de la position
        if (currentIndex < samples.Count - 1)
        {
            PDWSample b = samples[currentIndex + 1];

            float alpha = Mathf.InverseLerp(
                a.time, b.time, elapsedTime);

            distance = Mathf.Lerp(
                a.distance, b.distance, alpha);
        }

        // Warning et buzzer restent discrets
        ApplySample(distance, a.warning, a.buzzer);

        if (elapsedTime >= endTime)
        {
            Debug.Log("PDW : Simulation terminee.");
            enabled = false;
        }
    }

    private void ApplySample(
        float distance, int warning, int buzzer)
    {
        // Recul suivant l'axe mondial -Z
        float displacement = initialDistance - distance;

        vehicleRoot.position =
            initialPosition + Vector3.back * displacement;

        distanceText.text = "Distance : " +
            distance.ToString("F2", CultureInfo.InvariantCulture)
            + " m";

        warningText.text = "PDW : Niveau " + warning;

        switch (warning)
        {
            case 0:
                warningText.color = Color.green;
                break;
            case 1:
                warningText.color = Color.yellow;
                break;
            case 2:
                warningText.color = new Color(1f, 0.55f, 0f);
                break;
            case 3:
                warningText.color = Color.red;
                break;
            default:
                warningText.color = Color.magenta;
                break;
        }

        if (buzzerAudio != null)
            buzzerAudio.mute = buzzer == 0;
    }

    private void SetupBuzzer()
    {
        buzzerAudio = gameObject.AddComponent<AudioSource>();

        int sampleRate = 44100;
        int length = 4410;
        float[] signal = new float[length];

        for (int i = 0; i < length; i++)
        {
            signal[i] = Mathf.Sin(
                2f * Mathf.PI * 880f * i / sampleRate);
        }

        AudioClip clip = AudioClip.Create(
            "PDW_Buzzer", length, 1, sampleRate, false);

        clip.SetData(signal, 0);

        buzzerAudio.clip = clip;
        buzzerAudio.loop = true;
        buzzerAudio.playOnAwake = false;
        buzzerAudio.spatialBlend = 0f;
        buzzerAudio.volume = 0.12f;
        buzzerAudio.mute = true;
        buzzerAudio.Play();
    }

    void OnDisable()
    {
        if (buzzerAudio != null)
            buzzerAudio.Stop();
    }
}
