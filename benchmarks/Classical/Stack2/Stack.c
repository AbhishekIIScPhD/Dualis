#include <vcc.h>
#include <limits.h>

int sl = 0;

void push(int n){
    sl = sl +1;
}


void pop(){
    sl = sl -1;
}

int main(){
    int c=0, d=0, n;
    while (c < n){
        push(n);
        c = c+1;
    }
    while (sl != 0){
        pop();
        d = d+1;
    }
    _(assert d == n)
}
