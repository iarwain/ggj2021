/**
 * @file Table.cpp
 * @date 29-Jan-2021
 */

#include "Table.h"
#include "Player.h"

void orxFASTCALL Attract(const orxCLOCK_INFO *_pstClockInfo, void *_pContext)
{
    Table *poTable  = (Table *)_pContext;
    Card *poCard    = poTable->astSlots[orxMath_GetRandomU32(0, poTable->u32Count - 1)].poCard;

    poCard->SetAnim("Show");
    poTable->PushConfigSection();
    orxClock_AddGlobalTimer(Attract, orxConfig_GetFloat("AttractDelay"), 1, poTable);
    poTable->PopConfigSection();
}

void Table::OnCreate()
{
    // Init variables
    orxConfig_SetBool("IsTable", orxTRUE);

    // Deal cards
    Deal();
}

void Table::OnDelete()
{
    // Clear slots
    orxMemory_Free(astSlots);

    // Remove attract mode
    orxClock_RemoveGlobalTimer(Attract, -orxFLOAT_1, orxNULL);

    if(bSelect)
    {
        orxInput_SelectSet("MainInput");
    }
}

void Table::Update(const orxCLOCK_INFO &_rstInfo)
{
    if(!bAttract)
    {
        if(bSelect)
        {
            orxBOOL bUpdate = orxFALSE;
            orxU32  u32Decks, u32Counts, u32SelectDeck, u32SelectCount;

            u32Decks    = orxConfig_GetListCount("Decks");
            u32Counts   =  orxConfig_GetListCount("Counts");

            PushConfigSection();

            if(orxInput_HasBeenActivated("PreviousCount"))
            {
                orxConfig_SetU32("SelectCount", (orxConfig_GetU32("SelectCount") - 1 + u32Counts) % u32Counts);
                bUpdate = orxTRUE;
            }
            else if(orxInput_HasBeenActivated("NextCount"))
            {
                orxConfig_SetU32("SelectCount", (orxConfig_GetU32("SelectCount") + 1) % u32Counts);
                bUpdate = orxTRUE;
            }
            else if(orxInput_HasBeenActivated("PreviousDeck"))
            {
                orxConfig_SetU32("SelectDeck", (orxConfig_GetU32("SelectDeck") - 1 + u32Decks) % u32Decks);
                bUpdate = orxTRUE;
            }
            else if(orxInput_HasBeenActivated("NextDeck"))
            {
                orxConfig_SetU32("SelectDeck", (orxConfig_GetU32("SelectDeck") + 1) % u32Decks);
                bUpdate = orxTRUE;
            }

            u32SelectCount  = orxConfig_GetU32("SelectCount");
            u32SelectDeck   = orxConfig_GetU32("SelectDeck");

            PopConfigSection();

            if(bUpdate)
            {
                orxConfig_SetString("Deck", orxConfig_GetListString("Decks", u32SelectDeck));
                orxConfig_SetString("Count", orxConfig_GetListString("Counts", u32SelectCount));
                PushConfigSection();
                Deal();
                PopConfigSection();
            }

            if(orxInput_HasBeenActivated("1P"))
            {
                orxConfig_SetParent("Scene", "1P");
            }
            else if(orxInput_HasBeenActivated("2P"))
            {
                orxConfig_SetParent("Scene", "2P");
            }
            else if(orxInput_HasBeenActivated("3P"))
            {
                orxConfig_SetParent("Scene", "3P");
            }
            else if(orxInput_HasBeenActivated("4P"))
            {
                orxConfig_SetParent("Scene", "4P");
            }

            if(orxInput_HasBeenActivated("Start"))
            {
                orxObject_Delete(orxOBJECT(orxObject_GetOwner(GetOrxObject())));
                ggj2021::GetInstance().CreateObject("Scene");
            }
        }
        else
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
                    orxBOOL     bTie = orxFALSE;

                    for(Player *poPlayer = roGame.GetNextObject<Player>();
                        poPlayer;
                        poPlayer = roGame.GetNextObject<Player>(poPlayer))
                    {
                        if((!poWinner)
                        || (poPlayer->u32Score > poWinner->u32Score)
                        || ((poPlayer->u32Score == poWinner->u32Score)
                         && (poPlayer->u32Picks < poWinner->u32Picks)))
                        {
                            poWinner    = poPlayer;
                            bTie        = orxFALSE;
                        }
                        else if((poPlayer->u32Score == poWinner->u32Score)
                             && (poPlayer->u32Picks == poWinner->u32Picks))
                        {
                            bTie = orxTRUE;
                        }
                        poPlayer->astHands[0].poHand->Enable(orxFALSE);
                        poPlayer->astHands[1].poHand->Enable(orxFALSE);
                    }

                    orxConfig_PushSection("GameOver");
                    orxString_NPrint(acName, sizeof(acName) - 1, "%s", poWinner->GetModelName());
                    orxConfig_SetString("Winner", bTie ? "Tie" : acName);
                    orxString_UpperCase(acName);
                    orxConfig_SetString("WINNER", bTie ? "TIE" : acName);
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
    }
}

void Table::Deal()
{
    ggj2021    &roGame = ggj2021::GetInstance();
    Card       *apoShuffleCards[1024] = {};
    orxU32      u32CardIndex = 0;

    // Delete all cards
    if(astSlots)
    {
        for(orxU32 i = 0; i < u32Count; i++)
        {
            roGame.DeleteObject(astSlots[i].poCard);
        }
        orxMemory_Free(astSlots);
    }

    // Inits variables
    u32Count    = orxConfig_GetU32("Count");
    u32Width    = ((orxU32)orxMath_Ceil(orxMath_Sqrt(orxU2F(u32Count)))) & ~1;
    u32Height   = u32Count / u32Width;
    u32Count    = u32Width * u32Height;
    bGameOver   = orxFALSE;
    astSlots    = (Slot *) orxMemory_Allocate(u32Count * sizeof(Slot), orxMEMORY_TYPE_MAIN);

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
                orxObject_SetParent(pstParent, GetOrxObject());
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

    // No players?
    if(u32CardIndex == 0)
    {
        // Attract mode?
        if(orxConfig_GetBool("IsAttract"))
        {
            orxClock_AddGlobalTimer(Attract, orxConfig_GetFloat("AttractDelay"), 1, this);
            bAttract = orxTRUE;
        }
        else
        {
            for(orxU32 i = 0; i < u32Count; i++)
            {
                astSlots[i].poCard->SetAnim("Show");
            }
            orxInput_SelectSet("SelectInput");
            orxInput_SetValue("Start", orxFLOAT_0);
            bSelect = orxTRUE;
        }
    }
}
