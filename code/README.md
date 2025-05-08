Online Judge Local Runner

The local runner takes in the solution ```main.cpp```, and test cases, i.e., ```input``` and ```output```, Compile the cpp code into a binary, and execute it by taking all input and comparing against the groundtruth in output. If all test cases are passed, the runner will print 'Accpeted'


## Setup

On Mac, install the g++ compiler
```
brew install gcc@14
```
Check the compiler is installed:
```
which g++-14
```
You should see:
```
/opt/homebrew/bin/g++-14
```

## Run the local judge

```
python oj.py tests/7300/main.cpp tests/7300/7300.in tests/7300/7300.out
```
You will see that the main.cpp is compiled and executed, the result is:
```
✅ Accepted
```
If it fails any of the test cases, you will see
```
❌ Error
```
