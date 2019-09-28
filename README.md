# Seconds Since Last Injury
An LED counter increments every second. Pressing the button delivers a 48,000 Volt shock and resets the counter to zero. [Hilarity ensues](https://jasoneppink.com/seconds-since-last-injury/).

![Seconds Since Last Injury installation at Night Market 2019](https://github.com/jasoneppink/Seconds-Since-Last-Injury/blob/master/seconds_since_last_injury-night_market-2019.jpg)

## Hardware Requirements
* 4x [7-segment LEDs (12-inches tall, common cathode)](https://szljl.en.alibaba.com/product/60754533420-218297753/Large_12_inch_Black_Face_Red_Emitting_Color_1_Bit_7_Segment_LED_Display.html)
* 1x [Nova Violet Wand (Model SN-B)](https://www.violetwands.com/shop/nova-violetwand-model-snb/)
* 1x [P10 SMD LED Sign 40"x8"](https://www.amazon.com/Scrolling-Display-Message-Solution-Advertising/dp/B077G7D4ZH)
* 1x [Arduino Mega](https://store.arduino.cc/usa/mega-2560-r3)
* 1x [Adafruit Feather 32u4 Basic Proto](https://www.adafruit.com/product/2771)
* 4x [TPIC6C595N bit shifters](https://www.mouser.com/ProductDetail/texas-instruments/tpic6c595n/)
* 28x TIP125 PNP transistors
* 28x 68Ω resistors
* 28x 2KΩ resistors
* 2x 1KΩ resistor
* 1x 22KΩ resistor
* 1x 7.5Ω resistor
* 1x .22uf capacitor
* 2x [IR LEDs](https://www.adafruit.com/product/387)
* 1x [IR Receiver](https://www.adafruit.com/product/157)
* 1x [PN2222 Transistor](https://www.adafruit.com/product/756)
* 5x [proto boards](https://www.adafruit.com/product/571)
* 1x [button](https://www.mouser.com/ProductDetail/e-switch/pv10f2v0ss-311)
* 1x 24V Power Supply
* 1x 9V Power Supply

## Wiring
<img src="https://github.com/jasoneppink/Seconds-Since-Last-Injury/blob/master/wiring-schematic-main.png" width=600px />
<img src="https://github.com/jasoneppink/Seconds-Since-Last-Injury/blob/master/wiring-schematic-IR-transmit.png" width=400px />

## Notes for reducing EMI
* Create a compartment for the Violet Wand, wrap it in aluminum foil, and ground it (earth ground AND via extension cord).
* Wrap cables generously with [ferrite beads](https://www.amazon.com/eBoot-Pieces-Ferrite-Suppressor-Diameter/dp/B01E5E5IY4).
* Keep counter board suspended above (and insulated from) the ground.
* An [Isolation Transformer](https://www.tripplite.com/isolator-series-120v-250w-isolation-transformer-based-power-conditioner-2-outlets~IS250) may or may not be helpful.
