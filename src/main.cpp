#define UNICODE
#define _UNICODE

#include <windows.h>
#include <iostream>
#include <string>
#include <gdiplus/gdiplus.h>
#include "func.hpp"
#include <curl_http.h>
#include <map>
#include <Shellapi.h>
#include <client_id.h>

#define TEXT_INPUT 1001
#define TEXT_ENTER_USR 1002
#define CONFIRM_USERNAME_BUTTON 1003
#define TITLE_MAIN 1006
#define TITLE_OUT 1007
#define NEXT_BUTTON 1008
#define CONFIRM_GENRES_BUTTON 1009
#define GEN_MAIN_TEXT 1010
#define STATUS_MAIN_TEXT 1011

#define CONTENTS_MAIN 1020
#define CONTENTS_OUT 1030

#define GENRE_CHECKBOX 1100
#define STATUS_CHECKBOX 1200

// enum winMenu{
//     text_input = 1001,
//     text_enter_usr = 1002,
//     confirm_username_button = 1003,
//     username_main = 1004,
//     username_out = 1005,
//     title_main = 1006,
//     Title_out = 1007,
//     next_button = 1008,
// };

// enum class dataEnum {
//     completed,
//     airing,
//     not_yet_aired
// };

static std::map<std::string, int> status = {{"finished_airing", 1}, {"currently_airing", 1}, {"not_yet_aired", 1}};
static std::map<uint16_t, dataStatus> genres;
static std::vector<Anime> animeList;
static const int numOfGenres = 76;
static const int numOfTypeStatus = 3;
const unsigned int numOfContents = 10;

LRESULT CALLBACK WindowProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

HINSTANCE g_hInstance;

const int width = 600, height = 700;
std::string g_username;

