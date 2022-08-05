# 3x17
### this binary allow us to write to any address but problem is that where to write so that it will give control over binary
In this level I can't able to find where to write so that it will
give control to so I decide to read some elses writeup and then
I figured out that.init_array, and .fini_array which are two arrays containing 
information for constructor and destructor for the binary. 

then I thought of searching for .init_array, and .fini_array in section headers
with readelf -S 3x17 and I found out both.
```bash
readelf -S 3x17 | grep fini_array
  [16] .fini_array       FINI_ARRAY       0000k0000004b40f0  000b30f0
```
