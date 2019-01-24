#include "MicroBit.h"
#include "MicroBitUARTService.h"

MicroBit uBit;
MicroBitImage currentFrame;

const char *wakeAnim[] = {
    "0,0,0,0,0\n0,0,0,0,0\n0,0,1,0,0\n0,0,0,0,0\n0,0,0,0,0\n",
    "0,0,0,0,0\n0,1,1,1,0\n0,1,1,1,0\n0,1,1,1,0\n0,0,0,0,0\n",
    "1,1,1,1,1\n1,1,1,1,1\n1,1,1,1,1\n1,1,1,1,1\n1,1,1,1,1\n"
};
 
void wake()
{
    uBit.display.setBrightness(0);
    // Turn on all pixels.
    for(int y=0; y<5; y++) {
        for(int x=0; x<5; x++) {
            uBit.display.image.setPixelValue(x, y, 1);
        }
    }
    
    // Fade in all LEDs.
    for(int i=0; i<255; i++) {
        uBit.display.setBrightness(i);
        uBit.sleep(5);
    }
    // Fade out all LEDs.
    for(int i=255; i>0; i--) {
        uBit.display.setBrightness(i);
        uBit.sleep(5);
    }
    
    // Set brightness back to full and clear screen.
    uBit.display.image.clear();
    uBit.display.setBrightness(255);
    
    // Pulsing animation.
    int animDelay = 50;
    for(int j=0; j<15; j++) {
        for(int i=0; i<3; i++) {
            currentFrame = MicroBitImage(wakeAnim[i]);
            uBit.display.print(currentFrame,0,0,0,animDelay);
        }
        for(int i=2; i>-1; i--) {
            currentFrame = MicroBitImage(wakeAnim[i]);
            uBit.display.print(currentFrame,0,0,0,animDelay);
        }
        animDelay -= 3;
    }
    
    // Fade out last dot.
    for(int i=255; i>=0; i--) {
        uBit.display.setBrightness(i);
        uBit.sleep(1);
    }
    
    // Clear display and set brightnes back to full.
    uBit.display.image.clear();
    uBit.display.setBrightness(255);
    
    uBit.sleep(500);
    
}

void onConnected(MicroBitEvent)
{
    uBit.display.print("C");
}

void onDisconnected(MicroBitEvent)
{
    uBit.display.print("D");
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    // wake();
    uBit.display.scroll("PLAYSPOT!");
    uBit.display.print("D");

    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);

    new MicroBitAccelerometerService(*uBit.ble, uBit.accelerometer);
    new MicroBitButtonService(*uBit.ble);
    new MicroBitLEDService(*uBit.ble, uBit.display);
    new MicroBitMagnetometerService(*uBit.ble, uBit.compass);
    new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
