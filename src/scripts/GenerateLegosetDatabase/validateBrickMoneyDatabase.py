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

    brickMoneyDataBaseUpdated = scriptPath + "/BrickMoneyDatabase/BrickMoneyDatabaseUpdated.csv"
    imageFolderUpdated = scriptPath + "/BrickMoneyDatabase/imagesUpdated"

    h = html2text.HTML2Text()
    # Ignore converting links from HTML
    h.ignore_links = True

    legoDB = list() #[['setNum', 'name_en', 'name_de', 'year', 'rrp']]
    # setNum;name_en;name_de;year;recommendedRetailPrice €
    with open(brickMoneyDataBaseUpdated, 'rb') as file:
        content = file.read().decode("utf8")
        lines = content.splitlines()
        lines.pop(0)
        for line in lines:
            line = h.handle(line)
            cols = line.split(';')
            setNum = int (cols[0])
            name_en = cols[1].replace('\n', ' ')
            name_de = cols[2].replace('\n', ' ')
            year = cols[3]
            rrp = float(cols[4])
            legoDB.append([ setNum, name_en, name_de, year, rrp])
            #sort = sorted(csv1, key=operator.itemgetter(0))
        sort = sorted(legoDB, key=operator.itemgetter(0))
        uniqueSetNum = list()
        setNumSet = set()

        for row in sort:
            if row[0] not in setNumSet:
                setNumSet.add(row[0])
                uniqueSetNum.append(row)
        print(str(len(sort)) + ' ' + str(len(uniqueSetNum)))

        brickMoneyDataBaseUpdated2 = scriptPath + "/BrickMoneyDatabase/BrickMoneyDatabaseUpdated2.csv"
        with codecs.open(brickMoneyDataBaseUpdated2, 'w', encoding='utf8') as newDB:
            newDB.write('setNum;name_en;name_de;year;recommendedRetailPrice €\n')
            for legoSet in uniqueSetNum:
                newDB.write(str(legoSet[0]) + ';' + legoSet[1] + ';' + legoSet[2] + ';' + legoSet[3] + ';' + str(legoSet[4]) + '\n')
        
        for legoSet in uniqueSetNum:
            int(legoSet[3])



if __name__ == "__main__":
    main(sys.argv)