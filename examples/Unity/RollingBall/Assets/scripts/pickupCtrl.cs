using UnityEngine;
using System.Collections;

public class pickupCtrl : MonoBehaviour {
	
	/*
	 * This method make the animation of the items in the field.
	*/
	void Update () {
        transform.Rotate(new Vector3(20.0f, 15.0f, 45.0f) * Time.deltaTime);
	}
}
