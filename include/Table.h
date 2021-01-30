/**
 * @file Table.h
 * @date 29-Jan-2021
 */

#ifndef __TABLE_H__
#define __TABLE_H__

#include "Object.h"

/** Table Class
 */
class Table : public Object
{
public:


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &Info);


private:

                orxU32          u32CardCount;
                orxU32          u32Width;
                orxU32          u32Height;
};

#endif // __TABLE_H__
