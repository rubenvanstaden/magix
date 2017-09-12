#include <stdio.h>

// Process has done i out of n rounds,
// and we want a bar of width w and resolution r.
// http://www.ross.click/2011/02/creating-a-progress-bar-in-c-or-any-other-console-app/
void load_bar(int x, int n, int r, int w) 
{
    if (x % (n / r + 1) != 0) return;

    float ratio = x / (float) n;
    int c = ratio * w;

    printf("%3d%% [", (int) (ratio * 100));

    for (int x = 0; x < c; x++)
        printf("%c[1;36m=\033[0m", 27);
    for (int x = c; x < w; x++)
        printf(" ");

    if (ratio != 1)
        printf("]\n\033[F\033[J");
    else
        printf("]\n");
}
