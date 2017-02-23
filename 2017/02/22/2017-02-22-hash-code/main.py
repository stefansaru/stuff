#!/usr/bin/env python

# using python 2.7
import sys, argparse, itertools

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
def get_meta_info(slicing_possibilities):
  slice_head_list = []
  slice_index_list = []

  slice_head_entry = (None,None)
  slice_indexes = []

  for head, sizes in slicing_possibilities.items():
    if len(sizes) == 0: 
      continue

    slice_head_entry = head
    slice_indexes = range(-1,len(sizes))

    slice_head_list.append(slice_head_entry)
    slice_index_list.append(slice_indexes)
  
  # Here we generate all available permutations  
  index_permutations = list(itertools.product(*slice_index_list))

  return slice_head_list, index_permutations

def apply_single_permutation_square(slice_head_position, slice_slicing_variant_data, occupied, content, L):
  # head position must be vacant (not occupied == True)
  r = slice_head_position[0]
  c = slice_head_position[1]

  if occupied[r][c] == True:
    can_be_done = False
    return can_be_done, occupied,0

  # look for all of the slice's squares, and make sure they are vacant (not occupied == True)
  num_rows = slice_slicing_variant_data[0]
  num_cols = slice_slicing_variant_data[1]
  num_M = 0
  num_T = 0
  slice_squares = []

  ri = r
  for i in range (num_rows):
    ci = c
    for j in range (num_cols):
      if occupied[ri][ci] == True:
        can_be_done = False
        return can_be_done, occupied,0
      if content[ri][ci] == 'M':
        num_M+=1
      elif content[ri][ci] == 'T':
        num_T+=1
      slice_squares.append((ri,ci))
      ci+=1
    ri+=1

  # it can be done, since all required squares are vacant
  # now check if there are enough toppings
  if num_M < L or num_T < L:
    can_be_done = False
    score = 0
    return can_be_done, occupied, score

  # the squares are vacant and there are enough toppings
  # now mark the slice, by occupying its squares
  for square in slice_squares:
    ri = square[0]
    ci = square[1]
    occupied[ri][ci] = True

  can_be_done = True
  score = num_rows * num_cols
  return can_be_done, occupied, score

# --
def do_slicing_from_indexes(indexes_permutation, slice_head_list, slicing_possibilities, content, L):
  solution = []
  total_score = 0
  # Generate initial unoccupied map
  occupied = [[False for i in range(C)] for j in range(R)]
  #print ("indexes_permutation: {}".format(indexes_permutation))
  #print ("slice_head_list: {}".format(slice_head_list))
  #print ("content: {}".format(content))
  #print ("occupied: {}".format(occupied))


  for i in range(len(indexes_permutation)):
    #print ("Analyzing slice {}/{}".format(i,len(indexes_permutation)))
    slice_head_position = slice_head_list[i]
    #print ("Head position is {}".format(slice_head_position))
    slice_slicing_variant_index = indexes_permutation[i]
    #print ("Variant index is {}".format(slice_slicing_variant_index))

    # -1 means do not make this slice
    if slice_slicing_variant_index == -1:
      continue
    
    slice_slicing_variant_data = slicing_possibilities[slice_head_position][slice_slicing_variant_index]
    #print ("slice_slicing_variant_data={}".format(slice_slicing_variant_data))
    # apply individual square permutation
    can_be_done, occupied, score = apply_single_permutation_square(slice_head_position, slice_slicing_variant_data, occupied, content, L)
    # if this permutation can't be done, stop
    if can_be_done == False:
      solution = None
      total_score = 0
      return can_be_done, solution, total_score
    # If this permutation can be done, record the steps and update score
    solution.append((slice_head_position,slice_slicing_variant_data))
    total_score += score

  if total_score < 1:
    # a score of 0 means can't be done, since no slice was placed
    can_be_done = False
    #print("Permutation can't be done. No slice placed")
  else:
    #print("Permutation is valid, score={}".format(total_score))
    # This was a valid permutation
    can_be_done = True
  return can_be_done, solution, total_score
    
# --
if __name__ == "__main__":
  parser = argparse.ArgumentParser(description="Hash Code 2017 practice round. See https://goo.gl/4mtJR3 ")
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
    print("Input file: {}\nR,C,L,H: {}".format(args.input_file,str(tokens)))
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
    print("Maximum number of slices: {}".format(max_slice_count))
    print("Calculating how many ways can the slicing be done. (This may take a while)")
    # Calculate meta information required for permutations
    slice_head_list, indexes_permutations = get_meta_info(slicing_possibilities)
    # 
    print("Slicing can be done starting from {} points, in {} different ways".format(len(slice_head_list), len(indexes_permutations)))
    print("Searching for solutions. (This may take a while)")
    max_score = -1
    best_solution = None
    num_solutions = 0
    #x = 0
    # Iterate each permutation
    for indexes_permutation in indexes_permutations:
      # Apply permutation to the map
      can_be_done, solution, score = do_slicing_from_indexes(indexes_permutation, slice_head_list, slicing_possibilities, content, L)
      # Validate..
      if can_be_done == True:
        num_solutions += 1
        if score > max_score: 
          max_score = score
          best_solution = solution
          
      # Try next permutation, this variation causes slice overlap
      #x+=1
      #if x >2: sys.exit(0)

    # How many solutions found?
    print("Found a total of {} solutions.".format(num_solutions))
    print("Best solution score: {}".format(max_score))
    #print("Best solution: {}".format(best_solution))
    num_slices_best_solution = len(best_solution)
    print("Best solution slice count: {}".format(num_slices_best_solution))
    output_file.write("{}\n".format(num_slices_best_solution))
    for slice_item in best_solution:
      #print("slice_item: {}".format(slice_item))
      row_start = slice_item[0][0]
      col_start = slice_item[0][1]
      row_end = slice_item[1][0] + row_start - 1
      col_end = slice_item[1][1] + col_start - 1
      #print("row_start: {}".format(row_start))
      #print("row_end: {}".format(row_end))
      #print("col_start: {}".format(col_start))
      #print("col_end: {}".format(col_end))
      output_file.write("{} {} {} {}\n".format(row_start, col_start, row_end, col_end)) 
      
    print("Output file: Solution written according to format in {}".format (args.output_file))

  # --
  print("Done")
