# Arduino RC Transmitter

This project started because I was not completely able to decrypt the protocol of a simple RC transmitter which I already owned. I decided to build the electronics by my own, replaced the existing board, and reused the body and its buttons also because I wanted it to be more feature rich.

# Setup
The board has two controllers (Arduino Nano). The right one is listening for input signals (buttons and joysticks) and sends them periodically to the left one which controls the TFT-display and the 2.4GHz antenna.

## Functionality & Features
* The transmitter can be switched **on or off** by using the button in the center.
* There are **6 push buttons** and by using them they appear green on the **tft display**.
* There are **2 joysticks** and there using is also visualized on the tft display.
* There is **RGB LED** below the **2.4G antenna** which shows if the transmitter is switched on and changes the color if it is connected.
* After switching the transmitter on you have to calibrate the joysticks because their behavior depends on battery level.
* The 2 button in the center are for choosing a profile out of the range from A to Z. It is up to the receive how to react on it.
* If you press the 4 outter buttons at the same tinme you will land up in a **menu**. The menu contains **profiles** of different devices to connect to and it allows you to switch between them.  
* If the transmitter is connected to another device it will send it's status frequently. 
```c++
class NrfMessage : public ButtonValues {
public:
  char mode = '0';

  void print();
};

class ButtonValues {
public:
  bool initialized = false;

  bool btn_bottom_right = false;
  bool btn_bottom_left = false;
  bool btn_middle_right = false;
  bool btn_middle_left = false;
  bool btn_top_right = false;
  bool btn_top_left = false;

  unsigned char poti_right_X = 127;
  unsigned char poti_right_Y = 127;
  unsigned char poti_left_X = 127;
  unsigned char poti_left_Y = 127;

  void print();
};
```

# Subprojects
<table>
  <tr><td>
    Project Link: <a href="/Original_Adeept_Smart_Car">Software for Adeept RC Car - Original_Adeept_Smart_Car</a> <br />
    Product Link: <a href="http://www.adeept.com/adeept-remote-control-smart-car-kit-for-arduino-based-on-nrf24l01-24g-wireless-robot-starter-kit-with-pdf-guidebook-tutorial_p0025_s0020.html">Adeept Remote Control Smart Car Kit for Arduino based on NRF24L01 2.4G Wireless, Robot Starter Kit with PDF Guidebook/Tutorial</a>
  </td>
  <td><img src="/Original_Adeept_Smart_Car/pics/0a53ef6784.jpg" width="150"/></td>
</tr>
  
<tr><td>
Project Link: <a href="./4WD_RC_Car_AddeptDriverBoard">Software for 4WD Car - based on Arduino Uno and Adeept Smart Car Motor Driver</a>
  </td>
  <td><img src="/4WD_RC_Car_AddeptDriverBoard/pics/20181220_181335.jpg" width="150"/></td>
</td></tr>
  
</table>

# Pictures
<table><tr>
  <td><img src="/pics/20180218_134300.jpg" width="150"/></td>
  <td><img src="/pics/20180211_222924.jpg" width="150"/></td>
  <td><img src="/pics/20180211_224645.jpg" width="150"/></td>
  <td><img src="/pics/20180203_153620.jpg" width="150"/></td>
</tr><tr>
  <td><img src="/pics/20180211_224707.jpg" width="150"/></td>
  <td><img src="/pics/20180213_222834.jpg" width="150"/></td>
  <td><img src="/pics/20180213_222848.jpg" width="150"/></td>
  <td><img src="/pics/20180128_221520.jpg" width="150"/></td>
  <td><img src="/pics/schema.jpg" width="150"/></td>
</tr></table>
