//Demonstrate the use of different conversion specifiers and resulting output to allow
//the items to be printed.
/*


    %d → integer

    %u → unsigned integer

    %f → floating-point

    %lf → double

    %c → single character

    %s → string

    %x / %o → hexadecimal / octal

    %e / %g → scientific notation
*/
#include <stdio.h>

int main() {
    int num = -42;
    unsigned int unum = 42u;
    float pi = 3.14159f;
    double big = 12345.6789;
    char ch = 'A';
    char str[] = "Hello";

    printf("Integer (%%d): %d\n", num);
    printf("Unsigned (%%u): %u\n", unum);
    printf("Float (%%f): %f\n", pi);
    printf("Double (%%lf): %lf\n", big);
    printf("Character (%%c): %c\n", ch);
    printf("String (%%s): %s\n", str);
    printf("Hexadecimal (%%x): %x\n", num);
    printf("Octal (%%o): %o\n", num);
    printf("Scientific notation (%%e): %e\n", pi);
    printf("Shortest representation (%%g): %g\n", pi);

    return 0;
}
/*
Integer (%d): -42
Unsigned (%u): 42
Float (%f): 3.141590
Double (%lf): 12345.678900
Character (%c): A
String (%s): Hello
Hexadecimal (%x): ffffffd6
Octal (%o): 37777777626
Scientific notation (%e): 3.141590e+00
Shortest representation (%g): 3.14159
*/
