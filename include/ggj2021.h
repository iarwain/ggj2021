/**
 * @file ggj2021.h
 * @date 27-Jan-2021
 */

#ifndef __ggj2021_H__
#define __ggj2021_H__

#define __NO_SCROLLED__
#include "Scroll.h"

/** Game Class
 */
class ggj2021 : public Scroll<ggj2021>
{
public:


private:

                orxSTATUS       Bootstrap() const;

                void            Update(const orxCLOCK_INFO &_rstInfo);

                orxSTATUS       Init();
                orxSTATUS       Run();
                void            Exit();
                void            BindObjects();


private:
};

#endif // __ggj2021_H__
