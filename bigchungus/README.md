# House of big chungus

This repo contains the _pwn_ challenge `House of big chungus`.
This is a heap exploitation challenge based on the `House of force` technique.

# Flag(s) 
`DDC{===WIDE_WILDERNESS===}`

# Domain name (OR) Link to challenge on nextcloud
`chungus.hkn`
https://nextcloud.ntp-event.dk:8443/s/bWMq4Ri6DH2GexJ/download/Hobc.zip

# Description(s) (THIS IS WHAT THE USER WILL SEE, YOU CAN USE MARKDOWN HERE))
ZanderDK once said "I wish we could use the big chungus here",
I still have no clue what he meant.

https://nextcloud.ntp-event.dk:8443/s/bWMq4Ri6DH2GexJ/download/Hobc.zip

`nc chungus.hkn 1024`

# Prerequisites and Outcome

Prerequisites
- Know the house of force technique for heap exploitation.
Outcome
- We know you can use HOF.

# Solutions (FULL WRITEUP FOR EACH FLAG.)
The challenge consists of a single binary, it's source, and libc running on the remote server.

The binary is a janky P3 PPM image format reader.
PPM is an ascii format for images of the following form

```
P3
[sizex] [sizey]
[maxvalue]
[byte1] [byte2] ...
```

Reading the source for the parse one might notice the following statement:
```c
if (image->size > MAX_IMG_SIZE) {
        data = (char*)malloc(MAX_IMG_SIZE+1);
} else {
        data = (char*)malloc(image->size+1);
}
```
here image->size is a signed size. Meaning by suppliying a very large number, we can pass into the else branch with a very big image->size value.
We will then read in the image line by line into the `data` heap object.
By supplying `-1` as the image size, the expression at the malloc size argument will evaluate to 0, essentially allocating only 32 bytes but keeping the `image->size` very large.

This is one vulnerability of integer overflow, and signed comparison.
Next is the overflow that this allows the hacker to perform. Because the heap buffer is now 32 bytes big and the program will read up to `0xffffffffffffffff` bytes in, it will overflow into the top-chunk.

The HOF explains how by crafting a very large top chunk and controlling a later malloc'd size, we can make any subsequent malloc call return an arbitrary pointer.

Let us say we did the basic overflow and made the size of the top chunk `0xfffffffffffffff0` by sending a file such as this:
```
P3
2753074036095 6700417 // this = -1
0
255 255 255 255 255 255 255 255 255 255 255 255
100 100 100 100 100 100 200 200 200 200 200 200
240  255  255  255  255  255  255  255  20  20  20  20
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ This is the size field 
```
We now have a very big top chunk.
The next malloc in the program is here:
```
// Count the bytes
malloc(image->maxval);
```

And we want to allocate up until where we want to write, in my exploit i use the known __free_hook in libc. 
The size we want to pass as `maxval` is then 
```py
size = (free_hook-heap_base)-0x2c0
```
This address is easily bruteforcable when you understand the concept, the 0x2c0 constant i just found locally.

Now the next chunk that malloc will allocate will be at free_hook-8.

```c
char *y = (char*)malloc(255); // This returns __free_hook-8
for (int i = 0; i < image->size; i++) {
    y[data[i]]++;
}
for (int i = 0; i < 255; i++) {
    printf("%d:\t%d", i, y[i]);
}
```
Now it will count each byte in the image and insert into this address.
By eg. supplying `8 8 8` it would set the __free_hook lowest byte to 8.

We can calculate this type of payload simply like this
```
gadget = 0x4f302 # magic one gadget found from libc
gadgetfmt = ""
target = p64(libc_base + gadget)
for i in range(len(target)):
    gadgetfmt += f"{str(i+8)} " * target[i]
```

It will generate something like 
```
8 8 9 9 9 10 10 10 [...] 13 13 13 13 13 
```
Witch will set the address at __free_hook to the target value (in this case a one gadget).
When the final free is called, it will invoke the __free_hook and in this case drop us a shell.
The full POC is found in `solve.py`
It works 80% of the time.