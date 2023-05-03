union {
    int i;
    char c[sizeof(int)];
} x;

int main () { 
    x.i = 1;
    if(x.c[0] == 1)
        printf("little-endian\n");
    else    
        printf("big-endian\n");

}