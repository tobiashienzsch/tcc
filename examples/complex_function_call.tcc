
int foo(a) { return a * 2; }

int main()
{
    int x = 1 + 4;
    int y = foo(foo(x * 2));
    return x + y;
}
