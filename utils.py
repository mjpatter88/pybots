#!/usr/bin/python3

import os
import pyToC

def getProcessList():
    # processes = [(id, name)]
    processes = {pid: -1 for pid in os.listdir('/proc') if pid.isdigit()}
    for process in processes.keys():
        with open(os.path.join('/proc', process, 'stat'), 'r') as file:
            line = file.readline()
            name = line[line.index('(')+1 : line.index(')')]
        processes[process] = name
    return processes

def getPID(processName):
    """
    Get the PID of the process with the given name.
    Returns -1 if the process doesn't exist.
    """
    processes = getProcessList()
    for k, v in processes.items():
        if v == processName:
            return k
        else:
            continue
    return -1


def callCFunc():
    # s = pyToC.myFunction()
    # print(s)
    # pyToC.testPerformance()
    pyToC.setupWindow()
    pyToC.updateScreenshot()


if __name__ == '__main__':
    # print(getPID("gnome-mines"))
    callCFunc()
