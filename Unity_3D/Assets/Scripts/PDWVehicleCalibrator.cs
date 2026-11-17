
using UnityEngine;

public class PDWVehicleCalibrator : MonoBehaviour
{
    public Renderer parkingWall;
    public float targetDistance = 2.5f;

    private bool GetCarBounds(out Bounds bounds)
    {
        bounds = new Bounds();
        bool found = false;

        foreach (Renderer r in GetComponentsInChildren<Renderer>())
        {
            if (!r.enabled || !r.gameObject.activeInHierarchy)
                continue;

            if (!found)
            {
                bounds = r.bounds;
                found = true;
            }
            else
            {
                bounds.Encapsulate(r.bounds);
            }
        }

        return found;
    }

    [ContextMenu("Calibrer la voiture PDW")]
    public void Calibrate()
    {
        if (parkingWall == null || !GetCarBounds(out Bounds car))
        {
            Debug.LogError("Verifier le mur et la voiture.");
            return;
        }

        float wallFaceZ = parkingWall.bounds.max.z;

        float offsetY = -car.min.y;
        float offsetZ = wallFaceZ + targetDistance - car.min.z;

        transform.position += new Vector3(0, offsetY, offsetZ);

        GetCarBounds(out Bounds updated);

        float distance = updated.min.z - parkingWall.bounds.max.z;

        Debug.Log("===== CALIBRATION PDW =====");
        Debug.Log("Largeur : " + updated.size.x.ToString("F2") + " m");
        Debug.Log("Hauteur : " + updated.size.y.ToString("F2") + " m");
        Debug.Log("Longueur : " + updated.size.z.ToString("F2") + " m");
        Debug.Log("Distance voiture-mur : " + distance.ToString("F3") + " m");
    }
}
