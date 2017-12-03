#!/usr/bin/python
import curses
import time
import RPi.GPIO as GPIO
import math
import lights
print("Hello Wolrd!")
pin = 17
range_val = 100
value = range_val / 2

def fillFunc(fill):
    return (math.exp(fill) - 1) / (math.exp(1) - 1)

def func(stdscr):
    freq = 100.
    period = 1 / freq
    cursesUpdateTime = 0.003
    setOn = GPIO.HIGH
    setOff = GPIO.LOW
    stdscr.addstr("Hello World!")
    # stdscr.nodelay(True)
    stdscr.refresh()
    xpos = 0
    ypos = 2
    size = stdscr.getmaxyx()
    maxy = size[0]
    maxx = size[1]
    step = 1
    char = "X"
    diff_char = char
    update = True
    while 1:
        if xpos > maxx - 1:
            xpos = maxx - 1
        elif xpos == -1:
            xpos = 0
        if update:
            stdscr.addstr(ypos,xpos,diff_char)
            stdscr.refresh()
            update = False
        c = stdscr.getch()
        strobe = False
        if c == ord('q'):
            lights.set(0)
            break
        elif c == ord("h"):  
            xpos -= step
            diff_char = " "
            update = True
        elif c == ord("l"):
            xpos += step
            diff_char = "X"
            update = True
        elif c == ord (" "):
            stdscr.clear()
        elif c == ord ("s"):
            strobe = True
        elif c == ord ("i"):
            tmp = setOn
            setOn = setOff
            setOff = tmp
        if strobe:
            lights.set(range_val)
            time.sleep(0.1)
            lights.set(0)
        else:
        	value = int(float(range_val) * xpos / (maxx -1))
        	stdscr.addstr(1,0,"%d" % value)
        	lights.set(value) 
            # set the lights
            #startTime = time.time()
            # fill = fillFunc(1.0 * xpos / (maxx - 1))
            # onTime = max(0, period * fill)
            # offTime = max(0, period * (1 - fill) - cursesUpdateTime)
            # GPIO.output(17, setOn)
            # time.sleep(onTime)
            # GPIO.output(17, setOff)
            # stopTime = time.time()
            # #print(startTime - stopTime)
            # #time.sleep(3)
            # time.sleep(offTime)

lights.init(pin, value, range_val)

# Main window function
curses.wrapper(func)
