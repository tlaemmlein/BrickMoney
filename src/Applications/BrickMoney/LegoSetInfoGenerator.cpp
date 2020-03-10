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
    }
    else
    {
        emit imageUrl("qrc:/images/Empty.svg");
    }
}
