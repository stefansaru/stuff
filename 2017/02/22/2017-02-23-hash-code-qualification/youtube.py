#!/usr/bin/env python
# use python 3!! ^^^^

import argparse, itertools, yaml

# --
def debug(D):
  return yaml.dump(D, allow_unicode=True, default_flow_style=False)
  
# --
def debug_and_exit(D):
  print(debug(D))
  import sys
  sys.exit(0)

# --
def parse_args(D):
  arg_parser = argparse.ArgumentParser(description="Hash Code 2017 qualification round")
  arg_parser.add_argument("input_file")
  arg_parser.add_argument("output_file")
  parsed_args = arg_parser.parse_args()
  D['input file'] = parsed_args.input_file
  D['output file'] = parsed_args.output_file
  return D

# --
def load_input(D):
  with open (D['input file'],'r') as input_file:
    input_lines = input_file.read().splitlines()
    # --
    line_tokens = list(map(int, input_lines[0].split(' ')))
    V = line_tokens[0] # V: the number of videos
    E = line_tokens[1] # E: the number of endpoints
    R = line_tokens[2] # R: the number of request descriptions
    C = line_tokens[3] # C: the number of cache servers
    X = line_tokens[4] # X: the capacity of each cache server in megabytes
    # --
    input_lines = input_lines[1:]
    line_tokens = input_lines[0].split(' ')
    S = list(map(int,line_tokens)) # S: the sizes of individual videos
    D['videos'] = dict()
    D['videos']['count'] = V
    for video_id, video_size in zip(range(V), S):
      D['videos'][video_id] = dict()
      D['videos'][video_id]['size'] = video_size
      D['videos'][video_id]['best latency if stored in'] = -1 # -1 = datacenter, 0..C = cache server id
      D['videos'][video_id]['total delay if stored in'] = dict()
      D['videos'][video_id]['total delay if stored in']['datacenter'] = 0
      D['videos'][video_id]['requests from endpoint'] = dict()
    # --
    D['cache servers'] = dict()
    D['cache servers']['count'] = C
    D['cache servers']['capacity'] = X
    for cache_server_id in range(C):
      D['cache servers'][cache_server_id] = dict()
      D['cache servers'][cache_server_id]['endpoints'] = dict()
      D['cache servers'][cache_server_id]['endpoints']['count'] = 0
      D['cache servers'][cache_server_id]['endpoints']['latency'] = dict()
      for video_id in range(V):
        D['videos'][video_id]['total delay if stored in'][cache_server_id] = 0
      
    D['endpoints'] = dict()
    D['endpoints']['count'] = E
    for endpoint_id in range(E):
      input_lines = input_lines[1:]
      line_tokens = list(map(int, input_lines[0].split(' ')))
      datacenter_latency = line_tokens[0]
      cache_servers_count = line_tokens[1]
      D['endpoints'][endpoint_id] = dict()
      D['endpoints'][endpoint_id]['datacenter latency'] = datacenter_latency
      D['endpoints'][endpoint_id]['cache servers'] = dict()
      D['endpoints'][endpoint_id]['cache servers']['count'] = cache_servers_count
      D['endpoints'][endpoint_id]['cache servers']['latency'] = dict()
      for video_id in range(V):
        D['videos'][video_id]['requests from endpoint'][endpoint_id] = 0
      for _ in range(cache_servers_count):
        input_lines = input_lines[1:]
        line_tokens = list(map(int, input_lines[0].split(' ')))
        cache_server_id = line_tokens[0]
        cache_server_latency = line_tokens[1]
        D['endpoints'][endpoint_id]['cache servers']['latency'][cache_server_id] = cache_server_latency
        D['cache servers'][cache_server_id]['endpoints']['latency'][endpoint_id] = cache_server_latency
        D['cache servers'][cache_server_id]['endpoints']['count'] += 1
    # --
    D['requests'] = dict()
    D['requests']['from endpoint'] = dict()
    D['requests']['for video'] = dict()
    for index_request in range(R):
      input_lines = input_lines[1:]
      line_tokens = list(map(int, input_lines[0].split(' ')))
      video_id = line_tokens[0]
      endpoint_id = line_tokens[1]
      requests_count = line_tokens[2]
      D['requests']['from endpoint'][endpoint_id] = dict()
      D['requests']['from endpoint'][endpoint_id]['for video'] = dict()
      D['requests']['from endpoint'][endpoint_id]['for video'][video_id] = requests_count
      D['requests']['for video'][video_id] = dict()
      D['requests']['for video'][video_id]['from endpoint'] = dict()
      D['requests']['for video'][video_id]['from endpoint'][endpoint_id] = requests_count
      D['videos'][video_id]['requests from endpoint'][endpoint_id] += requests_count
      for cache_server_id in range(C):
        latency_datacenter = D['endpoints'][endpoint_id]['datacenter latency']
        D['videos'][video_id]['total delay if stored in']['datacenter'] += latency_datacenter * requests_count
        try:
          latency_cache = D['endpoints'][endpoint_id]['cache servers']['latency'][cache_server_id]
          D['videos'][video_id]['total delay if stored in'][cache_server_id] += latency_cache * requests_count
        except:
          print ('cumplm')
          pass # no connection to cache
        
  return D

# --
def analyze_latency(D):
  for video_id in range(D['videos']['count']):
    d = D['videos'][video_id]['total delay if stored in']
    k = min(d, key=d.get)
    print ("video {} best stored in {} impact {}".format(video_id,k,d[k]))
  return D
def save_output(D):
  with open (D['output file'],'w') as output_file:
    output_file.write(debug(D))
# --
def main():
  D = dict()
  D = parse_args(D)
  D = load_input(D)
  D = analyze_latency(D)
  save_output(D)
  #debug_and_exit(D)

# --
if __name__ == "__main__":
  main()