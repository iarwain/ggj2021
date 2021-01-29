/**
 * @file Player.cpp
 * @date 29-Jan-2021
 */

#include "Player.h"

const orxSTRING Player::GetConfigVar(const orxSTRING _zVar) const
{
    static orxCHAR sacName[64] = {};

    orxString_NPrint(sacName, sizeof(sacName) - 1, "%s%s", GetModelName(), _zVar);

    return sacName;
}

void Player::OnCreate()
{
    orxConfig_SetBool("IsPlayer", orxTRUE);
}

void Player::OnDelete()
{
}

void Player::Update(const orxCLOCK_INFO &_rstInfo)
{
}
