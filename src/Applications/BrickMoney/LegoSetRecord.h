#ifndef LEGOSET_RECORD_H
#define LEGOSET_RECORD_H

#include <QString>

#include <tuple>

using LegoSetRecord = std::tuple<  QString  //00: imageName
                                 , QString  //01: imageFilePath
                                 , int      //02: setnumber
                                 , QString  //03: description
                                 , int      //04: year
                                 , double   //05: rrp
                                 , double   //06: purchasing_price
                                 , QString  //07: Seller
                                 , QString  //08: Purchase date
                                 >;
#endif

