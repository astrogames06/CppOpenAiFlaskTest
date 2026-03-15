#include <raylib.h>
#include <raymath.h>
#include <raylibextra.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <string>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#define PLATFORM_WEB
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
	#include <emscripten/val.h>
	#include <emscripten/bind.h>
#endif

const int WIDTH = 850;
const int HEIGHT = 450;

std::string gpt_str; 

void set_gpt_response(const std::string &res) {
    gpt_str = res;
    std::cout << "GPT updated: " << gpt_str << std::endl;
}

// Bind it for JS access
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("set_gpt_response", &set_gpt_response);
}

void UpdateDrawFrame();

int main(void)
{
	InitWindow(WIDTH, HEIGHT, "raylib [core] example - basic window");

	#if defined(PLATFORM_WEB)
    	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
	#else
		SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
		//--------------------------------------------------------------------------------------

		// Main game loop
		while (!WindowShouldClose())    // Detect window close button or ESC key
		{
			UpdateDrawFrame();
		}
	#endif

	CloseWindow();

	return 0;
}

bool text_box = false;
char text_box_str[1024] = {0};
void UpdateDrawFrame()
{
	BeginDrawing();
	
	ClearBackground(RAYWHITE);

	DrawText("Ask AI", 100, 100, 20, BLACK);

	std::string gpt_d = "GPT: " + gpt_str;
    // DrawText(gpt_d.c_str(), 100, 130, 20, BLACK);
	int text_height = DrawTextWrapped(gpt_d.c_str(), 100, 130, 20, WIDTH/2, BLACK);

	if (GuiTextBox({100, (float)130+text_height+15, 100, 45}, text_box_str, 20, text_box)) text_box = !text_box;
    if (GuiButton({100, (float)130+text_height+60, 100, 45}, "SEND!"))
	{
		EM_ASM({
			ask_gpt_js(UTF8ToString($0)); // UFT8ToString converts C++ string to Javascript String
		}, text_box_str);
	}

	EndDrawing();
}