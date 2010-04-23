#ifndef DBUSERCARD_H
#define DBUSERCARD_H

#include <QSqlRecord>

#include "belenus.h"

class cDBUserCard
{
public:
    cDBUserCard();
    ~cDBUserCard();

    void            load( const unsigned int p_uiId )                                   throw( cSevException );
    void            save() const                                                        throw( cSevException );
    void            createNew()                                                         throw();
    unsigned int    id()                                                                throw();
    unsigned int    userCardTypeId()                                                    throw();
    void            setUserCardTypeId( const unsigned int p_uiUserCardTypeId )          throw();
    string          barCode()                                                           throw();
    void            setBarCode( const string p_stBarCode )                              throw();
    string          comment()                                                           throw();
    void            setComment( const string p_stComment )                              throw();
    unsigned int    units()                                                             throw();
    void            setUnits( const unsigned int p_uiUnits )                            throw();
    string          validDate()                                                         throw();
    unsigned int    validDateYear()                                                     throw();
    void            setValidDateYear( const unsigned int p_uiValidDateYear )            throw();
    unsigned int    validDateMonth()                                                    throw();
    void            setValidDateMonth( const unsigned int p_uiValidDateMonth )          throw();
    unsigned int    validDateDay()                                                      throw();
    void            setValidDateDay( const unsigned int p_uiValidDateDay )              throw();
    int             pinCode()                                                           throw();
    void            setPinCode( const int p_inPinCode )                                 throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiUserCardTypeId;
    string          m_stBarCode;
    string          m_stComment;
    unsigned int    m_uiUnits;
    unsigned int    m_uiValidDateYear;
    unsigned int    m_uiValidDateMonth;
    unsigned int    m_uiValidDateDay;
    int             m_inPinCode;

    void init( const unsigned int   p_uiId = 0,
               const unsigned int   p_uiUserCardTypeId = 0,
               const string         p_stBarCode = "",
               const string         p_stComment = "",
               const unsigned int   p_uiUnits = 0,
               const unsigned int   p_uiValidDateYear = 0,
               const unsigned int   p_uiValidDateMonth = 0,
               const unsigned int   p_uiValidDateDay = 0,
               const int            p_inPinCode = 0 )   throw();
    void init( const QSqlRecord &p_obRecord )           throw();

};

#endif // DBUSERCARD_H
