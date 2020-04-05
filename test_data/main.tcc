auto x = 1+4*3;
auto y = x+x+x+x;
auto z = (y+x)*2;
auto a = 2*z;
x = -(a - x);
return x;