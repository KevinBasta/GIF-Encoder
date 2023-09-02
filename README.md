# video-to-gif
A Video (MPEG-4 with sample data stored in AVC format) to GIF (Graphics Interchange Format) converter written in the c programming language. The goal of this project is to parse MP4 files according to the (ISO/IEC MPEG4)/(QuickTime) specification, parse AVC data out of the MPEG4 samples accoriding to the (ISO/IEC AVC) specification, transform the parsed MPEG-4/AVC data into GIF data, and use that data to encode GIFs according to the (GIF 89a Version) specification.

### This project is in progress.

### Project Status: 
- MPEG-4 parsing, decoding, and processing is currently supported and working 
- GIF full encoding almost complete, basic support complete
- AVC decoding incomplete, need to extract information out of NAL units in MPEG-4 samples
- Need to convert AVC format output to gif input


### Obligatory Copyright Information For File Formats
> The Graphics Interchange Format(c) is the Copyright property of CompuServe Incorporated.