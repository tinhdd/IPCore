#include <iostream>
#include<highgui.h>
#include<cv.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>  
#include <string>
#include"conio.h"
#include <vector>
#include <stack>

using namespace std;
using namespace cv;

#define pi 3.141529
int GaussianKernel[15][15];
int KernelWeight=0 ;
float Sigma = 1;

const double color_radius = 6.5;
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
			
			int				noOfCordinates;		//Number of elements in coordinate array
			CvPoint			*cordinates;		//Coordinates array to store model points	
			int				modelHeight;		//Template height
			int				modelWidth;			//Template width
			double			*edgeMagnitude;		//gradient magnitude
			double			*edgeDerivativeX;	//gradient in X direction
			double			*edgeDerivativeY;	//radient in Y direction	
			CvPoint			centerOfGravity;	//Center of gravity of template 
	
			bool			modelDefined;
	
			void CreateDoubleMatrix(double **&matrix,CvSize size);
			void ReleaseDoubleMatrix(double **&matrix,int size);
	  
		public:
			
			improcessImage(void);
			~improcessImage(void);
			improcessImage(IplImage* image);
			improcessImage(const void* templateArr);
			
		    void grayScale(IplImage* image, IplImage* grayImage);
		    void binaryImage(IplImage* image,IplImage* binary,int nguong);
		    void dilateImage(IplImage* binary,IplImage* dilateImage);
		    void erodeImage(IplImage* binary,IplImage* dilateImage);
		    void Stentiford(IplImage *img);
		    void openImage(IplImage* binary);
		    void closeImage(IplImage* binary);
		    inline int color_distance( const IplImage* img, int x1, int y1, int x2, int y2 );
		    int ECC(const IplImage* img, int **labels);
		    IplImage* dilateGray(IplImage* grayImage );
		    IplImage* erodeGray(IplImage* grayImage );
			
			IplImage* negative(IplImage* img_bin,int x_min,int x_max, int y_min, int y_max);
			int** limits(IplImage* img_bin);
			void thickening(IplImage* img);
		    
		    void increase(int window[],int n);
			int sumPixel(int window[],int n);
			IplImage* medianFilter(IplImage* img,int n);
			IplImage* meanFilter(IplImage* img,int n);
			IplImage* convolution(IplImage* image, float window[][3] );
			IplImage* LPF(IplImage* image);
			IplImage* HPF(IplImage* image);
			int computeOutput(int x, int r1, int s1, int r2, int s2);
			IplImage* stretch_contract(IplImage* image);
	
		    int CreateGeoMatchModel(const void* templateArr,double,double);
			double FindGeoMatchModel(const void* srcarr,double minScore,double greediness, CvPoint *resultPoint);
			void DrawContours(IplImage* pImage,CvPoint COG,CvScalar,int);
			void DrawContours(IplImage* pImage,CvScalar,int);
	};
}

void GenerateGaussianKernel(int N ,int &Weight );
void GaussianFilter( IplImage* gray,IplImage* out ,int KernelSize);


