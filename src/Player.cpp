/**
 * @file Player.cpp
 * @date 29-Jan-2021
 */

#include "Player.h"

void Player::Select(orxU32 _u32HandIndex, orxU32 _u32SlotIndex)
{
    astHands[_u32HandIndex].u32SlotIndex = _u32SlotIndex;
    if(_u32SlotIndex != orxU32_UNDEFINED)
    {
        orxObject_SetParent(astHands[_u32HandIndex].poHand->GetOrxObject(), poTable->astSlots[_u32SlotIndex].pstParent);
        astHands[_u32HandIndex].poHand->Enable(orxTRUE);
    }
    else
    {
        orxObject_SetParent(astHands[_u32HandIndex].poHand->GetOrxObject(), orxNULL);
        astHands[_u32HandIndex].poHand->Enable(orxFALSE);
    }
}

const orxSTRING Player::GetConfigVar(const orxSTRING _zVar) const
{
    static orxCHAR sacName[64] = {};

    orxString_NPrint(sacName, sizeof(sacName) - 1, "%s%s", GetName(), _zVar);

    return sacName;
}

void Player::OnCreate()
{
    ggj2021 &roGame = ggj2021::GetInstance();

    // Init variables
    u32Score = u32Picks = 0;
    bNew = orxTRUE;
    orxConfig_SetBool("IsPlayer", orxTRUE);
    orxConfig_SetU32("Score", u32Score);
    orxConfig_SetU32("Picks", u32Picks);
    for(orxU32 i = 0; i < 2; i++)
    {
        astHands[i].poHand = roGame.CreateObject<Object>(orxConfig_GetString("Hand"));
        orxObject_SetOwner(astHands[i].poHand->GetOrxObject(), GetOrxObject());
        astHands[i].poHand->SetAnim((i & 1) ? "Right" : "Left", orxTRUE);
        Select(i, orxU32_UNDEFINED);
    }
}

void Player::OnDelete()
{
}

