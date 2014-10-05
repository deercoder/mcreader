#include<string>
#include <stdio.h>
using namespace std;
#include<iostream>
#include<sstream>
#include<fstream>
#include<jni.h>
#include<android_log.h>

#include "siftmatcher.h"
#include "globals.h"
#include "descriptors.h"

#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

#ifdef __cplusplus
extern "C"
{
#endif


siftmatcher* matcher;

string libFileNames[NUMTESTS] = { "lib00.txt", "lib01.txt", "lib02.txt", "lib03.txt", "lib04.txt", "lib05.txt", "lib06.txt", "lib07.txt", "lib08.txt", "lib09.txt", "lib10.txt", "lib11.txt" };
string libPicNames[NUMTESTS] = { "lib00.ppm", "lib01.ppm", "lib02.ppm", "lib03.ppm", "lib04.ppm", "lib05.ppm", "lib06.ppm", "lib07.ppm", "lib08.ppm", "lib09.ppm", "lib10.ppm", "lib11.ppm" };

string findFirstName( string Filename );
int matchIndexToBill( int index );

int
Java_mcr_indep_MCRindepActivity_initializeCurrencyReader( JNIEnv* env, jobject obj )
{	
	LOGD("Initializing the currency reader.");
	matcher = new siftmatcher();

	//Only build the forest once
	matcher->buildForest(DescriptorsArray, DESCRIPTOR_COUNTS);

	return 1;
}

int
Java_mcr_indep_MCRindepActivity_processCurrencyImage( JNIEnv* env, jobject obj, jbyteArray jpeg, jint length )
{
	LOGD("Converting test image into ppm format.");

	//Decode the the jpeg image(could actually be an image of any format)
	unsigned char* cjpeg;
	cjpeg = (unsigned char*)env->GetByteArrayElements(jpeg,NULL);
	Mat jpegMat(1, length, CV_8U, cjpeg);
	Mat raw = imdecode(jpegMat, 1);
	LOGD("Decoded: %i x %i", raw.rows, raw.cols );

	//Resize the image to 300 x 400
	//Size newDims(300,400);
	Size newDims;
	newDims.width = 400;
	newDims.height = 300;
	Mat raw_resized(1, newDims.width * newDims.height, CV_8U);
	resize( raw, raw_resized, newDims,CV_INTER_AREA);
	if( raw_resized.empty() )
		LOGD("Raw Resized image is empty!");
	else
		LOGD("Decoded + Resized: %i x %i", raw_resized.rows, raw_resized.cols );

	//Encode the image into ppm
	vector<unsigned char> vppm;
	LOGD("Converting into ppm for siftmatcher input.");
	imencode(".ppm", raw_resized, vppm);
	LOGD("Blahhhh encoded.");
	unsigned char* ppm = new unsigned char[sizeof( int ) * vppm.size()];
	LOGD("Blahhhh blahdhhhdhadg encoded.");
	memcpy( ppm, &vppm[0], sizeof( int ) * vppm.size() );	//Crashing here on second image!!!!!!!!!!!!!

	//Process the converted ppm
	LOGD("Processing the converted test image.");
	matcher->processTestImage( ppm );

	//The data has been copied within the matcher... release the ByteArray
	env->ReleaseByteArrayElements(jpeg,(jbyte*)cjpeg,0);

	LOGD("Finding the matches.");
	int matches[NUMTESTS];
	matcher->findMatches(matches);
	int maxIndex = 0;
	LOGD("%i matches to library image %i.", matches[0], 0);
	for( int i=1; i<NUMTESTS; i++)
	{
		LOGD("%i matches to library image %i.", matches[i], i);
		if( matches[i] > matches[maxIndex] )
		{
			maxIndex = i;
		}
	}

	delete ppm;
	delete matcher;
	int result = matchIndexToBill( maxIndex );
	if( matches[maxIndex] == 0 )
		return -1;
	else
		return result;
}

void
Java_mcr_indep_MCRindepActivity_shutDownCurrencyReader( JNIEnv* env, jobject obj )
{
	LOGD("Deallocating currency reader resources.");

	delete matcher;

}


int matchIndexToBill( int index )
{
	switch( index )
	{
		case 0:
		case 1:
			return 1;
		case 2:
		case 3:
			return 5;
		case 4:
		case 5:
			return 10;
		case 6:
		case 7:
			return 20;
		case 8:
		case 9:
			return 50;
		case 10:
		case 11:
			return 100;
		default:
			return -1;
	}
	return -1;
}


#ifdef __cplusplus
}
#endif



/*
i_view32 "C:\Users\Alpha Dog\Documents\CurrencyReader\174B proj3\vlfeat-client\Pics\libraryPics\pp-mobile-currency-reader-read-only\*.jpg" /convert="C:\Users\Alpha Dog\Documents\CS 174C\indepCurrReader\indepCurrReader\TestImages\*.ppm"
*/
