![badge](https://github.com/yousifm/LoxTreeWalkInterpreter/actions/workflows/cmake.yml/badge.svg)

# LoxTreeWalkInterpreter
I am reading Crafting Interpreters by Robert Nystrom
This is an implementation of the first interpreter in C++

### Syntax Overview
#### Variable declaration and assignment
```
var x = 5;
var y;  // Sets to null
y = 4;
```
#### Control flow
```
if (condition) {
  // statement body
} else {
  // optional else clause
}

while (condition) {
  // statement body
}

for (init; condition; increment) {
  // statement body
} 
```
#### Functions
```
fun myFunction(arg1, arg2) {
  // do stuff
}

// function call
myFunction(x, y);
```
#### Classes
```
class Car {
  //python-style initializer function
  init(color) {
    this.color = color
  }
}

myCar = Car("green");
print myCar.color;   //green
```
#### Recursive Fibonacci example
```
fun fib(n) {
  if (n <= 0) return 0;
  if (n == 1 or n == 2) return 1;
  return fib(n - 1) + fib(n - 2);
}
```
