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
import os
import platform
import requests
import shutil
import sys
import urllib.request

class MyHTMLParser(HTMLParser):
    def handle_data(self, data):
        if data.find('UVP') >= 0:
            print("Data     :", data)

def extractRRP(htmlSource):
    searchterm = 'UVP: <strong>'
    start = htmlSource.find(searchterm)
    if start >= 0:
        end = htmlSource.find('</strong>', start)
        if end > 0:
            substring = htmlSource[start+len(searchterm):end]
            #print(substring)
            eurostr = substring.replace(',', '.').replace('&euro;', '').strip()
            #print(eurostr)
            rrp = float(eurostr)
            rrpStr = str(rrp)
            return rrpStr
            #print(rrpStr)
    return ''

def extractGermanName(htmlSource):
    searchterm = '<h1>'
    start = htmlSource.find(searchterm)
    if start >= 0:
        end = htmlSource.find('</h1>', start)
        if end > 0:
            substring = htmlSource[start+len(searchterm):end]
            #print(substring)
            name_de = substring.replace('<sup>&reg;</sup>', '®')
            #eurostr = substring.replace(',', '.').replace('&euro;', '').strip()
            #print(eurostr)
            #rrp = float(eurostr)
            #rrpStr = str(rrp)
            return name_de
            #print(rrpStr)
    return ''
    


def main(argv):
    logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

    logging.info("***************************************")
    logging.info("Start")

    scriptPath = os.path.dirname(os.path.realpath(__file__))

    brickMoneyDataBase = scriptPath + "/BrickMoneyDatabase/BrickMoneyDatabase.csv"
    imageFolder = scriptPath + "/BrickMoneyDatabase/images"
    brickMoneyDataBaseUpdated = scriptPath + "/BrickMoneyDatabase/BrickMoneyDatabaseUpdated.csv"
    imageFolderUpdated = scriptPath + "/BrickMoneyDatabase/imagesUpdated"
    if path.isdir(imageFolderUpdated):
      shutil.rmtree(imageFolderUpdated)  
    os.mkdir(imageFolderUpdated)


    with codecs.open(brickMoneyDataBaseUpdated, 'w', encoding='utf8') as newDB:
        newDB.write('setNum;name_en;name_de;year;recommendedRetailPrice €\n')
        with open(brickMoneyDataBase, 'rb') as oldDB:
            content = oldDB.read().decode("utf8")
            lines = content.splitlines()
            for line in lines:
                cols = line.split(';')
                set_num = cols[0]
                eng_name = cols[1]
                year = cols[2]
                print(set_num)
                url = 'https://www.brickmerge.de/' + set_num
                request = requests.get(url)
                if request.status_code == 200:
                    logging.info('Download from ' + url)
                    t = request.text
                    rrp = extractRRP(t)
                    if rrp != '':
                        print(rrp)
                        name_de = extractGermanName(t)
                        newDB.write(set_num + ';' + eng_name + ';' + name_de + ';' + year + ';' + rrp + '\n')
                        newDB.flush()
                        sourceImg = imageFolder + '/' + set_num +'.jpg'
                        targetImg = imageFolderUpdated + '/' + set_num +'.jpg'
                        shutil.copyfile(sourceImg,targetImg)

                else:
                    logging.warning('Does not exist: ' + url)



    # exit(0)

    # url = 'https://www.brickmerge.de/10264'
    # request = requests.get(url)
    # if request.status_code == 200:
    #     logging.info('Download from ' + url)
    #     #logging.info('Content ' + request.text)
    #     t = request.text
    #     #parser = MyHTMLParser()
    #     #parser.feed(t)
    #     #for line in t.splitlines:
    #     #    print(line)
    #     searchterm = 'UVP: <strong>'
    #     start = t.find(searchterm)
    #     if start >= 0:
    #         end = t.find('</strong>', start)
    #         if end > 0:
    #             substring = t[start+len(searchterm):end]
    #             print(substring)
    # else:
    #     logging.warning('Does not exist: ' + url)


    # #Ref: https://stackabuse.com/download-files-with-python/
    # #url = 'https://images.brickset.com/sets/AdditionalImages/75212-1/tn_75212_alt1_jpg.jpg'
    # #imageFilePath = imageFolder + "/brickset_75212.jpg"
    # #urllib.request.urlretrieve(url, imageFilePath)



if __name__ == "__main__":
    main(sys.argv)