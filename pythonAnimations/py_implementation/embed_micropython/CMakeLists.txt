
file(GLOB_RECURSE source_list "micropython_embed/*/*.c")
file(GLOB_RECURSE source_list_2 "micropython_embed/*/*/*.c")

message("embed_micropython")

include_directories(".")
include_directories("micropython_embed")
include_directories("micropython_embed/port")

add_library(MicroPythonAsLibrary ${source_list} ${source_list_2} "modules/leds/leds.c")