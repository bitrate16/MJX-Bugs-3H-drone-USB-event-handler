# MJX Bugs 3H drone USB event handler
 Utility that allows to parse drone's microusb port output codes to handle photo button and telemetry.
 
 MJX allows to control their official action cameras via MJX Bugs 3H transmitter photo button but when you don't have one of these cameras, you have nothong for to use that port.
 
 In this case you can use this small Arduino sketch to set it up for your drone and handle transmitter sending signals whenever Photo button is short or long pressed. Use custom micro usb plug, connect VCC to VCC, GND to GND and DATA- (Green wire) to PIN 8 in arduino micro. Check for codes match the photo button and enjoy.
