#include <iostream>
#include<highgui.h>
#include<cv.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>  
#include <string>
#include"conio.h"

using namespace std;
using namespace cv;

#define IMAGE_BMP_H
#define dataImage(img, x, y) ((uchar*)(img)->imageData)[(y) * (img)->widthStep + (x) * (img)->nChannels]

namespace Image
{
    class Bmp
    {
    public:
        // ctor/dtor
        Bmp();
        Bmp(const Bmp &rhs);
        ~Bmp();

        Bmp& operator=(const Bmp &rhs);             // assignment operator

        // load image header and data from a bmp file
        bool read(const char* fileName);

        // save an image as BMP format
        // It assumes the color order of input image is RGB, so it will convert to BGR order before save
        bool save(const char* fileName, int width, int height, int channelCount, const unsigned char* data);

        // getters
        int getWidth() const;                       // return width of image in pixel
        int getHeight() const;                      // return height of image in pixel
        int getBitCount() const;                    // return the number of bits per pixel (8, 24, or 32)
        int getDataSize() const;                    // return data size in bytes
        const unsigned char* getData() const;       // return the pointer to image data
        const unsigned char* getDataRGB() const;    // return image data as RGB order

        void printSelf() const;                     // print itself for debug purpose
        const char* getError() const;               // return last error message
        IplImage* loadImage(const char *FILE_NAME);
        
    protected:


    private:
        // member functions
        void init();                                // clear the existing values

        // shared functions (only 1 copy of the function, even if there are multiple instances of this class)
        static bool decodeRLE8(const unsigned char *encData, unsigned char *data);              // decode BMP 8-bit RLE to uncompressed
        static void flipImage(unsigned char *data, int width, int height, int channelCount);    // flip the vertical orientation
        static void swapRedBlue(unsigned char *data, int dataSize, int channelCount);           // swap the position of red and blue components
        static int  getColorCount(const unsigned char *data, int dataSize);                     // get the number of colors used in 8-bit grayscale image
        static void buildGrayScalePalette(unsigned char *palette, int paletteSize);

        // member variables
        int width;
        int height;
        int bitCount;
        int dataSize;
        unsigned char *data;                        // data with default BGR order
        unsigned char *dataRGB;                     // extra copy of image data with RGB order
        std::string errorMessage;
    };

    ///////////////////////////////////////////////////////////////////////////
    // inline functions
    ///////////////////////////////////////////////////////////////////////////
    inline int Bmp::getWidth() const { return width; }
    inline int Bmp::getHeight() const { return height; }

    // return bits per pixel, 8 means grayscale, 24 means RGB color, 32 means RGBA
    inline int Bmp::getBitCount() const { return bitCount; }

    inline int Bmp::getDataSize() const { return dataSize; }
    inline const unsigned char* Bmp::getData() const { return data; }
    inline const unsigned char* Bmp::getDataRGB() const { return dataRGB; }

    inline const char* Bmp::getError() const { return errorMessage.c_str(); }
    
    /************************************
	////////////this is class using improcess image simple.
	detail: convert rgb to gray 
	        convert rgb to binary.
	        zoom out image
	        zoom in image
	************************************/
	
	class improcessImage{
		private:
			IplImage* img;
			void langgieng(IplImage *img, int x, int y, int A[]);
			int dem(int A[]);
			int SoGiaoXR(int A[]);
			int ketnoi(int A[]);
	  
		public:
			improcessImage();
			improcessImage(IplImage* image);
		    void grayScale(IplImage* image, IplImage* grayImage);
		    void binaryImage(IplImage* image,IplImage* binary,int nguong);
		    void dilateImage(IplImage* binary,IplImage* dilateImage);
		    void erodeImage(IplImage* binary,IplImage* dilateImage);
		    void Stentiford(IplImage *img);
		    void openImage(IplImage* binary);
		    void closeImage(IplImage* binary);
	};
}