int main(int argc, char** argv) {
	
	const   char *FILE_NAME = "C:\\dilate.bmp";
	char c=0;
        
	cout<<"\n a. Load image\n";
	cout<<" b. Convert RGB to gray\n";
	cout<<" c. Convert RGB to binary\n";
	cout<<" d. Dilate algorithm\n";
	cout<<" e. Erode algorithm\n";
	cout<<" f. Thining algorithm\n";
	cout<<" g. open image algorithm\n";
	cout<<" h. close image algorithm\n";
	cout<<" i. Extracting conected components and Region fill algorithm\n";
	cout<<" k. thickening algorithm\n";
	cout<<" l. Canny algorithm\n";
	cout<<" m. Dilate gray algorithm\n";
	cout<<" n. Erode gray algorithm\n";
	cout<<" o. median Filter algorithm\n";
	cout<<" p. mean Filter algorithm\n";
	cout<<" q. LPF filter algorithm\n";
	cout<<" r. HPF filter algorithm\n";
	cout<<" s. stretch contract algorithm\n";
		
	cout<<"\n ESC. exit\n";
	cout<<"\n Chu y: Bam X anh hien thi de tro ve menu\n";

	cout<<"\n Moi nhap cac so theo chuc nang tren: ";

	while( c!='0')
	{
		c=getch();
		//cout<<"\n"<<c;

		Image::Bmp bmp; 

		// load BMP image
		if(!bmp.read(FILE_NAME))
			return 0;     // exit if failed load image
    
		IplImage* image;

		image=bmp.loadImage(FILE_NAME);
	
		int     imageX, imageY; 
		imageX=image->width;
		imageY=image->height;
	
		IplImage* grayImage=0;
		IplImage* binary;
		IplImage* imageDilate;
		IplImage* imageErode;
		IplImage* thiningImage;
	
		IplImage* imageOpen;
		IplImage* imageClose;
		IplImage* imageECC;
		IplImage* img_bin;
		
		grayImage=cvCreateImage(cvSize(imageX,imageY),8,1);
		binary=cvCreateImage(cvSize(imageX,imageY),8,1);
		imageDilate=cvCreateImage(cvSize(imageX,imageY),8,1);
		imageErode=cvCreateImage(cvSize(imageX,imageY),8,1);
		thiningImage=cvCreateImage(cvSize(imageX,imageY),8,1);
		img_bin=cvCreateImage(cvSize(imageX,imageY),8,1);
	
		imageOpen=cvCreateImage(cvSize(imageX,imageY),8,1);
		imageClose=cvCreateImage(cvSize(imageX,imageY),8,1);
		imageECC=cvCreateImage(cvSize(imageX,imageY),8,1);
	
		Image::improcessImage input;
	
		input.grayScale(image,grayImage);
		input.binaryImage(image,binary,100);
	
		cvCopyImage(binary,thiningImage);
		cvCopyImage(binary,imageOpen);
		cvCopyImage(binary,imageClose);
		cvCopyImage(binary,imageECC);
		cvCopyImage(binary,img_bin);
	    
		if( c=='a') cvShowImage("helloWorld",image);
		if( c=='b') cvShowImage("gray image",grayImage);
		if( c=='c') cvShowImage("bianry image",binary);
	
		if( c=='d' ) 
		{
			input.dilateImage(binary,imageDilate);
			cvShowImage("bianry image",binary);
			cvShowImage("dilate image",imageDilate);
		}

		if( c=='e' ) 
		{
			input.erodeImage(binary,imageErode);
			cvShowImage("bianry image",binary);
			cvShowImage("erode image",imageErode);
		}

		if( c=='f' )
		{
			input.Stentiford(thiningImage);
			cvShowImage("bianry image",binary);
			cvShowImage("thinning image",thiningImage);
		}

		if( c=='g' )
		{
			input.openImage(imageOpen);
			cvShowImage("bianry image",binary);
			cvShowImage("open image",imageOpen);
		}

		if( c=='h' )
		{
			input.closeImage(imageClose);
			cvShowImage("bianry image",binary);
			cvShowImage("close image",imageClose);
		}
        
        if( c=='i')
        {
        	IplImage* img;
        	
        	img = cvCreateImage(cvSize(imageX,imageY),8,3);
        	cvCvtColor(imageECC,img,CV_GRAY2BGR);
        	
			// Mean shift
			int **ilabels = new int *[img->height];
			for(int i=0;i<img->height;i++) ilabels[i] = new int [img->width];
			int regionCount = input.ECC(img, ilabels);
			vector<int> color(regionCount);
			CvRNG rng= cvRNG(cvGetTickCount());
			for(int i=0;i<regionCount;i++)
				color[i] = cvRandInt(&rng);
		
			// Draw random color
			for(int i=0;i<img->height;i++)
				for(int j=0;j<img->width;j++)
				{ 
					int cl = ilabels[i][j];
					((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0] = (color[cl])&255;
					((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 1] = (color[cl]>>8)&255;
					((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 2] = (color[cl]>>16)&255;
				}
			cvShowImage("MeanShift",img);
        }
        
		if( c=='k')
		{
			cvShowImage("original",img_bin);

			int **p;
			p=input.limits(img_bin);
			cout<<"\np min"<<p[0][0];

			img_bin=input.negative(img_bin,p[0][0],p[0][1],p[1][0],p[1][1] );

			//cvShowImage("negative",img_bin);

			input.Stentiford(img_bin);

			IplImage* img;
        	
			img = cvCreateImage(cvSize(img_bin->width, img_bin->height),8,3);
			cvCvtColor(img_bin,img,CV_GRAY2BGR);
        	
			// Mean shift
			int **ilabels = new int *[img->height];
			for(int i=0;i<img->height;i++) ilabels[i] = new int [img->width];
			int regionCount = input.ECC(img, ilabels);
		
			// Draw random color
			for(int i=0;i<img->height;i++)
				for(int j=0;j<img->width;j++)
				{ 
					int cl = ilabels[i][j];

					if( ((uchar *)(img_bin->imageData + i*img_bin->widthStep))[j*img_bin->nChannels + 0]==255 )
					{
						((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0] = 0;
						((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 1] = 0;
						((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 2] = 0;
						continue;
					}

					if( cl!=0)
					{
						((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0] = 255;
						((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 1] = 255;
						((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 2] = 255;
					}
				}

			cvShowImage("result",img);

		}

        if( c=='l')
        {
        	IplImage* out =cvCreateImage( cvGetSize(grayImage) ,8,1 );
			IplImage* canny =cvCreateImage( cvGetSize(grayImage) ,8,1 );
			cvZero(canny);

			GaussianFilter(grayImage,out,5);

			Image::improcessImage GM;

			int lowThreshold = 10;		
			int highThreashold = 100;	
			double minScore=0.7;		
			double greediness=0.8;		
			double score= 0;
			CvPoint result;

			GM.CreateGeoMatchModel(out,lowThreshold,highThreashold);
			GM.DrawContours(canny,CV_RGB( 255, 255, 255 ),1);

			cvShowImage("out",canny);
        }
        
        if( c=='m' )
        {
			IplImage* original=0;
			original=cvLoadImage(FILE_NAME);
			IplImage* gray= cvCreateImage(cvGetSize(original),8,1);

			cvCvtColor(original,gray,CV_BGR2GRAY);

        	IplImage* dilateGrayImage;
        	dilateGrayImage = input.dilateGray(gray);
			cvShowImage("dilate gray",dilateGrayImage);
        }
        
		 if( c=='n' )
        {
			IplImage* original=0;
			original=cvLoadImage(FILE_NAME);
			IplImage* gray= cvCreateImage(cvGetSize(original),8,1);

			cvCvtColor(original,gray,CV_BGR2GRAY);

        	IplImage* dilateGrayImage;
			dilateGrayImage = input.erodeGray(gray);
			cvShowImage("dilate gray",dilateGrayImage);
        }
        
         if( c=='o' )
        {
			IplImage* median_Filter_Image;
			median_Filter_Image = input.medianFilter(image,9);
			
			cvShowImage("original",image);
			cvShowImage("median filter",median_Filter_Image);
        }
        
        if( c=='p' )
        {
			IplImage* mean_Filter_Image;
			mean_Filter_Image = input.meanFilter(image,9);
			
			cvShowImage("original",image);
			cvShowImage("mean filter",mean_Filter_Image);
        }
        
        if( c=='q' )
        {
        	IplImage* LPF_Filter_Image;
        	LPF_Filter_Image = input.LPF(image);
        	
			cvShowImage("original",image);
			cvShowImage("LPF filter",LPF_Filter_Image);
        }
        
        if( c=='r' )
        {
        	IplImage* HPF_Filter_Image;
        	HPF_Filter_Image = input.HPF(image);
        	
        	cvShowImage("original",image);
        	cvShowImage("HPF filter",HPF_Filter_Image);
        }
        
        if( c=='s' )
        {
        	IplImage* tretch_contract_Image;
        	tretch_contract_Image = input.stretch_contract(image);
        	
        	cvShowImage("original",image);
        	cvShowImage("contract image",tretch_contract_Image);
        }
        
		cout<<"\n Successed\n"; 
		cvWaitKey(0);
		
		cvReleaseImage(&image);
		cvReleaseImage(&grayImage);
		cvReleaseImage(&binary);
		cvReleaseImage(&imageDilate);
		cvReleaseImage(&imageErode);
		cvReleaseImage(&thiningImage);
		cvReleaseImage(&imageOpen);
		cvReleaseImage(&imageClose);
		cvReleaseImage(&imageECC);
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
	noOfCordinates = 0;  // Initilize  no of cppodinates in model points
	modelDefined = false; 
}

improcessImage::~improcessImage(void)
{
	delete[] cordinates ;
	delete[] edgeMagnitude;
	delete[] edgeDerivativeX;
	delete[] edgeDerivativeY;
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

inline int improcessImage::color_distance( const IplImage* img, int x1, int y1, int x2, int y2 ) 
{
	int r = ((uchar *)(img->imageData + x1*img->widthStep))[y1*img->nChannels + 0]
	- ((uchar *)(img->imageData + x2*img->widthStep))[y2*img->nChannels + 0];
	return r*r;
}

int improcessImage::ECC(const IplImage* img, int **labels)
{
	int level = 1;
	double color_radius2=color_radius*color_radius;
	int minRegion = 50;

	// use Lab rather than L*u*v!
	// since Luv may produce noise points
	IplImage *result = cvCreateImage(cvGetSize(img),img->depth,1);
	cvCvtColor(img, result, CV_BGR2GRAY); 

	// Step Two. Cluster
	// Connect
	int regionCount = 0;
	float *mode = new float[img->height*img->width*3];
	{
		int label = -1;
		for(int i=0;i<img->height;i++) 
			for(int j=0;j<img->width;j++)
				labels[i][j] = -1;
		for(int i=0;i<img->height;i++) 
			for(int j=0;j<img->width;j++)
				if(labels[i][j]<0)
				{
					labels[i][j] = ++label;

					// Fill
					std::stack<CvPoint> neighStack;
					neighStack.push(cvPoint(i,j));
					const int dxdy[][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
					while(!neighStack.empty())
					{
						CvPoint p = neighStack.top();
						neighStack.pop();
						for(int k=0;k<8;k++)
						{
							int i2 = p.x+dxdy[k][0], j2 = p.y+dxdy[k][1];
							if(i2>=0 && j2>=0 && i2<img->height && j2<img->width && labels[i2][j2]<0 && color_distance(result, i,j,i2,j2)<color_radius2)
							{
								labels[i2][j2] = label;
								neighStack.push(cvPoint(i2,j2));
							}
						}
					}
				}
				//current Region count
				regionCount = label+1;
	}	

	std::cout<<"Mean Shift(Connect):"<<regionCount<<std::endl;
	int oldRegionCount = regionCount;

	cvReleaseImage(&result);
	delete []mode;
	return regionCount;
}

int** improcessImage::limits(IplImage* img_bin)
{
	int **p;
	
	p=new int* [2];
	for(int i=0; i< 2; i++)
	{
		p[i]=new int[2];
	}

	int x_min=10000;
	int x_max=0;
	int y_min=0;
	int y_max=0;
	int max=0;

	for(int i=0; i< img_bin->height ; i++)
	{
		for(int j=0; j< img_bin->width ; j++)
		{
			if( ((uchar *)(img_bin->imageData + i*img_bin->widthStep))[j*img_bin->nChannels + 0]==255 &&
				y_min==0 )
			{
				y_min=i;
			}
			if( ((uchar *)(img_bin->imageData + i*img_bin->widthStep))[j*img_bin->nChannels + 0]==255 )
			{
				y_max=i;
			}

			if( ((uchar *)(img_bin->imageData + i*img_bin->widthStep))[j*img_bin->nChannels + 0]==255 )
			{
				if( x_min > j ) x_min=j;
			}

			if( ((uchar *)(img_bin->imageData + i*img_bin->widthStep))[j*img_bin->nChannels + 0]==255 )
			{
				max=j;
			}
		}

		if( x_max < max ) x_max=max;
	}

	p[0][0]=x_min;
	p[0][1]=x_max;
	p[1][0]=y_min;
	p[1][1]=y_max;

	return p;
}

IplImage* improcessImage::negative(IplImage* img_bin,int x_min,int x_max, int y_min, int y_max)
{
	for(int i=y_min-1; i<= y_max+1 ; i++)
	{
		for(int j=x_min-1; j<= x_max+1 ; j++)
		{
			if( ((uchar *)(img_bin->imageData + i*img_bin->widthStep))[j*img_bin->nChannels + 0]==255 )
			{
				((uchar *)(img_bin->imageData + i*img_bin->widthStep))[j*img_bin->nChannels + 0]=0;
				continue;
			}
			else
			{
				((uchar *)(img_bin->imageData + i*img_bin->widthStep))[j*img_bin->nChannels + 0]=255;
			}
		}
	}

	return img_bin;
}

int improcessImage::CreateGeoMatchModel(const void *templateArr,double maxContrast,double minContrast)
{
	CvMat *gx = 0;		//Matrix to store X derivative
	CvMat *gy = 0;		//Matrix to store Y derivative
	CvMat *nmsEdges = 0;		//Matrix to store temp restult
	CvSize Ssize;
		
	// Convert IplImage to Matrix for integer operations
	CvMat srcstub, *src = (CvMat*)templateArr;
	src = cvGetMat( src, &srcstub );
	if(CV_MAT_TYPE( src->type ) != CV_8UC1)
	{	
		return 0;
	}
	
	// set width and height
	Ssize.width =  src->width;
	Ssize.height= src->height;
	modelHeight =src->height;		//Save Template height
	modelWidth =src->width;			//Save Template width

	noOfCordinates=0;											//initialize	
	cordinates =  new CvPoint[ modelWidth *modelHeight];		//Allocate memory for coorinates of selected points in template image
	
	edgeMagnitude = new double[ modelWidth *modelHeight];		//Allocate memory for edge magnitude for selected points
	edgeDerivativeX = new double[modelWidth *modelHeight];			//Allocate memory for edge X derivative for selected points
	edgeDerivativeY = new double[modelWidth *modelHeight];			////Allocate memory for edge Y derivative for selected points


	// Calculate gradient of Template
	gx = cvCreateMat( Ssize.height, Ssize.width, CV_16SC1 );		//create Matrix to store X derivative
	gy = cvCreateMat( Ssize.height, Ssize.width, CV_16SC1 );		//create Matrix to store Y derivative
	cvSobel( src, gx, 1,0, 3 );		//gradient in X direction			
	cvSobel( src, gy, 0, 1, 3 );	//gradient in Y direction
	
	nmsEdges = cvCreateMat( Ssize.height, Ssize.width, CV_32F);		//create Matrix to store Final nmsEdges
	const short* _sdx; 
	const short* _sdy; 
	double fdx,fdy;	
    double MagG, DirG;
	double MaxGradient=-99999.99;
	double direction;
	int *orients = new int[ Ssize.height *Ssize.width];
	int count = 0,i,j; // count variable;
	
	double **magMat;
	CreateDoubleMatrix(magMat ,Ssize);
	
	for( i = 1; i < Ssize.height-1; i++ )
    {
    	for( j = 1; j < Ssize.width-1; j++ )
        { 		 
				_sdx = (short*)(gx->data.ptr + gx->step*i);
				_sdy = (short*)(gy->data.ptr + gy->step*i);
				fdx = _sdx[j]; fdy = _sdy[j];        // read x, y derivatives

				MagG = sqrt((float)(fdx*fdx) + (float)(fdy*fdy)); //Magnitude = Sqrt(gx^2 +gy^2)
				direction =cvFastArctan((float)fdy,(float)fdx);	 //Direction = invtan (Gy / Gx)
				magMat[i][j] = MagG;
				
				if(MagG>MaxGradient)
					MaxGradient=MagG; // get maximum gradient value for normalizing.

				
					// get closest angle from 0, 45, 90, 135 set
                        if ( (direction>0 && direction < 22.5) || (direction >157.5 && direction < 202.5) || (direction>337.5 && direction<360)  )
                            direction = 0;
                        else if ( (direction>22.5 && direction < 67.5) || (direction >202.5 && direction <247.5)  )
                            direction = 45;
                        else if ( (direction >67.5 && direction < 112.5)||(direction>247.5 && direction<292.5) )
                            direction = 90;
                        else if ( (direction >112.5 && direction < 157.5)||(direction>292.5 && direction<337.5) )
                            direction = 135;
                        else 
							direction = 0;
				
			orients[count] = (int)direction;
			count++;
		}
	}
	
	count=0; // init count
	// non maximum suppression
	double leftPixel,rightPixel;
	
	for( i = 1; i < Ssize.height-1; i++ )
    {
		for( j = 1; j < Ssize.width-1; j++ )
        {
				switch ( orients[count] )
                {
                   case 0:
                        leftPixel  = magMat[i][j-1];
                        rightPixel = magMat[i][j+1];
                        break;
                    case 45:
                        leftPixel  = magMat[i-1][j+1];
						rightPixel = magMat[i+1][j-1];
                        break;
                    case 90:
                        leftPixel  = magMat[i-1][j];
                        rightPixel = magMat[i+1][j];
                        break;
                    case 135:
                        leftPixel  = magMat[i-1][j-1];
                        rightPixel = magMat[i+1][j+1];
                        break;
				 }
				// compare current pixels value with adjacent pixels
                if (( magMat[i][j] < leftPixel ) || (magMat[i][j] < rightPixel ) )
					(nmsEdges->data.ptr + nmsEdges->step*i)[j]=0;
                else
                    (nmsEdges->data.ptr + nmsEdges->step*i)[j]=(uchar)(magMat[i][j]/MaxGradient*255);
			
				count++;
			}
		}
	
	int RSum=0,CSum=0;
	int curX,curY;
	int flag=1;

	//Hysterisis threshold
	for( i = 1; i < Ssize.height-1; i++ )
    {
		for( j = 1; j < Ssize.width; j++ )
        {
			_sdx = (short*)(gx->data.ptr + gx->step*i);
			_sdy = (short*)(gy->data.ptr + gy->step*i);
			fdx = _sdx[j]; fdy = _sdy[j];
				
			MagG = sqrt(fdx*fdx + fdy*fdy); //Magnitude = Sqrt(gx^2 +gy^2)
			DirG =cvFastArctan((float)fdy,(float)fdx);	 //Direction = tan(y/x)
		
			////((uchar*)(imgGDir->imageData + imgGDir->widthStep*i))[j]= MagG;
			flag=1;
			if(((double)((nmsEdges->data.ptr + nmsEdges->step*i))[j]) < maxContrast)
			{
				if(((double)((nmsEdges->data.ptr + nmsEdges->step*i))[j])< minContrast)
				{
					
					(nmsEdges->data.ptr + nmsEdges->step*i)[j]=0;
					flag=0; // remove from edge
					////((uchar*)(imgGDir->imageData + imgGDir->widthStep*i))[j]=0;
				}
				else
				{   // if any of 8 neighboring pixel is not greater than max contraxt remove from edge
					if( (((double)((nmsEdges->data.ptr + nmsEdges->step*(i-1)))[j-1]) < maxContrast)	&&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*(i-1)))[j]) < maxContrast)		&&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*(i-1)))[j+1]) < maxContrast)	&&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*i))[j-1]) < maxContrast)		&&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*i))[j+1]) < maxContrast)		&&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*(i+1)))[j-1]) < maxContrast)	&&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*(i+1)))[j]) < maxContrast)		&&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*(i+1)))[j+1]) < maxContrast)	)
					{
						(nmsEdges->data.ptr + nmsEdges->step*i)[j]=0;
						flag=0;
						////((uchar*)(imgGDir->imageData + imgGDir->widthStep*i))[j]=0;
					}
				}
			}
			
			// save selected edge information
			curX=i;	curY=j;
			if(flag!=0)
			{
				if(fdx!=0 || fdy!=0)
				{		
					RSum=RSum+curX;	CSum=CSum+curY; // Row sum and column sum for center of gravity
					
					cordinates[noOfCordinates].x = curX;
					cordinates[noOfCordinates].y = curY;
					edgeDerivativeX[noOfCordinates] = fdx;
					edgeDerivativeY[noOfCordinates] = fdy;
					
					//handle divide by zero
					if(MagG!=0)
						edgeMagnitude[noOfCordinates] = 1/MagG;  // gradient magnitude 
					else
						edgeMagnitude[noOfCordinates] = 0;
															
					noOfCordinates++;
				}
			}
		}
	}

	centerOfGravity.x = RSum /noOfCordinates; // center of gravity
	centerOfGravity.y = CSum/noOfCordinates ;	// center of gravity
		
	// change coordinates to reflect center of gravity
	for(int m=0;m<noOfCordinates ;m++)
	{
		int temp;

		temp=cordinates[m].x;
		cordinates[m].x=temp-centerOfGravity.x;
		temp=cordinates[m].y;
		cordinates[m].y =temp-centerOfGravity.y;
	}
	
	////cvSaveImage("Edges.bmp",imgGDir);
	
	// free alocated memories
	delete[] orients;
	////cvReleaseImage(&imgGDir);
	cvReleaseMat( &gx );
    cvReleaseMat( &gy );
	cvReleaseMat(&nmsEdges);

	ReleaseDoubleMatrix(magMat ,Ssize.height);
	
	modelDefined=true;
	return 1;
}

