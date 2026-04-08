#pragma once

#include "NovaWidgets.hpp"

void InitializeNovaFonts() {
    if (Fonts::Large) NovaUI::FontBold = Fonts::Large;
}

void Backend::DrawImGui(ID3D11DeviceContext* context, ID3D11RenderTargetView* targetview) const {
    if (!context || !targetview) return;

    ImGuiIO& io = ImGui::GetIO();
    io.MouseDrawCursor = Features.OpenMenu;

    if (NovaUI::ThemeImages.empty()) {
        ID3D11Device* device = nullptr;
        context->GetDevice(&device);

        if (device) {
            using namespace NovaUI;
            int uW, uH;
            LoadTextureFromBytes(device, user, sizeof(user), &NovaUI::UserTexture, &uW, &uH);
            /*
                 auto AddThemeImg = [&](const char* name, ImVec4 col, unsigned char* data, size_t size) {
                     ThemeImageEntry entry;
                     entry.name = name;
                     entry.color = col;
                     entry.data = data;
                     entry.size = size;
                     entry.texture = nullptr;
                     entry.width = 0;
                     entry.height = 0;
                     LoadTextureFromBytes(device, data, (int)size, &entry.texture, &entry.width, &entry.height);
                     ThemeImages.push_back(entry);
                     };

                 AddThemeImg("Blue", ImVec4(0.0f, 0.0f, 1.0f, 1.0f), blue, sizeof(blue));
                 AddThemeImg("Yellow", ImVec4(1.0f, 1.0f, 0.0f, 1.0f), yellow, sizeof(yellow));
                 AddThemeImg("White", ImVec4(1.0f, 1.0f, 1.0f, 1.0f), white, sizeof(white));
                 AddThemeImg("Green", ImVec4(0.0f, 1.0f, 0.0f, 1.0f), green, sizeof(green));
                 AddThemeImg("Purple", ImVec4(0.6f, 0.0f, 0.8f, 1.0f), purple, sizeof(purple));
                 AddThemeImg("Red", ImVec4(1.0f, 0.0f, 0.0f, 1.0f), red, sizeof(red));
                 AddThemeImg("Pink", ImVec4(1.0f, 0.4f, 0.7f, 1.0f), pink, sizeof(pink));
            */
            device->Release();
        }
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (Features.frameCounter > 0) {
        Features.frameCounter--;
        if (Features.frameCounter == 0) {
            ImGui::InsertNotification({ ImGuiToastType::Info, 12000, "Nova Loaded, press RCTRL / RShift / Insert / F1 to open/close menu" });
        }
    }

    using namespace NovaUI;
    const float dt = ImGui::GetIO().DeltaTime;
    GlowTime += dt;

    ImGui::ThemeColor = NovaUI::ThemeColor;

    UpdateClosestTexture();

    float targetAlpha = Features.OpenMenu ? 1.0f : 0.0f;
    float animSpeed = Features.OpenMenu ? 12.0f : 15.0f;
    MenuAlpha = MenuAlpha + (targetAlpha - MenuAlpha) * ImClamp(animSpeed * dt, 0.0f, 1.0f);
    if (std::abs(MenuAlpha - targetAlpha) < 0.01f) MenuAlpha = targetAlpha;

    CurrentAlpha = MenuAlpha;

    if (MenuAlpha > 0.0f) {
        static bool hasInitialized = false;
        if (!hasInitialized && MenuAlpha > 0.99f) {
            Variables::Miscellaneous::DumpOfferItemTypesInitial = true;
            Variables::Miscellaneous::m_bAddXP = true;
            hasInitialized = true;
        }
        ImVec2 display = ImGui::GetIO().DisplaySize;
        ImDrawList* bgDl = ImGui::GetBackgroundDrawList();
        bgDl->AddRectFilled(ImVec2(0, 0), display, IM_COL32(0, 0, 5, static_cast<int>(CurrentAlpha * 180)));

        float scaledWidth = S(720.0f), scaledHeight = S(540.0f), scaledRounding = S(12.0f);
        if (WindowPos.x < 0 || WindowPos.y < 0) WindowPos = ImVec2((display.x - scaledWidth) * 0.5f, (display.y - scaledHeight) * 0.5f);

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, CurrentAlpha);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, scaledRounding);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, S(ImVec2(6, 6)));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImGui::ColorConvertU32ToFloat4(ColBg()));

        ImGui::SetNextWindowSize(ImVec2(scaledWidth, scaledHeight));
        ImGui::SetNextWindowPos(WindowPos, ImGuiCond_Always);

        if (ImGui::Begin("##NovaMenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
            ImVec2 wp = ImGui::GetWindowPos(), ws = ImGui::GetWindowSize();
            ImDrawList* dl = ImGui::GetWindowDrawList();

            ImVec2 mousePos = ImGui::GetMousePos();
            float sidebarW = S(115.0f), topBarH = S(52.0f), topBarPad = S(10.0f);

            float pad = S(10.0f);
            float sidebarX = wp.x + pad;
            float contentX = sidebarX + sidebarW + pad;
            float contentW = ws.x - sidebarW - pad * 3;
            float panelGap = S(10.0f);
            float panelW = (contentW - panelGap) * 0.5f;

            bool inDragArea = (mousePos.x >= wp.x && mousePos.x <= wp.x + sidebarW + topBarPad && mousePos.y >= wp.y && mousePos.y <= wp.y + ws.y) ||
                (mousePos.x >= wp.x + sidebarW && mousePos.x <= wp.x + ws.x && mousePos.y >= wp.y && mousePos.y <= wp.y + topBarPad + topBarH + topBarPad);

            if (inDragArea && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered() && !ImGui::IsPopupOpen((ImGuiID)0, ImGuiPopupFlags_AnyPopupId)) {
                IsDragging = true;
                DragOffset = ImVec2(mousePos.x - wp.x, mousePos.y - wp.y);
            }
            if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) IsDragging = false;
            if (IsDragging) {
                ImVec2 newPos = ImVec2(mousePos.x - DragOffset.x, mousePos.y - DragOffset.y);
                ImGuiViewport* vp = ImGui::GetMainViewport();
                WindowPos = ImVec2(ImClamp(newPos.x, vp->Pos.x - ws.x + S(50.0f), vp->Pos.x + vp->Size.x - S(50.0f)), ImClamp(newPos.y, vp->Pos.y, vp->Pos.y + vp->Size.y - S(50.0f)));
            }

            float glowPulse = 0.85f + 0.15f * sinf(GlowTime * 2.0f);
            bgDl->AddShadowRect(ImVec2(wp.x + S(6), wp.y + S(6)), ImVec2(wp.x + ws.x - S(6), wp.y + ws.y - S(6)),
                ImGui::ColorConvertFloat4ToU32(ImVec4(ThemeColor.x * glowPulse * 1.3f, ThemeColor.y * glowPulse * 1.3f, ThemeColor.z * glowPulse * 1.3f, 0.6f * CurrentAlpha)),
                S(70.0f), ImVec2(0, S(4.0f)), ImDrawFlags_RoundCornersAll, scaledRounding);

            ImVec2 topBarY(wp.y + topBarPad, wp.y + topBarPad + topBarH);

            ImVec2 topLeftMin(sidebarX, topBarY.x);
            ImVec2 topLeftMax(sidebarX + sidebarW, topBarY.y);
            dl->AddRectFilled(topLeftMin, topLeftMax, ColTopBar(), S(10.0f));

            ImVec2 topRightMin(contentX, topBarY.x);
            ImVec2 topRightMax(wp.x + ws.x - pad, topBarY.y);
            dl->AddRectFilled(topRightMin, topRightMax, ColTopBar(), S(10.0f));

            {
                float btnHeight = S(32.0f);
                float spacing = S(6.0f);
                float rightMargin = S(14.0f);
                float currentX = topRightMax.x - rightMargin;
                float fs = FontSize();

                auto DrawBtn = [&](const char* icon, const char* text, bool active, auto onClick) {
                    float textWidth = 0.0f;
                    if (text) {
                        ImVec2 txtSz = GetFont()->CalcTextSizeA(fs, FLT_MAX, 0.0f, text);
                        textWidth = txtSz.x + S(4.0f);
                    }

                    float totalWidth = btnHeight + (text ? textWidth : 0.0f);
                    float btnY = topRightMin.y + (topBarH - btnHeight) * 0.5f;

                    currentX -= totalWidth;
                    ImVec2 btnMin(currentX, btnY);
                    ImVec2 btnMax(btnMin.x + totalWidth, btnMin.y + btnHeight);

                    bool hovered = ImGui::IsMouseHoveringRect(btnMin, btnMax);

                    if (hovered || active)
                        dl->AddRectFilled(btnMin, btnMax, ImGui::GetColorU32(ImVec4(1.f, 1.f, 1.f, 0.08f)), S(6.0f));

                    ImVec2 tSz = ImGui::CalcTextSize(icon);
                    dl->AddText(ImVec2(btnMin.x + (btnHeight - tSz.x) * 0.5f, btnMin.y + (btnHeight - tSz.y) * 0.5f),
                        (hovered || active) ? ColTheme() : ColText(), icon);

                    if (text) {
                        dl->AddText(GetFont(), fs, ImVec2(btnMin.x + btnHeight + S(2.0f), btnMin.y + (btnHeight - tSz.y) * 0.5f),
                            (hovered || active) ? ColTheme() : ColTextDim(), text);
                    }

                    if (hovered && ImGui::IsMouseClicked(0)) onClick();
                    };

                DrawBtn(ICON_FA_ROTATE_RIGHT, "Reload", false, [&]() { Variables::Websocket::m_bReloadSocket = true; });

                currentX -= S(4.0f);
                float sepH = S(14.0f);
                dl->AddLine(ImVec2(currentX, topRightMin.y + (topBarH - sepH) * 0.5f),
                    ImVec2(currentX, topRightMin.y + (topBarH + sepH) * 0.5f), ColSeparator(), S(1.0f));
                currentX -= S(4.0f);

                char fpsBuf[32];
                snprintf(fpsBuf, 32, "%d FPS", (int)ImGui::GetIO().Framerate);
                ImVec2 fpsSz = GetFont()->CalcTextSizeA(fs, FLT_MAX, 0.0f, fpsBuf);
                currentX -= fpsSz.x;
                dl->AddText(GetFont(), fs, ImVec2(currentX, topRightMin.y + (topBarH - fpsSz.y) * 0.5f), ColTextDim(), fpsBuf);

                currentX -= S(4.0f);
                dl->AddLine(ImVec2(currentX, topRightMin.y + (topBarH - sepH) * 0.5f),
                    ImVec2(currentX, topRightMin.y + (topBarH + sepH) * 0.5f), ColSeparator(), S(1.0f));
                currentX -= S(4.0f);

                const char* verStr = "V1.2b+fix";
                ImVec2 verSz = GetFont()->CalcTextSizeA(fs, FLT_MAX, 0.0f, verStr);
                currentX -= verSz.x;
                dl->AddText(GetFont(), fs, ImVec2(currentX, topRightMin.y + (topBarH - verSz.y) * 0.5f), ColTextDim(), verStr);
            }

            float logoScale = 1.8f;
            float logoSize = FontSize() * logoScale;
            float logoY = topLeftMin.y + (topBarH - logoSize) * 0.5f;

            dl->AddText(GetFontBold(), logoSize, ImVec2(topLeftMin.x + S(16), logoY + S(2)), IM_COL32(0, 0, 0, 100), "NOVA");
            dl->AddText(GetFontBold(), logoSize, ImVec2(topLeftMin.x + S(14), logoY), ColLogo(), "NOVA");

            float novaW = ImGui::CalcTextSize("NOVA").x * Scale() * logoScale;
            float pulseFactor = (sinf(GlowTime * 3.0f) + 1.0f) * 0.5f;
            ImVec4 baseCol = ThemeColor;
            ImVec4 targetCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            ImVec4 finalPulseCol = ImLerp(baseCol, targetCol, pulseFactor * 0.4f);
            finalPulseCol.w = 1.0f;
            dl->AddText(GetFontBold(), logoSize, ImVec2(topLeftMin.x + S(18) + novaW, logoY), ImGui::GetColorU32(finalPulseCol), "+");

            float mainAreaY = wp.y + topBarPad + topBarH + topBarPad;
            float sidebarY = mainAreaY;
            float sidebarH = ws.y - mainAreaY - pad + wp.y;
            float contentY = sidebarY;
            float contentH = sidebarH;

            dl->AddRectFilled(ImVec2(sidebarX, sidebarY), ImVec2(sidebarX + sidebarW, sidebarY + sidebarH), ColSidebar(), S(8.0f));
            for (int i = 0; i < TabCount; i++) {
                ImGui::SetCursorScreenPos(ImVec2(sidebarX + S(3), sidebarY + S(8) + i * S(36)));
                ImGui::PushID(i);
                CategoryButton(i, TabNames[i], ActiveTab == i, sidebarW - S(6), S(32), i == TabCount - 1);
                ImGui::PopID();
            }

            if (CurrentSidebarTexture && CurrentSidebarWidth > 0 && CurrentSidebarHeight > 0) {
                float padding = 8.0f;
                float availableWidth = sidebarW - padding;
                float aspectRatio = (float)CurrentSidebarHeight / (float)CurrentSidebarWidth;
                float drawHeight = availableWidth * aspectRatio;

                ImVec2 imgMin(sidebarX + (sidebarW - availableWidth) * 0.5f, (sidebarY + sidebarH) - drawHeight - S(4.0f));
                ImVec2 imgMax(imgMin.x + availableWidth, imgMin.y + drawHeight);

                dl->AddImage(CurrentSidebarTexture, imgMin, imgMax, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, (int)(255 * CurrentAlpha)));
            }

            float singlePanelH = contentH;
            float halfPanelH = (contentH - panelGap) * 0.5f;

            ImVec2 p1(contentX, contentY), p2(contentX + panelW + panelGap, contentY), p3(contentX, contentY + halfPanelH + panelGap), p4(contentX + panelW + panelGap, contentY + halfPanelH + panelGap);

            ImVec2 pFullLeft(contentX, contentY);
            ImVec2 pFullRight(contentX + panelW + panelGap, contentY);

            switch (ActiveTab) {
            case 0: // Main
                BeginChildPanel("##weapon_adder", ICON_FA_PLUS, "WEAPON ADDER", p1, ImVec2(panelW, halfPanelH));
                {
                    SliderInt("Weapon Level", &Variables::Websocket::m_iWeaponLevel, 1, 65);
                    ImGui::Spacing();
                    float halfW = (CurrentPanelContentWidth - S(6.0f)) * 0.5f;

                    if (Button("Add All Weapons", halfW, S(24.0f))) Variables::Websocket::m_bAddAllWeapons = true;
                    ImGui::SameLine(0, S(6.0f));
                    if (Button("Add All Primary", halfW, S(24.0f))) Variables::Websocket::m_bAddAllPrimary = true;

                    if (Button("Add All Backup", halfW, S(24.0f))) Variables::Websocket::m_bAddAllBackup = true;
                    ImGui::SameLine(0, S(6.0f));
                    if (Button("Add All Melee", halfW, S(24.0f))) Variables::Websocket::m_bAddAllMelee = true;

                    if (Button("Add All Special", halfW, S(24.0f))) Variables::Websocket::m_bAddAllSpecial = true;
                    ImGui::SameLine(0, S(6.0f));
                    if (Button("Add All Sniper", halfW, S(24.0f))) Variables::Websocket::m_bAddAllSniper = true;

                    if (Button("Add All Heavy", halfW, S(24.0f))) Variables::Websocket::m_bAddAllHeavy = true;
                    ImGui::SameLine(0, S(6.0f));
                    if (Button("Craft Guns", halfW, S(24.0f))) Variables::Websocket::m_bAddAllCraftable = true;

                    if (Button("Weapon Parts", halfW, S(24.0f))) Variables::Websocket::m_bAddAllWeaponParts = true;
                }
                EndChildPanel();

                BeginChildPanel("##weapon_remover", ICON_FA_MINUS, "WEAPON REMOVER", p2, ImVec2(panelW, halfPanelH));
                {
                    float halfW = (CurrentPanelContentWidth - S(6.0f)) * 0.5f;

                    if (Button("All Primary", halfW, S(24.0f))) Variables::Websocket::m_bRemoveAllPrimary = true;
                    ImGui::SameLine(0, S(6.0f));
                    if (Button("All Backup", halfW, S(24.0f))) Variables::Websocket::m_bRemoveAllBackup = true;

                    if (Button("All Melee", halfW, S(24.0f))) Variables::Websocket::m_bRemoveAllMelee = true;
                    ImGui::SameLine(0, S(6.0f));
                    if (Button("All Special", halfW, S(24.0f))) Variables::Websocket::m_bRemoveAllSpecial = true;

                    if (Button("All Sniper", halfW, S(24.0f))) Variables::Websocket::m_bRemoveAllSniper = true;
                    ImGui::SameLine(0, S(6.0f));
                    if (Button("All Heavy", halfW, S(24.0f))) Variables::Websocket::m_bRemoveAllHeavy = true;

                    if (Button("Remove All")) Variables::Websocket::m_bRemoveAllWeapons = true;
                }
                EndChildPanel();

                BeginChildPanel("##misc_tab0", ICON_FA_ELLIPSIS, "MISC", p3, ImVec2(panelW, halfPanelH));
                {
                    ImGui::Spacing();
                    Text("Set Player Level");
                    SliderInt("Level", &Variables::Websocket::m_iSetLevel, 1, 65);
                    if (Button("Set Level")) Variables::Websocket::m_bSetLevel = true;
                    ImGui::Spacing();
                    if (Button("Complete Tutorial")) Variables::Websocket::m_bCompleteTutorial = true;
                    InputText("Friend ID", Variables::Websocket::m_cFriendID, 256);
                    ImGui::Spacing();
                    if (Button("Send Friend Request")) Variables::Websocket::m_bSendFriendRequest = true;
                    ImGui::Spacing();
                    InputText("Clan Message", Variables::Websocket::m_cClanMessage, 512);
                    ImGui::Spacing();
                    if (Button("Send Clan Message")) Variables::Websocket::m_bSendClanMessage = true;
                }
                EndChildPanel();

                BeginChildPanel("##adder_all", ICON_FA_CROWN, "ACCOUNT MAXING", p4, ImVec2(panelW, halfPanelH));
                {
                    float halfW = (CurrentPanelContentWidth - S(6.0f)) * 0.5f;
                    if (Button("Add Weapons", halfW, S(24.0f))) Variables::Websocket::m_bAddAllWeapons = true;
                    ImGui::SameLine(0, S(6.0f));
                    if (Button("Add Armors", halfW, S(24.0f))) Variables::Websocket::m_bAddAllArmor = true;
                    if (Button("Add Wears", halfW, S(24.0f))) Variables::Websocket::m_bAddAllWear = true;
                    ImGui::SameLine(0, S(6.0f));
                    if (Button("Add Avatars", halfW, S(24.0f))) Variables::Websocket::m_bAddAllAvatars = true;
                    if (Button("Add Trails", halfW, S(24.0f))) Variables::Websocket::m_bAddAllTrails = true;
                    ImGui::SameLine(0, S(6.0f));
                    if (Button("Add Gadgets", halfW, S(24.0f))) Variables::Websocket::m_bAddAllGadgets = true;
                    if (Button("Add Vehicles", halfW, S(24.0f))) Variables::Websocket::m_bAddAllVehicles = true;
                    ImGui::SameLine(0, S(6.0f));
                    if (Button("Add Pets", halfW, S(24.0f))) Variables::Websocket::m_bAddAllPets = true;
                    if (Button("Add Modules", halfW, S(24.0f))) Variables::Websocket::m_bAddAllModules = true;
                    ImGui::SameLine(0, S(6.0f));
                    if (Button("Add Graffiti", halfW, S(24.0f))) Variables::Websocket::m_bAddAllGraffiti = true;
                    if (Button("Weapon Skins", halfW, S(24.0f))) Variables::Websocket::m_bAddAllWeaponSkins = true;
                    ImGui::SameLine(0, S(6.0f));
                    if (Button("Gun Upgrades", halfW, S(24.0f))) Variables::Websocket::m_bAddAllFreeUpgrades = true;
                }
                EndChildPanel();
                break;
            case 1: // Gameplay
                BeginChildPanel("##gameplaymods", ICON_FA_GAMEPAD, "GAMEPLAY MODS", pFullLeft, ImVec2(contentW, singlePanelH));
                {
                    Checkbox("Shoot Lasers", &Variables::Gameplay::m_bRailgun);
                    Checkbox("Custom Chat", &Variables::Miscellaneous::m_bChatSpam);
                    Checkbox("Full Screen SPAM", &Variables::Miscellaneous::m_bChatFullScreen);
                    Checkbox("Shoot Rockets", &Variables::Gameplay::m_bBazooka);
                    Checkbox("Shoot Shotgun", &Variables::Gameplay::m_bShotgun);
                    Checkbox("Shoot Dash", &Variables::Gameplay::m_bDash);
                    Checkbox("Shoot Harpoon", &Variables::Gameplay::m_bHarpoon);
                    Checkbox("Exploding Bullets", &Variables::Gameplay::m_bExplodingBullets);
                    Checkbox("Quick Scope", &Variables::Gameplay::m_bQuickScope);
                    Checkbox("Stacking Blindness", &Variables::Gameplay::m_bBlindness);
                    Checkbox("Firerate", &Variables::Gameplay::m_bFastFireRate);
                    Checkbox("Ammo", &Variables::Gameplay::m_bInfiniteAmmo);
                }
                EndChildPanel();
                break;
            case 2: // Misc
                BeginChildPanel("##testing", ICON_FA_PEN, "WEBSOCKET CONSOLE", pFullLeft, ImVec2(panelW, singlePanelH));
                {
                    InputText("Event Name", Variables::Websocket::m_cCustomEventName, 256);
                    ImGui::Spacing();
                    InputText("Command", Variables::Websocket::m_cCustomJson, 999999);
                    ImGui::Spacing();
                    if (Button("Send Request")) Variables::Websocket::m_bSendCustomCommand = true;
                }
                EndChildPanel();
                BeginChildPanel("##dumps", ICON_FA_CIRCLE_DOWN, "DUMPS", pFullRight, ImVec2(panelW, singlePanelH));
                {
                    if (Button("Dump Offer Item Types")) Variables::Miscellaneous::DumpOfferItemTypes = true;
                    if (Button("Dump Game")) Variables::Miscellaneous::m_bDumpGame = true;
                    if (Button("Dump Event Names")) Variables::Miscellaneous::m_bDumpEventNames = true;
                    if (Button("HD SKIN TEST")) Variables::Miscellaneous::m_bSkinTest = true;
                    if (Button("Token")) Variables::Miscellaneous::m_bOutputToken = true;
                    if (Button("Auth Info")) Variables::Miscellaneous::m_bOutputAuthInfo = true;
                    if (Button("XP")) Variables::Miscellaneous::m_bAddXP = true;
                    if (Button("Achievements")) UnlockAllAchievements();
                }
                EndChildPanel();
                BeginChildPanel("##playerinfo", ICON_FA_CIRCLE_INFO, "PLAYER INFO", p3, ImVec2(panelW, halfPanelH));
                {
                    Text("Name: %s", Variables::Miscellaneous::m_sPlayerUsername.c_str());
                    Text("Level: %d", Variables::Miscellaneous::m_iPlayerLevel);
                    Text("Version: %s", Variables::Miscellaneous::m_sGameVersion.c_str());
                    Text("ID: %s", Variables::Miscellaneous::m_sPlayerID.c_str());
                    ImGui::Spacing();
                    ImGui::Separator();
                    Text("Player Cloning");
                    static char idbuffer[16] = "";
                    InputText("Player ID", idbuffer, sizeof(idbuffer));
                    Variables::Miscellaneous::m_sOtherPlayerID = idbuffer;
                    ImGui::Spacing();
                    float halfW = (CurrentPanelContentWidth - S(6.0f)) * 0.5f;
                    if (Button("Clone Name", halfW, S(24.0f))) Variables::Websocket::m_bCloneName = true;
                    ImGui::SameLine(0, S(6.0f));
                    if (Button("Clone Skins", halfW, S(24.0f))) Variables::Websocket::m_bCloneSkins = true;
                    if (Button("Clone Cape", halfW, S(24.0f))) Variables::Websocket::m_bCloneCape = true;
                    ImGui::SameLine(0, S(6.0f));
                    if (Button("Check Slots", halfW, S(24.0f))) Variables::Miscellaneous::m_bCheckAllOtherPlayerSlots = true;
                    ImGui::Spacing();
                    if (Button("Check My Slots")) Variables::Miscellaneous::m_bCheckAllMySlots = true;
                }
                EndChildPanel();
                break;
            case 3: // Settings
                BeginChildPanel("##settings_left", ICON_FA_PAINTBRUSH, "APPEARANCE", pFullLeft, ImVec2(panelW, singlePanelH));
                {
                    ColorEdit("Theme Color", &ThemeColor);
                    ImGui::Spacing();
                    Combo("Theme", &CurrentThemeIndex, ThemeNames, 45);
                    ImGui::Spacing();
                    ImGui::GetWindowDrawList()->AddText(GetFont(), FontSize(), ImGui::GetCursorScreenPos(), ColText(), "Menu Scale");
                    ImGui::Dummy(ImVec2(0, FontSize()));
                    Combo("Scale", &DPIIndex, DPILabels, 6);
                }
                EndChildPanel();
                BeginChildPanel("##settings_right", ICON_FA_BOOK, "CREDITS", pFullRight, ImVec2(panelW, singlePanelH));
                {
                    Text("Main Dev: Leminare");
                    Text("Femboy Support: Mazzy");
                    Text("Dari, SotoSapi(Proplamatic)");
                    Text("Yeet (I Pasted some Asteroid Stuff)");
                    Text("Dumper: Sxitxma");
                    Text("Hes black: Meth");
                    Text("Tweaks & Fork: .lurk_r");
                    TextDim("Special thanks to contributors!");
                }
                EndChildPanel();
                break;
            }
            ImGui::End();
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(4);
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(18.f / 255.f, 18.f / 255.f, 22.f / 255.f, 0.94f));
    ImGui::RenderNotifications();
    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(1);

    ImGui::EndFrame();
    ImGui::Render();
    context->OMSetRenderTargets(1, &targetview, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}