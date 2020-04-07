from html.entities import name2codepoint
from html.parser import HTMLParser
from os import listdir
from os import path
from os.path import isfile, join
from pathlib import Path
from sys import argv, exit
import codecs
import glob
import logging
import operator
import os
import platform
import requests
import shutil
import sys
import urllib.request
import html2text


def main(argv):
    logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

    logging.info("***************************************")
    logging.info("Start")

    scriptPath = os.path.dirname(os.path.realpath(__file__))

    brickMoneyDataBaseUpdated = scriptPath + "/BrickMoneyDatabase/BrickMoneyDatabaseUpdated3.csv"
    with open(brickMoneyDataBaseUpdated, 'rb') as file:
        content = file.read().decode("utf8")
        lines = content.splitlines()
        lines.pop(0)
        for line in lines:
            cols = line.split(';')
            setNum = int(cols[0])
            name_en = cols[1]
            name_de = cols[2]
            year = int(cols[3])
            rrp = float(cols[4])

    print('end')


if __name__ == "__main__":
    main(sys.argv)