
#ifndef ButtonValues_Def
#define ButtonValues_Def

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

#endif
