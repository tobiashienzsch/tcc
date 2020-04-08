int main() {
  int x = 1 + 4 * 3;
  int y = x + x + x + x;
  int z = (y + x) * 2;
  int a = 2 * z;
  x = -(a - x);
  return x;
}