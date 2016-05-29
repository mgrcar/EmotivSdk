using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using Emotiv;

public class playerCtrl : MonoBehaviour {
	public Text countText;
	public float speed, jumpConstant;
	public Camera cam;
	private Rigidbody rb;
	private int count;

	public Text logText;
	EmoEngine engine;

	/*
	 * This method handle the EmoEngine update event, 
	 * if the EmoState has the PUSH action, 
	 * a force is applied to the ball in the direction of the camara
	*/
	void engine_EmoStateUpdated(object sender, EmoStateUpdatedEventArgs e)
	{
		EmoState es = e.emoState;
		if (e.userId != 0) 
			return;
		Debug.Log ("Corrent action: " + es.MentalCommandGetCurrentAction().ToString());
		if (es.MentalCommandGetCurrentAction () == EdkDll.IEE_MentalCommandAction_t.MC_PUSH) {
			Vector3 movement = new Vector3(cam.transform.forward.x, cam.transform.forward.y, cam.transform.forward.z);
			rb.AddForce(movement * speed);
			Debug.Log ("Push");
		}
	}

	/*
	 * Set the handler for update event
	*/
    void Start () {
        rb = GetComponent<Rigidbody>();
		EmoEngine.Instance.EmoStateUpdated += new EmoEngine.EmoStateUpdatedEventHandler(engine_EmoStateUpdated);
	}

	/*
	 * You can also use the up arrow key to move the ball.
	*/
    void FixedUpdate() {
		float moveZ = Input.GetAxis ("Vertical");
		Vector3 movement = new Vector3(cam.transform.forward.x, cam.transform.forward.y, cam.transform.forward.z);
		rb.AddForce(movement * speed * moveZ);

    }

	/*
	 * If the ball collides with an item, it will be disabled 
	 * and a counter will be increased
	*/
	void OnTriggerEnter(Collider other){
		if (other.CompareTag ("pickup")) {
			other.gameObject.SetActive (false);
			count++;
		}
	}

	void SetCountText(){
		countText.text = "Count: " + count;
	}
}
