#ifndef PIEZO_BUZZER_H
#define PIEZO_BUZZER_H
#include <Arduino.h>

class PiezoBuzzer
{

public:
    PiezoBuzzer();
    void buzz();
    void init();
};

#endif
