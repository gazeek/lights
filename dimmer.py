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
# settings_tab = [30, 35,40, 45,50,55,60,65,70,75,80,85,90,100]
# settings_tab = range(30,70,2) + range(73,100,3) #[100]
settings_tab = range(30,100,7)
settings = [-1,13.403406,21.530804,28.454408,34.361889,39.286767,43.339989,46.942958,50.319221,53.511905,56.707125,59.663578,62.156546,64.478098,66.687709,68.57353,70.390296,71.91995,73.61323,74.860109,76.106988,77.261583,78.2837,79.176996,80.4164,81.234872,82.055779,82.752564,83.617061,83.995056,84.586033,85.1729862,85.7599394,86.3468926,86.9338458,87.520799,87.9798978,88.4389966,88.8980954,89.3571942,89.816293,90.2498612,90.6834294,91.1169976,91.5505658,91.984134,92.2605502,92.5369664,92.8133826,93.0897988,93.366215,93.6500124,93.9338098,94.2176072,94.5014046,94.785202,95.072776,95.36035,95.647924,95.935498,96.223072,96.4100892,96.5971064,96.7841236,96.9711408,97.158158,97.310116,97.462074,97.614032,97.76599,97.917948,97.9670674,98.0161868,98.0653062,98.1144256,98.163545,98.2597682,98.3559914,98.4522146,98.5484378,98.644661,100]

useful_range = 29+len(settings_tab)
useful_range=99
print(useful_range)

def getFillValue(setting):
    return setting
    if setting < 29:
        return setting
    else:
        return settings_tab[setting - 29]
# print(getFillValue(42))

def getFillValue2(setting):
    fillVal = next(x[0] for x in enumerate(settings) if x[1] >= setting)
    return fillVal

def sweeper(stdscr):
    flag = True
    stdscr.nodelay(True)
    change = 1
    value = 1
    while flag:
        c = stdscr.getch()
        if c == ord("q"):
            flag = False
            break
        fillVal = getFillValue2(value)
        # stdscr.addstr(1,0,"%d fill: %d" % (value, fillVal))
        lights.set(fillVal)
        time.sleep(0.01)
        if value == useful_range-1:
            change = -change
        if value == 0:
            change = -change
        value += change

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
    toggle_val = 1
    toggling = 0
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
        elif c == ord("z"):
            sweeper(stdscr)
        elif c == ord ("i"):
            tmp = setOn
            setOn = setOff
            setOff = tmp
        elif c == ord("c"):
            toggling = 1
            if toggle_val:
                value = 80
                stdscr.addstr(1,0,"%d" % value)
                lights.set(value)
                toggle_val = 0
            else:
                value = 100
                stdscr.addstr(1,0,"%d" % value)
                lights.set(value)
                toggle_val = 1
        if strobe:
            lights.set(range_val)
            time.sleep(0.1)
            lights.set(0)
        elif not toggling:
            value = min(useful_range-1, int(float(useful_range-1) * xpos / (maxx -1)))
            fillVal = getFillValue(value)
            stdscr.addstr(1,0,"%d\tfill:%d" % (value,fillVal))
            lights.set(fillVal) 
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
