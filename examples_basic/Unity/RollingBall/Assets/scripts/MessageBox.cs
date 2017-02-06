using UnityEngine;
using UnityEngine.UI;
using System.Collections;

/*
 * This is a custom delegate used in the MessageBox class to return the decision result
 * and execute a method in response.
*/
public delegate void Decision(bool d);

/*
 * A custom class that displays a message box with the "ok" and "cancel" options 
 * and can handle the user response.
*/
public class MessageBox : MonoBehaviour {
	public Text title;
	public Text message;
	public Button ok;
	public Button cancel;
	/*
	 * Init method allow to pass a title and message that will be displayed to the user, 
	 * and also a decision delegate
	*/
	public void init(string title, string message, Decision dec){
		this.title.text = title;
		this.message.text = message;
		cancel.onClick.AddListener(delegate {
			gameObject.SetActive(false);
			dec(false);
		});
		ok.onClick.AddListener(delegate {
			gameObject.SetActive(false);
			dec(true);
		});
		gameObject.SetActive (true);
	}

	/*
	 * On start make invisible the message box
	*/
	void Start () {
		gameObject.SetActive (false);
	}

}
