
int foo(a)
{
    int b = 30;
    int q = 10 * 2;
    int p = b - q;
    int c = p * 15;
    return c + a;
}

int main()
{
    int x = 1 + 4;
    int y = foo(foo(x));
    return x + y;
}
