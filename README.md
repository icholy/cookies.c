# Cookie Parser

> header only C library for parsing cookies from request headers.

Usage Example:

``` c
#include <stdio.h>
#include <cookies.h>

const char *cookies = "__utmc=1; logged_in=yes; tz=America%2FNew_York";

int main(void) {
    char *value = cookies_lookup(cookies, "logged_in");
    if (value) {
      printf("Logged In: %s\n", value);
    }
}
```
