
#include "sqlResult.h"



QByteArray * SqlResult::create(QSqlResult &sql)
{
    return 0;
}



SqlResult::SqlResult(QByteArray &b)
{
}



SqlResult::~SqlResult()
{
}



bool SqlResult::isValid()
{
    return false;
}



int SqlResult::size()
{
    return 0;
}



const char* SqlResult::fieldName(int i)
{
    return 0;
}



QVariant SqlResult::value(int i)
{
    return QVariant();
}



bool SqlResult::first()
{
    return false;
}



bool SqlResult::last()
{
    return false;
}



bool SqlResult::next()
{
    return false;
}



bool SqlResult::prev()
{
    return false;
}



bool SqlResult::seek(int i, bool relative)
{
    return false;
}
