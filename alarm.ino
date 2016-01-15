/*
PORTABLE SIREN
Arduino-Producing a wailing sound as a loud warning signal
LED Flsher for Visual Alert
source: http://www.electroschematics.com/9548/arduino-siren-sound-alarm/
*/

void play_alarm()
{
    int i = 0;
    int state = 0;
    for(i = 0; i < 255; i = i + 2)
    {
        analogWrite(LED_PIN, i);
        analogWrite(SPEAKERS_PIN, i);
        delay(10);
    }
    for(i = 255; i > 1; i = i - 2)
    {
        analogWrite(LED_PIN, i);
        analogWrite(SPEAKERS_PIN, i);
        delay(5);
    }
    for(i = 1; i <= 10; i++)
    {
        analogWrite(LED_PIN, 255);
        analogWrite(SPEAKERS_PIN, 200);
        delay(100);
        analogWrite(LED_PIN, 0);
        analogWrite(SPEAKERS_PIN, 25);
        delay(100);
    }
    analogWrite(SPEAKERS_PIN, 0);
}

