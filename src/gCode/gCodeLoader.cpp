//
// Created by Bartosz Jadczak on 15/10/2024.
//

#include "gCodeLoader.h"

#include <iostream>
#include <string>

#include "nfd.h"

std::string GCodeLoader::chooseFile ()
{
    NFD_Init();

    nfdchar_t *outPath;
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 0, nullptr);
    if (result == NFD_OKAY)
    {
        NFD_Quit();
        return {outPath};
    }
    else if (result == NFD_CANCEL)
    {
        NFD_Quit();
        return "";
    }
    else
    {
        NFD_Quit();
        std::cerr << "Error: " <<  std::string(NFD_GetError()) << "\n";
        return "";
    }

}

std::string GCodeLoader::saveFile ()
{
    NFD_Init();

    nfdchar_t *outPath;

    nfdresult_t result = NFD_SaveDialog(&outPath, filterItem, 0, NULL, "code.json");
    if (result == NFD_OKAY)
    {
        NFD_Quit();
        return {outPath};
    }
    else if (result == NFD_CANCEL)
    {
        NFD_Quit();
        return "";
    }
    else
    {
        NFD_Quit();
        std::cerr << "Error: " <<  std::string(NFD_GetError()) << "\n";
        return "";
    }

}