void improcessImage::CreateDoubleMatrix(double **&matrix,CvSize size)
{
	matrix = new double*[size.height];
	for(int iInd = 0; iInd < size.height; iInd++)
		matrix[iInd] = new double[size.width];
}

void improcessImage::ReleaseDoubleMatrix(double **&matrix,int size)
{
	for(int iInd = 0; iInd < size; iInd++) 
        delete[] matrix[iInd]; 
}
void GenerateGaussianKernel(int N ,int &Weight )
{
	int i ,j ;
	int SizeofKernel=N;
	float Kernel[15][15];
	float D1 ,D2 ;

	D1=1/(2*pi*Sigma*Sigma);
	D2=2*Sigma*Sigma;

	float min=1000;

	for(i=-SizeofKernel/2 ;i<= SizeofKernel/2 ;i++)
	{
		for( int j= -SizeofKernel/2 ; j<= SizeofKernel/2 ;j++)
		{
			 Kernel[SizeofKernel / 2 + i][ SizeofKernel / 2 + j] = ( (1 / D1) * (float)exp(-(i * i + j * j) / D2 ) );
			 if( Kernel[SizeofKernel / 2 + i][ SizeofKernel / 2 + j] < min )
                        min = Kernel[SizeofKernel / 2 + i][ SizeofKernel / 2 + j];

		}
	}
	int mult = (int)(1/min);
    int sum = 0;
	if( min>0 && min<1 )
	{
		for( i=-SizeofKernel/2 ; i<= SizeofKernel/2 ;i++)
		{
			for( j=-SizeofKernel/2 ;j<= SizeofKernel/2 ;j++)
			{
				Kernel[SizeofKernel/2+i][SizeofKernel/2+j]=(float)cvRound( Kernel[SizeofKernel/2+i][SizeofKernel/2+j]*mult);
				GaussianKernel[ SizeofKernel/2+i ][SizeofKernel/2+j]=(int)Kernel[SizeofKernel/2+i][SizeofKernel/2+j];
				sum=sum+GaussianKernel[SizeofKernel/2+i][SizeofKernel/2+j];
			}
		}
	}
	else
	{
		sum=0 ;
		for( i=-SizeofKernel/2 ;i<=SizeofKernel/2 ;i++)
		{
			for( j=-SizeofKernel/2 ;j<=SizeofKernel/2 ;j++)
			{
				Kernel[SizeofKernel/2+i][SizeofKernel/2+j]=(float)cvRound(Kernel[SizeofKernel/2+i][SizeofKernel/2+j]);
				GaussianKernel[SizeofKernel/2+i][SizeofKernel/2+j]=(int)Kernel[SizeofKernel/2+i][SizeofKernel/2+j];
				sum=sum+GaussianKernel[SizeofKernel/2+i][SizeofKernel/2+j];
			}
		}
	}
	Weight=sum;
}

