#include "../header/raylib.h"
#include "../header/ui.hpp"
#include "../header/keymanager.hpp"
#include "../header/vault.hpp"
#include <string>
#include <vector>
#include <tuple>
#include <iostream>

static std::vector<std::tuple<std::string,std::string,std::string>> vaultToList(const vault &v);

int main() 
{

    InitWindow(960, 640, "Password Manager");
    SetTargetFPS(60);

    UIState ui;
    InitUIState(ui);

    keymanager km;
    vault *theVault = nullptr;
    std::string currentFile;

    bool attemptLoad = false;
    bool createNew = false;

    bool openAdd = false;
    bool doSave = false;
    bool doLogout = false;
    int selectedIndex = -1;

    bool running = true;

    while (!WindowShouldClose() && running) 
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (ui.showLogin) 
        {
            DrawLogin(ui, attemptLoad, createNew);
        } 
        else if (ui.showMain) 
        {
            auto entries = theVault ? theVault->getAll() : std::vector<std::tuple<std::string,std::string,std::string>>{};
            DrawMain(ui, entries, openAdd, doSave, doLogout, selectedIndex);
        }

        if (ui.showAdd) 
        {
            bool added = false;
            DrawAddEntry(ui, added);
            if (added) 
            {
                if (theVault) 
                {
                    std::string site(ui.siteBuf);
                    std::string user(ui.userBuf);
                    std::string pass(ui.passBuf);
                    if (!site.empty()) 
                    {
                        theVault->add(site, user, pass);
                    }
                    ui.siteBuf[0] = ui.userBuf[0] = ui.passBuf[0] = 0;
                    ui.showAdd = false;
                }
            }
        }
        EndDrawing();

        if (attemptLoad) 
        {
            attemptLoad = false;
            km.derive(std::string(ui.masterBuf));
            currentFile = std::string(ui.fileBuf);
            if (currentFile.empty()) 
            currentFile = "vault.dat";
            if (theVault) 
            { 
                delete theVault; 
                theVault = nullptr; 
            }
            std::vector<uint8_t> key = km.getKey();
            std::vector<uint8_t> iv  = km.getIV();
            theVault = new vault(key, iv, currentFile);
            bool ok = theVault->load();
            
            if (ok) 
            {
                ui.showLogin = false;
                ui.showMain = true;
                ui.showAdd = false;
                ui.fade = 0.0f;
                selectedIndex = -1;
            } 
            else 
            {
                TraceLog(LOG_WARNING, "Load failed. Wrong password or corrupt file.");
            }
        }
        if (createNew) 
        {
            createNew = false;
            km.derive(std::string(ui.masterBuf));
            km.genRandomIV();
            currentFile = std::string(ui.fileBuf);
            if (currentFile.empty()) 
            currentFile = "vault.dat";
            if (theVault) 
            { 
                delete theVault; 
                theVault = nullptr; 
            }
            theVault = new vault(km.getKey(), km.getIV(), currentFile);
            bool ok = theVault->save();
            if (!ok) 
            {
                TraceLog(LOG_WARNING, "Failed to create new vault file");
            } 
            else 
            {
                ui.showLogin = false;
                ui.showMain = true;
                ui.showAdd = false;
                ui.fade = 0.0f;
            }
        }

        if (openAdd) 
        {
            openAdd = false;
            ui.showAdd = true;
        }

        if (doSave) 
        {
            doSave = false;
            if (theVault) 
            {
                if (!theVault->save()) 
                {
                    TraceLog(LOG_WARNING, "Save failed");
                }
            }
        }

        if (doLogout) 
        {
            doLogout = false;
            if (theVault) 
            {
                delete theVault;
                theVault = nullptr;
            }

            ui.showLogin = true;
            ui.showMain = false;
            ui.showAdd = false;
            selectedIndex = -1;

            ui.masterBuf[0] = 0;
        }

        if (selectedIndex >= 0 && theVault) 
        {
            if (IsKeyPressed(KEY_DELETE)) 
            {
                auto list = theVault->getAll();
                if (selectedIndex < (int)list.size()) 
                {
                    std::string site = std::get<0>(list[selectedIndex]);
                    auto all = list;

                    std::vector<uint8_t> key = km.getKey();
                    std::vector<uint8_t> iv  = km.getIV();
                    vault newv(key, iv, currentFile);
                    for (size_t i=0;i<all.size();++i) 
                    {
                        if ((int)i == selectedIndex) continue;
                        newv.add(std::get<0>(all[i]), std::get<1>(all[i]), std::get<2>(all[i]));
                    }

                    delete theVault;
                    theVault = new vault(key, iv, currentFile);
                    newv.save();
                    theVault->load();
                    selectedIndex = -1;
                }
            }
        }
    }
    if (theVault) 
    delete theVault;
    
    CloseWindow();
    return 0;
}
