
# SES Coursework Resit

I have attempted worksheets 2 through 7.  
  

## Worksheet 2

- Included another variable k
- Set up a watchpoint on k
- Printed the stack pointer with `print /x $sp`
- Created a breakpoint command that prints i and continues before main calls inc
- (Above all shown in video)
## Worksheet 3
### Pass exercices:
- Completed all pass exercices, but didn't show in video
    - Logic is visible but commented out in `main.c`
### Credit exercices:
- Created code to de-bounce the switch input, as shown in video
## Worksheet 4
### Pass exercises:
- Wrote `__io_getchar()` routine that takes input and prints it to the screen, as shown in video
### Credit exercises:
- Edited `__io_getchar()` to check for parity, framing, noise and overrun error flags
    - Prints `[ERROR: <error type>]` and flashes the yellow LED when error is detected, as shown in video
## Worksheet 6
### Pass exercises:
- Created small loop that prints `hello world` a few times, as shown in video
### Credit exercises:
- Created a small maths test that generates random questions, using the initial input key as the seed for the RNG engine (as shown in video)
    - Uses `printf()` and `scanf()` to write and read
    - Recognises correct and incorrect answers, asking for another input if an incorrect answer is given
    - Has some input validation; will ask again for an answer if input is invalid (i.e. not a number)
    - (You'll have to uncomment this logic to test it)
- Wrote a `debug_info()` function that prints the stack pointer, heap pointer, and end of data segment (as shown in video)
    - Called every time a new question is generated
    - (Also included within the commented math test logic)
## Worksheet 7
### Pass exercices:
- Changed the `TIM_Period` in `TimerInt.c` to alter the length of the LED flashes to ~1 second (as shown in video)
- In `buttona.c`, configured the button on GPIOC to toggle the green LED (as shown in video)
### Credit exercices:
- In `uart.c`, implemented interrupt driven IO with a buffer of 20 characters
    - As shown in video, a short phrase can be pasted into minicom, and will be printed as the buffer is worked through
    - An artificial delay was added to help visualise this