void GaussianFilter( IplImage* gray,IplImage* out ,int KernelSize)
{
	GenerateGaussianKernel(KernelSize ,KernelWeight );
	uchar* Data=(uchar* )(gray->imageData);
	uchar* OutData=(uchar* )(out->imageData);

	int i ,j , k ,l ;
	int Limit=KernelSize/2 ;
	float sum=0 ;
	
	for( i=Limit ; i<= (gray->width-1)-Limit ;i++)
	{
		for( int j=Limit ;j<(gray->height-1)-Limit ;j++)
		{
			sum=0;
			for( k= -Limit ;k< Limit ;k++)
			{
				for( l=-Limit ;l<=Limit ;l++)
				{
					sum=sum + ( (float)Data[(j+k)*gray->widthStep+(i+l)*gray->nChannels]*GaussianKernel[Limit+k][Limit+l]);
				}
			}
			OutData[j*out->widthStep+i*out->nChannels]=(int)cvRound(sum/(float)KernelWeight) ;
		}
	}
}

// draw contour at template image
void improcessImage::DrawContours(IplImage* source,CvScalar color,int lineWidth)
{
	CvPoint point;
	for(int i=0; i<noOfCordinates; i++)
	{
		point.y=cordinates[i].x + centerOfGravity.x;
		point.x=cordinates[i].y + centerOfGravity.y;
		cvLine(source,point,point,color,lineWidth);
	}
}

