#include <ButtonValues.h>

#ifndef NrfMessage_Def
#define NrfMessage_Def

class NrfMessage : public ButtonValues {
public:
  char mode = '0';

  void print();
};

#endif
