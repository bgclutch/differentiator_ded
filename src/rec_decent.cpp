#include "../inc/rec_decent.h"
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <string.h>

const char* string = "cos(123)";
int position = 0;

double RecursiveDecent(){
    return GetG();
}

double GetG() {
    double val = GetE();
    if (string[position] != '\0')
        SyntaxError(__FILE__, __LINE__);
    position++;
    return val;
}

double GetE(){
    double val_first = GetT();
    while (string[position] == '+' || string[position] == '-'){
        int oper = string[position];
        position++;
        double val_second = GetT();
        if (oper == '+')
            val_first += val_second;
        else
            val_first -= val_second;
    }

    return val_first;
}

double GetT(){
    double val_first = GetFunction();
    while (string[position] == '*' || string[position] == '/'){
        int oper = string[position];
        position++;
        double val_second = GetFunction();
        if (oper == '*')
            val_first *= val_second;
        else
            val_first /= val_second;
    }
    return val_first;
}

double GetP(){
    if (string[position] == '('){
        position++;
        double val = GetE();
        if (string[position] != ')')
            SyntaxError(__FILE__, __LINE__);
        position++;
        return val;
    }
    else
        return GetN();
}

double GetPower(){
    double val_first = GetP();
    if (string[position] == '^'){
        position++;
        double val_second = GetP();
        return pow(val_first, val_second);
    }

    return val_first;
}

double GetFunction(){
    if (strncmp(string + position, "sin", strlen("sin")) == 0){
        position += (int)(strlen("sin"));
        return sin(GetPower());
    }
    else if (strncmp(string + position, "cos", strlen("cos")) == 0){
        position += (int)(strlen("cos"));
        return cos(GetPower());
    }
    else if (strncmp(string + position, "tan", strlen("tan")) == 0){
        position += (int)(strlen("tan"));
        return tan(GetPower());
    }
    else if (strncmp(string + position, "ln", strlen("ln")) == 0){
        position += (int)(strlen("ln"));
        return log(GetPower());
    }
    else {
        return GetPower();
    }
}

double GetN(){
    double val      = 0;
    int counter     = 0;
    int flag_sign   = 0;
    int flag_double = 0;
    int old_position = position;
    while (('0' <= string[position] && string[position] <= '9') || string[position] == '.' || string[position] == '-'){
        if (flag_double)
            counter++;
        if (string[position] == '.')
            flag_double = 1;
        else if (string[position] == '-')
            flag_sign = 1;
        else
            val = val * 10 + (string[position] - '0');

        position++;
    }
    if (old_position == position)
        SyntaxError(__FILE__, __LINE__);
    val /= pow(10, counter);
    if (flag_sign)
        val *= -1;

    return val;
}

void SyntaxError(const char* file, const size_t line){
    fprintf(stderr, "%s:%lu\n", file, line);
    assert(0);
}