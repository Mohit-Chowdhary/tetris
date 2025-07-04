#include <iostream>
#include <thread>
#include <vector>


#include <chrono>
#include <stdio.h>
#include <windows.h>

//does this even work
using namespace std;

wstring tetromino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char *pField = nullptr;
int nScreenWidth = 120;
int nScreenHeight = 30;

// Add score variable
int nScore = 0;


int Rotate(int px, int py, int r){
    switch(r % 4){
        case 0: return py*4+px; //0 degrees
        case 1: return 12+py-px*4; //90 degrees
        case 2: return 15-py*4-px; //180 degrees
        case 3: return 3-py+px*4; //270 degrees
    }
    return 0;
}



bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY){
    for(int px = 0; px < 4; px++)
        for(int py = 0; py < 4; py++){
            //get index into piece
            int pi = Rotate(px, py, nRotation);
            //get index into field
            int fi = (nPosY+py)*nFieldWidth+(nPosX+px);

            if(nPosX+px >= 0 && nPosX+px < nFieldWidth){
                if(nPosY+py >= 0 && nPosY+py < nFieldHeight){
                    if(tetromino[nTetromino][pi] != L'.' && pField[fi] != 0)
                        return false;
                }
            }
        }
    
    return true;
}



int main(){
    //assets
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");

    tetromino[1].append(L"..X.");
    tetromino[1].append(L".XX.");
    tetromino[1].append(L".X..");
    tetromino[1].append(L"....");

    tetromino[2].append(L".X..");
    tetromino[2].append(L".XX.");
    tetromino[2].append(L"..X.");
    tetromino[2].append(L"....");

    tetromino[3].append(L"....");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L"....");

    tetromino[4].append(L"..X.");
    tetromino[4].append(L".XX.");
    tetromino[4].append(L"..X.");
    tetromino[4].append(L"....");

    tetromino[5].append(L"....");
    tetromino[5].append(L".XX.");
    tetromino[5].append(L"..X.");
    tetromino[5].append(L"..X.");

    tetromino[6].append(L"....");
    tetromino[6].append(L".XX.");
    tetromino[6].append(L".X..");
    tetromino[6].append(L".X..");


    pField = new unsigned char[nFieldWidth*nFieldHeight];
    for(int x = 0; x < nFieldWidth; x++)
        for(int y = 0; y < nFieldHeight; y++)
            pField[y*nFieldWidth+x] = (x==0 || x==nFieldWidth-1 || y==nFieldHeight-1) ? 9 : 0;

    wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
    for(int i = 0; i < nScreenWidth*nScreenHeight; i++)
        screen[i] = L' ';

    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);

    DWORD dwBytesWritten = 0;



    bool bGameOver = false;

    int nCurrentPiece = 0;
    int nCurrentRotation = 0;
    int nCurrentX = nFieldWidth/2;
    int nCurrentY = 0;

    bool bKey[4];
    bool bRotateHold = false;

    int nSpeed = 20;
    int nSpeedCounter = 0;
    bool bForceDown = false;

    vector<int> vLines;
    
    while(!bGameOver){
        // Game timing =====
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        nSpeedCounter++;
        bForceDown = (nSpeedCounter == nSpeed);

        // Input =====
        for(int k = 0; k < 4; k++){                               // R, L, D, Z
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;
        }

        // Game logic =====
        nCurrentX -= (bKey[1]) && (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX-1, nCurrentY)) ? 1:0;

        nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX+1, nCurrentY)) ? 1:0;

        nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY+1)) ? 1:0;

        if(bKey[3]){
            nCurrentRotation += (!bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation+1, nCurrentX, nCurrentY)) ? 1:0;
            bRotateHold = true;
        }
        else{
            bRotateHold = false;
        }

        if(bForceDown){
            if(DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY+1)){
                nCurrentY++; // Always move down if possible
            }
            else {
                // It can't move down, lock the piece in the field
                for(int px = 0; px < 4; px++)
                    for(int py = 0; py < 4; py++)
                        if(tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                            pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;
                
                // Add score for placing a piece
                nScore += 25;
                
                //chec if new line
                for(int py = 0; py < 4; py++)
                    if(nCurrentY+py < nFieldHeight -1)
                    {
                        bool bLine = true;
                        for(int px = 1; px < nFieldWidth - 1; px++)
                            bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

                        if(bLine)
                        {
                            // Mark the line for clearing, e.g. set to 8 or push to a vector
                            for(int px = 1; px < nFieldWidth - 1; px++)
                                pField[(nCurrentY + py) * nFieldWidth + px] = 8;
                            // (You can add line-clearing logic here)
                            vLines.push_back(nCurrentY+py);
                        }
                    }
                
                // Pick a new piece
                nCurrentPiece = rand() % 7;
                nCurrentRotation = 0;
                nCurrentX = nFieldWidth / 2;
                nCurrentY = 0;
        
                // If new piece doesn't fit, game over
                bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
            }
            nSpeedCounter=0;
        }

        // Render output =====


        // Draw Field
        for(int x = 0; x < nFieldWidth; x++)
            for(int y = 0; y < nFieldHeight; y++)
                screen[y*nScreenWidth+x] = L" ABCDEFG=#"[pField[y*nFieldWidth+x]];

        // Draw Current Piece
        for(int px = 0; px < 4; px++)
            for(int py = 0; py < 4; py++)
                if(tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                    screen[(nCurrentY+py)*nScreenWidth+(nCurrentX+px)] = nCurrentPiece+65;

        // Display score
        swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", nScore);

        if (!vLines.empty())
        {
            // Display frame with completed lines highlighted
            WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth * nScreenHeight, {0,0}, &dwBytesWritten);
            std::this_thread::sleep_for(std::chrono::milliseconds(400)); // Pause to show the completed lines

            // Remove the completed lines
            for (auto &v : vLines)
                for (int px = 1; px < nFieldWidth - 1; px++)
                {
                    for (int py = v; py > 0; py--)
                        pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
                    pField[px] = 0;
                }

            // Add score for cleared lines
            nScore += (1 << vLines.size()) * 100;

            vLines.clear();
        }
        else
        {
            // Display frame as usual
            WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth * nScreenHeight, {0,0}, &dwBytesWritten);
        }

    }

    return 0;

}