## Getting Started Guide

This guide walks you through setting up the Docker environment and
performing a basic "kick-the-tires" test to verify the artifact
functions correctly. This initial phase is designed to take less than
30 minutes to complete.

### 1. Prerequisites
* **Docker:** Ensure Docker is installed and running on your host
  machine.
* **OS Compatibility:** Compatible with Linux.

### 2. Environment Setup
Navigate to the directory (Dualis) of the artifact (where the `Dockerfile`
is located) and build the image:

```bash
docker image build -t dualis:latest .
```
and run the image :

```bash
docker run --rm -it dualis:latest
```

### 3. Running Basic Tests (for kick the tires)

In this phase we run all the three learners **LLM**, **HornICELLM**,
**HornICE** for **BinaryTree** and **Stack** benchmarks.

Details on the Benchmarks:

>> BinaryTree

The client uses an implementation of **BinaryTree** (library). It
executes a loop for `N` iterations (where `N` is chosen
nondeterministically), in which it inserts a value `n` (also chosen
nondeterministically) only if it is non-negative (`n >= 0`). After the
loop, it searches for a value `v` (chosen nondeterministically) and
asserts that the returned value `ret` is always `false`. The abstract
program is shown below.

```
BinaryTree bt;

while(*) {
    int n;
    if (n >= 0) {
        bt.insert(n);
    }
}

int v = *;
assume(v < 0);
ret = bt.search(v)
assert (ret == false);
```

We learn the following **modular** specifications. 

```
insert (n) := min'(o) = min(o) || min'(o) = n
search (v) :=  v >= 0 || ret = false, ret has the return value of the method.
inv := empty(o) || min(o) >= 0

``` 

Note the specifications of each of the functions make use of
"observer" methods, _min(o)_, _min'(o)_, _empty(o)_ and
_empty'(o)_. For more details on "observer" methods refer to
the section Illustrative Example in the paper.

And the following **contextual** specifications.

```
insert (n) := min'(o) = min(o) || min'(o) = n
search (v) :=  v >= 0 || ret = false, ret captures the return value.
inv := empty(o) || min(o) >= 0
```

>> Stack

The client uses an implementation of **Stack** (library). It first
chooses a nondeterministic value `N` such that `N > 0`. In the first
loop, the client pushes the value `N` onto the stack `N` times while
incrementing the counter `c`. After this, the client repeatedly pops
elements from the stack until the stack becomes empty, incrementing
another counter `d` for every pop operation. Finally, the client
asserts that the number of pop operations performed is equal to `N`.

The abstract program is shown below.

'''
Stack st;
int c = 0, d = 0, N = *;

assume(N > 0)
while (c < n)
{
	st.push(n);
    c = c+1;
}

while (sl != 0) {
    st.pop();
    d = d+1;
}

assert (d == n)
'''

The following learners learn **equivalent** specifications to the ones
discussed above.

### LLM
**BinaryTree**

```
insert(n) :=
search(v) :=
inv := 
```

**Stack**

```
push(n) :=
pop() :=
inv1 :=
inv2 :=
```

### HornICELLM
**BinaryTree**

```
insert(n) :=
search(v) :=
inv := 
```

**Stack**

```
push(n) :=
pop() :=
inv1 :=
inv2 :=
```

### HornICE
**BinaryTree**

```
insert(n) :=
search(v) :=
inv := 
```

**Stack**

```
push(n) :=
pop() :=
inv1 :=
inv2 :=
```

## Step-by-Step instructions for evaluations
