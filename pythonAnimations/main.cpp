extern "C" {
    #include "port/micropython_embed.h"
    #include <stdio.h>

    void set_pixel_id_r_g_b(int id , int r , int g, int b){
        printf("Main.cpp id %d color %d %d %d \n", id, r,g,b);
    }

    int get_number_of_pixels() {
        return 10;
    }
}
// This is example 1 script, which will be compiled and executed.
static const char *example_1 =
    "print('hello world!', list(x + 1 for x in range(10)), end='eol\\n')";

// This is example 2 script, which will be compiled and executed.
static const char *example_2 =
    "for i in range(10):\n"
    "    print('iter {:08}'.format(i))\n"
    "\n"
    "\n"
    "print('finish')\n"
    "import leds\n"
    "leds.set_pixel_id_r_g_b(0,2,3,4)\n"
    "print(leds.get_number_of_pixels())"
    ;

// This array is the MicroPython GC heap.
static char heap[8 * 1024];

int main() {
    // Initialise MicroPython.
    mp_embed_init(&heap[0], sizeof(heap));

    // Run the example scripts (they will be compiled first).
    mp_embed_exec_str(example_1);
    mp_embed_exec_str(example_2);

    // Deinitialise MicroPython.
    mp_embed_deinit();

    return 0;
}