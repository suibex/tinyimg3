# tinyimg3
tinyimg3 is a tiny ARM Versatile Platform Baseboard kernel that uses firmware that is something like IMG3 format.
<h1>Motivation</h1>
<p>I always loved doing low-level programming, hardware programming...</p>
<p>Also i've been always kind of intrigued of the iPhone 2G design choices, and OS-dev.</p>

<h1>IMG3 File format</h1>

<p>All numbers are stored in little endian byte order; this includes the magic numbers (magic and ident).</p>
    typedef struct img3File {
        uint32_t magic;       // ASCII_LE("Img3")
        uint32_t fullSize;    // full size of fw image
        uint32_t sizeNoPack;  // size of fw image without header
        uint32_t sigCheckArea;// although that is just my name for it, this is the
                              // size of the start of the data section (the code) up to
                              // the start of the RSA signature (SHSH section)
        uint32_t ident;       // identifier of image, used when bootrom is parsing images
                              // list to find LLB (illb), LLB parsing it to find iBoot (ibot),
                              // etc. 
        img3Tag  tags[];      // continues until end of file
    };
    typedef struct img3Tag {
        uint32_t magic;            // see below
        uint32_t totalLength;      // length of tag including "magic" and these two length values
        uint32_t dataLength;       // length of tag data
        uint8_t  data[dataLength];
        uint8_t  pad[totalLength - dataLength - 12]; // Typically padded to 4 byte multiple
    };

<br>

<h1>What does this kernel do?</h1>
<p>First, it downloads "custom" firmware and writes it to NOR flash, then it uses PL110 CLCD component to display gradient on the screen.</p>
<img src="https://github.com/suibex/tinyimg3/assets/59802817/98ed4902-4eda-4370-b260-4e5fc8500783">
