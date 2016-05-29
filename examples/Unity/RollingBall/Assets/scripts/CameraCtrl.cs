using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using Emotiv;

public class CameraCtrl : MonoBehaviour {
    public GameObject player;
	public Text message;
    private Vector3 offset;
	public float rotationY;
	public float sensitivity;

	/*
	 * Get the offset distance between the camera and the ball for a future use
	*/
	void Start () {
        offset = transform.position - player.transform.position;
	}

	
	/*
	 * LateUpdate happens after the ball is drawn in Update.
	 * Here is when we move the camera position using the offset distance and 
	 * fix the camera orientation using the gyroscope coordinates
	*/
	void LateUpdate () {
		int x=0, y=0;
		transform.position = player.transform.position + offset;
		try{
			EmotivCtrl.engine.HeadsetGetGyroDelta ((uint)EmotivCtrl.engineUserID, out x, out y);	
		}
		catch(Emotiv.EmoEngineException e){
			message.text = e.Message;
		}
		transform.RotateAround (player.transform.position, Vector3.up, x*sensitivity);
		offset = transform.position - player.transform.position;
	}
}
