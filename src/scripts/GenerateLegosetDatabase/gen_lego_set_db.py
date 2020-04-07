"""
gen_lego_set_db.py 
"""

from sys import argv, exit
from pathlib import Path
from os import path
from os import listdir
from os.path import isfile, join
import os
import platform
import sys
import glob
import logging
import urllib.request
import requests

def main(argv):
    logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

    logging.info("***************************************")
    logging.info("Start gen_lego_set_db.py")

    scriptPath = os.path.dirname(os.path.realpath(__file__))

    imageFolder = scriptPath + "/BrickMoneyDatabase/images"

    bricksetDatabase = scriptPath +"/BricksetDatabase/sets.csv"

    brickMoneyDataBase = scriptPath + "/BrickMoneyDatabase/BrickMoneyDatabase.csv"

    with open(brickMoneyDataBase, 'w') as brickMoneyDataBaseFile:
        brickMoneyDataBaseFile.write('setNum;name;year;recommendedRetailPrice\n')
        with open(bricksetDatabase, 'rb') as file:
            content = file.read().decode("utf8")
            lines = content.splitlines()
            for line in lines:
                cols = line.split(',')
                pre_set_num = cols[0]
                set_num = pre_set_num.rpartition('-')[0]
                print(set_num)
                urlsToCheck = [   'https://images.brickset.com/sets/AdditionalImages/' + set_num + '-1/tn_' +set_num+'_alt1_jpg.jpg'
                                , 'https://images.brickset.com/sets/AdditionalImages/' + set_num + '-1/tn_' +set_num+'_Box1_V29_jpg.jpg'
                                , 'https://www.brickmerge.de/img/sets/s/LEGO_' + set_num +'_alt1.jpg' ]
                for url in urlsToCheck:
                    request = requests.get(url)
                    if request.status_code == 200:
                        logging.info('Download from ' + url)
                        imageFilePath = imageFolder + '/' + set_num +'.jpg'
                        urllib.request.urlretrieve(url, imageFilePath)
                        brickMoneyDataBaseFile.write(set_num + ';' + cols[1] + ';' + cols[2] + ';\n')
                        brickMoneyDataBaseFile.flush()
                        break
                    else:
                        logging.warning('Does not exist: ' + url)



    # https://images.brickset.com/sets/AdditionalImages/75212-1/tn_75212_alt1_jpg.jpg
    # https://images.brickset.com/sets/AdditionalImages/10264-1/tn_10264_Box1_v29_jpg.jpg
    # https://www.brickmerge.de/img/sets/s/LEGO_75885_alt1.jpg

    #Ref: https://stackabuse.com/download-files-with-python/
    #url = 'https://images.brickset.com/sets/AdditionalImages/75212-1/tn_75212_alt1_jpg.jpg'
    #imageFilePath = imageFolder + "/brickset_75212.jpg"
    #urllib.request.urlretrieve(url, imageFilePath)



if __name__ == "__main__":
    main(sys.argv)