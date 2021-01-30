/**
 * @file Table.cpp
 * @date 29-Jan-2021
 */

#include "Table.h"

void Table::OnCreate()
{
    // Inits variables
    orxConfig_SetBool("IsTable", orxTRUE);
    u32CardCount    = orxConfig_GetU32("CardCount");
    u32Width        = ((orxU32)orxMath_Sqrt(orxU2F(u32CardCount))) & ~1;
    u32Height       = u32CardCount / u32Width;
    u32CardCount    = u32Width * u32Height;
}

void Table::OnDelete()
{
}

void Table::Update(const orxCLOCK_INFO &_rstInfo)
{
}
