# Cookie Parser

> header only C library for parsing cookies from request headers.

Usage Example:

``` c
#include <stdio.h>
#include <cookies.h>

const char *cookies = "__utmc=1; logged_in=yes; tz=America%2FNew_York";

int main(void) {

    // parse an individual cookie's value
    char *value = cookies_lookup(cookies, "logged_in");
    if (value) {
      printf("Logged In: %s\n", value);
    }
    free(value);

    // parse all cookies into a jar
    cookies_jar_t jar = cookies_loads(cookies);
    size_t i;
    for (i = 0; i < jar.length; i++) {
      printf("Key: %s, Value: %s\n", jar.entries[i]->key, jar.entries[i]->value);
    }
    cookies_jar_clear(&jar);
}
```