int main(int argc, char** argv) {
	
	const   char *FILE_NAME = "lena.bmp";
	char c=0;
        
	cout<<"\n 0. Load image\n";
	cout<<" 1. Convert RGB to gray\n";
	cout<<" 2. Convert RGB to binary\n";
	cout<<" 3. Dilate algorithm\n";
	cout<<" 4. Erode algorithm\n";
	cout<<" 5. Thining algorithm\n";
	cout<<" 6. open image algorithm\n";
	cout<<" 7. close image algorithm\n";
	cout<<" 8. exit\n";
	cout<<"\n Chu y: Bam X anh hien thi de tro ve menu\n";

	cout<<"\n Moi nhap cac so theo chuc nang tren: ";

	while( c!='8')
	{
		c=getch();
		//cout<<"\n"<<c;

		Image::Bmp bmp; 

		// load BMP image
		if(!bmp.read(FILE_NAME))
			return 0;     // exit if failed load image
    
		IplImage* img;

		img=bmp.loadImage(FILE_NAME);
	
		int     imageX, imageY; 
		imageX=img->width;
		imageY=img->height;
	
		IplImage* grayImage=0;
		IplImage* binary;
		IplImage* imageDilate;
		IplImage* imageErode;
		IplImage* thiningImage;
	
		IplImage* imageOpen;
		IplImage* imageClose;
		
		grayImage=cvCreateImage(cvSize(imageX,imageY),8,1);
		binary=cvCreateImage(cvSize(imageX,imageY),8,1);
		imageDilate=cvCreateImage(cvSize(imageX,imageY),8,1);
		imageErode=cvCreateImage(cvSize(imageX,imageY),8,1);
		thiningImage=cvCreateImage(cvSize(imageX,imageY),8,1);
	
		imageOpen=cvCreateImage(cvSize(imageX,imageY),8,1);
		imageClose=cvCreateImage(cvSize(imageX,imageY),8,1);
	
		Image::improcessImage input;
	
		input.grayScale(img,grayImage);
		input.binaryImage(img,binary,100);
	
		cvCopyImage(binary,thiningImage);
		cvCopyImage(binary,imageOpen);
		cvCopyImage(binary,imageClose);
	    
		if( c=='0') cvShowImage("helloWorld",img);
		if( c=='1') cvShowImage("gray image",grayImage);
		if( c=='2') cvShowImage("bianry image",binary);
	
		if( c=='3' ) 
		{
			input.dilateImage(binary,imageDilate);
			cvShowImage("bianry image",binary);
			cvShowImage("dilate image",imageDilate);
		}

		if( c=='4' ) 
		{
			input.erodeImage(binary,imageErode);
			cvShowImage("bianry image",binary);
			cvShowImage("erode image",imageErode);
		}

		if( c=='5' )
		{
			input.Stentiford(thiningImage);
			cvShowImage("bianry image",binary);
			cvShowImage("thinning image",thiningImage);
		}

		if( c=='6' )
		{
			input.openImage(imageOpen);
			cvShowImage("bianry image",binary);
			cvShowImage("open image",imageOpen);
		}

		if( c=='7' )
		{
			input.closeImage(imageClose);
			cvShowImage("bianry image",binary);
			cvShowImage("close image",imageClose);
		}

		cout<<"\n Successed\n"; 
		cvWaitKey(0);
	}
	
	return 0;
}

using namespace Image;

///////////////////////////////////////////////////////////////////////////////
// default constructor
///////////////////////////////////////////////////////////////////////////////
Bmp::Bmp() : width(0), height(0), bitCount(0), dataSize(0), data(0), dataRGB(0),
             errorMessage("No error.")
{
}


///////////////////////////////////////////////////////////////////////////////
// copy constructor
// We need DEEP COPY for dynamic memory variables because the compiler inserts
// default copy constructor automatically for you, BUT it is only SHALLOW COPY
///////////////////////////////////////////////////////////////////////////////
Bmp::Bmp(const Bmp &rhs)
{
    // copy member variables from right-hand-side object
    width = rhs.getWidth();
    height = rhs.getHeight();
    bitCount = rhs.getBitCount();
    dataSize = rhs.getDataSize();
    errorMessage = rhs.getError();

    if(rhs.getData())       // allocate memory only if the pointer is not NULL
    {
        data = new unsigned char[dataSize];
        memcpy(data, rhs.getData(), dataSize); // deep copy
    }
    else
        data = 0;           // array is not allocated yet, set to 0

    if(rhs.getDataRGB())    // allocate memory only if the pointer is not NULL
    {
        dataRGB = new unsigned char[dataSize];
        memcpy(dataRGB, rhs.getDataRGB(), dataSize); // deep copy
    }
    else
        dataRGB = 0;        // array is not allocated yet, set to 0
}



///////////////////////////////////////////////////////////////////////////////
// default destructor
///////////////////////////////////////////////////////////////////////////////
Bmp::~Bmp()
{
    // deallocate data array
    delete [] data;
    data = 0;
    delete [] dataRGB;
    dataRGB = 0;
}



///////////////////////////////////////////////////////////////////////////////
// override assignment operator
///////////////////////////////////////////////////////////////////////////////
Bmp& Bmp::operator=(const Bmp &rhs)
{
    if(this == &rhs)        // avoid self-assignment (A = A)
        return *this;

    // copy member variables
    width = rhs.getWidth();
    height = rhs.getHeight();
    bitCount = rhs.getBitCount();
    dataSize = rhs.getDataSize();
    errorMessage = rhs.getError();

    if(rhs.getData())       // allocate memory only if the pointer is not NULL
    {
        data = new unsigned char[dataSize];
        memcpy(data, rhs.getData(), dataSize);
    }
    else
        data = 0;

    if(rhs.getDataRGB())   // allocate memory only if the pointer is not NULL
    {
        dataRGB = new unsigned char[dataSize];
        memcpy(dataRGB, rhs.getDataRGB(), dataSize);
    }
    else
        dataRGB = 0;

    return *this;
}



