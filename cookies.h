#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct {
    const char* key;
    size_t      key_length;
    const char* value;
    size_t      value_length;
} cookies_pair_t;

static char *cookies_pair_key(cookies_pair_t pair) {
  return strndup(pair.key, pair.key_length);
}

static char *cookies_pair_value(cookies_pair_t pair) {
  return strndup(pair.value, pair.value_length);
}

static void cookies_pair_trim_whitespace(cookies_pair_t *pair) {
    while (isspace(*pair->key)) {
        pair->key++;
        pair->key_length--;
    }
    while (isspace(pair->key[pair->key_length - 1])) {
        pair->key_length--;
    }
    while (isspace(*pair->value)) {
        pair->value++;
        pair->value_length--;
    }
    while (isspace(pair->value[pair->value_length - 1])) {
        pair->value_length--;
    }
}

typedef int (*cookie_handler_t)(const cookies_pair_t pair, void *userdata);

int cookies_parse(const char* text, cookie_handler_t handler, void *userdata) {

    const char *text_pos = text;
    const char *text_end = text + strlen(text);

    const char *key_end;
    const char *value_end;

    cookies_pair_t pair;

    while (text_pos < text_end) {

        // Find the key
        key_end = strchr(text_pos, '=');
        if (!key_end) {
            break;
        }
        pair.key = text_pos;
        pair.key_length = (size_t)(key_end - pair.key);

        // Find the value
        value_end = strchr(key_end, ';');
        if (!value_end) {
            value_end = text_end;
        }
        pair.value = key_end + 1;
        pair.value_length = (size_t)(value_end - pair.value);

        // trim whitespace
        cookies_pair_trim_whitespace(&pair);

        // invoke callback
        int res = handler(pair, userdata);
        if (res) {
            return res;
        }

        // move the position to the next key/value pair
        text_pos = value_end + 1;
    }

    return 0;
}

typedef struct {
    const char *key;
    char *value;
} cookies_lookup_t;

static int cookies_handle_lookup(const cookies_pair_t pair, void *data) {
    cookies_lookup_t *lookup = (cookies_lookup_t*)data;
    if (strncasecmp(lookup->key, pair.key, pair.key_length) == 0) {
        lookup->value = cookies_pair_value(pair);
        return 1;
    }
    return 0;
}

static void cookies_init_lookup(cookies_lookup_t *lookup, const char* key) {
    lookup->key = key;
    lookup->value = NULL;
}

char *cookies_lookup(const char* text, const char *key) {
    cookies_lookup_t lookup;

    cookies_init_lookup(&lookup, key);
    cookies_parse(text, cookies_handle_lookup, &lookup);
    return lookup.value;
}
