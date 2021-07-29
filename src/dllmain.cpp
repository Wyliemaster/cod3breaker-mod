#include "includes.h"
#include <windows.h>
#include <string>

//stole this from mat
void clipboardText(const char* text) {
    if (!OpenClipboard(NULL)) return;
    if (!EmptyClipboard()) return;
    auto len = std::strlen(text);
    auto mem = GlobalAlloc(GMEM_MOVEABLE, len + 1);
    memcpy(GlobalLock(mem), text, len + 1);
    GlobalUnlock(mem);
    SetClipboardData(CF_TEXT, mem);
    CloseClipboard();
}


class SecretLayer2 : public cocos2d::CCLayer {
    BYTE pad[0x164];

public:
    void cod3breakerCallback(cocos2d::CCObject* pSender) {
        auto GM = gd::GameManager::sharedState();
        auto code = "The <cr>Cod3breaker</c> code is: <cy>" + std::to_string(GM->m_nSecretNumberRand1 - GM->m_nSecretNumberRand2) + "</c>";
        gd::FLAlertLayer::create(nullptr, "Cod3breaker", "Thanks!", nullptr, code)->show();

        //copy clipboard code
        clipboardText(std::to_string(GM->m_nSecretNumberRand1 - GM->m_nSecretNumberRand2).c_str());

    }
};
    

bool (__thiscall* secretLayer2T)(SecretLayer2* self);

bool  __fastcall secretLayer2H(SecretLayer2* self) {
    bool init = secretLayer2T(self);

    auto sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png");

    auto btn = gd::CCMenuItemSpriteExtra::create(
        sprite,
        self,
        menu_selector(SecretLayer2::cod3breakerCallback)
    );

    auto menu = cocos2d::CCMenu::create();

    menu->setPosition(45, 45);
    menu->addChild(btn);

    self->addChild(menu);

    return init;
}

DWORD WINAPI thread_func(void* hModule) {


    MH_Initialize();

    auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));

    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x21FE10),
        secretLayer2H,
        reinterpret_cast<void**>(&secretLayer2T)
    );

    MH_EnableHook(MH_ALL_HOOKS);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0x100, thread_func, handle, 0, 0);
    }
    return TRUE;
}