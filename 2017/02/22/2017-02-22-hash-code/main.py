#!/usr/bin/env ddddddddddddddddddddddpython

import sys

R = int(sys.argv[1])
C = int(sys.argv[2])
L = int(sys.argv[3])
H = int(sys.argv[4])

def validate_parameters(R,C,L,H):
  for item, name in [(R,'R'),(C,'C'),(L,'L'),(H,'H')]:
    if item < 1 or item > 1000:
      raise Exception("{}={} (expected 1<={}<=1000)".format(name,str(item),name))

def get_slice_sizes(R,C,L,H):
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

validate_parameters(R,C,L,H)
print get_slice_sizes(R,C,L,H)
