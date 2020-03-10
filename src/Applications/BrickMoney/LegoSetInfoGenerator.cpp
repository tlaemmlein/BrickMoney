#include "LegoSetInfoGenerator.h"

LegoSetInfoGenerator::LegoSetInfoGenerator(QObject *parent) : QObject(parent)
{

}

void LegoSetInfoGenerator::querySetNumber(int num)
{
    //ToDo: Search for lego set information
    // and send it, if it is found.
    emit imageUrl("qrc:/images/WonderWoman.png");
}
