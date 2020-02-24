#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
    
#define MAXVAL 100
#define NUMBER '0'
#define MAXOP // max value for operand/operator

int getch_(void);
void ungetch_(int);
int getop(char []);
void push(double);
double pop(void);

size_t sp = 0;
double val[MAXVAL];

char buf[BUFSIZ];
size_t bufp = 0;

int getch_(void) {
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch_(int c) {
    if(bufp >= BUFSIZ)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

int getop(char s[]) {
    int i, c;
    
    while((s[0] = c = getch_()) == ' ' || c == '\t') { }
    s[1] = '\0';
    if(!isdigit(c) && c != '.')
        return c; //if not a digit, return
    i = 0;
    if(isdigit(c))
        while(isdigit(s[++i] = c = getch_())) { }
    if(c == '.')
        while(isdigit(s[++i] = c = getch_())) { }
    s[i] = '\0';
    if(c !=EOF)
        ungetch_(c);
    return NUMBER; //number stored in s
}

void push(double f) {
    if(sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %g\n", f);
}

double pop(void) {
    if(sp > 0)
        return val[--sp];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

double head(void) { // getting head without popping it
    if (sp > 0)
          return val[sp - 1];
      else {
          printf("error: stack empty\n");
          return 0.0;
      }
}

void duplicate(void) {
    push(head());
}

void swap(void) {
    double swap;
    size_t i;

    if(sp < 2)
      printf("error: not enough elements to swap\n");
    else {
      i = sp - 1;
        swap = val[i - 1];
        val[i - 1] = val[i];
        val[i] = swap;
    }
}

void clear(void) {
    while (--sp >= 0)
          val[sp] = 0.0;
    sp = 0;
}

void rpnc(void) {
    int type;
    double op2;
    char s[BUFSIZ];
    
    while((type = getop(s)) != EOF) {
        switch(type) {
            case NUMBER:
                push(atof(s));
                break;
            case '+':
                push(pop() + pop());
                break;
            case '*':
                push(pop() * pop());
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case '/':
                op2 = pop();
                if(op2 != 0.0)
                    push(pop() / op2);
                else
                    printf("error: zero divisor\n");
                break;
            case '%':
                op2 = pop();
                if(op2 != 0.0)
                    push(fmod(pop(), op2));
                else
                    printf("error: zero divisor\n");
                break;
            case '?':
                printf("Top of stack: %f\n", head());
                break;
            case 'd':
                printf("Duplcate top of stack\n");
                duplicate();
                break;
            case 's':
                printf("Swap top two elements of stack\n");
                swap();
                break;
            case 'c':
                printf("Clear stack\n");
                clear();
                break;
            case '\n':
                printf("\t%.8g\n", pop());
                break;
            default:
                printf("error: unknown command %s\n", s);
                break;
        }
    }
}

int main(int argc, const char* argv[]) {
    rpnc();
    
    return 0;
}
