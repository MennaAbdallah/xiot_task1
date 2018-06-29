# Project Description :

•	Control one input and output (IO Pins) the input pin is a switch as interrupt once it pressed it should lighting the led connected to output pin.
•	If the switch pressed must send “pressed” to the serial monitor only one time also send the LED states ”ON” or “OFF”.
•	Send continuous serial date comes from temperature sensor with 3 second rate ,at the same time.

# My Thought Process :

## To make the led change its state by switch pressing 
I prefered to use ISR (interrupt service routine) to prevent interfering between other function.
## Temperature Sensor
It should be connected to microcontroller using adc ( analog digital converter) as the degree of temperature is analog and the microcontroller understands only digital signals and to be able to display the degree on the serial monitor.
## Serial Monitor Connection
It wasn't too easy. As I prefer LCD which is more tidy and comfortable in displaying.
### To diplay "Pressed" switch and the state of the led
I've added this part to the ISR of the led toggling.
### To display the temperature each 3 seconds
I used normal timer with an interrupt , as the timer finishes 3 seconds it displays the temperature using serial monitor.

# What I Learned :

1. Integrating header files and code files to use them in main function properly.
2. Write useful comments as much as possible to make code readable.
3. Use interrupt , adc and uart all together in the same project.


