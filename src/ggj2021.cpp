/**
 * @file ggj2021.cpp
 * @date 27-Jan-2021
 */

#define __SCROLL_IMPL__
#include "ggj2021.h"
#undef __SCROLL_IMPL__

#include "Object.h"

#define orxARCHIVE_IMPL
#include "orxArchive.h"
#undef orxARCHIVE_IMPL

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
}

/** Init function, it is called when all orx's modules have been initialized
 */
orxSTATUS ggj2021::Init()
{
    // Display a small hint in console
    orxLOG("\n* This template project creates a simple scene"
    "\n* You can play with the config parameters in ../data/config/ggj2021.ini"
    "\n* After changing them, relaunch the executable to see the changes.");

    // Initialize archive (ZIP) resource type
    orxArchive_Init();

    // Create the scene
    CreateObject("Scene");

    // Done!
    return orxSTATUS_SUCCESS;
}

/** Run function, it should not contain any game logic
 */
orxSTATUS ggj2021::Run()
{
    // Return orxSTATUS_FAILURE to instruct orx to quit
    return orxSTATUS_SUCCESS;
}

/** Exit function, it is called before exiting from orx
 */
void ggj2021::Exit()
{
    // Let Orx clean all our mess automatically. :)
}

/** BindObjects function, ScrollObject-derived classes are bound to config sections here
 */
void ggj2021::BindObjects()
{
    // Bind the Object class to the Object config section
    ScrollBindObject<Object>("Object");
}

/** Bootstrap function, it is called before config is initialized, allowing for early resource storage definitions
 */
orxSTATUS ggj2021::Bootstrap() const
{
    // Add a config storage to find the initial config file
    orxResource_AddStorage(orxCONFIG_KZ_RESOURCE_GROUP, "../data/config", orxFALSE);

    // Return orxSTATUS_FAILURE to prevent orx from loading the default config file
    return orxSTATUS_SUCCESS;
}

/** Main function
 */
int main(int argc, char **argv)
{
    // Execute our game
    ggj2021::GetInstance().Execute(argc, argv);

    // Done!
    return EXIT_SUCCESS;
}
