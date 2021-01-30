/**
 * @file Card.h
 * @date 29-Jan-2021
 */

#ifndef __CARD_H__
#define __CARD_H__

#include "Object.h"

/** Card Class
 */
class Card : public Object
{
public:

                orxBOOL         IsHidden() const    {return orxObject_IsCurrentAnim(GetOrxObject(), "Hidden");}
                orxBOOL         IsVisible() const   {return orxObject_IsCurrentAnim(GetOrxObject(), "Visible");}
                orxBOOL         IsRevealed() const  {return orxObject_IsCurrentAnim(GetOrxObject(), "Revealed");}


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &Info);


private:
};

#endif // __CARD_H__
