# Agent Instructions

- This is a single-file C++ ray tracer; the executable entrypoint is `main.cpp`.
- No build system or test suite is configured. Compile directly with a C++ compiler, for example `c++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o raytracer`.
- Run the generated executable with `./raytracer > image.ppm`; it writes a 256x256 P3 PPM image to stdout, so redirect output rather than treating it as terminal text.
- Keep generated binaries and rendered `*.ppm` files out of source changes unless explicitly requested.
