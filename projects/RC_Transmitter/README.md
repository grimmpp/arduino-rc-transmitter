# Arduino RC Transmitter

I wanted to reuse an existing remote control, which I purchased a couple years ago, for steering the smart remote car from [Adeept](http://www.adeept.com). I wanted to try out and hack the protocol from the [Syma X5C Remote Controller](http://www.symatoystore.com/New-arrival-hot-sale-Syma-X5-X5C-X5C-1-2-4G-New-Version-rc-helicopter-Spare-Part-Transmitter-Mode2?filter_name=x5c%20transmitter). The decryption of the protocol and communication to the smart car worked but unfortunately it was very unstable. Therefore I decided to only reuse the body and buttons of the original remote control and replaced all inner parts. 
I created the electronics completely new by my own and placed two Arduino Nano controllers ([Arduino Nano Pinout](https://simba-os.readthedocs.io/en/latest/_images/arduino-nano-pinout.png)) on it.

<a href="/projects/RC_Transmitter/pics/20170912100400_84941.jpg">
<img src="/projects/RC_Transmitter/pics/20170912100400_84941_small.jpg"/> </a>

<a href="/projects/RC_Transmitter/pics/20180218_134300.jpg">
<img src="/projects/RC_Transmitter/pics/20180218_134300_small.jpg"/></a>

<a href="/projects/RC_Transmitter/pics/20180213_222848.jpg">
<img src="/projects/RC_Transmitter/pics/20180213_222848_small.jpg"/></a>

<a href="/projects/Original_Adeept_Car_Kit_from2016/pics/RC-car.jpg">
<img src="/projects/Original_Adeept_Car_Kit_from2016/pics/RC-car_small.jpg"/></a>

<a href="/projects/RC_Transmitter/pics/schema.jpg">
<img src="/projects/RC_Transmitter/pics/schema_small.jpg"/></a>

Further picture about this project can be found in [here](/projects/RC_Transmitter/pics).

## Setup
The board has two controllers (Arduino Nano). The right one is listening for input signals (buttons and joysticks) and sends their state periodically to the left controller which controls the TFT-display and the 2.4GHz antenna.

## Functionality & Features
* The transmitter can be switched **on or off** by using the button in the center.
* There are **6 push buttons** and by using them they appear green on the **tft display**.
* There are **2 joysticks** and there using is also visualized on the tft display.
* There is **RGB LED** below the **2.4G antenna** which shows if the transmitter is switched on and changes the color if it is connected.
* After switching the transmitter on you have to calibrate the joysticks because their behavior depends on battery level.
* The 2 button in the center are for choosing a profile out of the range from A to Z. It is up to the receive how to react on it.
* If you press the 4 outer buttons at the same time you will land up in a **menu**. The menu contains **profiles** of different devices to connect to and it allows you to switch between them.  
* If the transmitter is connected to another device it will send it's status frequently. 

## Shared Message Schema

The following are among others the classes which are shared between remote device and remote controller. The libraries can be found in the [shared lib folder](/lib). All files in here will be consider and shared during the build of each project.

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
