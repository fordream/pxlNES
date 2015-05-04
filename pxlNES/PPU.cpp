#include "PPU.h"
#include <iostream>
#include <sstream>
#include "CPU.h"

int8 VRAM[VRAM_EMULATED_SIZE];
int8 OAM[OAM_SIZE];
int8 PPU_registers[8];

std::thread PPU_thread;
bool PPU_thread_exit = false;

PXL_Window PPU_display;
PXL_Text debug_text;

void PPU_run() {
    PPU_display.create_window(800, 720, "NES emulator");
    PXL_init();

    float t = 0;
    float fps = 60;
    float ms_per_frame = 1000 / fps;
    PXL_Timer start_time;
    PXL_Timer start_second_time;
    int frame_counter = 0;

    PXL_Font square("square.ttf", 24);
    debug_text = PXL_Text(&square, "", 0, 0, 24);
    debug_text.colour.set_colour(0, 0, 0, 1);

    PXL_Texture chr;
    chr.create_texture("chr.png");

    PXL_Rect rect(0, 0, 800, 720);

    PXL_Batch batch(&PPU_display, PXL_BATCH_TINY);

    PXL_set_clear_colour(1, 0, 1, 1);
    PXL_Event e;
    start_second_time.start();
    while (!PPU_thread_exit) {
        while (PPU_display.poll_event(e)) {
            if (e.type == PXL_KEY_P)) {
                CPU_process();
            }
        }

        start_time.start();

        PXL_clear();

        debug_text.render(&batch);

        batch.render_all();

        PPU_display.display();

        double ms = start_time.end() / 1000.0f;
        if (ms >= 0 && ms < ms_per_frame) { PXL_sleep(ms_per_frame - ms); }

        ++frame_counter;
        if (start_second_time.end() / 1000.0f >= 1000) {
            PXL_print << "fps: " << frame_counter << "\n";

            frame_counter = 0;
            start_second_time.start();
        }
    }
}

void PPU_init() {
    for (int n = 0; n < 8; ++n) {
        PPU_registers[n] = 0;
    }

    PPU_thread = std::thread(PPU_run);
}

void PPU_dispose() {
    PPU_thread_exit = true;
    PPU_thread.detach();
    delete[] VRAM;
    delete[] OAM;
}

DebugPrinter::~DebugPrinter() {
    if (debug_text.text_loaded) {
        std::string str = debug_text.get_text();
        if (str.length() >= 100) {
            str = str.substr(str.find("\n") + 1, str.length());
        }
        str += msg.str();
        debug_text.set_text(str);
    }
}