# terminal_tetris

> A full-featured Tetris game rendered entirely in the Windows terminal. Built from scratch in C++ using raw console buffers, real-time input handling, piece rotation logic, and line-clearing mechanics.

---

## 🎮 Features

- **Real-time controls** (← → ↓ Z to rotate)  
- **7 Tetromino types** with custom rotation logic  
- **Collision detection** and piece locking  
- **Line-clearing system** with scoring  
- **Score tracking** with exponential bonus on clears  
- **Runs in a Windows console** using `WriteConsoleOutputCharacterW`  

---

## 🛠 Built With

- **C++**
- **Windows Console API**
- `std::thread`, `std::chrono` for timing
- `GetAsyncKeyState()` for live input
- Manual buffer manipulation

---

## ⚙️ How to Compile

You need **G++ 10 or later** and **Windows** (because of console API calls).

### 🔧 Using g++

```bash
g++ -Wall -Wextra -std=c++17 -o tetris.exe tetris.cpp -static

```
▶️ Run it
```bash
./tetris.exe
```

What I Learned
Threading and precise frame timing with sleep_for

Framebuffer graphics using character arrays

Debugging g++ toolchain issues (like clock_gettime64 errors on Windows)

The pain of manually rotating 2D blocks in array memory

Handling compiler hell — from broken G++ 6 to misconfigured G++ 15 — and surviving it

Screenshot


https://github.com/user-attachments/assets/5b745bbc-705e-46b7-9fb9-45fb8348099b




⚠️ Make sure your terminal is 120x30, else edit the code to fit your terminal size.



PS:
okay first of all, most i’ve learnt from this proj is not even tetris or C++ — it’s g++ being something else entirely.

what actually happened: so like, i started writing this game just fine — until i put

#include std::this_thread::sleep_for(...);

and the compiler went: ❌ "this_thread was not declared in this scope"

so now i'm checking is it my syntax? do i need std::? should i include chrono? is it windows? nope. turns out my g++ was version 6 (which basically means ancient, unusable garbage in 2025 terms)

so then i "upgrade" to g++ 15 using msys2, add add that path to system variables cuz the first time that i run it still showed g++ 6 now it doesn't even compile — just throws exit code 1 with zero errors. no .exe created. i'm thinking “is this vscode’s fault? powershell? me??” tried diffrent ides to run, even termial always error 1 for ANY program

eventually had to wipe everything and reinstall mingw-w64-x86_64-toolchain clean inside MINGW64

after hours of pure compiler pain and self doubt, the test.cpp finally compiled.(print statment)

i hate this
