#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emscripten.h>

#define NUM_CIRCLES 100

struct Circle {
    int x, y;
    int r;
    int cr, cg, cb;
};

struct CircleAnimationData {
    int x, y;
    int r;
    int vx, vy;
    int dx, dy;
};
 
struct Circle circles[NUM_CIRCLES];
struct CircleAnimationData animationData[NUM_CIRCLES];

int getRand(int max) {
    return (rand() % max);
}

int main() {
    srand(time(NULL));

    for (int i = 0; i < NUM_CIRCLES; i++) {
        int radius = getRand(50);
        int x = getRand(1000) + radius;
        int y = getRand(1000) + radius;

        animationData[i].x = x;
        animationData[i].y = y;
        animationData[i].r = radius;
        animationData[i].vx = getRand(10);
        animationData[i].vy = getRand(10);
        animationData[i].dx = 1;
        animationData[i].dy = 1;

        circles[i].x = x;
        circles[i].y = y;
        circles[i].r = radius;
        circles[i].cr = getRand(255);
        circles[i].cg = getRand(255);
        circles[i].cb = getRand(255);
    }

    // emscripten_run_script("render()");
    EM_ASM({ render($0, $1); }, NUM_CIRCLES * 6, 6);
}

struct Circle * getCircles(int canvasWidth, int canvasHeight) {
    for (int i = 0; i < NUM_CIRCLES; i++) {
        if (animationData[i].x + animationData[i].r >= canvasWidth) animationData[i].dx = -1;
        if (animationData[i].x - animationData[i].r <= 0) animationData[i].dx = 1;
        if (animationData[i].y + animationData[i].r >= canvasHeight) animationData[i].dy -= 1;
        if (animationData[i].y - animationData[i].r <= 0) animationData[i].dy = 1;
        
        animationData[i].x += (animationData[i].dx == 1) ? animationData[i].vx : (animationData[i].vx * -1);
        animationData[i].y += (animationData[i].dy == 1) ? animationData[i].vy : (animationData[i].vy * -1);

        circles[i].x = animationData[i].x;
        circles[i].y = animationData[i].y;
    }
    
    return circles;
}