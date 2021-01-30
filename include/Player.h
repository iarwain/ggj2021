/**
 * @file Player.h
 * @date 29-Jan-2021
 */

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Object.h"

/** Player Class
 */
class Player : public Object
{
public:


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &Info);


private:
    const       orxSTRING       GetConfigVar(const orxSTRING _zVar) const;
};

#endif // __PLAYER_H__
