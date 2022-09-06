#include "UltraEngine.h"
#include "GLOBAL.h"

using namespace UltraEngine;


bool Callback(const Event& ev, shared_ptr<Object> extra)
{
    //Resize window if desired
    auto window = ev.source->As<Window>();
    window->SetShape(ev.position, ev.size);

    //Get the user interface
    auto ui = extra->As<Interface>();
    ui->SetScale(float(ev.data) / 100.0f);

    return true;
}



int main(int argc, const char* argv[])
{
    //Get the displays
    auto displays = GetDisplays();

    //Create a window
    auto window = CreateWindow(APP_NAME, 0, 0, WWIDTH, WHEIGHT, displays[0], WINDOW_TITLEBAR | WINDOW_RESIZABLE | WINDOW_CENTER);
    window->SetMinSize(WWIDTH, WHEIGHT);
	
    //Create User Interface
    auto ui = CreateInterface(window);

    //Create widget
    auto sz = ui->root->GetSize();
    auto wrapPanel = CreatePanel(0, 0, sz.x, sz.y, ui->root);
    wrapPanel->SetLayout(1,1,1,1);

#pragma region topMenu
    auto topMenu = CreateMenu(L"", ui->root);
    
    //Top menu file section
    auto topMenu_file = CreateMenu(L"File", topMenu);
    auto topMenu_file_new = CreateMenu(L"New", topMenu_file);
    auto topMenu_file_open = CreateMenu(L"Open", topMenu_file);
    CreateMenu(L"", topMenu_file);
    auto topMenu_file_update = CreateMenu(L"Search for update", topMenu_file);
    CreateMenu(L"", topMenu_file);
    auto topMenu_file_quit = CreateMenu(L"Quit", topMenu_file);
	
	//Top menu about section
    auto topMenu_about = CreateMenu(L"About", topMenu);
    auto topMenu_about_SR = CreateMenu(L"Steam reviewer", topMenu_about);
    auto topMenu_about_me = CreateMenu(L"Me", topMenu_about);

	//Top menu help section
    auto topMenu_help = CreateMenu(L"Help", topMenu);
	
#pragma endregion The top menu code.

	auto mainPanel = CreatePanel(margin, margin+ topMenu->GetSize().y, sz.x - margin*2, sz.y - margin * 2 - topMenu->GetSize().y, wrapPanel, PANEL_BORDER);
    mainPanel->SetLayout(1, 1, 1, 1);
    iVec2 mainPanelSize = mainPanel->GetSize();
    auto mainPanel_formWrap = CreatePanel(0, 0, mainPanelSize.x /1.5, mainPanelSize.y, mainPanel);
	mainPanel_formWrap->SetLayout(1, 1, 1, 1);
#ifdef DEBUG
    mainPanel_formWrap->SetColor(Vec4(1, 0, 0, 1));
#endif
	//Creating the main interface form on the right wrap pane
	
    auto mainPanel_rightPanelWrap = CreatePanel(mainPanel_formWrap->GetSize().x, 0, mainPanelSize.x-mainPanel_formWrap->GetSize().x-margin, mainPanelSize.y, mainPanel);
	mainPanel_rightPanelWrap->SetLayout(0, 1, 1, 1);
#ifdef DEBUG
	mainPanel_rightPanelWrap->SetText(L"General review information");
    mainPanel_rightPanelWrap->SetColor(Vec4(0, 1, 0, 1));
#endif
	
    //Creating the left side interface with review and general information
	
	//TODO: change the text of the button by the thumbs up / down icon from Steam (TODO: download the thumbs up / down icon from Steam!)
	auto rightPanel_reviewTU = CreateButton(L"Thumbs up", margin, margin, mainPanel_rightPanelWrap->GetSize().x/2-margin*4, 24, mainPanel_rightPanelWrap, BUTTON_TOGGLE);
    auto rightPanel_reviewTD = CreateButton(L"Thumbs down", mainPanel_rightPanelWrap->GetSize().x / 2 + margin * 2, margin, mainPanel_rightPanelWrap->GetSize().x / 2 - margin * 4, 24, mainPanel_rightPanelWrap, BUTTON_TOGGLE);


	
	
    ListenEvent(EVENT_WINDOWDPICHANGED, window, Callback, ui);
    //Trigger a rescale if the display scale is not 100%
    if (displays[0]->scale != 1.0f)
    {
        EmitEvent(EVENT_WINDOWDPICHANGED, window, Round(displays[0]->scale * 100.0f), 0, 0, WWIDTH * displays[0]->scale, WHEIGHT * displays[0]->scale);
    }
	
    while (true)
    {
        const Event ev = WaitEvent();
        switch (ev.id)
        {
            case EVENT_WIDGETACTION:
                if (ev.source == topMenu_file_quit) {
					//We want to close the app
                    EmitEvent(EVENT_WINDOWCLOSE, window);
                }
				
				//We check for what button has been pressed between the two rightPanel_reviewTU and TD
				//When one of these two gets activated, the other one gets deactivated
				if (ev.source == rightPanel_reviewTU) {
					rightPanel_reviewTD->SetState(0);
                }
                else if (ev.source == rightPanel_reviewTD) {
                    rightPanel_reviewTU->SetState(0);
                }
                break;
            case EVENT_WIDGETSELECT:
               
                break;
            case EVENT_QUIT:
            case EVENT_WINDOWCLOSE:
                return 0;
                break;
            default: break;
        }
    }
    return 0;
}
