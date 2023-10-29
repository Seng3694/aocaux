#include <aoc/md5.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

typedef struct {
  const char input[0x100];
  const char expected[33];
} test_data;

int main(void) {
  const test_data data[] = {
      {.input = "", .expected = "d41d8cd98f00b204e9800998ecf8427e"},
      {.input = "hello world!", .expected = "fc3ff98e8c6a0d3087d515c0473f8677"},
      {.input = "hi", .expected = "49f68a5c8493ec2c0bf489821c21fc3b"},
      {.input = "test1", .expected = "5a105e8b9d40e1329780d62ea2265d8a"},
      {.input = "test2", .expected = "ad0234829205b9033196ba818f7a872b"},
  };
  const size_t count = ARR_SIZE(data);

  char buffer[33] = {0};
  uint8_t hashBuffer[16] = {0};
  for (size_t i = 0; i < count; ++i) {
    aoc_md5(data[i].input, strlen(data[i].input), hashBuffer);
    aoc_md5_string(hashBuffer, buffer);
    printf("%s\n", buffer);
    assert(strcmp(data[i].expected, buffer) == 0);
  }
  printf("MD5 Test: %zu tests finished successfully\n", count);
}
