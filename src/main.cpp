//  JASMINES NORMALMAP GENERATION HELPER ART PROGRAM <3

#include "globals.h"

#include <iostream>


#include "imgui.h"
#include "glad.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include "glfw3.h"

#include "renderer.h"
#include "sprite.h"
#include "texture.h"
#include "render_texture.h"

#include "normal_generation.h"
#include "ui_element_instances.h"
#include "ui_elements.h"

#include "windows_io.h"

int main()
{
    // Initiate GLFW
    glfwInit();

    // Decide GL+GLSL versions - for compatibility
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // demonstrate a new window
    GLFWwindow* window = glfwCreateWindow(GLOBALS::WIDTH, GLOBALS::HEIGHT, "TestWindow1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "ERROR" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Setup GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    ImGui_ImplOpenGL3_Init(glsl_version);

    static float clear_color[4] = {0.8, 0.7, 0.84, 1.0};

    // a bool value to control the show figure window
    bool showFigure = false;


    //Renderer screen_renderer("assets/shaders/test_shader.vs", "assets/shaders/test_shader.fs");


    
    Sprite missing_sprite("Missing Sprite");
    missing_sprite.load_from_file("assets/images/missing.bmp");

    SpriteViewport view1_height(0.05f, 0.25f, 0.4f, 0.65f);
    view1_height.set_sprite(missing_sprite);

    SpriteViewport view2_normal(0.55f, 0.25f, 0.4f, 0.65f);
    view2_normal.set_sprite(missing_sprite);

    


    Button_Reset_Viewport button_reset_viewport_1(&view1_height);
    button_reset_viewport_1.set_position(0.05, 0.15);
    button_reset_viewport_1.set_size(0.1, 0.1);

    Button_Reset_Viewport button_reset_viewport_2(&view2_normal);
    button_reset_viewport_2.set_position(0.55, 0.15);
    button_reset_viewport_2.set_size(0.1, 0.1);


    Sprite loaded_sprite = missing_sprite;


    Button_Load_FS_Sprite load_sprite_button(&loaded_sprite);
    load_sprite_button.set_position(0.1, 0.05);
    load_sprite_button.set_size(0.1, 0.1);

    Button_Save_Sprite save_sprite_button(&view2_normal);
    save_sprite_button.set_position(0.8, 0.05);
    save_sprite_button.set_size(0.1, 0.1);



    float sharpness = 1.0f;
    unsigned char delta_threshold = 255;

    Normalmap::EDGE_MODE edge_mode = Normalmap::EDGE_MODE::EXTEND;
    bool is_dx_colours = false;
    bool is_half_resolution = false;




    while (!glfwWindowShouldClose(window))
    {
        // check and call events and swap the buffers
        glfwPollEvents();

        


        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glfwGetFramebufferSize(window, &GLOBALS::WIDTH, &GLOBALS::HEIGHT); // updates globals

        // Main Window
        
        //ImGui::SetNextWindowSize(mainWindowSize);
        ImGui::Begin("Background Colour", NULL, ImGuiChildFlags_AlwaysAutoResize);
        ImGui::ColorEdit4("CE", clear_color);
        ImGui::End();

        //ImGui::ShowDemoWindow();



        if (imgui_windows::customisation(&sharpness, &delta_threshold, &edge_mode, &is_half_resolution, &is_dx_colours)) {

            view2_normal.set_sprite(Normalmap::generate(&loaded_sprite, sharpness, delta_threshold, is_half_resolution, edge_mode, is_dx_colours));
        }

        imgui_windows::help();

        //std::cout << GLOBALS::WIDTH << std::endl;



        //std::cout << GLOBALS::WIDTH;
        



        Rendering::clear(window, clear_color); 

        view1_height.render();

        view2_normal.render();

        button_reset_viewport_1.render();
        button_reset_viewport_2.render();

        if (load_sprite_button.render()) {
            view1_height.set_sprite(loaded_sprite);
            view2_normal.set_sprite(Normalmap::generate(&loaded_sprite, sharpness, delta_threshold, is_half_resolution, edge_mode, is_dx_colours));
        }

        save_sprite_button.render(view2_normal.sprite);


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

    }

    // Cleanup

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;

}




































/* ATTEMPT ONE 
#include <iostream>
#include <cstring>
#include <chrono> // measure time / frame
#include <thread> // time sleep

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_glfw.h"
#include "imgui_freetype.h"


#include "viewport.h"
#include <windows.h>

#include <glad.h>   // this does #include <GL/GL.h>
#include <glfw3.h>
#include <tchar.h>


//#include "ui_elements.h"

#include "txtrtool_maths.h"


//#define min(a,b)

// Data stored per platform window
struct WGL_WindowData { HDC hDC; };

// Static Data
static HGLRC            g_hRC;
static WGL_WindowData   g_MainWindow;
static int              g_Width;
static int              g_Height;

// Forward declarations of helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data);
void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data);
void ResetDeviceWGL();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static void ShowExampleAppSimpleOverlay(bool* p_open);
static void ShowStaticBox();




static int textscale = 1.0f;
static float fps_limit = 60.0f; // does not work above 60fps as windows only respects sleep commands of >15.6ms

WindowDrawer background;


// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_OWNDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui Win32+OpenGL3 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1920, 1080, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize OpenGL
    if (!CreateDeviceWGL(hwnd, &g_MainWindow))
    {
        CleanupDeviceWGL(hwnd, &g_MainWindow);
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    wglMakeCurrent(g_MainWindow.hDC, g_hRC);



    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls


    // Application Init: Load various small fonts --- for font test
    //ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontFromFileTTF("NotoSans-Regular.ttf", 13.0f);





    std::cout << (GLADloadproc)glfwGetProcAddress << std::endl;











    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL(hwnd);
    ImGui_ImplOpenGL3_Init();


    // initialise GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }





    //FONT SETTINGSS -- freetype enabled via imconfig.h

    ImFont* font;
    font = io.Fonts->AddFontFromFileTTF("assets/font_m5x7.ttf", 26.0f * textscale); // loads font - 26pt for 1080p screens. Scaling option?
    IM_ASSERT(font != nullptr);


    // enable correct rasterisation IF font scale is a factor of 2 & big enough to fit to pixels.
    int FreeTypeBuilderFlags = txmaths::is_pow_2(textscale) && textscale >= 0.5 ? 0x00000100 : 0x00000000;

    ImFontAtlas* atlas = ImGui::GetIO().Fonts;
    atlas->FontBuilderIO = ImGuiFreeType::GetBuilderForFreeType();
    atlas->FontBuilderFlags = FreeTypeBuilderFlags;
    atlas->Build();
    ImGui_ImplOpenGL3_DestroyDeviceObjects(); // reloads font renderer
    ImGui_ImplOpenGL3_CreateDeviceObjects();















    // Our state
    bool show_demo_window = false;
    bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); //bg colour



    // ######################################## #   Main loop   # ##########################################
    double frame_calc_time = 0;

    bool done = false;
    while (!done)
    {
        std::chrono::steady_clock::time_point frame_clock = std::chrono::high_resolution_clock::now(); // frame timer









        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;
        if (::IsIconic(hwnd)) // minimised
        {
            ::Sleep(10);
            continue;
        }



        //if (freetype_test.PreNewFrame())
        //{


            // REUPLOAD FONT TEXTURE TO GPU
            //ImGui_ImplXXX_DestroyDeviceObjects();
            //ImGui_ImplXXX_CreateDeviceObjects();
        //}


        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();






        //freetype_test.ShowFontsOptionsWindow();


        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;


            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application render %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate); // framerate measured in seconds^-1
            ImGui::Text("Application compute %.3f ms/frame (%.1f PROJ. FPS)", frame_calc_time * 1000.0f, 1.0f / frame_calc_time); // frame_calc_time measured in s
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");

            ImGui::SliderFloat("FPS", &fps_limit, 1.0f, 120.0f, "%.4f", ImGuiSliderFlags_::ImGuiSliderFlags_Logarithmic);

            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }



        //4. main menu bar

        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New");
            ImGui::MenuItem("Create");
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();




        // static box 1

        bool booler = true;

        //ShowExampleAppSimpleOverlay(&booler);
        //ShowStaticBox();


















        // Final Rendering
        ImGui::Render();

        
        background.set_bgcolour(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        //background.render();
        
        
        glViewport(0, 0, g_Width, g_Height);

        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Present
        ::SwapBuffers(g_MainWindow.hDC);















        // frame time wait calculations

        std::chrono::steady_clock::time_point frame_clock_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> frame_time = frame_clock_end - frame_clock;
        frame_calc_time = frame_time.count();

        // -0.002 for windows quirks
        // as windows uses time slices of 15.6m. dont try to sleep for more than that/less than that - best you're getting is 64fps as a result of that.
        double remaining_wait_time = max(0.0, (1.0 / fps_limit) - frame_time.count() - 0.0078); // stops it upsetting my gpu - figures out fps frames/second

        std::this_thread::sleep_for(std::chrono::microseconds((int)(1000000.0 * remaining_wait_time))); // microseconds as preserves accuracy in int

    }











    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceWGL(hwnd, &g_MainWindow);
    wglDeleteContext(g_hRC);
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}














// Helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    HDC hDc = ::GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    const int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0)
        return false;
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
        return false;
    ::ReleaseDC(hWnd, hDc);

    data->hDC = ::GetDC(hWnd);
    if (!g_hRC)
        g_hRC = wglCreateContext(data->hDC);
    return true;
}

void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    wglMakeCurrent(nullptr, nullptr);
    ::ReleaseDC(hWnd, data->hDC);
}
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            g_Width = LOWORD(lParam);
            g_Height = HIWORD(lParam);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}









//-----------------------------------------------------------------------------
// [SECTION] Example App: Simple overlay / ShowExampleAppSimpleOverlay()
//-----------------------------------------------------------------------------

// Demonstrate creating a simple static window with no decoration
// + a context-menu to choose which corner of the screen to use.


// fix up this one

static void ShowExampleAppSimpleOverlay(bool* p_open)
{
    static int location = 0;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (location >= 0)
    {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD); // nerd stuff to pixk positions
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    else if (location == -2)
    {
        // Center window
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Example: Simple overlay", p_open, window_flags))
    {
        ImGui::Text("Simple overlay\n" "(right-click to change position)");
        ImGui::Separator();
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>");
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
            if (ImGui::MenuItem("Center", NULL, location == -2)) location = -2;
            if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
            if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
            if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
            if (p_open && ImGui::MenuItem("Close")) *p_open = false;
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

static void ShowStaticBox() {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;


    ImGui::SetNextWindowSize(ImVec2(400, 200));

    ImGui::SetNextWindowPos(ImVec2(100, 100));

    bool trueref = true;
    if (ImGui::Begin("Box1", &trueref, window_flags))
    {
        ImGui::Text("WINDOW WINDOW BOX WINDOW");
    }
    ImGui::End();

}



*/