###Arduino-Python implementation for Emotiv SDK
`Contributed by [John Naulty Jr.](https://github.com/jnaulty)`

####For Python dev:

* You need to link to directory with `edk.dll`
* You can reference the files, or insert them in directory running the python script (which is what I did).
* Requires pySerial

1. Upload `emoArduino.ino` to Arduino Board

2. Run Xavier Composer if not connected to device

3. Run Python script `insight_to_arduino.py`

####Python
* `send_emo_state_to_arduino()` is the function in the script that packs a tuple with various Emotiv States.
* `valToArduino(emoStateTuple)` sends the data in the tuple over serial to the Arduino and takes a tuple as the argument.
* [`arduinoCom.py`](https://github.com/jnaulty/insight_sdk/blob/Insight*beta*release/examples/Python/arduinoCom.py) contains utility functions for setting, sending, and receiving data over serial.

####Python API Calls

* `disconnect(self)`

* `connect(self)`

* `get_state(self, eEvent)`

* `get_event_type(self, eEvent)`

* `get_engine_event_emo_state(self, eEvent, eState)`

* `get_userID(self, eEvent, user)`

* `get_time_from_start(self, eState)`

* `get_wireless_signal_status(self, eState)`

* `get_facial_expression_is_blink(self, eState)`

* `get_left_wink(self, eState)`

* `get_right_wink(self, eState)`

* `get_upper_face_action(self, eState)`

* `get_upper_face_action_power(self, eState)`

* `get_lower_face_action(self, eState)`

* `get_lower_face_action_power(self, eState)`

* `get_short_term_excitement_score(self, eState)`

* `get_long_term_excitement_score(self, eState)`

* `get_engagement_boredom_score(self, eState)`

* `get_mental_command_current_action(self, eState)`

* `get_mental_command_current_action_power(self, eState)`

* `lower_facial_expression_states(self, eState)`

* `upper_facial_expression_states(self, eState)`

* `get_surprise(self, eState)`

* `get_frown(self, eState)`

* `get_smile(self, eState)`

* `get_clench(self, eState)`


####Arduino Control From Insight

[`insight_to_arduino.py`](https://github.com/jnaulty/insight_sdk/blob/Insight-beta-release/examples/Python/insight_to_arduino.py) and [`emoArduino.ino`](https://github.com/jnaulty/insight_sdk/blob/Insight-beta-release/examples/Python/Arduino/emoArduino/emoArduino.ino) are the scripts made to allow interfacing between the Emotiv Insight and Arduino Uno.

As of now, the python scripts sends values from the Emotiv Insight API to the Arduino over serial. The Arduino parses those values, assigns them to variables then sends over serial.

* Arduino receives serial and parses out values IN ORDER.

    `Time`, `userID`, `wirelessSigStatus`, `Blink`,

    `leftWink`, `rightWink`, `surprise`, `frown`, `clench`,

    `smile`, `mentalCommandAction`, `mentalCommandPower`.

* Blinky Lights Demo: `blinkLEDs` is a function in the Arduino code that will blink the built in LED on the Arduino Uno based on the time from the `get_time_from_start(self, eState)` call.
