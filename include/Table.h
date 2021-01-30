/**
 * @file Table.h
 * @date 29-Jan-2021
 */

#ifndef __TABLE_H__
#define __TABLE_H__

#include "Object.h"
#include "Card.h"

/** Table Class
 */
class Table : public Object
{
public:

                orxU32          GetCardCount() const    {return u32CardCount;}
                orxU32          GetWidth() const        {return u32Width;}
                orxU32          GetHeight() const       {return u32Height;}
                Card ** const   GetCards() const        {return apoCards;}

protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &Info);


private:

                void            Deal();

                Card          **apoCards;
                orxU32          u32CardCount;
                orxU32          u32Width;
                orxU32          u32Height;
};

#endif // __TABLE_H__
