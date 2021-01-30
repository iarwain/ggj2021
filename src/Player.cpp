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

    orxString_NPrint(sacName, sizeof(sacName) - 1, "%s%s", GetModelName(), _zVar);

    return sacName;
}

void Player::OnCreate()
{
    ggj2021 &roGame = ggj2021::GetInstance();

    // Init variables
    orxConfig_SetBool("IsPlayer", orxTRUE);
    for(orxU32 i = 0; i < 2; i++)
    {
        astHands[i].poHand          = roGame.CreateObject<Object>("Hand");
        orxObject_SetOwner(astHands[i].poHand->GetOrxObject(), GetOrxObject());
        astHands[i].poHand->SetAnim((i & 1) ? "Right" : "Left");
        Select(i, orxU32_UNDEFINED);
    }

    // Enable its inputs
    orxInput_EnableSet(orxConfig_GetString("Input"), orxTRUE);
}

void Player::OnDelete()
{
}

void Player::Update(const orxCLOCK_INFO &_rstInfo)
{
    // Push config section
    PushConfigSection();

    // Select input set
    const orxSTRING zSet = orxInput_GetCurrentSet();
    orxInput_SelectSet(orxConfig_GetString("Input"));

    const orxSTRING aazMoveList[2][4] =
    {
        {"MoveLRight", "MoveLLeft", "MoveLDown", "MoveLUp"},
        {"MoveRRight", "MoveRLeft", "MoveRDown", "MoveRUp"},
    };

    for(orxU32 i = 0; i < 2; i++)
    {
        // Move
        orxVECTOR vMove = {orxU2F(orxInput_HasBeenActivated(aazMoveList[i][0])) - orxU2F(orxInput_HasBeenActivated(aazMoveList[i][1])), orxU2F(orxInput_HasBeenActivated(aazMoveList[i][2])) - orxU2F(orxInput_HasBeenActivated(aazMoveList[i][3])), orxFLOAT_0};

        if(astHands[i].u32SlotIndex != orxU32_UNDEFINED)
        {
            orxU32 u32NewSlotIndex = astHands[i].u32SlotIndex;

            if(vMove.fX < -0.5f)
            {
                u32NewSlotIndex = (u32NewSlotIndex + poTable->u32CardCount - 1 + (((u32NewSlotIndex % poTable->u32Width) == 0) ? poTable->u32Width : 0)) % poTable->u32CardCount;
            }
            else if(vMove.fX > 0.5f)
            {
                u32NewSlotIndex = (u32NewSlotIndex + 1 - (((u32NewSlotIndex % poTable->u32Width) == poTable->u32Width - 1) ? poTable->u32Width : 0)) % poTable->u32CardCount;
            }
            else if(vMove.fY < -0.5f)
            {
                u32NewSlotIndex = (u32NewSlotIndex + poTable->u32CardCount - poTable->u32Width) % poTable->u32CardCount;
            }
            else if(vMove.fY > 0.5f)
            {
                u32NewSlotIndex = (u32NewSlotIndex + poTable->u32Width) % poTable->u32CardCount;
            }
            if(u32NewSlotIndex != astHands[i].u32SlotIndex)
            {
                Select(i, u32NewSlotIndex);
            }
        }
    }

    // Deselect input set
    orxInput_SelectSet(zSet);

    // Pop config section
    PopConfigSection();
}