IplImage* improcessImage::dilateGray(IplImage* grayImage)
{
	IplImage* dilateImage=cvCreateImage(cvGetSize(grayImage),8,1);
	
	int max=0;
	int gt=0;

	int kernel[5][5]={ {1,1,1},
	                   {1,2,1},
	                   {1,1,1} };

	//cout<<(int)kernel[1][1];
	
	for(int i=1; i< grayImage->height-1 ; i++)
	{
		for(int j=1; j< grayImage->width-1 ; j++)
		{
			max=0;
			gt=0;

			for(int u=-1; u<=1 ; u++)
			{
				for(int v=-1; v<=1 ; v++)
				{
					gt=  kernel[u+1][v+1]+ 
						 (int)((uchar *)(grayImage->imageData + (i+u)*grayImage->widthStep))[(j+v)*grayImage->nChannels + 0];

					if( max <= gt ) max=gt;
				}
			}

			if( max >= 255 )
			{
				((uchar *)(dilateImage->imageData + i*dilateImage->widthStep))[j*dilateImage->nChannels + 0]=255;
			}
			
			if( max < 255 )
			{
				((uchar *)(dilateImage->imageData + i*dilateImage->widthStep))[j*dilateImage->nChannels + 0]=(uchar)max;
			}
		}
	}
	
	return dilateImage;
}

