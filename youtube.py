#!/usr/bin/env  python3
#ydl2.py
import os
from bs4 import BeautifulSoup
import re
import requests
import youtube_dl

req = requests.get("https://www.youtube.com/c/SMLMUSIC88/videos")
video = re.sub("(https://i\.ytimg\.com/vi/|/hqdefault\.jpg)", '', re.findall(b"https://i.ytimg.com/vi/.*?\.jpg", req.content)[0].decode())

def my_hook(d):
    if d['status'] == 'finished':
        print('Done downloading, now converting ...')

ydl_opts = {
    'format': 'bestaudio/best',       
    'outtmpl': '%(id)s',        
    'noplaylist' : True,        
    'progress_hooks': [my_hook],  
}

home = os.getenv("HOME")
os.chdir(f"{home}/Music/")
if (len(sys.argv) == 2):
    os.chdir(sys.argv[1])
with youtube_dl.YoutubeDL(ydl_opts) as ydl:
    ydl.download(["https://youtu.be/" + video])
