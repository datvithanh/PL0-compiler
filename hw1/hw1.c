#include <stdio.h>
#include <string.h>
enum state  {A,B,C,D};
int Delta[4][2]={B,A,B,C,B,D,B,A}; 
int process(char* str) {
    int i, l;
    char c;
    enum state q = A;
    i = 0;
    l = strlen(str);
    c = str[i];
    while (i<l) {
        if(c == 'a' || c == 'b'){
            q = Delta[q][c-'a'];
            c = str[++i];
        } else return -1;
    }
    
    if(q==D)
        return 1;
    else return 0;
}
int main(int argc, char* argv[]){
    int i = 0;
    printf("number of words: %d\n", argc-1);
    if(argc > 1) {
        for(i=1; i<argc; ++i){
            int res = process(argv[i]);
            switch(res) {
                case -1:
                    printf("%s ERROR\n", argv[i]);
                    break;
                case 0:
                    printf("%s NO\n", argv[i]);
                    break;
                case 1:
                    printf("%s YES\n", argv[i]);
                    break;
                default:
                    break;
            }
        }
    }
}