IplImage* improcessImage::erodeGray(IplImage* grayImage)
{
	IplImage* dilateImage=cvCreateImage(cvGetSize(grayImage),8,1);
	
	int max=0;
	int gt=0;

	int kernel[5][5]={ {1,1,1},
	                   {1,2,1},
	                   {1,1,1} };

	//cout<<(int)kernel[1][1];
	
	for(int i=1; i< grayImage->height-1 ; i++)
	{
		for(int j=1; j< grayImage->width-1 ; j++)
		{
			max=1000;
			gt=0;

			for(int u=-1; u<=1 ; u++)
			{
				for(int v=-1; v<=1 ; v++)
				{
					gt=  kernel[u+1][v+1]+ 
						 (int)((uchar *)(grayImage->imageData + (i+u)*grayImage->widthStep))[(j+v)*grayImage->nChannels + 0];

					if( max > gt ) max=gt;
				}
			}

			if( max >= 255 )
			{
				((uchar *)(dilateImage->imageData + i*dilateImage->widthStep))[j*dilateImage->nChannels + 0]=255;
			}
			
			if( max < 255 )
			{
				((uchar *)(dilateImage->imageData + i*dilateImage->widthStep))[j*dilateImage->nChannels + 0]=(uchar)max;
			}
		}
	}
	
	return dilateImage;
}

