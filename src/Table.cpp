/**
 * @file Table.cpp
 * @date 29-Jan-2021
 */

#include "Table.h"
#include "Player.h"

void Table::OnCreate()
{
    // Init variables
    orxConfig_SetBool("IsTable", orxTRUE);
    u32Count    = orxConfig_GetU32("CardCount");
    u32Width    = ((orxU32)orxMath_Ceil(orxMath_Sqrt(orxU2F(u32Count)))) & ~1;
    u32Height   = u32Count / u32Width;
    u32Count    = u32Width * u32Height;
    bGameOver   = orxFALSE;
    astSlots    = (Slot *) orxMemory_Allocate(u32Count * sizeof(Slot), orxMEMORY_TYPE_MAIN);

    // Deal cards
    Deal();
}

void Table::OnDelete()
{
    // Clear variables
    orxMemory_Free(astSlots);
}

void Table::Update(const orxCLOCK_INFO &_rstInfo)
{
    if(!bGameOver)
    {
        // Check cards
        bGameOver = orxTRUE;
        for(orxU32 i = 0; i < u32Count; i++)
        {
            if(astSlots[i].poCard != orxNULL)
            {
                bGameOver = orxFALSE;
                break;
            }
        }

        // Game Over?
        if(bGameOver)
        {
            ggj2021    &roGame = ggj2021::GetInstance();
            Player     *poWinner = orxNULL;
            orxCHAR     acName[128] = {};

            for(Player *poPlayer = roGame.GetNextObject<Player>();
                poPlayer;
                poPlayer = roGame.GetNextObject<Player>(poPlayer))
            {
                if((!poWinner)
                || (poPlayer->u32Score > poWinner->u32Score)
                || ((poPlayer->u32Score == poWinner->u32Score)
                 && (poPlayer->u32Picks < poWinner->u32Picks)))
                {
                    poWinner = poPlayer;
                }
                poPlayer->astHands[0].poHand->Enable(orxFALSE);
                poPlayer->astHands[1].poHand->Enable(orxFALSE);
            }

            orxConfig_PushSection("GameOver");
            orxString_NPrint(acName, sizeof(acName) - 1, "%s", poWinner->GetModelName());
            orxConfig_SetString("Winner", acName);
            orxString_UpperCase(acName);
            orxConfig_SetString("WINNER", acName);
            orxConfig_PopSection();
            roGame.CreateObject("GameOver");
        }
    }
    else
    {
        if(orxInput_HasBeenActivated("Restart"))
        {
            orxInput_SetValue("Reset", orxFLOAT_1);
        }
    }
}

void Table::Deal()
{
    ggj2021    &roGame = ggj2021::GetInstance();
    Card       *poCard, *apoShuffleCards[1024] = {};
    orxU32      u32CardIndex = 0;

    // Delete all cards
    while((poCard = roGame.GetNextObject<Card>()) != orxNULL)
    {
        roGame.DeleteObject(poCard);
    }

    // Create cards
    orxConfig_PushSection(orxConfig_GetString("Deck"));
    for(orxU32 i = 0; i < u32Count; i += 2)
    {
        const orxSTRING zCard = orxConfig_GetString("Cards");
        for(orxU32 j = 0; j < 2; j++)
        {
            do
            {
                u32CardIndex = orxMath_GetRandomU32(0, 1024);
            } while(apoShuffleCards[u32CardIndex] != orxNULL);
            apoShuffleCards[u32CardIndex] = roGame.CreateObject<Card>(zCard);
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
            Card *poCard;
            if((poCard = apoShuffleCards[u32CardIndex]) != orxNULL)
            {
                orxOBJECT *pstParent;
                pstParent = orxObject_CreateFromConfig("Slot");
                orxObject_SetPosition(pstParent, &vPos);
                orxObject_SetOwner(pstParent, GetOrxObject());
                orxObject_SetOwner(poCard->GetOrxObject(), GetOrxObject());
                orxObject_SetParent(poCard->GetOrxObject(), pstParent);
                astSlots[u32Count].poCard   = poCard;
                astSlots[u32Count].pstParent= pstParent;
                if(++u32Count % u32Width == 0)
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

    // Init players
    u32CardIndex = 0;
    for(Player *poPlayer = roGame.GetNextObject<Player>();
        poPlayer;
        poPlayer = roGame.GetNextObject<Player>(poPlayer))
    {
        poPlayer->poTable = this;
        poPlayer->Select(0, u32CardIndex++);
        poPlayer->Select(1, u32CardIndex++);
    }
}
