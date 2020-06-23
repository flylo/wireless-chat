#include <PiezoBuzzer.h>

const int BUZZER_PIN = 9;
PiezoBuzzer::PiezoBuzzer()
{
}

void PiezoBuzzer::buzz()
{
    tone(BUZZER_PIN, 1000);
    delay(500);
    tone(BUZZER_PIN, 600);
    delay(200);
    tone(BUZZER_PIN, 1000);
    delay(100);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, 1000);
    delay(100);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, 1000);
    delay(100);
    noTone(BUZZER_PIN);
}

void PiezoBuzzer::init()
{
    pinMode(BUZZER_PIN, OUTPUT);
}
