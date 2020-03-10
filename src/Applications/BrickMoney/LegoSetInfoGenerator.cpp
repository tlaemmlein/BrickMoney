#include "LegoSetInfoGenerator.h"

LegoSetInfoGenerator::LegoSetInfoGenerator(QObject *parent) : QObject(parent)
{

}

void LegoSetInfoGenerator::querySetNumber(int num)
{
    emit setNumberNotFound();
}
