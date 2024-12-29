/**
 * @file ggj2021.cpp
 * @date 27-Jan-2021
 */

#define __SCROLL_IMPL__
#include "ggj2021.h"
#undef __SCROLL_IMPL__

#include "orxExtensions.h"

#include "Object.h"
#include "Card.h"
#include "Table.h"
#include "Player.h"

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
        // Should restart?
        sbRestart = orxInput_HasBeenActivated("Reset");

        // Send close event
        orxEvent_SendShort(orxEVENT_TYPE_SYSTEM, orxSYSTEM_EVENT_CLOSE);
    }
    // Screenshot?
    else if(orxInput_HasBeenActivated("Screenshot"))
    {
        orxScreenshot_Capture();
    }
    // Reset?
    else if(orxInput_HasBeenActivated("Reset"))
    {
        PauseGame(orxFALSE);
        for(ScrollObject *poObject = GetNextObject();
            poObject;
            poObject = GetNextObject())
        {
            DeleteObject(poObject);
        }
        CreateObject("Select");
    }
}

/** Init function, it is called when all orx's modules have been initialized
 */
orxSTATUS ggj2021::Init()
{
    // Init extensions
    InitExtensions();

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
    // Save highscores
    orxConfig_Save(orxFile_GetApplicationSaveDirectory("LostEcho/score.dat"), orxTRUE, &SaveCallback);

    // Exit from extensions
    ExitExtensions();
}

/** BindObjects function, ScrollObject-derived classes are bound to config sections here
 */
void ggj2021::BindObjects()
{
    // Bind all object classes
    BindObject(Object);
    BindObject(Card);
    BindObject(Table);
    BindObject(Player);
}

/** Bootstrap function, it is called before config is initialized, allowing for early resource storage definitions
 */
orxSTATUS ggj2021::Bootstrap() const
{
    // Bootstrap extensions
    BootstrapExtensions();

    // Load scores
    orxConfig_Load(orxFile_GetApplicationSaveDirectory("LostEcho/score.dat"));

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
