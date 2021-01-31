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

    struct Slot {
        Card       *poCard;
        orxOBJECT  *pstParent;
    };

                Slot           *astSlots                = {};
                orxU32          u32Count                = 0;
                orxU32          u32Width                = 0;
                orxU32          u32Height               = 0;
                orxBOOL         bGameOver               = orxFALSE;
                orxBOOL         bAttract                = orxFALSE;


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_rstInfo);


private:

                void            Deal();
};

#endif // __TABLE_H__
