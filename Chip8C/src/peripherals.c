#include <SDL2/SDL.h>

#include <peripherals.h>

SDL_Window* screen;

SDL_Renderer* renderer;

int DISPLAY_WIDTH;
int DISPLAY_HEIGHT;

int BUF_WIDTH;
int BUF_HEIGHT;

SDL_Scancode keymap[16] = 
{
    SDL_SCANCODE_X, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, 
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_A, 
    SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_Z, SDL_SCANCODE_C,
    SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F, SDL_SCANCODE_V
};

void init_display(int width, int height, int buf_width, int buf_height)
{
    DISPLAY_WIDTH = width;
    DISPLAY_HEIGHT = height;
    BUF_WIDTH = buf_width;
    BUF_HEIGHT = buf_height;

    SDL_Init(SDL_INIT_VIDEO);

    screen = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);

    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
}

void draw(unsigned char* display_buf)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    //clear the render target with the drawing colour
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //iterate through the display
    for (int i = 0; i < BUF_HEIGHT*BUF_WIDTH; i++)
    {
        int x = i % BUF_WIDTH;
        int y = i / BUF_WIDTH;
        
        SDL_Rect rect;

        rect.x = x * (DISPLAY_WIDTH / BUF_WIDTH);
        rect.y = y * (DISPLAY_HEIGHT / BUF_HEIGHT);
        rect.w = DISPLAY_WIDTH / BUF_WIDTH;
        rect.h = DISPLAY_HEIGHT / BUF_HEIGHT;

        if (display_buf[i] == 1)
        {
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    //update screen
    SDL_RenderPresent(renderer);
}

int sdl_ehandler(unsigned char* keypad) 
{
    SDL_Event event;

    // check for event
    if (SDL_PollEvent(&event)) {
        // get snapshot of current state of the keyboard
        const Uint8* state = SDL_GetKeyboardState(NULL);

        switch (event.type) {
            case SDL_QUIT:
                return -1;
                break;
            default:
                if (state[SDL_SCANCODE_ESCAPE]) {
                    return -1;
                }

                // updating the keypad with the current state
                for (int keycode = 0; keycode < 16; keycode++) {
                    keypad[keycode] = state[keymap[keycode]];
                }

                break;
        }
    }
    return 0;
}

void stop_display(void) 
{
    SDL_DestroyWindow(screen);
    SDL_Quit();
}