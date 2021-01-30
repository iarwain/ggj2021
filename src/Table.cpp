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
    apoCards        = (Card **) orxMemory_Allocate(u32CardCount * sizeof(Card *), orxMEMORY_TYPE_MAIN);

    // Deal cards
    Deal();
}

void Table::OnDelete()
{
    // Clears variables
    orxMemory_Free(apoCards);
}

void Table::Update(const orxCLOCK_INFO &_rstInfo)
{
}

void Table::Deal()
{
    ggj2021 &   roGame = ggj2021::GetInstance();
    Card *      poCard;
    Card *      apoShuffleCards[1024] = {};
    orxU32      u32CardIndex = 0;

    // Deletes all cards
    while((poCard = roGame.GetNextObject<Card>()) != orxNULL)
    {
        roGame.DeleteObject(poCard);
    }

    // Create cards
    orxConfig_PushSection(orxConfig_GetString("Deck"));
    for(orxU32 i = 0; i < u32CardCount; i += 2)
    {
        const orxSTRING zCard = orxConfig_GetString("Cards");
        for(orxU32 j = 0; j < 2; j++)
        {
            do
            {
                u32CardIndex = orxMath_GetRandomU32(0, 1024);
            } while(apoShuffleCards[u32CardIndex] != orxNULL);
            apoShuffleCards[u32CardIndex] = roGame.CreateObject<Card>(zCard);
            orxObject_SetOwner(apoShuffleCards[u32CardIndex]->GetOrxObject(), GetOrxObject());
        }
    }
    orxConfig_PopSection();

    // Position cards
    if(apoShuffleCards[u32CardIndex])
    {
        orxVECTOR   vPadding, vPos, vInit, vSize, vScale;
        orxU32      u32Count;
        orxConfig_GetVector("Padding", &vPadding);
        orxVector_Mul(&vSize, &apoShuffleCards[u32CardIndex]->GetSize(vSize), &apoShuffleCards[u32CardIndex]->GetScale(vScale));
        orxVector_Set(&vPos, -0.5f * orxU2F(u32Width - 1) * (vSize.fX + vPadding.fX), -0.5f * orxU2F(u32Height - 1) * (vSize.fY + vPadding.fY), orxFLOAT_0);
        orxVector_Add(&vPadding, &vPadding, &vSize);
        orxVector_Copy(&vInit, &vPos);

        for(u32CardIndex = u32Count = 0; u32CardIndex < 1024; u32CardIndex++)
        {
            if(apoShuffleCards[u32CardIndex] != orxNULL)
            {
                apoCards[u32Count++] = apoShuffleCards[u32CardIndex];
                apoShuffleCards[u32CardIndex]->SetPosition(vPos);
                if(u32Count % u32Width == 0)
                {
                    vPos.fX = vInit.fX;
                    vPos.fY += vPadding.fY;
                }
                else
                {
                    vPos.fX += vPadding.fX;
                }
            }
        }
    }
}
