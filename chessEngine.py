from stockfish import Stockfish
import serial
import io
import re
import serial.tools.list_ports
import os
import pathlib

stockfish = Stockfish("stockfish_20090216_x64")
ports = serial.tools.list_ports.comports()
ports = sorted(ports)
ser = serial.Serial(ports[0].device, 230400, timeout = 1)
ser.flushInput()
ser.flushOutput()
while(True):
    if ser.in_waiting != 0:
        serBytes = ser.readline()
        result = str(serBytes, 'iso-8859-1')
        serBytes = ser.read(8192)
        ser.flushInput()
        ser.flushOutput()
        if result[0:3] == "FEN":
            stockfish.set_fen_position(result[4:])
            print(stockfish.get_board_visual())
            bestMove = stockfish.get_best_move_time(300)
            print(bestMove)
            ser.write(bestMove.encode())
