#define UNICODE
#define _UNICODE

#include <windows.h>
#include <iostream>
#include <string>
#include <curl_http.h>
#include <gdiplus/gdiplus.h>
#include "func.hpp"


#define TEXT_INPUT 1001
#define TEXT_ENTER_USR 1002
#define CONFIRM_USERNAME_BUTTON 1003
#define USERNAME_MAIN 1004
#define USERNAME_OUT 1005
#define TITLE_MAIN 1006
#define TITLE_OUT 1007
#define NEXT_BUTTON 1008

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


static std::vector<Anime> animeList;

LRESULT CALLBACK WindowProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

HINSTANCE g_hInstance;

const int width = 600, height = 700;
std::string g_username;
std::string client_id;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR LpCmdLine, int nCmdShow)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
    WNDCLASS wc = {};
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = L"MainClass";
    if( !RegisterClass(&wc))
    {
        MessageBoxW(NULL, L"Coś się odjebało", NULL, MB_ICONEXCLAMATION | MB_OK );
        return 1;
    }
    HWND hwnd = CreateWindowEx(
    0,                              // Optional window styles.
    wc.lpszClassName,               // Window class
    L"App",                         // Window text
    WS_OVERLAPPEDWINDOW,            // Window stylem
                                // Size and position
    CW_USEDEFAULT, CW_USEDEFAULT, width, height,
    NULL,                           // Parent window
    NULL,                           // Menu
    hInstance,                      // Instance handle
    NULL                            // Additional application data
    );
    if (hwnd == NULL)
    {
        return 0;
    }
    // creating a button
    const int button_width = 150, button_height = 30;
    const int button_x = (width-button_width)/2, button_y = height/2-50;
    HWND hButton = CreateWindowExW( 0, L"BUTTON", L"Confirm", WS_CHILD | BS_PUSHBUTTON, button_x, button_y, button_width, button_height, hwnd, HMENU(CONFIRM_USERNAME_BUTTON), hInstance, NULL );
    // MessageBox(hButton, L"Hello!", L"Message", MB_OK);
    //creating a text box
    const int input_width = 150, input_height = 30;
    const int input_x = (width-input_width)/2, input_y = height/2-100;
    HWND hText = CreateWindowEx(0 , L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, input_x, input_y, input_width, input_height, hwnd, HMENU(TEXT_INPUT), hInstance, NULL );

    HWND hStaticText = CreateWindowEx(WS_EX_LEFT, L"Static", NULL, WS_CHILD | WS_VISIBLE, input_x, input_y-input_height, 150, 20, hwnd, HMENU(TEXT_ENTER_USR), hInstance, NULL);
    SetWindowText( hStaticText, L"Enter Username:" );

    HWND hContents[4];
    hContents[0] = CreateWindowEx(WS_EX_RIGHT, L"Static", NULL, WS_CHILD, 50, 50, 80, 20, hwnd, HMENU(USERNAME_MAIN), hInstance, NULL);
    hContents[1] = CreateWindowEx(WS_EX_RIGHT, L"Static", NULL, WS_CHILD, 50, 100, 80, 20, hwnd, HMENU(TITLE_MAIN), hInstance, NULL);
    hContents[2] = CreateWindowEx(WS_EX_LEFT, L"Edit", NULL, WS_CHILD | ES_READONLY, 150, 50, 200, 20, hwnd, HMENU(USERNAME_OUT), hInstance, NULL );
    hContents[3] = CreateWindowEx(WS_EX_LEFT, L"Edit", NULL, WS_CHILD | ES_READONLY, 150, 100, 200, 20, hwnd, HMENU(TITLE_OUT), hInstance, NULL );

    HWND hNextButton = CreateWindowExW(0, L"BUTTON", L"Next", WS_CHILD | BS_PUSHBUTTON, button_x, button_y, button_width, button_height, hwnd, HMENU(NEXT_BUTTON), hInstance, NULL);
    
    ShowWindow(hwnd, nCmdShow);
    ShowWindow(hButton, SW_SHOW);
    UpdateWindow( hwnd );
    client_id = "353482731bcf9d0986b8e17046c49d33";

    MSG message;
    while( GetMessage( & message, NULL, 0, 0 ) )
    {
        TranslateMessage( & message );
        DispatchMessage( & message );
    }
    Gdiplus::GdiplusShutdown(gdiplusToken);
    return message.wParam;
}

