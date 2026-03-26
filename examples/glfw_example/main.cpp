#include <fstream>
#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#include <system_theme_pp/system_theme.hpp>

struct AppData {
    GLFWwindow*                      window;
    system_theme_pp::SystemThemeInfo info;
};

static GLuint          fontTexture;
static stbtt_bakedchar charData[96];  // ASCII 32..127
static wchar_t         current_font_path[512] = {0};
static float           current_font_size      = 0.0f;

// Windows dark mode support. On other platforms this is a no-op.
#ifdef _WIN32
#include <dwmapi.h>

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

void setWindowDarkMode(HWND hwnd, bool darkMode) {
    BOOL useDarkMode = darkMode ? TRUE : FALSE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode));
}

#define SET_DARK_MODE(hwnd, darkMode) setWindowDarkMode(hwnd, darkMode)

#else
#define SET_DARK_MODE(hwnd, darkMode) ((void) 0)

#endif

#define FONT_SIZE 24.0f

void initFont(const wchar_t* fontPath, float fontSize) {
    if(std::wcscmp(current_font_path, fontPath) == 0 && current_font_size == fontSize) {
        return;  // font already loaded
    }

    // Convert wchar_t* to std::string for cross-platform ifstream compatibility
    char narrowPath[512];
    std::wcstombs(narrowPath, fontPath, sizeof(narrowPath) - 1);
    narrowPath[sizeof(narrowPath) - 1] = '\0';

    std::ifstream file(narrowPath, std::ios::binary);
    if(!file) {
        std::wcerr << L"Failed to open font file: " << fontPath << L"\n";
        return;
    }

    std::wcsncpy(current_font_path, fontPath, sizeof(current_font_path) / sizeof(current_font_path[0]) - 1);
    current_font_path[sizeof(current_font_path) / sizeof(current_font_path[0]) - 1] = L'\0';
    current_font_size = fontSize;

    std::vector<unsigned char> fontBuffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::vector<unsigned char> bitmap(512 * 512);
    stbtt_BakeFontBitmap(fontBuffer.data(), 0, fontSize, bitmap.data(), 512, 512, 32, 96, charData);

    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void drawText(const char* text, float x, float y, system_theme_pp::ThemeColors color) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);

    glBegin(GL_QUADS);
    while(*text) {
        if(*text >= 32 && *text < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(charData, 512, 512, *text - 32, &x, &y, &q, 1);
            glTexCoord2f(q.s0, q.t0);
            glVertex2f(q.x0, q.y0);
            glTexCoord2f(q.s1, q.t0);
            glVertex2f(q.x1, q.y0);
            glTexCoord2f(q.s1, q.t1);
            glVertex2f(q.x1, q.y1);
            glTexCoord2f(q.s0, q.t1);
            glVertex2f(q.x0, q.y1);
        }
        text++;
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

float getTextWidth(const char* text) {
    float width = 0;
    while(*text) {
        if(*text >= 32 && *text < 128) {
            width += charData[*text - 32].xadvance;
        }
        text++;
    }
    return width;
}

void drawRect(float x, float y, float w, float h, system_theme_pp::ThemeColors color) {
    glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void drawWindow(GLFWwindow* window, const system_theme_pp::SystemThemeInfo& info) {
    SET_DARK_MODE(glfwGetWin32Window(window), info.isDarkMode);
    initFont(info.systemDefaultFont, FONT_SIZE * info.systemDefaultFontScale);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);

    glClearColor(
        info.backgroundColor.r / 255.0f, info.backgroundColor.g / 255.0f, info.backgroundColor.b / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);  // flip Y so text is not upside down

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    const char text[] = "Hello, GLFW!";

    drawRect(0, 0, width, 20, info.accentColor);
    drawText(text, (width - getTextWidth(text)) / 2.0f, (height + FONT_SIZE) / 2.0f, info.foregroundColor);

    glfwSwapBuffers(window);
}

void themeChangeCallback(const system_theme_pp::SystemThemeInfo& info, void* data) {
    auto appData  = static_cast<AppData*>(data);
    appData->info = info;  // update info from background thread
    glfwPostEmptyEvent();  // wake up glfwWaitEvents if sleeping
}

int main() {
    if(!glfwInit()) return -1;

    AppData appData;
    appData.window = glfwCreateWindow(640, 480, "Hello, GLFW!", nullptr, nullptr);
    if(!appData.window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(appData.window);

    auto theme   = system_theme_pp::SystemTheme::getInstance();
    appData.info = theme.getCurrentThemeInfo();
    theme.setThemeChangeCallback(themeChangeCallback, &appData);

    drawWindow(appData.window, appData.info);
    std::wcout << L"Current theme: " << appData.info.themeName << L"\n";

    while(!glfwWindowShouldClose(appData.window)) {
        glfwWaitEvents();                          // sleep until an event occurs

        drawWindow(appData.window, appData.info);  // redraw on main thread
    }

    glfwDestroyWindow(appData.window);
    glfwTerminate();
    return 0;
}