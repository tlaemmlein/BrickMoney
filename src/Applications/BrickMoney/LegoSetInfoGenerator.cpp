#include "LegoSetInfoGenerator.h"

LegoSetInfoGenerator::LegoSetInfoGenerator(QObject *parent) : QObject(parent)
{

}

void LegoSetInfoGenerator::querySetNumber(int num)
{
    //ToDo: Search for lego set information
    // and send it, if it is found.

    if ( num == 41599)
    {
        emit imageUrl("qrc:/images/WonderWoman.png");
        emit description("LEGO® BrickHeadz 41599 Wonder Woman 1");
        emit year(2018);
        emit recommendedRetailPrice(9.99);
    }
    else
    {
        QString imageurl = QString("file:///C:/Users/tl/AppData/Roaming/BrickMoney/LegoDatabase/images/brickset%1.jpg").arg(num);
        emit imageUrl(imageurl);
        emit description("LEGO® BrickHeadz 41599 Wonder Woman 2 And then there was a famous person which wants all the stuff of them all");
        emit year(2018);
        emit recommendedRetailPrice(19.99);
    }
}
