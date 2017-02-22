#!/usr/bin/env python

# using python 2.7
import sys, argparse

# --
def validate_parameters(R,C,L,H):
  for item, name in [(R,'R'),(C,'C'),(L,'L'),(H,'H')]:
    if item < 1 or item > 1000:
      raise Exception("{}={} (expected 1<={}<=1000)".format(name,str(item),name))

# --
count_M = 0
count_T = 0
def validate_content(R,C,content):
  global count_M, count_T
  old_size = len(content)
  content = [x for x in content if len(x.strip())]
  new_size = len(content)
  if old_size > new_size:
    print("Input file: ignoring {} lines with blanks".format(str(old_size - new_size)))
  if len(content) != R:
    raise Exception("Input file: expected R={} rows in content, actual rows={}".format(R,len(content)))
  for r in range(R):
    if len(content[r]) != C:
      raise Exception("Input file: expected C={} columns in content row={}, actual columns={}".format(C,r,len(content[r])))
    for c in range(C):
      if content[r][c] == 'M':
        count_M += 1
      elif content[r][c] == 'T':
        count_T +=1
      else:
        raise Exception("Input file: expected character 'M' or 'T' in content row={} column={}, actual='{}'".format(r,c,content[r][c]))

# --
def get_slicing_options(R,C,L,H):
  preliminary_slicing_options = []
  max_row = min(R,H)
  max_col = min(C,H)
  min_size = L + L
  for r in range(1,1+max_row):
    for c in range(1,1+max_col):
      product = r * c
      if product <= H and product >= min_size:
        preliminary_slicing_options.append((r,c))
  if len(preliminary_slicing_options) == 0:
    raise Exception("Slicing is not possible for: R={} C={} L={} H={}".format(str(R),str(C),str(L),str(H)))
  return preliminary_slicing_options

# --
def is_valid_slicing_option(content,r,c,size,R,C,L):
  num_rows = size[0]
  num_cols = size[1]
  num_M = 0
  num_T = 0
  ri = r
  for i in range (num_rows):
    if ri >= R:
      return False
    ci = c
    for j in range (num_cols):
      if ci >= C:
        return False
      if content[ri][ci] == 'M':
        num_M += 1
      elif content[ri][ci] == 'T':
        num_T += 1
      ci+=1
    ri+=1
  if num_M < L:
    return False
  if num_T < L:
    return False
  return True

# --
total_possibilities = 0
def get_slicing_possibilities(R,C,L,H,content,preliminary_slicing_options):
  global total_possibilities
  possibilities = dict()
  for r in range(R):
    for c in range(C):
      possibilities[(r,c)] = []
      for size in preliminary_slicing_options:
        if is_valid_slicing_option(content,r,c,size,R,C,L):
          possibilities[(r,c)].append(size)
          total_possibilities += 1
  return possibilities

# --
if __name__ == "__main__":
  parser = argparse.ArgumentParser(description="Hash Code 2017 practice round. See https://goo.gl/4mtJR3")
  parser.add_argument("input_file", help="input file")
  parser.add_argument("output_file", help="output file")
  args = parser.parse_args()
  with open(args.input_file,'r') as input_file, open(args.output_file,'w') as output_file:
    content = [line.rstrip('\n') for line in input_file]
    if len(content) == 0:
      raise Exception("Input file: should not be empty.")
    if len(content) < 2:
      raise Exception("Input file: should have at least two lines.")
    first_line = content[0]
    tokens = first_line.split(' ')
    if len(tokens) != 4:
      raise Exception("Input file: Expected 4 numbers on first line")
    print("Input file {}: R,C,L,H: {}".format(args.input_file,str(tokens)))
    R = int(tokens[0])
    C = int(tokens[1])
    L = int(tokens[2])
    H = int(tokens[3])
    validate_parameters(R,C,L,H)
    preliminary_slicing_options = get_slicing_options(R,C,L,H)
    # Remove first line containing parameters
    content = content[1:]
    validate_content(R,C,content)
    # Show this only if the parameters=valid and content=valid
    print("Preliminary slicing options for each point: {}".format(str(preliminary_slicing_options)))
    slicing_possibilities = get_slicing_possibilities(R,C,L,H,content,preliminary_slicing_options)
    print("Total number of unique slicing possibilities: {}".format(total_possibilities))
    # Calculate boundaries of slice count
    max_slice_count = min(count_M,count_T) / L
    #min_slice_count = 

