#!/usr/bin/env python
import sys, os

# http://stackoverflow.com/questions/19268548/python-ignore-certicate-validation-urllib2
import ssl
unsafe_context = ssl.create_default_context()
unsafe_context.check_hostname = False
unsafe_context.verify_mode = ssl.CERT_NONE
 
# http://stackoverflow.com/questions/24346872/python-equivalent-of-a-given-wget-command
def get_url_as_string(url_target, timeout_seconds = 2, num_retries = 2):
  import urllib2
  global unsafe_context, visited_urls
  while num_retries > 0:
    num_retries -= 1
    print("download_url(url_target=[{}]): Starting...".format(url_target))
    try:
      response = urllib2.urlopen(url_target, timeout = timeout_seconds, context = unsafe_context)
      webpage_contents = response.read()
      print("download_url(url_target=[{}]): Finished!".format(url_target))
      visited_urls[url_target] = True
      return webpage_contents
    except:
      print("download_url(url_target=[{}]): Error! Retries: {}".format(url_target,num_retries))
  visited_urls[url_target] = False
  return ""

# -- already visited ['URL'] = True/False
visited_urls = dict()

# --
def get_links_from_webpage(url_target,
    regex_match_url = r'http[s]?://(?:[a-zA-Z]|[0-9]|[$-_@.&+]|[!*\(\),]|(?:%[0-9a-fA-F][0-9a-fA-F]))+',
    regex_match_extension =  r".*\.(xls|ppt|doc|docx|pdf|tiff|wav|mp3|mpeg|avi|flac|jpg|png|gif)$"):
  global visited_urls
  import re

  webpage_contents = get_url_as_string(url_target)
  webpage_urls = re.findall(regex_match_url,webpage_contents)
  webpage_urls = list(set(webpage_urls))

  not_ending_in_extension = lambda x: re.match(regex_match_extension,x.lower()) is None
  webpage_urls = filter(not_ending_in_extension, webpage_urls)

  not_visited = lambda x: (x not in visited_urls)
  webpage_urls = filter(not_visited, webpage_urls)
  
  print("get_links_from_webpage(url_target=[{}]) Detected urls: {}".format(url_target,len(webpage_urls)))
  return webpage_urls

def crawl(url_target,depth_now=1,depth_max=2):
  global urls
  if depth_now > depth_max:
    return []
  print ("crawl(url_target=[{}], depth_now=[{}], depth_max=[{}])".format(url_target, str(depth_now), str(depth_max)))
  urls = get_links_from_webpage(url_target)
  urls = list(set(urls))
  for u in urls:
    new_depth = depth_now + 1
    urls += crawl(u, depth_now = new_depth)
    urls = list(set(urls))
  return urls

if len(sys.argv) < 4:
  print ('Expected arguments: [start url], [output file] and [depth].')
  print ('Example: {} http://example.com out.txt 2'.format(sys.argv[0]))
  sys.exit(1)

start_link = sys.argv[1]
out_file = sys.argv[2]
depth = sys.argv[3]

with open(out_file,'w') as out:
  urls = crawl(start_link,depth_max = int(depth))
  for u in urls:
    out.write(u + os.linesep)
  print ('Done. File written: {}'.format(out_file))
