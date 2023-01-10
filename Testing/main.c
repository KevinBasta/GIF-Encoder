#include <stdio.h>
#include <stdlib.h>
#define W 1280
#define H 720

unsigned char frame[H][W][3] = {0};

int main(int argc, char* argv[]) {
    int x, y, count;

    FILE *pipein = popen("ffmpeg -i op.mp4 -f image2pipe -vcodec rawvideo -pix_fmt rgb24 -", "r");
    FILE *pipeout = popen("ffmpeg -y -f rawvideo -vcodec rawvideo -pix_fmt rgb24 -s 1280x720 -r 25 -i - -f mp4 -q:v 5 -an -vcodec mpeg4 output.mp4", "w");

    while (1) { 
        count = fread(frame, 1, H*W*3, pipein);

        if (count != H*W*3) break;

        for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j) {
                frame[i][j][0] = 255 - frame[i][j][0];
                frame[i][j][1] = 255 - frame[i][j][1];
                frame[i][j][2] = 255 - frame[i][j][2];
            }
        
        fwrite(frame, 1, H*W*3, pipeout);
    }

    fflush(pipein);
    pclose(pipein);
    fflush(pipeout);
    pclose(pipeout);
}