from random import randint
from asciimatics.screen import Screen
import time

def demo(screen):
    for i in range(1,12953):
        with open("output/output"+str(i)+".txt",'r') as f:
            lines = f.readlines()
            count = 0;
            for line in lines:
                screen.print_at(line,0,count)
                count = count + 1
            screen.refresh()

Screen.wrapper(demo)
