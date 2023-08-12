# video-to-gif
a video to gif converter written in c
parseMPEG-4.c parses mp4 files according to the (iso MPEG4) specification.

This project is in progress.

Project Status: 
- Extracting structures from the MPEG-4 file format is currently supported and working. 
- Need to extract information out of NAL units in samples for AVC format
- Need to make a gif encoder
- Need to convert AVC format output to gif input

mpeg atoms/boxes that can support 64 bit sizes in future: 
- general box atoms when they have their size set to 1
- stco can contian 64 bit chunk offsets