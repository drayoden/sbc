#### SMALL BOARD COMPUTER -- pi, arduino, esp32 -- you get it!
---

  ##### monolith clock
    - esp32 testing
    - [ ] - blink
    - [ ] - wifi test -- using wifimanager 
        - NOTE: the redirect to the captive site does not work on Android. Workaround: connect to the
        monolithAP, once connected click on "monlithAP" and then click on "manage router"??? go figuure.
        I did read somewhere that changing the captive site IP address may allow redirection. Will try 
        later.
    - [ ] - neopixel testing
        - [ ] - strandtest example
        - [ ] - neopixel-001 -- simple test, for loop, etc. 
        - [ ] - neopixel-002 -- used the digit array from original instructable -- using 23 LEDS, not 24 (one not used).
        - [ ] - neopixel-003 -- using non-blocking! VERY cool! can send digits to LED array every X seconds while blinking the colon LED every second!
    - [ ] - time testing -- using builtin time library, just had to #include <time.h>.
        - [ ] - time-001 - Worked! -- created the wifiinit() function and used it in setup(). need to see what is up with the day light savings offset, not sure where that comes into play.

        