///////////////////////////////////////////////////////////////////////////////
// clear out the exsiting values
///////////////////////////////////////////////////////////////////////////////
void Bmp::init()
{
    width = height = bitCount = dataSize = 0;
    errorMessage = "No error.";

    delete [] data;
    data = 0;
    delete [] dataRGB;
    dataRGB = 0;
}



///////////////////////////////////////////////////////////////////////////////
// print itself for debug
///////////////////////////////////////////////////////////////////////////////
void Bmp::printSelf() const
{
    cout << "===== Bmp =====\n"
         << "Width: " << width << " pixels\n"
         << "Height: " << height << " pixels\n"
         << "Bit Count: " << bitCount << " bits\n"
         << "Data Size: " << dataSize  << " bytes\n"
         << endl;
}



///////////////////////////////////////////////////////////////////////////////
// read a BMP image header infos and datafile and load
///////////////////////////////////////////////////////////////////////////////
bool Bmp::read(const char* fileName)
{
    this->init();   // clear out all values

    // check NULL pointer
    if(!fileName)
    {
        errorMessage = "File name is not defined (NULL pointer).";
        return false;
    }

    // open a BMP file as binary mode
    ifstream inFile;
    inFile.open(fileName, ios::binary);         // binary mode
    if(!inFile.good())
    {
        errorMessage = "Failed to open a BMP file to read.";
        return false;            // exit if failed
    }

    // list of entries in BMP header
    char id[2];             // magic identifier "BM" (2 bytes)
    int fileSize;           // file size in bytes (4)
    short reserved1;        // reserved 1 (2)
    short reserved2;        // reserved 2 (2)
    int dataOffset;         // starting offset of bitmap data (4)
    int infoHeaderSize;     // info header size (4)
    int width;              // image width (4)
    int height;             // image height (4)
    short planeCount;       // # of planes (2)
    short bitCount;         // # of bits per pixel (2)
    int compression;        // compression mode (4)
    int dataSizeWithPaddings; // bitmap data size with paddings in bytes (4)
    //int xResolution;        // horizontal pixels per metre (4)
    //int yResolution;        // vertical pixels per metre (4)
    //int colorCount;         // # of colours used (4)
    //int importantColorCount;// # of important colours (4)

    // read BMP header infos
    inFile.read(id, 2);                         // should be "BM"
    inFile.read((char*)&fileSize, 4);           // should be same as file size
    inFile.read((char*)&reserved1, 2);          // should be 0
    inFile.read((char*)&reserved2, 2);          // should be 0
    inFile.read((char*)&dataOffset, 4);
    inFile.read((char*)&infoHeaderSize, 4);     // should be 40
    inFile.read((char*)&width, 4);
    inFile.read((char*)&height, 4);
    inFile.read((char*)&planeCount, 2);         // should be 1
    inFile.read((char*)&bitCount, 2);           // 1, 4, 8, 24, or 32
    inFile.read((char*)&compression, 4);        // 0(uncompressed), 1(8-bit RLE), 2(4-bit RLE), 3(RGB with mask)
    inFile.read((char*)&dataSizeWithPaddings, 4);
    //inFile.read((char*)&xResolution, 4);
    //inFile.read((char*)&yResolution, 4);
    //inFile.read((char*)&colorCount, 4);
    //inFile.read((char*)&importantColorCount, 4);

    // check magic ID, "BM"
    if(id[0] != 'B' && id[1] != 'M')
    {
        // it is not BMP file, close the opened file and exit
        inFile.close();
        errorMessage = "Magic ID is invalid.";
        return false;
    }

    // it supports only 8-bit grayscale, 24-bit BGR or 32-bit BGRA
    if(bitCount < 8)
    {
        inFile.close();
        errorMessage = "Unsupported format.";
        return false;
    }

    // it supports only uncompressed and 8-bit RLE compressed format
    if(compression > 1)
    {
        inFile.close();
        errorMessage = "Unsupported compression mode.";
        return false;
    }

    // do not trust the file size in header, recalculate it
    inFile.seekg(0, ios::end);
    fileSize = inFile.tellg();

    // compute the number of paddings
    // In BMP, each scanline must be divisible evenly by 4.
    // If not divisible by 4, then each line adds
    // extra paddings. So it can be divided evenly by 4.
    int paddings = (4 - ((width * bitCount / 8) % 4)) % 4;

    // compute data size without paddings
    int dataSize = width * height * bitCount / 8;

    // recompute data size with paddings (do not trust the data size in header)
    dataSizeWithPaddings = fileSize - dataOffset;   // it maybe greater than "dataSize+(height*paddings)" because 4-byte boundary for file size

    // now it is ready to store info and image data
    this->width = width;
    this->height = height;
    this->bitCount = bitCount;
    this->dataSize = dataSize;

    // allocate data arrays
    // add extra bytes for paddings if width is not divisible by 4
    data = new unsigned char [dataSizeWithPaddings];
    dataRGB = new unsigned char [dataSize];

    if(compression == 0)                    // uncompressed
    {
        inFile.seekg(dataOffset, ios::beg); // move cursor to the starting position of data
        inFile.read((char*)data, dataSizeWithPaddings);
    }
    else if(compression == 1)               // 8-bit RLE(Run Length Encode) compressed
    {
        // get length of encoded data
        int size = fileSize - dataOffset;

        // allocate tmp array to store the encoded data
        unsigned char *encData = new unsigned char[size];

        // read data from file
        inFile.seekg(dataOffset, ios::beg);
        inFile.read((char*)encData, size);

        // decode RLE into image data buffer
        decodeRLE8(encData, data);

        // deallocate encoded data buffer after decoding
        delete [] encData;
    }

    // close it after reading
    inFile.close();

    // we don't need paddings, trim paddings from each line
    // Note that there is no padding in RLE compressed data
    if(compression == 0 && paddings > 0)
    {
        int lineWidth = width * bitCount / 8;

        // copy line by line
        for(int i = 1; i < height; ++i)
        {
            memcpy(&data[i*lineWidth], &data[i*(lineWidth+paddings)], lineWidth);
        }
    }

    // BMP is bottom-to-top orientation by default, flip image vertically
    // But if the height is negative value, then it is top-to-bottom orientation.
    if(height > 0)
        flipImage(data, width, height, bitCount/8);

    // the colour components order of BMP image is BGR
    // convert image data to RGB order for convenience
    memcpy(dataRGB, data, dataSize);    // copy data to dataRGB first
    if(bitCount == 24 || bitCount == 32)
        swapRedBlue(dataRGB, dataSize, bitCount/8);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// save an image as an uncompressed BMP format
// We assume the source image is RGB order, so it must be converted BGR order.
///////////////////////////////////////////////////////////////////////////////
bool Bmp::save(const char* fileName, int w, int h, int channelCount, const unsigned char* data)
{
    // reset error message
    errorMessage = "No error.";

    if(!fileName || !data)
    {
        errorMessage = "File name is not specified (NULL pointer).";
        return false;
    }

    if(w == 0 || h == 0)
    {
        errorMessage = "Zero width or height.";
        return false;
    }

    // list of entries in BMP header
    char id[2];             // magic identifier "BM" (2 bytes)
    int fileSize;           // file size in bytes (4)
    short reserved1;        // reserved 1 (2)
    short reserved2;        // reserved 2 (2)
    int dataOffset;         // starting offset of bitmap data (4)
    int infoHeaderSize;     // info header size (4)
    int width;              // image width (4)
    int height;             // image height (4)
    short planeCount;       // # of planes (2)
    short bitCount;         // # of bits per pixel (2)
    int compression;        // compression mode (4)
    int dataSizeWithPaddings; // bitmap data size in bytes with padding (4)
    int xResolution;        // horizontal pixels per metre (4)
    int yResolution;        // vertical pixels per metre (4)
    int colorCount;         // # of colours used (4)
    int importantColorCount;// # of important colours (4)
    
    int paletteSize;        // size of palette block in bytes

    // compute paddings per each line
    // In BMP, each scanline must be divisible evenly by 4
    // If not, add extra paddings in each line, it can be divisible by 4.
    int paddings = (4 - ((w * channelCount) % 4)) % 4;

    // compute data size without paddings
    int dataSize = w * h * channelCount;

    // fill vars for BMP header infos
    id[0] = 'B';
    id[1] = 'M';
    reserved1 = reserved2 = 0;
    width = w;
    height = h;
    planeCount = 1;
    bitCount = channelCount * 8;
    compression = 0;
    dataSizeWithPaddings = dataSize + (h * paddings);
    xResolution = yResolution = 2835;   // 72 pixels/inch = 2835 pixels/m
    colorCount = 0;
    importantColorCount = 0;
    infoHeaderSize = 40;                // should be 40 bytes
    dataOffset = 54;                    // fileHeader(14) + infoHeader(40)
    fileSize = dataSizeWithPaddings + dataOffset;

    // 8-bit grayscale image need palette
    // correct colorCount, dataOffset and fileSize
    if(channelCount == 1)
    {
        colorCount = 256;                   // always use max number of colors for 8-bit gray scale
        paletteSize = colorCount * 4;       // BGRA for each
        dataOffset = 54 + paletteSize;      // add up palette size
        fileSize = dataSizeWithPaddings + dataOffset;   // reset file size
    }

    // allocate output data array
    unsigned char* tmpData = new unsigned char [dataSize];

    // copy image data
    memcpy(tmpData, data, dataSize);

    // flip the image upside down
    // If height is  negative, then it is bottom-top orientation (no need to flip)
    if(height > 0)
        flipImage(tmpData, width, height, channelCount);

    // convert RGB to BGR order
    if(channelCount == 3 || channelCount == 4)
        swapRedBlue(tmpData, dataSize, channelCount);

    // add paddings(0s) if the width of image is not divisible by 4
    unsigned char* dataWithPaddings = 0;
    if(paddings > 0)
    {
        // allocate an array
        // add extra bytes for paddings in case the width is not divisible by 4
        dataWithPaddings = new unsigned char [dataSizeWithPaddings];

        int lineWidth = width * channelCount;       // line width in bytes

        // copy single line at a time
        for(int i = 0; i < height; ++i)
        {
            // restore data by adding paddings
            memcpy(&dataWithPaddings[i*(lineWidth+paddings)], &tmpData[i*lineWidth], lineWidth);

            // insert 0s for paddings after copying the current line
            for(int j = 1; j <= paddings; ++j)
                dataWithPaddings[(i+1)*(lineWidth+paddings) - j] = (unsigned char)0;
        }
    }

    // open output file to write data
    ofstream outFile;
    outFile.open(fileName, ios::binary);
    if(!outFile.good())
    {
        errorMessage = "Failed to open an optput file.";
        return false;   // exit if failed
    }

    // write header
    outFile.put(id[0]);
    outFile.put(id[1]);
    outFile.write((char*)&fileSize, 4);
    outFile.write((char*)&reserved1, 2);
    outFile.write((char*)&reserved2, 2);
    outFile.write((char*)&dataOffset, 4);
    outFile.write((char*)&infoHeaderSize, 4);
    outFile.write((char*)&width, 4);
    outFile.write((char*)&height, 4);
    outFile.write((char*)&planeCount, 2);
    outFile.write((char*)&bitCount, 2);
    outFile.write((char*)&compression, 4);
    outFile.write((char*)&dataSizeWithPaddings, 4);
    outFile.write((char*)&xResolution, 4);
    outFile.write((char*)&yResolution, 4);
    outFile.write((char*)&colorCount, 4);
    outFile.write((char*)&importantColorCount, 4);

    // For 8-bit grayscale, insert palette between header block and data block
    if(bitCount == 8)
    {
        unsigned char* palette = new unsigned char[paletteSize]; // each entry has 4 bytes(B,G,R,A)
        buildGrayScalePalette(palette, paletteSize);

        // write palette to the file
        outFile.write((char*)palette, paletteSize);
        delete [] palette;
    }

    // write image data
    if(paddings == 0)
        outFile.write((char*)tmpData, dataSize);                        // without padding
    else
        outFile.write((char*)dataWithPaddings, dataSizeWithPaddings);   // with paddings

    // close the opened file
    outFile.close();

    // deallocate tmp buffer
    delete [] tmpData;
    delete [] dataWithPaddings;

    return true;
}

bool Bmp::decodeRLE8(const unsigned char *encData, unsigned char *outData)
{
    // check NULL pointer
    if(!encData || !outData)
        return false;

    unsigned char first, second;
    int i;
    bool stop = false;

    // start decoding, stop when it reaches at the end of decoded data
    while(!stop)
    {
        // grab 2 bytes at the current position
        first = *encData++;
        second = *encData++;

        if(first)                   // encoded run mode
        {
            for(i=0; i < first; ++i)
                *outData++ = second;
        }
        else
        {
            if(second == 1)         // reached the end of bitmap
                stop = true;        // must stop decoding

            else if(second == 2)    // delta mark
                encData += 2;       // do nothing, but move the cursor 2 more bytes

            else                    // unencoded run mode (second >= 3)
            {
                for(i=0; i < second; ++i)
                    *outData++ = *encData++;

                if(second % 2)      // if it is odd number, then there is a padding 0. ignore it
                    encData++;
            }
        }
    }

    return true;
}



///////////////////////////////////////////////////////////////////////////////
// BMP is bottom-to-top orientation. Flip the image vertically, so the image
// can be rendered from top to bottom orientation
///////////////////////////////////////////////////////////////////////////////
void Bmp::flipImage(unsigned char *data, int width, int height, int channelCount)
{
    if(!data) return;

    int lineSize = width * channelCount;
    unsigned char* tmp = new unsigned char [lineSize];
    int half = height / 2;

    int line1 = 0;                          // first line
    int line2 = (height - 1) * lineSize;    // last line

    // scan only half of height
    for(int i = 0; i < half; ++i)
    {
        // copy line by line
        memcpy(tmp, &data[line1], lineSize);
        memcpy(&data[line1], &data[line2], lineSize);
        memcpy(&data[line2], tmp, lineSize);

        // move to next line
        line1 += lineSize;
        line2 -= lineSize;
    }

    // deallocate temp arrays
    delete [] tmp;
}



///////////////////////////////////////////////////////////////////////////////
// swap the position of the 1st and 3rd color components (RGB <-> BGR)
///////////////////////////////////////////////////////////////////////////////
void Bmp::swapRedBlue(unsigned char *data, int dataSize, int channelCount)
{
    if(!data) return;
    if(channelCount < 3) return;            // must be 3 or 4
    if(dataSize % channelCount) return;     // must be divisible by the number of channels

    unsigned char tmp;
    int i;

    // swap the position of red and blue components
    for(i=0; i < dataSize; i+=channelCount)
    {
        tmp = data[i];
        data[i] = data[i+2];
        data[i+2] = tmp;
    }
}



///////////////////////////////////////////////////////////////////////////////
// compute the number of used colors in the 8-bit grayscale image
///////////////////////////////////////////////////////////////////////////////
int Bmp::getColorCount(const unsigned char* data, int dataSize)
{
    if(!data) return 0;

    const int MAX_COLOR = 256;  // max number of colors in 8-bit grayscale
    int i;
    int colorCount = 0;
    unsigned int colors[MAX_COLOR];

    // clear all to 0s
    memset((void*)colors, 0, MAX_COLOR);

    // increment at the same index
    for(i = 0; i < dataSize; ++i)
        colors[data[i]]++;

    // count backward the number of color used in this data
    colorCount = MAX_COLOR;
    for(i = 0; i < MAX_COLOR; ++i)
    {
        if(colors[i] == 0)
            colorCount--;
    }

    return colorCount;
}

// when read image file.bmp, type data image unsigned char . we want to display image, we need graphic ,
/// I using funtion cvShowImage with iplImage variable.

IplImage* Bmp::loadImage(const char* FILE_NAME)
{
	int w=this->width;
	int h=this->width;
	
	IplImage* img;
	img=cvCreateImage(cvSize(w,h),8,3);

	int k=0;
	for(int i=0; i<h ; i++)
	{
		for(int j=0; j<w ; j++)
		{
			((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0]=(unsigned char)(this->dataRGB[k+2]);
			((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 1]=(unsigned char)(this->dataRGB[k+1]);
			((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 2]=(unsigned char)(this->dataRGB[k+0]);
			
			k=k+3;
		}
	}

	return img;
}

///////////////////////////////////////////////////////////////////////////////
// build palette for 8-bit grayscale image
// Each component(B,G,R,A) of palette will have the same value as data value
// because it is grayscale.
///////////////////////////////////////////////////////////////////////////////
void Bmp::buildGrayScalePalette(unsigned char* palette, int paletteSize)
{
    if(!palette) return;

    // fill B, G, R, with same value and A is 0
    int i, j;
    for(i = 0, j = 0; i < paletteSize; i+=4, j++)
    {
        palette[i] = palette[i+1] = palette[i+2] = (unsigned char)j;
        palette[i+3] = (unsigned char)0;
    }
}

improcessImage::improcessImage()
{
	img=0;
}

improcessImage::improcessImage(IplImage* image)
{
	img=image;
}

void improcessImage::grayScale(IplImage* image, IplImage* grayImage)
{
	for(int i=0; i< image->height ; i++)
	{
		for(int j=0; j< image->width ; j++)
		{
				((uchar *)(grayImage->imageData + i*grayImage->widthStep))[j*grayImage->nChannels + 0]=
				       0.114*((uchar *)(image->imageData + i*image->widthStep))[j*image->nChannels + 0]+
				       0.587*((uchar *)(image->imageData + i*image->widthStep))[j*image->nChannels + 1]+
				       0.299*((uchar *)(image->imageData + i*image->widthStep))[j*image->nChannels + 2];
		}
	}
}

void improcessImage::binaryImage(IplImage* image,IplImage* binary, int nguong)
{
	IplImage* grayImage=cvCreateImage(cvSize(image->width,image->height),8,1);
	
	for(int i=0; i< image->height ; i++)
	{
		for(int j=0; j< image->width ; j++)
		{
				((uchar *)(grayImage->imageData + i*grayImage->widthStep))[j*grayImage->nChannels + 0]=
				       0.114*((uchar *)(image->imageData + i*image->widthStep))[j*image->nChannels + 0]+
				       0.587*((uchar *)(image->imageData + i*image->widthStep))[j*image->nChannels + 1]+
				       0.299*((uchar *)(image->imageData + i*image->widthStep))[j*image->nChannels + 2];
		}
	}
	
	for(int i=0; i< image->height; i++ )
	{
		for(int j=0; j< image->width ; j++)
		{
			if( 	((uchar *)(grayImage->imageData + i*grayImage->widthStep))[j*grayImage->nChannels + 0] > nguong )
			{
					((uchar *)(binary->imageData + i*binary->widthStep))[j*binary->nChannels + 0]=255;
			}
			else
			{
					((uchar *)(binary->imageData + i*binary->widthStep))[j*binary->nChannels + 0]=0;
			}
		}
	}
}
void improcessImage::dilateImage(IplImage* binary,IplImage* dilateImage)
{
	for(int i=0; i< binary->height ; i++)
	{
		for(int j=0; j<binary->width ; j++)
		{
			((uchar *)(dilateImage->imageData + (i)*dilateImage->widthStep))[(j)*dilateImage->nChannels + 0]=0;
		}
	}
	
	for(int i=0; i< binary->height ; i++)
	{
		for(int j=0; j<binary->width ; j++)
		{
			if( ((uchar *)(binary->imageData + i*binary->widthStep))[j*binary->nChannels + 0]==255 )
			{
				((uchar *)(dilateImage->imageData + (i)*dilateImage->widthStep))[(j)*dilateImage->nChannels + 0]=255;
				((uchar *)(dilateImage->imageData + (i-1)*dilateImage->widthStep))[(j-1)*dilateImage->nChannels + 0]=255;
				((uchar *)(dilateImage->imageData + (i-1)*dilateImage->widthStep))[j*dilateImage->nChannels + 0]=255;
				((uchar *)(dilateImage->imageData + (i-1)*dilateImage->widthStep))[(j+1)*dilateImage->nChannels + 0]=255;
				((uchar *)(dilateImage->imageData + i*dilateImage->widthStep))[(j-1)*dilateImage->nChannels + 0]=255;
				((uchar *)(dilateImage->imageData + i*dilateImage->widthStep))[(j+1)*dilateImage->nChannels + 0]=255;
				((uchar *)(dilateImage->imageData + (i+1)*dilateImage->widthStep))[(j-1)*dilateImage->nChannels + 0]=255;
				((uchar *)(dilateImage->imageData + (i+1)*dilateImage->widthStep))[j*dilateImage->nChannels + 0]=255;
				((uchar *)(dilateImage->imageData + (i+1)*dilateImage->widthStep))[(j+1)*dilateImage->nChannels + 0]=255;
			}
		}
	}
}
void improcessImage::erodeImage(IplImage* binary,IplImage* dilateImage)
{
	for(int i=0; i< binary->height ; i++)
	{
		for(int j=0; j<binary->width ; j++)
		{
			((uchar *)(dilateImage->imageData + (i)*dilateImage->widthStep))[(j)*dilateImage->nChannels + 0]=0;
		}
	}
	
	for(int i=0; i< binary->height ; i++)
	{
		for(int j=0; j<binary->width ; j++)
		{
			if( 
				((uchar *)(binary->imageData + (i)*binary->widthStep))[(j)*binary->nChannels + 0]==255 &&
				((uchar *)(binary->imageData + (i-1)*binary->widthStep))[(j-1)*binary->nChannels + 0]==255 &&
				((uchar *)(binary->imageData + (i-1)*binary->widthStep))[j*binary->nChannels + 0]==255 &&
				((uchar *)(binary->imageData + (i-1)*binary->widthStep))[(j+1)*binary->nChannels + 0]==255 &&
				((uchar *)(binary->imageData + i*binary->widthStep))[(j-1)*binary->nChannels + 0]==255 &&
				((uchar *)(binary->imageData + i*binary->widthStep))[(j+1)*binary->nChannels + 0]==255 &&
				((uchar *)(binary->imageData + (i+1)*binary->widthStep))[(j-1)*binary->nChannels + 0]==255 &&
				((uchar *)(binary->imageData + (i+1)*binary->widthStep))[j*binary->nChannels + 0]==255 &&
				((uchar *)(binary->imageData + (i+1)*binary->widthStep))[(j+1)*binary->nChannels + 0]==255
			)
			{
				((uchar *)(dilateImage->imageData + (i)*dilateImage->widthStep))[(j)*dilateImage->nChannels + 0]=255;
			}
		}
	}
}

void improcessImage::langgieng(IplImage *img, int x, int y, int A[])
{
	A[1] = dataImage(img, x + 1, y);
	A[2] = dataImage(img, x + 1, y + 1);
	A[3] = dataImage(img, x, y + 1);
	A[4] = dataImage(img, x - 1, y + 1);
	A[5] = dataImage(img, x - 1, y);
	A[6] = dataImage(img, x - 1, y - 1);
	A[7] = dataImage(img, x, y - 1);
	A[8] = dataImage(img, x + 1, y - 1);
}

int improcessImage::dem(int A[])
{
	int tong = 0;
	for(int i = 1; i <= 8; i++)
		if(A[i] != 0) ++tong;
	return tong;
}

int improcessImage::ketnoi(int A[])
{
	int kq = 0;
	int a[9];
	for(int k = 1; k <= 8; k++)
	{
		if(A[k] == 255) a[k] = 1;
		else a[k] = 0;
	}
	kq += a[1] - a[1] * a[2] * a[3];
	kq += a[3] - a[3] * a[4] * a[5];
	kq += a[5] - a[5] * a[6] * a[7];
	kq += a[7] - a[7] * a[8] * a[1];
	return kq;
}

void improcessImage::Stentiford(IplImage *img)
{
	int tieptuc = 1;
	int delpixel;
	int x, y;
	IplImage *copy = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
	cvCopyImage(img, copy);
	int A[9];
	int tong;
	while(tieptuc == 1)
	{
		// Case 1:
		delpixel = 0;
		for( y = 1; y < img->height - 1; y++)
			for( x = 1; x < img->width - 1; x++)
			{
				langgieng(img, x, y, A);
				if(dataImage(img, x, y) == 255 && dataImage(img, x, y + 1) == 0 && dataImage(img, x, y - 1) == 255)
				{
					if(dem(A) > 1 && ketnoi(A) == 1)
					{
						//cout<<"case 1\n";
						dataImage(copy, x, y) = 0;
						delpixel = 1;
					}
				}
			}

		// Case 2:
		cvCopyImage(copy, img);
		//////////////////
		for( y = 1; y < img->height - 1; y++)
			for( x = 1; x < img->width - 1; x++)
			{
				langgieng(img, x, y, A);
				if(dataImage(img, x, y) == 255 && dataImage(img, x - 1, y) == 0 && dataImage(img, x + 1, y) == 255)
				{
					if(dem(A) > 1 && ketnoi(A) == 1)
					{
						//cout<<"case 2\n";
						dataImage(copy, x, y) = 0;
						delpixel = 1;
					}
				}
			}

		 // Case 3:
		cvCopyImage(copy, img);
		////////////////
		for( y = 1; y < img->height - 1; y++)
			for( x = 1; x < img->width - 1; x++)
			{
				langgieng(img, x, y, A);
				if(dataImage(img, x, y) == 255 && dataImage(img, x, y + 1) == 255 && dataImage(img, x, y - 1) == 0)
				{
					if(dem(A) > 1 && ketnoi(A) == 1)
					{
						//cout<<"case 3\n";
						dataImage(copy, x, y) = 0;
						delpixel = 1;
					}
				}

			}

		// Case 4:
		cvCopyImage(copy, img);
		////////////////////
		for( y = 1; y < img->height - 1; y++)
			for( x = 1; x < img->width - 1; x++)
			{
				langgieng(img, x, y, A);
				if(dataImage(img, x, y) == 255 && dataImage(img, x - 1, y) == 255 && dataImage(img, x + 1, y) == 0)
				{
					if(dem(A) > 1 && ketnoi(A) == 1)
					{
						//cout<<"case 4\n";
						dataImage(copy, x, y) = 0;
						delpixel = 1;
					}
				}

			}
		cvCopyImage(copy, img);
		if(delpixel == 1) tieptuc = 1;
		else tieptuc = 0;
	}
}

void improcessImage::openImage(IplImage* binary)
{
    IplImage* result;
	result=cvCreateImage(cvSize(binary->width,binary->height),8,1);
	
	IplImage* result1;
	result1 =cvCreateImage(cvSize(binary->width,binary->height),8,1);
	
	Image::improcessImage *input;
    input = new improcessImage();
    
	input->dilateImage(binary,result);
	input->erodeImage(result,result1);
	cvCopyImage(result1,binary);
	
	delete input;
}

void improcessImage::closeImage(IplImage* binary)
{
    IplImage* result;
	result=cvCreateImage(cvSize(binary->width,binary->height),8,1);
	
	IplImage* result1;
	result1 =cvCreateImage(cvSize(binary->width,binary->height),8,1);
	
	Image::improcessImage *input;
    input=new improcessImage();
    
	input->erodeImage(binary,result);
	input->dilateImage(result,result1);
	cvCopyImage(result1,binary);
	
	delete input;
}


