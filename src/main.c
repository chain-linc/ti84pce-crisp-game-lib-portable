#include <string.h>

#include <debug.h>

#include <sys/timers.h>
#include <keypadc.h>
#include <graphx.h>

#include "cglp.h"
#include "machineDependent.h"

//#define SPRITE_BUFFER_SIZE 64
#define GFX_SPRITE_T_DATA_START 2
#define CHARACTER_SIZE (CHARACTER_WIDTH * CHARACTER_HEIGHT)
#define SPRITE_SIZE (GFX_SPRITE_T_DATA_START + CHARACTER_SIZE)
#define TICKS2SECOND 533
/* xlibc is in format rrrbbggg */
#define RGBToIndex(r, g, b) ((r & 0b11100000) | ((b >> 6) << 3) | (g >> 5)), 
#define CHAR_RGBTO1555(r, g, b) (gfx_RGBTo1555(r, g, b) & 0xFF), ((gfx_RGBTo1555(r, g, b) >> 8) & 0xFF)


unsigned int xOffset;
unsigned int yOffset;
unsigned int canvasWidth;
unsigned int canvasHeight;

double currentTicks = 0;
double lastTicks = 0;
bool fullRedraw = true;

//unsigned char spriteBuffer[SPRITE_BUFFER_SIZE][CHARACTER_SIZE]; // buffer for sprite drawing
//int spriteHashes[SPRITE_BUFFER_SIZE];                           // hashes for sprites in buffer
//int spriteBufferIndex = 0;                                      // current index in sprite buffer
//int spriteAllocatedSize = 0;                                    // size of cached sprites
//int spriteAllocatedIndex = 0;                                   // index of current sprite within buffer

unsigned char sprite[SPRITE_SIZE] = {CHARACTER_WIDTH, CHARACTER_HEIGHT}; // sprite for character drawing

unsigned char palette[COLOR_COUNT * 2] = { \
    CHAR_RGBTO1555(255, 255, 255), /* WHITE */ \
    CHAR_RGBTO1555(255, 0, 0),     /* RED */ \
    CHAR_RGBTO1555(0, 255, 0),     /* GREEN */ \
    CHAR_RGBTO1555(255, 255, 0),   /* YELLOW */ \
    CHAR_RGBTO1555(0, 0, 255),     /* BLUE */ \
    CHAR_RGBTO1555(255, 0, 255),   /* PURPLE */ \
    CHAR_RGBTO1555(0, 255, 255),   /* CYAN */ \
    CHAR_RGBTO1555(0, 0, 0),       /* BLACK */ \
    CHAR_RGBTO1555(255, 128, 128), /* LIGHT RED */ \
    CHAR_RGBTO1555(128, 255, 128), /* LIGHT GREEN */ \
    CHAR_RGBTO1555(255, 255, 128), /* LIGHT YELLOW */ \
    CHAR_RGBTO1555(128, 128, 255), /* LIGHT BLUE */ \
    CHAR_RGBTO1555(255, 128, 255), /* LIGHT PURPLE */ \
    CHAR_RGBTO1555(128, 255, 255), /* LIGHT CYAN */ \
    CHAR_RGBTO1555(128, 128, 128), /* LIGHT BLACK */ \
    CHAR_RGBTO1555(16, 16, 16),    /* DARK GREY */ \
    CHAR_RGBTO1555(224, 224, 224), /* LIGHT GREY */ \
};


/* Machine dependent functions*/
void md_drawRect(float x, float y, float w, float h, unsigned char color)
{
    gfx_SetColor(color);
    gfx_FillRectangle((int)(x + xOffset), (int)(y + yOffset), (int)w, (int)h);
}

