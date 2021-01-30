/**
 * @file ggj2021.cpp
 * @date 27-Jan-2021
 */

#define __SCROLL_IMPL__
#include "ggj2021.h"
#undef __SCROLL_IMPL__

#include "Object.h"
#include "Card.h"
#include "Table.h"
#include "Player.h"

#define orxARCHIVE_IMPL
#include "orxArchive.h"
#undef orxARCHIVE_IMPL

static orxBOOL sbRestart = orxTRUE;

orxBOOL orxFASTCALL SaveCallback(const orxSTRING _zSectionName, const orxSTRING _zKeyName, const orxSTRING _zFileName, orxBOOL _bUseEncryption)
{
    return (!orxString_Compare(_zSectionName, "Save")) ? orxTRUE : orxFALSE;
}

/** Update function, it has been registered to be called every tick of the core clock
 */
void ggj2021::Update(const orxCLOCK_INFO &_rstInfo)
{
    // Should quit?
    if(orxInput_IsActive("Quit"))
    {
        // Send close event
        orxEvent_SendShort(orxEVENT_TYPE_SYSTEM, orxSYSTEM_EVENT_CLOSE);
    }
    // Screenshot?
    else if(orxInput_HasBeenActivated("Screenshot"))
    {
        orxScreenshot_Capture();
    }
}

/** Init function, it is called when all orx's modules have been initialized
 */
orxSTATUS ggj2021::Init()
{
    // Push game section
    orxConfig_PushSection("Game");

    // Create all viewports
    for(orxS32 i = 0, iCount = orxConfig_GetListCount("ViewportList"); i < iCount; i++)
    {
        orxViewport_CreateFromConfig(orxConfig_GetListString("ViewportList", i));
    }

    // Go to title
    CreateObject("Title");

    // Done!
    return orxSTATUS_SUCCESS;
}

/** Run function, it should not contain any game logic
 */
orxSTATUS ggj2021::Run()
{
    return orxSTATUS_SUCCESS;
}

/** Exit function, it is called before exiting from orx
 */
void ggj2021::Exit()
{
}

/** BindObjects function, ScrollObject-derived classes are bound to config sections here
 */
void ggj2021::BindObjects()
{
    // Bind all object classes
    ScrollBindObject<Object>("Object");
    ScrollBindObject<Card>("Card");
    ScrollBindObject<Table>("Table");
    ScrollBindObject<Player>("Player");
}

/** Bootstrap function, it is called before config is initialized, allowing for early resource storage definitions
 */
orxSTATUS ggj2021::Bootstrap() const
{
    // Initialize archive (ZIP) resource type
    orxArchive_Init();

    // Add a config storage to find the initial config file
    orxResource_AddStorage(orxCONFIG_KZ_RESOURCE_GROUP, "game.dat", orxFALSE);
    orxResource_AddStorage(orxCONFIG_KZ_RESOURCE_GROUP, "../data/config", orxFALSE);
    orxResource_AddStorage(orxCONFIG_KZ_RESOURCE_GROUP, "../data/font", orxFALSE);

    // Load scores
    orxConfig_Load(orxFile_GetApplicationSaveDirectory("GGJ2021/score.dat"));

    // Return orxSTATUS_FAILURE to prevent orx from loading the default config file
    return orxSTATUS_SUCCESS;
}

/** Main function
 */
int main(int argc, char **argv)
{
    // Should restart?
    while(sbRestart)
    {
        // Clear restart
        sbRestart = orxFALSE;

        // Execute our game
        ggj2021::GetInstance().Execute(argc, argv);
    }

    // Done!
    return EXIT_SUCCESS;
}
