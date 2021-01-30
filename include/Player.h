/**
 * @file Player.h
 * @date 29-Jan-2021
 */

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Object.h"
#include "Table.h"

/** Player Class
 */
class Player : public Object
{
public:

    struct Hand {
        Object     *poHand;
        orxU32      u32SlotIndex;
    };

    const       Table          *poTable             = {};
    const       orxSTRING       zTarget             = {};
                Hand            astHands[2]         = {};

                void            Select(orxU32 _u32HandIndex, orxU32 _u32SlotIndex = orxU32_UNDEFINED);


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_Info);


private:
    const       orxSTRING       GetConfigVar(const orxSTRING _zVar) const;

                orxBOOL         bBusy               = {};
};

#endif // __PLAYER_H__