void Player::Update(const orxCLOCK_INFO &_rstInfo)
{
    if(!poTable->bGameOver)
    {
        if(!bNew)
        {
            // Push config section
            PushConfigSection();

            // Busy?
            if(bBusy)
            {
                Card *poLeftCard, *poRightCard;

                poLeftCard  = poTable->astSlots[astHands[0].u32SlotIndex].poCard;
                poRightCard = poTable->astSlots[astHands[1].u32SlotIndex].poCard;

                if((poLeftCard != poRightCard)
                && (poLeftCard)
                && (poRightCard)
                && (poLeftCard->IsVisible())
                && (poRightCard->IsVisible()))
                {
                    // Valid?
                    if(!orxString_Compare(poLeftCard->GetName(), poRightCard->GetName()))
                    {
                        // Should score?
                        if(!bScore)
                        {
                            // Score
                            AddTrack("ScorePoint");
                            orxConfig_SetU32("Score", ++u32Score);
                            bScore = orxTRUE;
                        }

                        // Drop/Shuffle?
                        if(orxInput_HasBeenActivated("Drop") || orxInput_HasBeenActivated("Shuffle"))
                        {
                            orxVECTOR   vPos, vOffset;
                            orxOBJECT  *pstEmptySlot;
                            orxObject_Attach(poLeftCard->GetOrxObject(), GetOrxObject());
                            poTable->astSlots[astHands[0].u32SlotIndex].poCard = orxNULL;
                            orxObject_AddFX(poTable->astSlots[astHands[0].u32SlotIndex].pstParent, "Drop");
                            orxObject_Attach(poRightCard->GetOrxObject(), GetOrxObject());
                            poTable->astSlots[astHands[1].u32SlotIndex].poCard = orxNULL;
                            orxObject_AddFX(poTable->astSlots[astHands[1].u32SlotIndex].pstParent, "Drop");
                            pstEmptySlot = orxObject_CreateFromConfig("EmptySlot");
                            orxObject_SetParent(pstEmptySlot, poTable->astSlots[astHands[0].u32SlotIndex].pstParent);
                            orxObject_SetOwner(pstEmptySlot, poTable->astSlots[astHands[0].u32SlotIndex].pstParent);
                            pstEmptySlot = orxObject_CreateFromConfig("EmptySlot");
                            orxObject_SetParent(pstEmptySlot, poTable->astSlots[astHands[1].u32SlotIndex].pstParent);
                            orxObject_SetOwner(pstEmptySlot, poTable->astSlots[astHands[1].u32SlotIndex].pstParent);

                            orxVector_Mulf(&vOffset, orxConfig_GetVector("Offset", &vOffset), orxU2F(2 * u32Score));
                            orxConfig_PushSection("MoveLeft");
                            poLeftCard->GetPosition(vPos);
                            orxConfig_SetVector("EndValue", orxVector_Add(&vPos, orxVector_Neg(&vPos, &vPos), &vOffset));
                            orxConfig_PopSection();
                            poLeftCard->SetAnim("RevealLeft");
                            orxVector_Mulf(&vOffset, orxConfig_GetVector("Offset", &vOffset), orxU2F(2 * u32Score + 1));
                            orxConfig_PushSection("MoveRight");
                            poRightCard->GetPosition(vPos);
                            orxConfig_SetVector("EndValue", orxVector_Add(&vPos, orxVector_Neg(&vPos, &vPos), &vOffset));
                            orxConfig_PopSection();
                            poRightCard->SetAnim("RevealRight");
                            bBusy = orxFALSE;
                        }
                    }
                    else
                    {
                        // Drop/Shuffle?
                        if(orxInput_HasBeenActivated("Drop") || orxInput_HasBeenActivated("Shuffle"))
                        {
                            // Shuffle?
                            if(((poTable->bSolo)
                             && (orxConfig_GetBool("Shuffle?")))
                            || ((!poTable->bSolo)
                             && (orxInput_HasBeenActivated("Shuffle"))))
                            {
                                orxVECTOR vPos;
                                orxObject_Attach(poLeftCard->GetOrxObject(), poTable->astSlots[astHands[1].u32SlotIndex].pstParent);
                                poTable->astSlots[astHands[1].u32SlotIndex].poCard = poLeftCard;
                                orxObject_Attach(poRightCard->GetOrxObject(), poTable->astSlots[astHands[0].u32SlotIndex].pstParent);
                                poTable->astSlots[astHands[0].u32SlotIndex].poCard = poRightCard;

                                orxConfig_PushSection("MoveLeft");
                                poLeftCard->GetPosition(vPos);
                                orxConfig_SetVector("EndValue", orxVector_Neg(&vPos, &vPos));
                                orxConfig_PopSection();
                                poLeftCard->SetAnim("ShuffleLeft");
                                orxConfig_PushSection("MoveRight");
                                poRightCard->GetPosition(vPos);
                                orxConfig_SetVector("EndValue", orxVector_Neg(&vPos, &vPos));
                                orxConfig_PopSection();
                                poRightCard->SetAnim("ShuffleRight");
                            }
                            else
                            {
                                poLeftCard->SetAnim("HideLoud");
                                poRightCard->SetAnim("Hide");
                            }
                            bBusy = orxFALSE;
                        }
                    }
                }
                else
                {
                    // Drop/Shuffle?
                    if(orxInput_HasBeenActivated("Drop") || orxInput_HasBeenActivated("Shuffle"))
                    {
                        AddTrack("Error");
                    }
                }
            }
            else
            {
                // Move hands
                const orxSTRING aazMoveList[2][4] =
                {
                    {"MoveLRight", "MoveLLeft", "MoveLDown", "MoveLUp"},
                    {"MoveRRight", "MoveRLeft", "MoveRDown", "MoveRUp"},
                };
                orxBOOL bWrapAround = orxConfig_GetBool("WrapAround");
                for(orxU32 i = 0; i < 2; i++)
                {
                    orxVECTOR vMove = {orxU2F(orxInput_HasBeenActivated(aazMoveList[i][0])) - orxU2F(orxInput_HasBeenActivated(aazMoveList[i][1])), orxU2F(orxInput_HasBeenActivated(aazMoveList[i][2])) - orxU2F(orxInput_HasBeenActivated(aazMoveList[i][3])), orxFLOAT_0};

                    if(astHands[i].u32SlotIndex != orxU32_UNDEFINED)
                    {
                        orxU32 u32NewSlotIndex = astHands[i].u32SlotIndex;

                        if(bWrapAround)
                        {
                            if(vMove.fX < -0.5f)
                            {
                                u32NewSlotIndex = (u32NewSlotIndex + poTable->u32Count - 1 + (((u32NewSlotIndex % poTable->u32Width) == 0) ? poTable->u32Width : 0)) % poTable->u32Count;
                            }
                            else if(vMove.fX > 0.5f)
                            {
                                u32NewSlotIndex = (u32NewSlotIndex + 1 - (((u32NewSlotIndex % poTable->u32Width) == poTable->u32Width - 1) ? poTable->u32Width : 0)) % poTable->u32Count;
                            }
                            else if(vMove.fY < -0.5f)
                            {
                                u32NewSlotIndex = (u32NewSlotIndex + poTable->u32Count - poTable->u32Width) % poTable->u32Count;
                            }
                            else if(vMove.fY > 0.5f)
                            {
                                u32NewSlotIndex = (u32NewSlotIndex + poTable->u32Width) % poTable->u32Count;
                            }
                        }
                        else
                        {
                            if(vMove.fX < -0.5f)
                            {
                                u32NewSlotIndex = ((u32NewSlotIndex % poTable->u32Width) == 0) ? u32NewSlotIndex : u32NewSlotIndex - 1;
                            }
                            else if(vMove.fX > 0.5f)
                            {
                                u32NewSlotIndex = ((u32NewSlotIndex % poTable->u32Width) == poTable->u32Width - 1) ? u32NewSlotIndex : u32NewSlotIndex + 1;
                            }
                            else if(vMove.fY < -0.5f)
                            {
                                u32NewSlotIndex = ((u32NewSlotIndex / poTable->u32Width) == 0) ? u32NewSlotIndex : u32NewSlotIndex - poTable->u32Width;
                            }
                            else if(vMove.fY > 0.5f)
                            {
                                u32NewSlotIndex = ((u32NewSlotIndex / poTable->u32Width) == poTable->u32Height - 1) ? u32NewSlotIndex : u32NewSlotIndex + poTable->u32Width;
                            }
                        }
                        if(u32NewSlotIndex != astHands[i].u32SlotIndex)
                        {
                            Select(i, u32NewSlotIndex);
                            AddTrack("Move");
                        }
                    }
                }

                // Pick?
                if(orxInput_HasBeenActivated("Pick"))
                {
                    Card *poLeftCard, *poRightCard;

                    poLeftCard  = poTable->astSlots[astHands[0].u32SlotIndex].poCard;
                    poRightCard = poTable->astSlots[astHands[1].u32SlotIndex].poCard;

                    if((poLeftCard != poRightCard)
                    && (poLeftCard)
                    && (poRightCard)
                    && (poLeftCard->IsHidden())
                    && (poRightCard->IsHidden()))
                    {
                        poLeftCard->SetAnim("ShowLoud");
                        poRightCard->SetAnim("Show");
                        orxConfig_SetU32("Picks", ++u32Picks);
                        bBusy   = orxTRUE;
                        bScore  = orxFALSE;
                    }
                    else
                    {
                        AddTrack("Error");
                    }
                }
            }

            // Pop config section
            PopConfigSection();
        }
        else
        {
            bNew = orxFALSE;
        }
    }
}