void improcessImage::increase(int window[],int n)
{
	int temp=0;
	for( int i=0 ;i<n ;i++)
	{
		for( int j=0 ;j<n ;j++)
		{
			if( window[i]<window[j] )
			{
				temp=window[i];
				window[i]=window[j];
				window[j]=temp;
			}
		}
	}
}

int improcessImage::sumPixel(int window[],int n)
{
	int sum=0;
	for(int i=0; i< n ; i++)
	{
		sum+=window[i];
	}
	return sum;
}

IplImage* improcessImage::medianFilter(IplImage* img,int n)
{
	int window[9];
	IplImage* outimg;
	outimg=cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);

	int proPixel=0;
	uchar *data ;
	uchar *Newdata ;
	int anpha=0;

	data=(uchar*)(img->imageData);
	Newdata=(uchar*)(outimg->imageData);

	for(int i=0 ;i<n;i++)
	{
		window[i]=0;
	}

	for( int i=1 ;i<img->height-1 ;i++)
	{
		for(int j=1 ;j<img->width-1 ;j++)
		{
			for( int k=0 ;k<3 ;k++)
			{
				Newdata[i*(outimg->widthStep)+j*(outimg->nChannels)+k]=0;
				window[0]=(int)data[(i-1)*(img->widthStep)+ (j-1)*(img->nChannels)+k];
				window[1]=(int)data[(i-1)*(img->widthStep)+ (j)*(img->nChannels)+k];
				window[2]=(int)data[(i-1)*(img->widthStep)+ (j+1)*(img->nChannels)+k];
				window[3]=(int)data[(i)*(img->widthStep)+ (j-1)*(img->nChannels)+k];
				window[4]=(int)data[(i)*(img->widthStep)+ (j)*(img->nChannels)+k];
				window[5]=(int)data[(i)*(img->widthStep)+ (j+1)*(img->nChannels)+k];
				window[6]=(int)data[(i+1)*(img->widthStep)+ (j-1)*(img->nChannels)+k];
				window[7]=(int)data[(i+1)*(img->widthStep)+ (j)*(img->nChannels)+k];
				window[8]=(int)data[(i+1)*(img->widthStep)+ (j+1)*(img->nChannels)+k];

				increase(window,n);

				proPixel=window[4];

				if( ( data[i*(img->widthStep)+ j*(img->nChannels)+k]-window[4])<=anpha )
				{
					Newdata[i*(outimg->widthStep)+j*(outimg->nChannels)+k]=data[(i)*(img->widthStep)+ (j)*(img->nChannels)+k];
				}
				
				else 
				{
					Newdata[i*(outimg->widthStep)+j*(outimg->nChannels)+k]=(uchar)proPixel;
				}
			}
		}
	}
	return outimg;
}

