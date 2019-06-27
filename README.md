# CPEG 222 Project 2 Stopwatch/Timer Displayer
During this project, you will implement a simple stopwatch/timer displayer. Two seven segment display (SSD) devices will be used to display minutes and seconds. You are supposed to make the display work in either countdown stopwatch or timer mode. Note that timing should be accurate using timer interrupt on chip.
## Goals of this project
* Practice state machine design and implementation.
* Learn the method of displaying digits using PmodSSD
* Learn how to use and coordinate multiple external Pmod devices
* Learn how to use interrupt and timer
## Minimal Hardware
* One chipKIT Pro MX7 Board
* Two PmodSSDs
## Inputs
* Three on-board buttons: Btn1, Btn2 and Btn3

(Hint: To use BTN3, you should disable JTAG first, please check PIC32 reference manual)
## Outputs
* Four on-board LEDs: LD1, LD2, LD3 and LD4
* Two external PmodSSDs
## Functional description
Functionally, our displayer has five operation modes, and all displayed values are in decimal. Note that LD1-LD4 refers to on-board LEDs.
*	Mode 1 – Initial/Ready
    *	LD1-LD4 are ALL ON.
    *	When power on at first, the two SSDs display 00/00, representing 0 minutes and 0 seconds. In other cases of jumping back to mode 1, the two SSDs remain displaying the values from the previous mode, and should NOT reset to 00/00.
    * Pressing Btn3 enters Mode 2, pressing Btn2 enters Mode 4, while pressing Btn1 enters Mode 5. Actions upon pressing each button should take place immediately without any delay.
* Mode 2 –Set Min
    *	LD2 is ON, while other onboard LDs are OFF.
    *	Initially, the two SSDs remain displaying the values from mode 1.
    *	Every time when Btn2 is pressed, the value on the left SSD (minutes) increases by 1. The value rotates between 00 and 99. For example, if the current number is 98 and you want to set it to 02, you need to press Btn2 for 4 times to make it go through 98→99→00→01→02. Note that the value on the right SSD (seconds) retains during this process.
    *	Every time when Btn1 is pressed, the value on the left SSD (minutes) deceases by 1. The value rotates between 00 and 99. For example, if the current number is 02 and you want to set it to 98, you need to press Btn1 for 4 times to make it go through 02→01→00→99→98. Note that the value on the right SSD (seconds) retains during this process.
    *	Pressing Btn3 enters Mode 3 immediately.
*	Mode 3 – Set Sec
    *	LD1 is ON, while other onboard LDs are OFF.
    *	Initially, the two SSDs remain displaying the values from mode 2.
    *	Every time when Btn2 is pressed, the value on the right SSD (seconds) increases by 1. The value rotates between 00 and 59. For example, if the current number is 58 and you want to set it to 02, you need to press Btn2 for 4 times to make it go through 58→59→00→01→02. Note that the value on the left SSD (minutes) retains during this process.
    *	Every time when Btn1 is pressed, the value on the right SSD (seconds) deceases by 1. The value rotates between 00 and 59. For example, if the current number is 02 and you want to set it to 58, you need to press Btn1 for 4 times to make it go through 02→01→00→59→58. Note that the value on the left SSD (minutes) retains during this process.
    *	Pressing Btn3 enters Mode 1 immediately.
* Mode 4 – Timer/Count up
    *	LD3 and LD4 are OFF, LD1 and LD2 are ON.
    *	The seconds displayer (right SSD) will increase by one each time automatically. The update serial should be ranging from 00 to 59. For example, if the current number is 00, next time it would be 01; and if current number is 59, next time it would be 00, while the minute’s displayer (left SSD) will increase by one accordingly (Note: if current minute’s displayer is 99, increasing one will turn to be 00.)
    *	The increment frequency should be exactly 1Hz (every one second), controlled by timer interrupt.
    * When Btn2 is pressed, the increment process pauses. It resumes when Btn2 is pressed for another time.
    *	Pressing Btn3 enters Mode 1 immediately. Pressing Btn1 has not effects.
