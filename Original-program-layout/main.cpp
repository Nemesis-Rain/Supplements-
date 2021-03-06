#include <Arduino.h>
#include <SmartLeds.h>

#include "gridui.h"
#include "rbprotocol.h"
#include "rbwebserver.h"
#include "rbwifi.h"

using namespace rb;
using namespace gridui;

// You can include layout.hpp in many .cpp files,
// but ONE of those must have this define before it.
#define GRIDUI_LAYOUT_DEFINITION
#include "layout.hpp"

static Protocol* gProt = nullptr;

static void onPacketReceived(const std::string& cmd, rbjson::Object* pkt) {
    // Let GridUI handle its packets
    if (UI.handleRbPacket(cmd, pkt))
        return;

    // ...any other non-GridUI packets
}

const int LED_COUNT = 8;
const int DATA_PIN = 21;
const int CHANNEL = 0;

// SmartLed -> RMT driver (WS2812/WS2812B/SK6812/WS2813)
SmartLed leds(LED_WS2812, LED_COUNT, DATA_PIN, CHANNEL, DoubleBuffer);

// const int CLK_PIN = 23;
// APA102 -> SPI driver
//Apa102 leds(LED_COUNT, CLK_PIN, DATA_PIN, DoubleBuffer);

void SetLedAll(uint8_t R, uint8_t G, uint8_t B) {
    for (int i = 0; i < LED_COUNT; i++)
        leds[i] = Rgb { R, G, B };
    leds.wait();
    leds.show();
}
// ------------------------------------------------------------------------------------------------

int a = 0, c = -1, slider = 0; // testovaci promenne

void setup() {
    // Initialize WiFi
    WiFi::startAp("LEDLED", "ledkyledky"); // toto je pro vytvoreni vlastni wi-fi, heslo minimalne 8 znaku
    // na tu wi-fi se v mobilu musim prihlasit a potom zapnout RBControl, vyplnit majitele -> Kralova
    // a pak po najeti stranky Ovladani na ni poklepat -> objevi se ovladani, ktere chci

    // Initialize RBProtocol
    gProt = new Protocol("Kralova", "Ovladani", "Compiled at " __DATE__ " " __TIME__, onPacketReceived);
    gProt->start();

    // Start serving the web page
    rb_web_start(80);

    // Initialize the UI builder
    UI.begin(gProt);

    // Build the UI widgets. Positions/props are set in the layout, so most of the time,
    // you should only set the event handlers here.
    auto builder = Layout.begin();

    builder.Checkbox1.onChanged([&c](Checkbox& b) { // to v hranat??ch z??vork??ch se pou????v?? na "chyt??n??" = cappture glob??ln??ch prom??nn??ch zvenku programu, nap????klad [&x] chyt?? prom??nnou x (tj. ??te hodnotu x nastavenou vn?? funkce a kdy?? ji uvnit?? funkce p??ep????e, tak bude vn?? funkce zm??n??n??; [&] chyt?? v??echny prom??nn??  ),
        printf("Checkbox changed: %d\n", (int)b.checked());
        c = (int)b.checked();
    });

    builder.Slider1.onChanged([&slider](Slider& s) {
        printf("Slider changed: %d\n", (int)s.value());
        slider = (int)s.value();
    });

    // Commit the layout. Beyond this point, calling any builder methods on the UI is invalid.
    builder.commit();

    Serial.begin(115200);
}

void loop() {
    for (int g = 0; g < 64; g++) {
        SetLedAll(0, g, g);
        delayMicroseconds(50000); // delay(50);
    }

    for (int g = 64; g >= 0; g--) {
        SetLedAll(0, g, g);
        delayMicroseconds(50000);
    }

    leds.wait();
    leds.show();

    Layout.workLED.setOn(!Layout.workLED.on());
    a++;
    printf("a: %i \n", a); // vypada to, jako kdyby se hlavni smycka do okamziku pripojeni terminalu vubec nerozjela ... smycka ceka na printf ?
    printf("b: %i \n", c);
    printf("slider: %i \n", slider);
}
