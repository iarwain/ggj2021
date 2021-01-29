/**
 * @file Card.cpp
 * @date 29-Jan-2021
 */

#include "Card.h"

void Card::OnCreate()
{
    orxConfig_SetBool("IsCard", orxTRUE);
}

void Card::OnDelete()
{
}

void Card::Update(const orxCLOCK_INFO &_rstInfo)
{
}
