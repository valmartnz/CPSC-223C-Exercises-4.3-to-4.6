#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
    
#define MAXVAL 100
#define NUMBER '0'
#define MATH '1'
#define STACK '2'
#define VARIABLE '3'
#define CONSTANT '4'
#define MAXOP 100 // max value for operand/operator

size_t sp = 0;
double val[MAXVAL];

char variables[BUFSIZ];
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

void build_string(char s[]) {
    int c, i = 0;
    while(isalpha(s[i++] = c = getch_())) { }
    s[i] = '\0';
}

int getop(char s[]) {
    int i, c;
    
    while((s[0] = c = getch_()) == ' ' || c == '\t') { }
    s[1] = '\0';
    
    if(c == '@')                { build_string(s); return STACK; }
    if(c == '=' || c == '?')    { build_string(s); return VARIABLE; }
    if(isalpha(c))              { build_string(s); return MATH; }
    if(c == '_')                { build_string(s); return CONSTANT; }
    if(!isdigit(c) && c != '.') { return c; }

    i = 0;
    if(isdigit(c))
        while(isdigit(s[++i] = c = getch_())) { }
    if(c == '.')
        while(isdigit(s[++i] = c = getch_())) { }
    s[i] = '\0';
    if(c != EOF)
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

double peek(void) { // getting head without popping it
    if (sp > 0)
          return val[sp - 1];
      else {
          printf("error: stack empty\n");
          return 0.0;
      }
}

void duplicate(void) {
    push(peek());
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

void stack(char s[]) {
    ++s;
    if(strcmp(s, "duplicate") == 0) {
      duplicate();
    } else if(strcmp(s, "peak") == 0) {
      peek();  
    } else if(strcmp(s, "swap") == 0) {
      swap();
    } else if(strcmp(s, "clear") == 0) {
      clear();  
    } else {
      printf("Unknown stack operation\n");
    }
}

void mathfunc(char s[]) {
    double op1, op2, result = 0;
    
    if(strcmp(s,"pow") == 0) {
      op2 = pop();
      op1 = pop();
      result = pow(op1, op2);
    } else if(strcmp(s,"sin") == 0) { result = sin(pop()); 
    } else if(strcmp(s,"cos") == 0) { result = cos(pop());
    } else if(strcmp(s,"exp") == 0) { result = exp(pop());
    } else if(strcmp(s,"tan") == 0) { result = tan(pop());
    } else if(strcmp(s,"sqrt") == 0) { result = sqrt(pop());
    } else { printf("error: %s is not supported\n",s); }

    push(result);
    printf("%.3f\n", result);
}

void variable(char s[]) {
    if(*s == '=') { variables[*++s - 'A'] = pop(); } 
    else if(*s == '?') { push(variables[*++s - 'A']); }
}
            
void constant(char s[]) {
    ++s;
    if(strcmp(s, "PI") == 0) { push(3.141592); }
    else if(strcmp(s, "e") == 0) { push(2.71828); }
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
            case MATH:
                mathfunc(s);
                break;
            case STACK:
                stack(s);
                break;
            case VARIABLE:
                variable(s);
                break;
            case CONSTANT:
                constant(s);
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