LRESULT CALLBACK WindowProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
    case WM_CLOSE:
        DestroyWindow( hwnd );
        break;

    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;
    case  WM_COMMAND:
        switch(LOWORD(wParam))
        {
            case CONFIRM_USERNAME_BUTTON:
            {
                LPSTR tekstLosowy = new char[256];
                GetDlgItemTextA(hwnd, TEXT_INPUT, tekstLosowy, 256);
                // MessageBox( hwnd, tekstLosowy, L"Tekst z Pola", MB_ICONINFORMATION );
                DestroyWindow(GetDlgItem(hwnd, TEXT_INPUT));
                DestroyWindow(GetDlgItem(hwnd, CONFIRM_USERNAME_BUTTON));
                DestroyWindow(GetDlgItem(hwnd, TEXT_ENTER_USR));
                g_username = tekstLosowy;
                if(g_username == "")
                {
                    MessageBoxW(hwnd, L"Nie podałeś użytkownika", L"Nie psuj", MB_ICONERROR);
                    PostQuitMessage(3);
                    break;
                }
                // std::cout<< temp << std::endl;
                animeList = returnTableOfAnime(client_id, g_username);
                // MessageBoxA(hwnd, std::to_string(animeData.list.size()).c_str(), "size w vector", MB_ICONERROR);
                if(animeList.size() == 0)
                {
                    MessageBoxA(hwnd, std::to_string(animeList.size()).c_str(), "Brak wpisow", MB_ICONERROR);
                    PostQuitMessage(1);
                    break;
                }
                if(animeList[0].title == "Error")
                {
                    MessageBoxA(hwnd, "Error", animeList[0].picture_url.c_str(), MB_ICONERROR);
                    PostQuitMessage(1);
                    break;
                }
                ShuffleVect(animeList);
                ShowWindow(GetDlgItem(hwnd, USERNAME_MAIN), SW_SHOW);
                SetWindowTextA(GetDlgItem(hwnd, USERNAME_MAIN), "User Name:");

                ShowWindow(GetDlgItem(hwnd, USERNAME_OUT), SW_SHOW);
                SetWindowTextA(GetDlgItem(hwnd, USERNAME_OUT), g_username.c_str());

                ShowWindow(GetDlgItem(hwnd, TITLE_MAIN), SW_SHOW);
                SetWindowTextA(GetDlgItem(hwnd, TITLE_MAIN), "Title:");

                ShowWindow(GetDlgItem(hwnd, TITLE_OUT), SW_SHOW);
                SetWindowTextA(GetDlgItem(hwnd, TITLE_OUT), animeList[animeList.size()-1].title.c_str());

                HDC hdcGraph;
                Gdiplus::Graphics graphics(hwnd);
                Gdiplus::Image image(L"res/img/image.png");
                graphics.DrawImage(&image, 60, 10);

                ShowWindow(GetDlgItem(hwnd, NEXT_BUTTON), SW_SHOW);
                break;
            }
            case NEXT_BUTTON:
                // MessageBoxA( hwnd, "aaa", "Przycizg", MB_ICONINFORMATION );

                animeList.pop_back();
                if(animeList.size() == 0)
                {
                    MessageBoxW(hwnd, L"End of list", L"Koniec Tytułów", MB_ICONERROR);
                    PostQuitMessage(3);
                }
                SetWindowTextA(GetDlgItem(hwnd, TITLE_OUT), animeList[animeList.size()-1].title.c_str());
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
            EndPaint(hwnd, &ps);
            break;
        }
    case WM_CTLCOLORSTATIC:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            // std::cout<<"dupci"<<std::endl;
            // All painting occurs here, between BeginPaint and EndPaint.
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            EndPaint(hwnd, &ps);
            break;
        }
        default:
        return DefWindowProc( hwnd, msg, wParam, lParam );
    }
    return 0;
}