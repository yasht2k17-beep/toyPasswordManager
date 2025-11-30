#define RAYGUI_IMPLEMENTATION
#include "../header/raygui.h"
#include "../header/raylib.h"
#include "../header/ui.hpp"
#include <cstring>
#include <algorithm>

void InitUIState(UIState &s)
{
    s.showLogin = true;
    s.showMain  = false;
    s.showAdd   = false;

    s.fade = 0.0f;
    s.listOffset = 0;

    memset(s.masterBuf, 0, sizeof(s.masterBuf));
    memset(s.fileBuf,   0, sizeof(s.fileBuf));
    memset(s.siteBuf,   0, sizeof(s.siteBuf));
    memset(s.userBuf,   0, sizeof(s.userBuf));
    memset(s.passBuf,   0, sizeof(s.passBuf));

    std::strncpy(s.fileBuf, "vault.dat", sizeof(s.fileBuf) - 1);

    s.editMaster = false;
    s.editFile   = false;
    s.editSite   = false;
    s.editUser   = false;
    s.editPass   = false;
}
void DrawLogin(UIState &s, bool &attemptLoad, bool &createNew)
{
    int W = GetScreenWidth();
    int H = GetScreenHeight();

    s.fade = std::min(1.0f, s.fade + 0.05f);

    DrawText("Password Vault", W/2 - 140, 30, 36, BLACK);

    DrawText("Master password:", 60, 120, 20, DARKGRAY);
    if (GuiTextBox({60,150,400,36}, s.masterBuf, sizeof(s.masterBuf), s.editMaster))
    {
        s.editMaster = !s.editMaster;
        s.editFile = false;
    }

    DrawText("Vault file:", 60, 210, 20, DARKGRAY);
    if (GuiTextBox({60,240,400,28}, s.fileBuf, sizeof(s.fileBuf), s.editFile))
    {
        s.editFile = !s.editFile;
        s.editMaster = false;
    }

    if (GuiButton({60, 300, 120, 36}, "Load"))
        attemptLoad = true;

    if (GuiButton({200, 300, 120, 36}, "Create New"))
        createNew = true;

    DrawText("Tip: create a new vault file name if you want multiple vaults.", 
             60, 360, 12, GRAY);
}

void DrawMain(UIState &s,const std::vector<std::tuple<std::string,std::string,std::string>> &entries,bool &openAdd, bool &doSave, bool &doLogout,int &selectedIndex)
{
    int W = GetScreenWidth();
    int H = GetScreenHeight();

    DrawRectangle(0, 0, W, 64, Fade(SKYBLUE, 0.9f));
    DrawText("Vault", 16, 18, 24, BLACK);

    if (GuiButton({(float)(W-380), 12, 100, 40}, "Add"))
        openAdd = true;

    if (GuiButton({(float)(W-270), 12, 100, 40}, "Save"))
        doSave = true;

    if (GuiButton({(float)(W-160), 12, 100, 40}, "Logout"))
        doLogout = true;

    int listX = 20;
    int listY = 80;
    int listW = W - 40;
    int itemH = 60;

    int visible = (H - listY - 20) / itemH;
    int total = entries.size();

    if (GuiButton({(float)(W-60), (float)listY, 40, 28}, "Up"))
        s.listOffset = std::max(0, s.listOffset - 1);

    if (GuiButton({(float)(W-60), (float)(listY + 36), 40, 28}, "Dn"))
        s.listOffset = std::min(std::max(0, total - visible), s.listOffset + 1);

    int start = s.listOffset;
    int end = std::min(total, start + visible);

    for (int i = start; i < end; ++i)
    {
        int idx = i - start;
        int y = listY + idx * itemH;

        DrawRectangle(listX, y, listW - 80, itemH - 8, Fade(LIGHTGRAY, 0.6f));

        if (i == selectedIndex)
            DrawRectangleLines(listX, y, listW - 80, itemH - 8, DARKBLUE);

        auto &t = entries[i];
        std::string site = std::get<0>(t);
        std::string user = std::get<1>(t);
        std::string pass = std::get<2>(t);

        DrawText(site.c_str(), listX + 8, y + 6, 18, BLACK);
        DrawText(("user: " + user).c_str(), listX + 8, y + 28, 14, DARKGRAY);

        if (GuiButton({(float)(listX + listW - 200), (float)(y + 10), 80, 28}, "Reveal"))
        {
            if (selectedIndex == i) selectedIndex = -1;
            else selectedIndex = i;
        }

        if (GuiButton({(float)(listX + listW - 110), (float)(y + 10), 80, 28}, "Delete"))
        {
            selectedIndex = i;
        }

        if (selectedIndex == i)
            DrawText(("pass: " + pass).c_str(), listX + 8, y + 40, 14, MAROON);
    }

    DrawText(TextFormat("Entries: %d", total), 20, H - 28, 14, GRAY);
}

void DrawAddEntry(UIState &s, bool &added)
{
    int W = GetScreenWidth();
    int H = GetScreenHeight();

    int w = 520, h = 260;
    int x = W/2 - w/2;
    int y = H/2 - h/2;

    DrawRectangle(x, y, w, h, Fade(RAYWHITE, 0.95f));
    DrawRectangleLines(x, y, w, h, LIGHTGRAY);

    DrawText("Add entry", x + 16, y + 10, 20, BLACK);

    DrawText("Site:", x + 16, y + 48, 14, DARKGRAY);
    if (GuiTextBox({(float)(x + 80), (float)(y + 44), 380, 28},
                   s.siteBuf, sizeof(s.siteBuf), s.editSite))
    {
        s.editSite = !s.editSite;
        s.editUser = s.editPass = false;
    }

    DrawText("User:", x + 16, y + 88, 14, DARKGRAY);
    if (GuiTextBox({(float)(x + 80), (float)(y + 84), 380, 28},
                   s.userBuf, sizeof(s.userBuf), s.editUser))
    {
        s.editUser = !s.editUser;
        s.editSite = s.editPass = false;
    }

    DrawText("Pass:", x + 16, y + 128, 14, DARKGRAY);
    if (GuiTextBox({(float)(x + 80), (float)(y + 124), 380, 28},
                   s.passBuf, sizeof(s.passBuf), s.editPass))
    {
        s.editPass = !s.editPass;
        s.editSite = s.editUser = false;
    }

    if (GuiButton({(float)(x + 80), (float)(y + 180), 120, 36}, "Add"))
        added = true;

    if (GuiButton({(float)(x + 220), (float)(y + 180), 120, 36}, "Cancel"))
    {
        s.siteBuf[0] = s.userBuf[0] = s.passBuf[0] = 0;
        added = false;
    }
}
