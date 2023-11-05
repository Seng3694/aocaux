import itertools
import sys

if len(sys.argv) != 3:
  print("wrong argument count")
  print("example: generate_test_data.py \"abcdef\" 3")
  exit()

input = sys.argv[1]
output_length = int(sys.argv[2])

data = [
  ('test/permutation_test_data.h',     itertools.permutations),
  ('test/combination_test_data.h',     itertools.combinations),
  ('test/combination_rep_test_data.h', itertools.combinations_with_replacement),
]

for (path, func) in data:    
  with open(path, 'w') as file:
    file.write('#include <stddef.h>\n')
    file.write(f"static const char test_input[] = \"{input}\";\n")
    file.write(f"static const size_t test_output_length = {output_length};\n")
    file.write('static char *test_data[] = {\n')
    for line in func(input, output_length):
      file.write(f"  \"{''.join(line)}\",\n")
    file.write('};\n')
    