
int foo(a, b) { return a * 2; }

int main()
{
    int x = 1 + 4;
    int y = foo(foo(x * 2, 5 + 6), 1 + 2 * 3);
    return x + y;
}