Gdiplus::Image* image = nullptr, *bg_img = nullptr;
Gdiplus::Graphics* graphics = nullptr, *graphics_background = nullptr;
pos_size* imgPos = new pos_size;
pos_size* bg_imgPos = new pos_size;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR LpCmdLine, int nCmdShow)
{
    HWND hwnd_console = GetConsoleWindow();
    ShowWindow(hwnd_console, SW_HIDE);
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
    WNDCLASS wc = {};
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = L"MainClass";
    wc.hIcon = (HICON) LoadImage( // returns a HANDLE so we have to cast to HICON
    NULL,             // hInstance must be NULL when loading from a file
    L"res/ico/RandomizeAnime.ico",   // the icon file name
    IMAGE_ICON,       // specifies that the file is an icon
    0,                // width of the image (we'll specify default later on)
    0,                // height of the image
    LR_LOADFROMFILE|  // we want to load a file (as opposed to a resource)
    LR_DEFAULTSIZE|   // default metrics based on the type (IMAGE_ICON, 32x32)
    LR_SHARED);
    if( !RegisterClass(&wc))
    {
        MessageBoxW(NULL, L"Coś się odjebało", NULL, MB_ICONEXCLAMATION | MB_OK );
        return 1;
    }
    HWND hwnd = CreateWindowEx(
    0,                              // Optional window styles.
    wc.lpszClassName,               // Window class
    L"RandomizeAnime",                         // Window text
    WS_OVERLAPPEDWINDOW,            // Window style
    CW_USEDEFAULT, CW_USEDEFAULT, width, height, // Size and position
    NULL,                           // Parent window
    NULL,                           // Menu
    hInstance,                      // Instance handle
    NULL                            // Additional application data
    );
    if (hwnd == NULL)
    {
        return 0;
    }
    HFONT button_font = CreateFont(18, 0, 0, 0, FW_BOLD, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        TEXT("Calibri"));
    HFONT static_font = CreateFont(18, 0, 0, 0, FW_BOLD, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        TEXT("Cambria"));
    HFONT enter_font = CreateFont(16, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        TEXT("Arial"));
    // creating a button
    pos_size button = {0, 0, 150, 30};
    button.x = (width-button.width)/2;
    button.y = height/2-50;
    HWND hButton = CreateWindowExW(0, L"BUTTON", L"Confirm", WS_CHILD | BS_PUSHBUTTON, button.x, button.y, button.width, button.height,
    hwnd, HMENU(CONFIRM_USERNAME_BUTTON), hInstance, NULL );
    SendMessage(hButton, WM_SETFONT, (WPARAM)button_font, TRUE);
    pos_size input = {0, 0, 150, 30};
    input.x = (width-input.width)/2;
    input.y = height/2-100;
    HWND hText = CreateWindowEx(0 , L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, input.x, input.y, input.width, input.height,
    hwnd, HMENU(TEXT_INPUT), hInstance, NULL );
    SendMessage(hText, WM_SETFONT, (WPARAM)enter_font, TRUE);

    HWND hStaticTextUsr = CreateWindowEx(WS_EX_LEFT, L"Static", NULL, WS_CHILD | WS_VISIBLE, input.x, input.y-input.height, 150, 20,
    hwnd, HMENU(TEXT_ENTER_USR), hInstance, NULL);
    SendMessage(hStaticTextUsr, WM_SETFONT, (WPARAM)static_font, TRUE);
    SetWindowText( hStaticTextUsr, L"Enter Username:" );

    HWND hStaticTextStatus = CreateWindowEx(WS_EX_LEFT, L"Static", NULL, WS_CHILD, width/2 - 100/2 -10, 20, 100, 20,
    hwnd, HMENU(STATUS_MAIN_TEXT), hInstance, NULL);
    SendMessage(hStaticTextStatus, WM_SETFONT, (WPARAM)static_font, TRUE);
    HWND hStatusCheckBoxes[numOfTypeStatus];
    {
        pos_size basicCheckBox = {100, 50, 110, 20};
        int i = 0;
        HFONT chechBox_font = CreateFont(18, 0, 0, 0, FW_BOLD, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        TEXT("Calibri"));
        for(HWND &checkBox : hStatusCheckBoxes)
        {
            checkBox = CreateWindowExW(0, L"BUTTON", L"status", WS_CHILD | BS_AUTOCHECKBOX, basicCheckBox.x+i*(basicCheckBox.width+20), basicCheckBox.y, basicCheckBox.width, basicCheckBox.height,
            hwnd, HMENU(STATUS_CHECKBOX+i), hInstance, NULL);
            SendMessage(checkBox, WM_SETFONT, (WPARAM) chechBox_font, TRUE);
            i++;
        }
    }

    HWND hStaticTextGen = CreateWindowEx(WS_EX_LEFT, L"Static", NULL, WS_CHILD, width/2 - 100/2 -10, 100, 100, 20,
    hwnd, HMENU(GEN_MAIN_TEXT), hInstance, NULL);
    SendMessage(hStaticTextGen, WM_SETFONT, (WPARAM)static_font, TRUE);
    HWND hGenreCheckBoxes[numOfGenres];
    {
        pos_size basicCheckBox = {20, 130, 100, 20};
        const int numOfColumns = (width-2*basicCheckBox.x)/(basicCheckBox.width+10);
        int i = 0, j = 0;
        HFONT chechBox_font = CreateFont(13, 0, 0, 0, FW_BOLD, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        TEXT("Calibri"));
        for(HWND &checkBox: hGenreCheckBoxes)
        {
            if(i>=numOfColumns)
            {
                j++;
                i=0;
            }
            //std::cout<<"Print"<<std::endl;
            checkBox = CreateWindowExW(0, L"BUTTON", L"box", WS_CHILD | BS_AUTO3STATE,
            basicCheckBox.x+i*(basicCheckBox.width+10), basicCheckBox.y+j*(basicCheckBox.height+10), basicCheckBox.width, basicCheckBox.height,
            hwnd, HMENU(GENRE_CHECKBOX+i+j*(numOfColumns)), hInstance, NULL);
            SendMessage(checkBox, WM_SETFONT, (WPARAM) chechBox_font, TRUE);
            SetWindowLong(checkBox, GWL_EXSTYLE, GetWindowLong(checkBox, GWL_EXSTYLE) | WS_EX_LAYERED);
            SetLayeredWindowAttributes(checkBox, RGB(255, 255, 255), 0, LWA_COLORKEY);
            i++;
        }
    }
    HWND GenButton = CreateWindowExW(0, L"BUTTON", L"Confirm", WS_CHILD | BS_PUSHBUTTON, width/2-button.width/2-10, height-50-button.height, button.width, button.height,
    hwnd, HMENU(CONFIRM_GENRES_BUTTON), hInstance, NULL);
    SendMessage(GenButton, WM_SETFONT, (WPARAM)button_font, TRUE);

    imgPos->x = 350;
    imgPos->y = 50;
    bg_imgPos->x = 0;
    bg_imgPos->y = height-230;

    HWND hContents[numOfContents*2];

        pos_size static_pos_size = {30, 30, 80, 20}, edit_pos_size = {115, 30, 235, 50};
        for(int i = 0; i<numOfContents; i++)
        {
            hContents[i] = CreateWindowEx(WS_EX_RIGHT, L"Static", NULL, WS_CHILD,
            static_pos_size.x, static_pos_size.y+i*50, static_pos_size.width, static_pos_size.height,
            hwnd, HMENU(CONTENTS_MAIN+i), hInstance, NULL);
            hContents[i+numOfContents] = CreateWindowEx(WS_EX_LEFT, L"Edit", NULL, WS_CHILD | ES_READONLY | ES_MULTILINE,
            edit_pos_size.x, edit_pos_size.y+i*50, edit_pos_size.width, edit_pos_size.height,
            hwnd, HMENU(CONTENTS_OUT+i), hInstance, NULL);
        }

    // hContents[0] = CreateWindowEx(WS_EX_RIGHT, L"Static", NULL, WS_CHILD, 50, 50, 80, 20, hwnd, HMENU(USERNAME_MAIN), hInstance, NULL);
    // hContents[1] = CreateWindowEx(WS_EX_RIGHT, L"Static", NULL, WS_CHILD, 50, 100, 80, 20, hwnd, HMENU(TITLE_MAIN), hInstance, NULL);
    // hContents[2] = CreateWindowEx(WS_EX_LEFT, L"Edit", NULL, WS_CHILD | ES_READONLY, 150, 50, 200, 20, hwnd, HMENU(USERNAME_OUT), hInstance, NULL );
    // hContents[3] = CreateWindowEx(WS_EX_LEFT, L"Edit", NULL, WS_CHILD | ES_READONLY, 150, 100, 200, 20, hwnd, HMENU(TITLE_OUT), hInstance, NULL );

    HWND hiperlink = CreateWindowEx(WS_EX_LEFT, L"Static", L"Redict to MAL", WS_CHILD | SS_NOTIFY,
            edit_pos_size.x, edit_pos_size.y+numOfContents*50, static_pos_size.width+20, static_pos_size.height,
            hwnd, HMENU(CONTENTS_OUT+numOfContents), hInstance, NULL);

    HWND hNextButton = CreateWindowExW(0, L"BUTTON", L"Next", WS_CHILD | BS_PUSHBUTTON,
    width/2-button.width/2-10, height-50-button.height, button.width, button.height,
    hwnd, HMENU(NEXT_BUTTON), hInstance, NULL);
    SendMessage(hNextButton, WM_SETFONT, (WPARAM)button_font, TRUE);
    ShowWindow(hwnd, nCmdShow);
    ShowWindow(hButton, SW_SHOW);
    UpdateWindow( hwnd );

    graphics_background = new Gdiplus::Graphics(hwnd);
    // bg_img = new Gdiplus::Image(L"res/ico/RandomizeAnime.ico");
    // graphics_background->DrawImage(bg_img, 0, 0);
    bg_img = new Gdiplus::Image(L"res/bg/anya.png");
    graphics_background->DrawImage(bg_img, bg_imgPos->x, bg_imgPos->y);

    MSG message;
    while(GetMessage(& message, NULL, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    Gdiplus::GdiplusShutdown(gdiplusToken);
    std::cout<<"loooo"<<std::endl;
    return message.wParam;
}

LRESULT CALLBACK WindowProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    int dupa;
    switch( msg )
    {
    case WM_CLOSE:    
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_QUIT:
        DestroyWindow(hwnd);
        break;
    case  WM_COMMAND:
        switch(LOWORD(wParam))
        {
            case CONFIRM_USERNAME_BUTTON:
            {
                LPSTR input_username = new char[256];
                GetDlgItemTextA(hwnd, TEXT_INPUT, input_username, 256);
                g_username = input_username;
                if(g_username == "")
                {
                    MessageBoxW(hwnd, L"You haven't provided username", L"Nie psuj", MB_ICONERROR);
                    // PostQuitMessage(3);
                    break;
                }
                // MessageBox( hwnd, input_username, L"Tekst z Pola", MB_ICONINFORMATION );
                // DestroyWindow(GetDlgItem(hwnd, TEXT_INPUT));
                // DestroyWindow(GetDlgItem(hwnd, CONFIRM_USERNAME_BUTTON));
                // DestroyWindow(GetDlgItem(hwnd, TEXT_ENTER_USR));
                // g_username = input_username;
                // if(g_username == "")
                // {
                //     MessageBoxW(hwnd, L"Nie podałeś użytkownika", L"Nie psuj", MB_ICONERROR);
                //     PostQuitMessage(3);
                //     break;
                // }
                // std::cout<< temp << std::endl;
                animeList = returnArrayOfAnime(client_id, g_username);
                // MessageBoxA(hwnd, std::to_string(animeData.list.size()).c_str(), "size w vector", MB_ICONERROR);
                if(animeList.size() == 0)
                {
                    MessageBoxA(hwnd, "No plan to watch's entries found on user's list", "No entries", MB_ICONERROR);
                    // PostQuitMessage(1);
                    break;
                }
                if(animeList[0].title == "Error")
                {
                    MessageBoxA(hwnd, animeList[0].picture_url.c_str() ,"Error", MB_ICONERROR);
                    // PostQuitMessage(1);
                    break;
                }
                DestroyWindow(GetDlgItem(hwnd, TEXT_INPUT));
                DestroyWindow(GetDlgItem(hwnd, CONFIRM_USERNAME_BUTTON));
                DestroyWindow(GetDlgItem(hwnd, TEXT_ENTER_USR));
                genres = allGenresInObjects(animeList);
                std::vector<dataStatus> genName = returnArrayOfElements(genres);
                std::vector<std::string> statusName = returnArrayOfKeys(status);

                Gdiplus::SolidBrush brush(Gdiplus::Color::White);
                graphics_background->FillRectangle(&brush, 0, height-230, bg_img->GetWidth(), bg_img->GetHeight());
                delete bg_img;
                delete graphics_background;
                graphics_background = nullptr;
                bg_img = nullptr;

                ShowWindow(GetDlgItem(hwnd, GEN_MAIN_TEXT), SW_SHOW);
                SetWindowTextA(GetDlgItem(hwnd, GEN_MAIN_TEXT), "Select Genres:");
                ShowWindow(GetDlgItem(hwnd, STATUS_MAIN_TEXT), SW_SHOW);
                SetWindowTextA(GetDlgItem(hwnd, STATUS_MAIN_TEXT), "Select status:");
                for(int i = 0; i < numOfTypeStatus; i++)
                {
                    std::string copy = statusName[i];
                    findAndChangeCharInString(copy, '_',' ');
                    // std::cout<<copy<<std::endl;
                    SetWindowTextA(GetDlgItem(hwnd, STATUS_CHECKBOX+i), copy.c_str());
                    SendMessage(GetDlgItem(hwnd, STATUS_CHECKBOX+i), BM_SETCHECK, BST_CHECKED, 0);
                    ShowWindow(GetDlgItem(hwnd, STATUS_CHECKBOX+i), SW_SHOW);
                }

                int sizeGen = genres.size();

                for(int i = 0; i < sizeGen; i++)
                {
                    SetWindowTextA(GetDlgItem(hwnd, GENRE_CHECKBOX+i), genName[i].data.c_str());
                    SendMessage(GetDlgItem(hwnd, GENRE_CHECKBOX+i), BM_SETCHECK, BST_INDETERMINATE, 0);
                    ShowWindow(GetDlgItem(hwnd, GENRE_CHECKBOX+i), SW_SHOW);
                }
                for(int i = sizeGen; i<numOfGenres; i++)
                {
                    DestroyWindow(GetDlgItem(hwnd, GENRE_CHECKBOX+i));
                }
                // SetWindowTextA(GetDlgItem(hwnd, GENRE_CHECKBOX), "Action");

                ShowWindow(GetDlgItem(hwnd, CONFIRM_GENRES_BUTTON), SW_SHOW);
                SetFocus(hwnd);
                break;
            }
            case CONFIRM_GENRES_BUTTON:
            {
                std::vector<std::string> statusKeys = returnArrayOfKeys(status);
                for(int i = 0; i < numOfTypeStatus; i++)
                {
                    int boxStatus = SendMessage(GetDlgItem(hwnd, STATUS_CHECKBOX+i), BM_GETCHECK, 0, 0);
                    // DestroyWindow(GetDlgItem(hwnd, STATUS_CHECKBOX+i));
                    status.at(statusKeys.at(i)) = boxStatus;
                }
                // for(auto p : status)
                //      std::cout<<p.first<<" "<<p.second<<std::endl;

                int sizeGen = genres.size();
                auto genKeys = returnArrayOfKeys(genres);
                for(int i = 0; i < sizeGen; i++)
                {
                    int boxStatus = SendMessage(GetDlgItem(hwnd, GENRE_CHECKBOX+i), BM_GETCHECK, 0, 0);
                    // DestroyWindow(GetDlgItem(hwnd, GENRE_CHECKBOX+i));
                    genres.at(genKeys.at(i)).status = boxStatus;
                }
                // for(auto p : genres)
                //      std::cout<<p.first<<" "<<p.second.data<<" "<<p.second.status<<std::endl;
                auto animeListCopy = animeList;
                for(auto i = animeListCopy.begin(); i != animeListCopy.end();)
                {
                    // std::cout<<i->status<<std::endl;
                    // std::cout<<status.at(i->status)<<std::endl;
                    if(status.at(i->status) == 0)
                    {
                        animeListCopy.erase(i);
                        // std::cout<<"Erased! "<<std::endl;
                    }
                    else
                    {
                        bool deleted = 0;
                        for(auto p : genres)
                        {
                            if(p.second.status == 2)
                                continue;
                            if(i->genreExist(p.first) != p.second.status)
                            {
                                // std::cout<<p.second.data<<" "<<i->title<<std::endl;
                                animeListCopy.erase(i);
                                deleted = 1;
                                break;
                            }
                        }
                        if(deleted == 0)
                            i++;
                    }
                }
                animeListCopy.shrink_to_fit();
                if(animeListCopy.size() == 0)
                {
                    MessageBoxW(hwnd, L"There are no entries with these parameters", L"No entries",MB_ICONERROR);
                    // PostQuitMessage(3);
                    break;
                }
                DestroyWindow(GetDlgItem(hwnd, GEN_MAIN_TEXT));
                DestroyWindow(GetDlgItem(hwnd, STATUS_MAIN_TEXT));
                DestroyWindow(GetDlgItem(hwnd, CONFIRM_GENRES_BUTTON));
                for(int i = 0; i < numOfTypeStatus; i++)
                    DestroyWindow(GetDlgItem(hwnd, STATUS_CHECKBOX+i));
                for(int i = 0; i < sizeGen; i++)
                    DestroyWindow(GetDlgItem(hwnd, GENRE_CHECKBOX+i));
                animeList = animeListCopy;
                animeListCopy.clear();
                int Ansize = animeList.size();
                std::string textInfo = "Found ";
                textInfo += std::to_string(Ansize);
                textInfo += " Titles";
                // std::cout<<"Dupaaaaaaaaaaaaa";
                int downloadImg = MessageBoxA(hwnd, textInfo.c_str(),"Do you want to download images?", MB_YESNO);
                if(downloadImg == IDYES)
                {
                    for(auto& an : animeList)
                    {
                        std::fstream file;
                        std::string temp = "res/img/";
                        temp += std::to_string(an.returnId());
                        temp += ".png";
                        an.img_path = temp;
                        file.open(an.img_path, std::ios::in);
                        if(file.good())
                        {
                            std::cout<<"Znaleziono obrazek w plikach lokalnych"<<std::endl;
                            file.close();
                            continue;
                        }
                        std::cout<<"Pobieranie obrazka"<<std::endl;
                        bool success = curl_save_img(an.picture_url, an.img_path);
                        if(!success)
                            {
                            std::cout<<"Blad pobrania obrazka"<<std::endl;
                            an.img_path = "res/img/no_photo.png";
                            }
                        file.close();
                    }
                }
                else if(downloadImg == IDNO)
                {
                    for(auto& an : animeList)
                    {
                        std::fstream file;
                        std::string temp = "res/img/";
                        temp += std::to_string(an.returnId());
                        temp += ".png";
                        an.img_path = temp;
                        file.open(an.img_path, std::ios::in);
                        if(file.good())
                        {
                            std::cout<<"Znaleziono obrazek w plikach lokalnych"<<std::endl;
                            file.close();
                            continue;
                        }
                        an.img_path = "res/img/no_photo.png";
                        file.close();
                    }
                }
                else
                {
                    MessageBoxA(hwnd, "Failed to load response","Error", MB_ICONERROR);
                    PostQuitMessage(4);
                    break;
                }
                int it = 0;
                ShuffleVect(animeList);
                Anime* an_pointer = &animeList[animeList.size()-1];
                HFONT fontPrint_default = CreateFont(16, 0, 0, 0, FW_BOLD, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                TEXT("Times New Roman"));
                HFONT fontPrint_out = CreateFont(16, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                TEXT("Arial"));
                HFONT font_hiperlink = CreateFont(16, 0, 0, 0, FW_NORMAL, false, true, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                TEXT("Times New Roman"));
                for(int i = 0; i<numOfContents; i++)
                {
                    SendMessage(GetDlgItem(hwnd, CONTENTS_OUT+i), WM_SETFONT, (WPARAM)fontPrint_out, TRUE);
                    SendMessage(GetDlgItem(hwnd, CONTENTS_MAIN+i), WM_SETFONT, (WPARAM)fontPrint_default, TRUE);
                    ShowWindow(GetDlgItem(hwnd, CONTENTS_MAIN+i), SW_SHOW);
                    ShowWindow(GetDlgItem(hwnd, CONTENTS_OUT+i), SW_SHOW);
                }
                SendMessage(GetDlgItem(hwnd, CONTENTS_OUT+numOfContents), WM_SETFONT, (WPARAM)font_hiperlink, TRUE);
                ShowWindow(GetDlgItem(hwnd, CONTENTS_OUT+numOfContents), SW_SHOW);
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_MAIN+it), "Username:");
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), g_username.c_str());
                it++;
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_MAIN+it), "Title:");
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), an_pointer->title.c_str());
                it++;
                std::string status = an_pointer->status;
                findAndChangeCharInString(status, '_', ' ');
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_MAIN+it), "Status:");
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), status.c_str());
                it++;
                float rate = an_pointer->score;
                std::string rate_str;
                if(rate == 0.0f)
                    rate_str = "N/A";
                else
                    rate_str = std::to_string(rate);
                rate_str = rate_str.substr(0, 4);
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_MAIN+it), "Score:");
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), rate_str.c_str());
                it++;
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_MAIN+it), "Members:");
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), std::to_string(an_pointer->popularity).c_str());
                it++;
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_MAIN+it), "Rating:");
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), an_pointer->rating.c_str());
                it++;
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_MAIN+it), "Genres:");
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), an_pointer->stringOfGenres().c_str());
                it++;
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_MAIN+it), "Type:");
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), an_pointer->mediaType.c_str());
                it++;
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_MAIN+it), "Episodes:");
                uint32_t ep = an_pointer->numOfEp;
                std::string ep_str;
                if(ep == 0)
                    ep_str = "Unknown";
                else
                    ep_str = std::to_string(ep);
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), ep_str.c_str());
                it++;
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_MAIN+it), "Season:");
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), an_pointer->stringOfSeasonData().c_str());
                ShowWindow(GetDlgItem(hwnd, NEXT_BUTTON), SW_SHOW);
                HDC hdcGraph;
                graphics = new Gdiplus::Graphics(hwnd);
                std::wstring path = string_to_wstring(an_pointer->img_path);
                std::wcout<<path<<std::endl;
                image = new Gdiplus::Image(path.c_str());
                graphics->DrawImage(image, imgPos->x, imgPos->y);
                graphics_background = new Gdiplus::Graphics(hwnd);
                bg_img = new Gdiplus::Image(L"res/bg/kon.png");
                bg_imgPos->x = width-200;
                bg_imgPos->y = height-260;
                graphics->DrawImage(bg_img, bg_imgPos->x, bg_imgPos->y);

                SetFocus(hwnd);
                an_pointer = nullptr;
                break;
            }
            case NEXT_BUTTON:
            {
                // MessageBoxA( hwnd, "aaa", "Przycizg", MB_ICONINFORMATION );

                animeList.pop_back();
                if(animeList.size() == 0)
                {
                    MessageBoxW(hwnd, L"No more entries", L"End of list", MB_ICONERROR);
                    PostQuitMessage(3);
                    break;
                }
                Anime* an_pointer = &animeList[animeList.size()-1];
                Gdiplus::SolidBrush brush(Gdiplus::Color::White);
                graphics->FillRectangle(&brush, imgPos->x, imgPos->y, image->GetWidth(), image->GetHeight());
                delete image;
                delete graphics;
                image = nullptr;
                graphics = nullptr;
                HDC hdcGraph;
                graphics = new Gdiplus::Graphics(hwnd);
                std::wstring path = string_to_wstring(animeList[animeList.size()-1].img_path);
                image = new Gdiplus::Image(path.c_str());
                graphics->DrawImage(image, imgPos->x, imgPos->y);
                // SendMessage(GetDlgItem(hwnd, CONTENTS_OUT+1), WM_SETTEXT, 0, reinterpret_cast<LPARAM>(L""));
                // InvalidateRect(GetDlgItem(hwnd, CONTENTS_OUT+1), NULL, TRUE);
                // UpdateWindow(GetDlgItem(hwnd, CONTENTS_OUT+1));
                int it = 1;
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), an_pointer->title.c_str());
                it++;
                std::string status = an_pointer->status;
                findAndChangeCharInString(status, '_', ' ');
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), status.c_str());
                it++;
                float rate = an_pointer->score;
                std::string rate_str;
                if(rate == 0.0f)
                    rate_str = "N/A";
                else
                    rate_str = std::to_string(rate);
                rate_str = rate_str.substr(0, 4);
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), rate_str.c_str());
                it++;
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), std::to_string(an_pointer->popularity).c_str());
                it++;
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), an_pointer->rating.c_str());
                it++;
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), an_pointer->stringOfGenres().c_str());
                it++;
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), an_pointer->mediaType.c_str());
                it++;
                uint32_t ep = an_pointer->numOfEp;
                std::string ep_str;
                if(ep == 0)
                    ep_str = "Unknown";
                else
                    ep_str = std::to_string(ep);
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), ep_str.c_str());
                it++;
                SetWindowTextA(GetDlgItem(hwnd, CONTENTS_OUT+it), an_pointer->stringOfSeasonData().c_str());
                SetFocus(hwnd);
                break;
            }
            case CONTENTS_OUT+numOfContents:
                    Anime* an_pointer = &animeList[animeList.size()-1];
                    std::string temp = "https://myanimelist.net/anime/";
                    temp += std::to_string(an_pointer->returnId());
                    std::cout<<temp<<std::endl;
                    ShellExecuteA(NULL, "open", temp.c_str(), NULL, NULL, SW_SHOWNORMAL);
                break;
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            // std::cout<<"dupci"<<std::endl;
            // All painting occurs here, between BeginPaint and EndPaint.
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            if(graphics_background!=nullptr)
                graphics_background->DrawImage(bg_img, bg_imgPos->x, bg_imgPos->y);
            if(graphics!=nullptr)
                graphics->DrawImage(image, imgPos->x, imgPos->y);
            EndPaint(hwnd, &ps);
            break;
        }
    case WM_KEYDOWN:
        {
            if(wParam == VK_RETURN)
            {
                std::cout<<"Enter Clicked"<<std::endl;
                if(IsWindowVisible(GetDlgItem(hwnd, CONFIRM_USERNAME_BUTTON)))
                {
                    std::cout<<"First clicked!"<<std::endl;
                    SendMessage(GetDlgItem(hwnd, CONFIRM_USERNAME_BUTTON), BM_CLICK, 0, 0);
                }
                else if(IsWindowVisible(GetDlgItem(hwnd, CONFIRM_GENRES_BUTTON)))
                {
                    std::cout<<"Second clicked!"<<std::endl;
                    SendMessage(GetDlgItem(hwnd, CONFIRM_GENRES_BUTTON), BM_CLICK, 0, 0);
                }
                else if(IsWindowVisible(GetDlgItem(hwnd, NEXT_BUTTON)))
                {
                    std::cout<<"Next clicked!"<<std::endl;
                    SendMessage(GetDlgItem(hwnd, NEXT_BUTTON), BM_CLICK, 0, 0);
                }
            }
        }
    case WM_CTLCOLORSTATIC:
        {
            // PAINTSTRUCT ps;
            // HDC hdc = BeginPaint(hwnd, &ps);
            // // std::cout<<"dupci"<<std::endl;
            // // All painting occurs here, between BeginPaint and EndPaint.
            // FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            // EndPaint(hwnd, &ps);
            // break;
            if((HWND)lParam == GetDlgItem(hwnd, CONTENTS_OUT+numOfContents))
            {
                HDC hdcStatic = (HDC)wParam;
                SetTextColor(hdcStatic, RGB(0, 0, 255));
                SetBkMode(hdcStatic, TRANSPARENT);
                return (INT_PTR)GetStockObject(NULL_BRUSH);
            }
            HDC hdcStatic = (HDC)wParam;
            SetBkColor(hdcStatic, RGB(255, 255, 255)); // Ustawienie koloru tła na biały
            return (LRESULT)GetStockObject(WHITE_BRUSH);
        }
        case WM_SETFOCUS:
            //std::cout<<"Aplikacja uzyskala focus"<<std::endl;
        case WM_KILLFOCUS:
            //std::cout<<"Aplikacja stracila focus"<<std::endl;

        default:
        return DefWindowProc( hwnd, msg, wParam, lParam );
    }
    return 0;
}