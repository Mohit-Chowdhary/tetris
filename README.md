terminal_tetris

okay first of all, most i’ve learnt from this proj is not even tetris or C++ — it’s g++ being something else entirely.

what actually happened: so like, i started writing this game just fine — until i put

#include std::this_thread::sleep_for(...);

and the compiler went: ❌ "this_thread was not declared in this scope"

so now i'm checking is it my syntax? do i need std::? should i include chrono? is it windows? nope. turns out my g++ was version 6 (which basically means ancient, unusable garbage in 2025 terms)

so then i "upgrade" to g++ 15 using msys2, add add that path to system variables cuz the first time that i run it still showed g++ 6 now it doesn't even compile — just throws exit code 1 with zero errors. no .exe created. i'm thinking “is this vscode’s fault? powershell? me??” tried diffrent ides to run, even termial always error 1 for ANY program

eventually had to wipe everything and reinstall mingw-w64-x86_64-toolchain clean inside MINGW64

after hours of pure compiler pain and self doubt, the test.cpp finally compiled.(print statment)

i hate this
