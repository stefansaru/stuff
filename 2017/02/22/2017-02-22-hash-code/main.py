#!/usr/bin/env python

# using python 2.7
import sys, argparse

def validate_parameters(R,C,L,H):
  for item, name in [(R,'R'),(C,'C'),(L,'L'),(H,'H')]:
    if item < 1 or item > 1000:
      raise Exception("{}={} (expected 1<={}<=1000)".format(name,str(item),name))

def get_slicing_options(R,C,L,H):
  sizes = []
  max_row = min(R,H)
  max_col = min(C,H)
  min_size = L + L
  for r in range(1,1+max_row):
    for c in range(1,1+max_col):
      product = r * c
      if product <= H and product >= min_size:
        sizes.append((r,c))
        print "{}*{}={} <= {}".format(str(r),str(c),str(r*c),str(H))
  if len(sizes) == 0:
    raise Exception("Slicing is not possible for R={} C={} L={} H={}".format(str(R),str(C),str(L),str(H)))
  return sizes

if __name__ == "__main__":
  parser = argparse.ArgumentParser(description="Hash Code 2017 practice round. See https://goo.gl/4mtJR3")
  parser.add_argument("input_file", help="input file")
  parser.add_argument("output_file", help="output file")
  args = parser.parse_args()
  with open(args.input_file,'r') as input_file, open(args.output_file,'w') as output_file:
    content = [line.rstrip('\n') for line in input_file]
    if len(content) == 0:
      raise Exception("Input file should not be empty.")
    if len(content) < 2:
      raise Exception("Input file should have at least two lines.")
    first_line = content[0]
    tokens = first_line.split(' ')
    if len(tokens) != 4:
      raise Exception("Expected 4 numbers on first line")
    print("Input file R,C,L,H = {}".format(str(tokens)))
    R = int(tokens[0])
    C = int(tokens[1])
    L = int(tokens[2])
    H = int(tokens[3])
    validate_parameters(R,C,L,H)
    slicing_options = get_slicing_options(R,C,L,H)
    print("Slicing options are: {}".format(str(slicing_options)))

