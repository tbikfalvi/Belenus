#ifndef DBPRODUCT_H
#define DBPRODUCT_H

#include <QSqlRecord>

#include "../framework/sevexception.h"

class cDBProduct
{
public:
    cDBProduct();
    ~cDBProduct();

    void            load( const unsigned int p_uiId )                                   throw( cSevException );
    void            save()                                                              throw( cSevException );
    void            createNew()                                                         throw();
    unsigned int    id()                                                                throw();
    string          name()                                                              throw();
    void            setName( const string &p_stName )                                   throw();
    string          barcode()                                                           throw();
    void            setBarcode( const string &p_stBarcode )                             throw();
    int             priceIn()                                                           throw();
    void            setPriceIn( const int &p_inPriceIn )                                throw();
    int             priceOut()                                                          throw();
    void            setPriceOut( const int &p_inPriceOut )                              throw();
    int             count()                                                             throw();
    void            setCount( const int &p_inCount )                                    throw();

private:
    unsigned int    m_uiId;
    string          m_stName;
    string          m_stBarCode;
    int             m_inPriceIn;
    int             m_inPriceOut;
    int             m_inCount;

    void init( const unsigned int p_uiId = 0,
               const string &p_stName = "",
               const string &p_stBarCode = "",
               const int p_inPriceIn = 0,
               const int p_inPriceOut = 0,
               const int p_inCount = 0 )               throw();
    void init( const QSqlRecord &p_obRecord )          throw();

};

#endif // DBPRODUCT_H