IplImage* improcessImage::meanFilter(IplImage* img,int n)
{
	int window[9];
	IplImage* outimg;
	outimg=cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);

	uchar *data ;
	uchar *Newdata ;
	data=(uchar*)(img->imageData);
	Newdata=(uchar*)(outimg->imageData);

	int sum=0;

	for(int i=0 ;i<n;i++)
	{
		window[i]=0;
	}

	for( int i=1 ;i<img->height-1 ;i++){
		for(int j=1 ;j<img->width-1 ;j++){
			for( int k=0 ;k<3 ;k++){
				Newdata[i*(outimg->widthStep)+j*(outimg->nChannels)+k]=0;

				window[0]=(int)data[(i-1)*(img->widthStep)+ (j-1)*(img->nChannels)+k];
				window[1]=(int)data[(i-1)*(img->widthStep)+ (j)*(img->nChannels)+k];
				window[2]=(int)data[(i-1)*(img->widthStep)+ (j+1)*(img->nChannels)+k];
				window[3]=(int)data[(i)*(img->widthStep)+ (j-1)*(img->nChannels)+k];
				window[4]=(int)data[(i)*(img->widthStep)+ (j)*(img->nChannels)+k];
				window[5]=(int)data[(i)*(img->widthStep)+ (j+1)*(img->nChannels)+k];
				window[6]=(int)data[(i+1)*(img->widthStep)+ (j-1)*(img->nChannels)+k];
				window[7]=(int)data[(i+1)*(img->widthStep)+ (j)*(img->nChannels)+k];
				window[8]=(int)data[(i+1)*(img->widthStep)+ (j+1)*(img->nChannels)+k];

				sum=sumPixel(window,n);

				sum=sum/9;

				Newdata[i*(outimg->widthStep)+j*(outimg->nChannels)+k]=(uchar)sum;
			}
		}
	}
	return outimg;
}

IplImage* improcessImage::convolution(IplImage* image, float window[][3] )
{
	IplImage* output_image=cvCreateImage(cvGetSize(image),8,3);
	float sum=0;

	for(int i=1; i< image->height-1 ; i++){
		for(int j=1; j< image->width -1 ; j++ ){
			for(int k=0; k< 3 ; k++){
				sum=0;
				for(int u=-1; u<=1 ; u++){
					for(int v=-1; v<=1 ; v++){
						sum+=(float)((uchar *)(image->imageData + (i+u)*image->widthStep))[(j+v)*image->nChannels + k]*(float)window[u+1][v+1];}
				}
				((uchar *)(output_image->imageData + i*output_image->widthStep))[j*output_image->nChannels + k]=(uchar)(int)sum;
			}
		}
	}

	return output_image;
}

IplImage* improcessImage::LPF(IplImage* image)
{
	IplImage* LPF_image=cvCreateImage(cvGetSize(image),8,3);

	float window[3][3]={    {0,0.125,0},
	                     {0.125,0.5,0.125},
	                        {0,0.125,0}     };

	LPF_image = convolution(image,window);

	return LPF_image;
}

IplImage* improcessImage::HPF(IplImage* image)
{
	IplImage* HPF_image=cvCreateImage(cvGetSize(image),8,3);

	float window[3][3]={   {0,-0.25,0},
	                     {-0.25,2,-0.25},
	                       {0,-0.25,0}     };

	HPF_image = convolution(image,window);

	return HPF_image;
}


int improcessImage::computeOutput(int x, int r1, int s1, int r2, int s2)
{
    float result;
    if(0 <= x && x <= r1){
        result = s1/r1 * x;
    }else if(r1 < x && x <= r2){
        result = ((s2 - s1)/(r2 - r1)) * (x - r1) + s1;
    }else if(r2 < x && x <= 255){
        result = ((255 - s2)/(255 - r2)) * (x - r2) + s2;
    }

    return (int)result;
}

IplImage* improcessImage::stretch_contract(IplImage* image)
{
	IplImage* output_image=cvCreateImage(cvGetSize(image),8,3);

	int r1, s1, r2, s2;
	r1=70; s1=0;
	r2=140; s2=255;

	int color=0;

	for(int i = 0; i < image->height; i++){
		for(int j = 0; j < image->width; j++){
            for(int k = 0; k < 3; k++){

				color = (int)((uchar *)(image->imageData + i*image->widthStep))[j*image->nChannels + k];

                int output = computeOutput(color, r1, s1, r2, s2);

				if( output < 0 ) ((uchar *)(output_image->imageData + i*output_image->widthStep))[j*output_image->nChannels + k]=0;

				if( output > 255 ) ((uchar *)(output_image->imageData + i*output_image->widthStep))[j*output_image->nChannels + k]=255;

                if( output <= 255 && output >= 0 ) 
					((uchar *)(output_image->imageData + i*output_image->widthStep))[j*output_image->nChannels + k]=(uchar)output;
            }
        }
    }

	return output_image;
}
