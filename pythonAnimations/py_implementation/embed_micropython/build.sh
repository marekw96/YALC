rm -rf micropython_embed
rm -rf build
rm -rf build-embed


make -f embed.mk USER_C_MODULES=modules CFLAGS_EXTRA=-DMODULE_LEDS_ENABLED=1
mkdir build
cd build
cmake ..
make
