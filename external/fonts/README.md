# Font files

Get fonts from
* https://www.fontsquirrel.com/fonts/droid-sans-mono
* https://fonts.google.com/noto/specimen/Noto+Sans+Mono
* https://github.com/FortAwesome/Font-Awesome/tree/6.x/webfonts
* https://github.com/intel/intel-one-mono/tree/main/fonts/ttf
* https://fonts.google.com/specimen/Roboto+Mono
* https://fonts.google.com/specimen/Cousine
* https://github.com/adobe-fonts/source-code-pro/tree/release/TTF

Generate C++ variables in base .cpp files

```bash
$ cd imgui/misc/fonts
$ g++ binary_to_compressed_c.cpp -o binary_to_compressed_c
$ ./binary_to_compressed_c DroidSans.ttf DroidSans > DroidSans.cpp
$ ./binary_to_compressed_c RobotoMono-Regular.ttf RobotoMono > RobotoMono.cpp
$ ./binary_to_compressed_c NotoSansMono-Regular.ttf NotoSansMono > NotoSansMono.cpp
$ ./binary_to_compressed_c intelone-mono-font-family-regular.ttf IntelOneMon
o > IntelOneMono.cpp
$ ./binary_to_compressed_c Cousine-Regular.ttf Cousine > Cousine.cpp
$ ./binary_to_compressed_c SourceCodePro-Regular.ttf SourceCodePro > SourceCodePro.cpp
$ ./binary_to_compressed_c fa-brands-400.ttf FontAwesome6_brands > FontAwesome6_brands.cpp
$ ./binary_to_compressed_c fa-regular-400.ttf FontAwesome6_regular > FontAwesome6_regular.cpp
$ ./binary_to_compressed_c fa-solid-900.ttf FontAwesome6_solid > FontAwesome6_solid.cpp
```

remove `static` storage from variables and add

```cpp
#include "compiled_fonts.h"
```

# Icon files

Take from https://github.com/juliettef/IconFontCppHeaders
