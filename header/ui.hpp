#ifndef UI_H
#define UI_H

#include <string>
#include <vector>
#include <tuple>

struct UIState {
    bool showLogin = true;
    bool showMain  = false;
    bool showAdd   = false;

    float fade = 0.0f;

    char masterBuf[128];
    char fileBuf[256];
    char siteBuf[128];
    char userBuf[128];
    char passBuf[128];

    int listOffset = 0;

    bool editMaster = false;
    bool editFile   = false;
    bool editSite   = false;
    bool editUser   = false;
    bool editPass   = false;
};

void InitUIState(UIState &s);

void DrawLogin(UIState &s,bool &attemptLoad,bool &createNew);

void DrawMain(UIState &s,const std::vector<std::tuple<std::string,std::string,std::string>> &entries,bool &openAdd,bool &doSave,bool &doLogout,int &selectedIndex);

void DrawAddEntry(UIState &s,bool &added);

#endif
