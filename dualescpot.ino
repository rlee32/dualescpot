#include "Component.h"
#include "Esc.h"
#include "MomentaryPushButton.h"
#include "Pin.h"
#include "Potentiometer.h"
#include "RgbLed.h"

// Needs the extra pins provided by Arduino Mega

constexpr int EscLevels = 25;
OutputPin esc0out(10);
PowerPin esc0power(9);
GroundPin esc0ground(8);
Esc<EscLevels> esc0(esc0out, esc0power, esc0ground);
OutputPin esc1out(11);
PowerPin esc1power(12);
GroundPin esc1ground(13);
Esc<EscLevels> esc1(esc1out, esc1power, esc1ground);

InputPin buttoninput(14);
PowerPin buttonpower(15);
GroundPin buttonground(16);
MomentaryPushButton button(buttoninput, buttonpower, buttonground);

OutputPin red0(17);
PowerPin ledpower0(18);
OutputPin green0(19);
OutputPin blue0(20);
RgbLed<PowerPin> led0(red0, ledpower0, green0, blue0);
OutputPin red1(21);
PowerPin ledpower1(22);
OutputPin green1(23);
OutputPin blue1(24);
RgbLed<PowerPin> led1(red1, ledpower1, green1, blue1);

PowerPin pot0power(A0);
InputPin pot0input(A1);
GroundPin pot0ground(A2);
Potentiometer pot0(pot0power, pot0input, pot0ground, 23, 1000, 0, EscLevels - 1);
PowerPin pot1power(A3);
InputPin pot1input(A4);
GroundPin pot1ground(A5);
Potentiometer pot1(pot1power, pot1input, pot1ground, 23, 1000, 0, EscLevels - 1);

void setup()
{
    led0.setup();
    led1.setup();
    button.setup();
    esc0.setup();
    esc1.setup();
    pot0.setup();
    pot1.setup();
    Component::CheckSetup();
    Pin::ValidateSingleUsage();
    // Serial.begin(9600);
    // esc.printLevels();
}

void ThrottleRoutine(
    const Potentiometer& pot, Esc<EscLevels>& esc, const RgbLed<PowerPin>& led)
{
    int level = pot.readScaled();
    level = esc.throttle(level);
    if (level == EscLevels - 1)
    {
        led.blue();
    }
    else if (level > EscLevels / 2)
    {
        led.magenta();
    }
    else if (level > 0)
    {
        led.cyan();
    }
    else if (level == 0)
    {
        led.yellow();
    }
}

void loop()
{
    if (button.high())
    {
        ThrottleRoutine(pot0, esc0, led0);
        ThrottleRoutine(pot1, esc1, led1);
    }
    else
    {
        esc0.off();
        esc1.off();
        led0.red();
        led1.red();
    }
}
