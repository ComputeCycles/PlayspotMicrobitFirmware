/*
1/24/18 - rvs

Follow instructions here to install required build tools:

    https://lancaster-university.github.io/microbit-docs/offline-toolchains/#installation-on-mac-osx

Basically you are getting the python "yotta" tool installed.

Because you need to adjust the defaults for BLE, Then copy MicroBitConfig.h.settings to:

    {project root}/yotta_modules/microbit-dal/inc/core/MicroBitConfig.h

Then in the root dir of the project do:

    yt target bbc-microbit-classic-gcc && yt clean && yt build

Then drop:

    {project root}/build/bbc-microbit-classic-gcc/source/microbit-samples-combined.hex

on the mount that pops up when you plug the microbit into your computer

 */

#include "MicroBit.h"
#include "MicroBitUARTService.h"

MicroBit uBit;
MicroBitImage currentFrame;

MicroBitImage cross("255,0,0,0,255\n0,255,0,255,0\n0,0,255,0,0\n0,255,0,255,0\n255,0,0,0,255\n");
MicroBitImage check("0,0,0,0,255\n0,0,0,255,0\n255,0,255,0,0\n0,255,0,0,0\n0,0,0,0,0\n");

int connectedState = 0;
 
void onConnected(MicroBitEvent)
{
    connectedState = 1;
    uBit.display.print(check, 0, 0, 0);
}

void onDisconnected(MicroBitEvent)
{
    connectedState = 0;
    uBit.display.print(cross, 0, 0, 0);
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    // wake();
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);

    new MicroBitAccelerometerService(*uBit.ble, uBit.accelerometer);
    new MicroBitButtonService(*uBit.ble);
    new MicroBitLEDService(*uBit.ble, uBit.display);
    new MicroBitMagnetometerService(*uBit.ble, uBit.compass);
    new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);

    uBit.display.print(connectedState == 1 ? check : cross, 0, 0, 0);
  
    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