void md_drawCharacter (

    unsigned char grid[CHARACTER_HEIGHT][CHARACTER_WIDTH], float x, float y,
    int hash)
{
    (void)hash;

    memcpy(sprite + GFX_SPRITE_T_DATA_START, grid, CHARACTER_SIZE);
    gfx_Sprite((gfx_sprite_t *)sprite, (int)(x + xOffset), (int)(y + yOffset));

    /*
    spriteAllocatedIndex = -1;
    for (int i = 0; i < spriteAllocatedSize; i++) {
        if (spriteHashes[i] == hash) {
            spriteAllocatedIndex = i;
            break;
        }
    }

    if (spriteAllocatedIndex == -1)
    {
        dbg_printf("Allocating character thats at (%f, %f) with hash %d at index %d\n", x, y, hash, spriteBufferIndex);
        spriteHashes[spriteBufferIndex] = hash;
        spriteBuffer[spriteBufferIndex][0] = CHARACTER_WIDTH;
        spriteBuffer[spriteBufferIndex][1] = CHARACTER_HEIGHT;

        for (int i = 0; i < CHARACTER_SIZE; i++)
        {
            // grid1D is in format RGB
            spriteBuffer[spriteBufferIndex][GFX_SPRITE_T_DATA_START + i] =  RGBToPalette(grid1D[i + 0], grid1D[i + 1], grid1D[i + 2]);
        }

        // Update sprite buffer index and size
        spriteAllocatedIndex = spriteBufferIndex;

        // if overflow start deleting old buffer entries
        if (++spriteBufferIndex >= SPRITE_BUFFER_SIZE)
        {
            dbg_printf("SpriteBufferIndex overflow\n");
            spriteBufferIndex = 0;
        }

        // if overflow keep at same size
        if (spriteAllocatedSize < SPRITE_BUFFER_SIZE)
        {
            spriteAllocatedSize++;
        } else {
            dbg_printf("Sprite cache full, overwriting old entries\n");
        }
    }

    gfx_Sprite((gfx_sprite_t *)spriteBuffer[spriteAllocatedIndex], (int)(x + xOffset), (int)(y + yOffset));
    */

}

void md_clearView(unsigned char color)
{
    gfx_SetColor(color);
    gfx_FillRectangle(xOffset, yOffset, canvasWidth, canvasHeight);
}

void md_clearScreen(unsigned char color)
{
    gfx_FillScreen(color);
    fullRedraw = true;
}

void md_playTone(float freq, float duration, float when)
{
    /* No audio on the ti84pce */
    (void)freq;
    (void)duration;
    (void)when;
}

void md_stopTone()
{
    /* No audio on the ti84pce */
    return;
}

float md_getAudioTime()
{
    /* No audio on the ti84pce */
    return 0.0f;
}

void md_initView(int w, int h)
{
    canvasWidth = w;
    canvasHeight = h;

    xOffset = (GFX_LCD_WIDTH - w) / 2;
    yOffset = (GFX_LCD_HEIGHT - h) / 2;
    gfx_SetClipRegion(xOffset, yOffset, xOffset + canvasWidth, yOffset + canvasHeight);

}

void md_consoleLog(char *msg)
{
    dbg_printf("%s", msg);
}


/* Entry point of the app */
int main(void)
{
    dbg_printf("Program Started\n");

    /* Initialize graphics drawing */
    gfx_Begin();
    gfx_SetDrawBuffer();

    /* Set the palette for sprites */
    gfx_SetPalette(palette, COLOR_COUNT * 2, 0);
    /* Set a transparent color */
    gfx_SetTransparentColor(TRANSPARENT);


    dbg_printf("initGame() start\n");
    /* Initialize the game */
    initGame();
    dbg_printf("initGame() done\n");

    dbg_printf("starting loop\n");

    bool menuWasJustEntered = false;
    bool exit = false;
    /* Check for clear key to exit */
    while (!exit)
    {
        /* Call update 60 times a second */

        currentTicks = timer_Get(1) / TICKS2SECOND;
        if (currentTicks < lastTicks + TICKS2SECOND)
        {
            dbg_printf("FPS: %f\n", 60 / (currentTicks - lastTicks));
            lastTicks = currentTicks;

            /* Update game input state */
            setButtonState(
                kb_IsDown(kb_KeyLeft),      // left
                kb_IsDown(kb_KeyRight),     // right
                kb_IsDown(kb_KeyUp),        // up
                kb_IsDown(kb_KeyDown),      // down
                kb_IsDown(kb_KeyAlpha),     // b
                kb_IsDown(kb_Key2nd)        // a
            );
            updateFrame();

            /* update screen */
            if (fullRedraw)
            {
                gfx_BlitBuffer();
                fullRedraw = false;
            } else 
            {
                gfx_BlitRectangle(gfx_buffer, xOffset, yOffset, canvasWidth, canvasHeight);
            }

            /* update keyboard state (before clear key check) */
            kb_Scan();
            if (kb_IsDown(kb_KeyClear))
            {
                if (isInMenu && !menuWasJustEntered) {
                    exit = true;
                } else {
                    goToMenu();
                    menuWasJustEntered = true;
                }
            } else
            {
                menuWasJustEntered = false;
            }
        } else {
            dbg_printf("60fps reached limiting framerate now");
        }
    };

    /* End graphics drawing */
    gfx_End();

    /* Return to OS */
    return 0;
}