*	Mode 5 – Stopwatch/Countdown 
    *	LD1 and LD2 are OFF, LD3 and LD4 are ON.
    *	The seconds displayer (right SSD) will decrease by one each time automatically. The update serial should be ranging from 00 to 59. For example, if the current number is 02, next time it would be 01; and if current number is 00, next time it would be 59, while the minute displayer (left SSD) will decrease by one accordingly (Note: if current minute display is 00, stopwatch has to stop.)
    *	The decrement frequency should be exactly 1Hz (every one second), controlled by timer interrupt.
    *	When Btn1 is pressed, the decrement process pauses. It resumes when Btn1 is pressed for another time.
    *	Pressing Btn3 enters Mode 1 immediately. Pressing Btn2 has not effects.

## A sample SSD displayer
*	You are provided with a well-documented template to start with. It uses Btn1 to choose either the left digit or the right digit for display on a PmodSSD. The template code can be downloaded from Sakai.
## Display quality requirements
*	No flickering of display of PmodLED or SSDs. All 4 digits of two SSDs should be always illuminated without observable flickering, and the luminance of each digit should be the same (at least cannot see the difference with eyes). If the value is 0, you should also display 00/00, other than single 0.
*	At all modes, when display value changes, both PmodSSDs should stay lit for a noticeable amount of time and then change at the same time. 
*	The update frequency should be exactly 1Hz.
*	The display mode changes or value updates as soon as a button is pressed. However, no matter how long a button is pressed, the display or mode changes at most once per each button press.
## Customer Satisfaction Requirements
Any of the following may account for deduction of your score: 
*	Incorrect/unexpected operation
*	Unfamiliarity with any aspect of your project
*	Showing up late for your demo
*	Poorly presented demo
*	Sloppy, undocumented program code
## Questions (to be submitted together with the flow chart)
*	Each PmodSSD is supposed to display two digits at the same time. For each digit, seven inputs are required to control all the seven segments. So it seems that fourteen inputs are needed for displaying two digits. However, if you check the pins of your PmodSSD, you will find that there are only 8 inputs available for each PmodSSD (despite GND and VCC). Using these limited 8 inputs, how would you display both digits at the same time (or make both digits look like displaying at the same time)? Please write down your solution and provide a brief explanation. (Hint: please check offered SSD reference manual, it is related with human virtual feature and device frequency)

*	When you finish setting the minutes and seconds, your displayer enters mode 4 or 5 and displays and updates the values. Obviously you need a subroutine to compute the updated value for minutes and seconds. Please write a function void Compute_value (int old_minutes, int old_seconds) in C code to implement this function. Suppose you are in mode 4 or 5, and it is the moment to increase/decrease the value by one, how the function code should be to make sure all corner cases are considered. Here the parameter (int old_minutes, int old_seconds) are the current displayed values, before updating. Assume int minute and int seconds representing time, and int halt which is set to 1 to stop the stopwatch, as global variables within the scope of main function, to be updated after function is called.

*	In previous projects, our setting of the frequency of counting/LED blinking was very inaccurate. In this project, you will learn to precisely control the frequency via timer interrupt. Assuming the MCU is using an 80MHz (80*106) system clock (SYSCLK) and 40MHz peripheral bus clock (PBCLK), how to configure the Timer interrupt to reach an interrupt frequency of 4Hz? Please show the necessary initialization and configuration codes with comments (you can refer to Section 14 of the reference manual for more information).  You can choose any timer (timer 1-5) as example.
## Mid-stage checkpoint (in class device demo)
As a mid-stage checkpoint, show that you are able to display 2 digits on one SSD at the same time. Note that your display should have no obvious observable flickering on digits and all digits should have the same luminance. Button pressing would not affect displaying at all.
*	Initially, all on board LEDs are ON and 00 is expected to be displayed on PMOD SSD. Then each time you press btn2, 2-digit decimal number increases by 1. Then each time you press btn1, 2-digit decimal number decrease by 1. For 99, increasing by 1 will result in 00, and for 00 decreasing by 1 will result in 99. 
*	When you press btn3, only LD1 is expected to be ON among on board LEDs and the value shown on PMOD SSD will increase by one periodically. The update frequency is not constrained and don’t need to be accurate at that time, but it should be around 1-2Hz.  






