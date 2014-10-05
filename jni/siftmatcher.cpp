#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>
#include<cmath>
#include<algorithm>
#include<vector>
using namespace std;

extern "C" {
#include "generic.h"
#include "sift.h"
#include "kdtree.h"
}
#include "Image.h"
#include "Exception.h"

#include "siftmatcher.h"
#include "android_log.h"


	////////////////Constructor/////////////////////
	siftmatcher::siftmatcher()
	{
		testDescriptors = new float[DESCRIPTOR_DIMENSION*MAXDESCRIPTORS];
		libraryDescriptors = new float[DESCRIPTOR_DIMENSION*MAXDESCRIPTORS*NUMTESTS];
		numTestDescriptors = 0;
		numLibraryDescriptors = 0;
	}

	/////////////////////////////////////////////////////

	////////////////Deconstructor/////////////////////
	siftmatcher::~siftmatcher()
	{
		delete testDescriptors;
		delete libraryDescriptors;
		vl_kdforest_delete(forest);
	}

	/////////////////////////////////////////////////////


	///////////////////processTestImage//////////////////
	void siftmatcher::processTestImage( string imageName )
	{
		Image testI;
		vl_sift_pix* testGray = initializeImage( imageName, testI );			//Initialize the first image
		numTestDescriptors = sift(testDescriptors, testGray, testI);				//sift and store the descriptors in testDescr, and return the # of descriptors
		normalize(testDescriptors, numTestDescriptors);				//normalize the descriptor values
	}

	void siftmatcher::processTestImage( unsigned char* data )
	{
		Image testI;
		vl_sift_pix* testGray = initializeImage( data, testI );			//Initialize the first image

		LOGD("Initialized image dims: %i x %i", testI.getHeight(), testI.getWidth());

		numTestDescriptors = sift(testDescriptors, testGray, testI);				//sift and store the descriptors in testDescr, and return the # of descriptors
		normalize(testDescriptors, numTestDescriptors);				//normalize the descriptor values
	}

	/////////////////////////////////////////////////////

	//////////////////////buildForest////////////////////
	void siftmatcher::buildForest(const double** descrArray, const int* descrSizes)
	{
		forest = vl_kdforest_new(DESCRIPTOR_DIMENSION, 1);
		forest->thresholdingMethod = VL_KDTREE_MEDIAN;
		uploadAllDescriptors(descrArray, descrSizes);
		vl_kdforest_build(forest, numLibraryDescriptors, libraryDescriptors);
	}

	/////////////////uploadDescriptors////////////////
	void siftmatcher::uploadAllDescriptors(const double** descrArray, const int* descrSizes)
	{
		int totalDescriptors = 0;
		matchDelimeter[0] = 0;
		for( int i=0; i<NUMTESTS; i++ )
		{
			localizeDescriptor( libraryDescriptors+totalDescriptors*DESCRIPTOR_DIMENSION, descrArray[i], descrSizes[i] );		//Read the descriptors in from a file
			totalDescriptors += descrSizes[i];
			matchDelimeter[i+1] = totalDescriptors;
		}
		numLibraryDescriptors = totalDescriptors;
	}

	/////////////////////////////////////////////////////
	
	void siftmatcher::localizeDescriptor( float* ResultDescript, const double* descr, int size )
	{
		for(int i = 0; i<size; i++)
		{
			for(int k = 0; k<DESCRIPTOR_DIMENSION; k++ )
			{
				ResultDescript[i*DESCRIPTOR_DIMENSION+k] = descr[i*DESCRIPTOR_DIMENSION+k];
			}
		}
	}

	/////////////////////findMatches/////////////////////
	void siftmatcher::findMatches(int* matches)
	{
		//vl_kdforest_query runs a query on a SINGLE descriptor and returns the nearest neighbors
		_VlKDForestNeighbor* neighbors = new _VlKDForestNeighbor[numTestDescriptors];
		//_VlKDForestNeighbor* neighborMatches[1000];
		for( int i=0; i<NUMTESTS; i++ )
			matches[i] = 0;
		for( int i=0; i<numTestDescriptors; i++ )
		{
			vl_kdforest_query(forest, neighbors, NUM_CLOSEST_NEIGHBORS, testDescriptors+i*DESCRIPTOR_DIMENSION);
			int numNeighborsMatched = 0;
			for( int k=0; k<NUM_CLOSEST_NEIGHBORS; k++)		//If descriptor matched too many neighbors(libImages), ignore it
			{
				if( neighbors[k].distance < THRESHOLD)
					numNeighborsMatched++;
			}
			if( numNeighborsMatched < NUM_CLOSEST_NEIGHBORS )
			{
				for( int k=0; k<NUM_CLOSEST_NEIGHBORS; k++){
					if( neighbors[k].distance < THRESHOLD)
					{
						//neighborMatches[matches] = *(neighbors+i);
						for( int j=0; j<NUMTESTS; j++ )
							if(neighbors[k].index >= matchDelimeter[j] && neighbors[k].index < matchDelimeter[j+1] )	//Find out which lib image the match descriptor belonged to
								matches[j]++;
					}
				}
			}
		}
		delete [] neighbors;
		//delete [] neighborMatches;
	}

vl_sift_pix* siftmatcher::initializeImage( string name, Image& image )
{
	ifstream ifs(name.c_str(), ios_base::in | ios_base::binary) ;
    if(!ifs) {
      throw Exception("Could not open a file") ;
    }
	ifs>>image;
	vl_sift_pix* gray = new vl_sift_pix[image.getHeight()*image.getWidth()];
	convertToGrayscale(image, gray );
	return gray;
}

vl_sift_pix* siftmatcher::initializeImage( unsigned char* data, Image& image )
{
	stringstream sstr((const char*)data, ios_base::in | ios_base::binary) ;

	sstr>>image;
	vl_sift_pix* gray = new vl_sift_pix[image.getHeight()*image.getWidth()];
	convertToGrayscale(image, gray );
	return gray;
}
void siftmatcher::normalize( float* descr, int numDescr )
{
	for ( int i=0; i < numDescr*DESCRIPTOR_DIMENSION; i++ )			//Normalize the values
	{
		float x = 512.f * descr[i];
		x = (x < 255.f) ? x : 255.0 ;
		descr[i] = x;
	}
}

void siftmatcher::convertToGrayscale(Image img, vl_sift_pix *gray )
{
	if( img.getDataSize() == img.getHeight()*img.getWidth() )		//already grayscale, since pixelsize = 1
		for ( int i=0; i<img.getHeight()*img.getWidth(); i++ )
			gray[i] = (float)*(img.getDataPt()+i);
	else															//converting rgb to grayscale
		for ( int i=0; i < img.getHeight()*img.getWidth(); i++ )
			gray[i] = (float)( 0.3 * *(img.getDataPt()+3*i) + 0.59 * *(img.getDataPt()+3*i+1) + 0.11 * *(img.getDataPt()+3*i+2) );
}


int siftmatcher::sift( float* d, float* pixels, Image I )				//returns the number of keypoints
{
	VlSiftFilt *sift = vl_sift_new(I.getWidth(),I.getHeight(),-1,3,0);
	vl_sift_process_first_octave(sift, pixels) ;			//process over the first octave
	int count = 0;
	do
	{
		vl_sift_detect(sift);
		const VlSiftKeypoint* key = vl_sift_get_keypoints(sift);
		for(int i=0; i<sift->nkeys; i++)
		{
			double angles[4];
			int numOrients = vl_sift_calc_keypoint_orientations(sift,angles,key+i);
			for(int j=0;j<numOrients; j++)
			{
				vl_sift_calc_keypoint_descriptor(sift,(d+DESCRIPTOR_DIMENSION*count),key+i,angles[j]);		//calculate the descriptor
				count++;
			}
		}
	}while( vl_sift_process_next_octave(sift) != VL_ERR_EOF );		//iterate over the rest of the octaves
	vl_sift_delete(sift);
	return count;
}


void siftmatcher::outputLibraryDescriptorsHeader( string* testNames)
{
	Image testIms[NUMTESTS];
	vl_sift_pix* gray[NUMTESTS];
	int counts[NUMTESTS];
	float* libraryDescrs = new float[DESCRIPTOR_DIMENSION*MAXDESCRIPTORS];
	string outputArrNames[NUMTESTS];
	
	//open the file
	ofstream myfile;
	myfile.open("descriptors.h");

	//output the header file protector
	string ifdefString = "SIFT_DESCRIPTORS_H";
	myfile << "#ifndef "<< ifdefString << endl;
	myfile << "#define "<< ifdefString << endl << endl;

	//output the individual descriptor arrays
	for( int i=0; i < NUMTESTS; i++ )
	{
		gray[i] =  initializeImage( testNames[i], testIms[i] );
		convertToGrayscale(testIms[i], gray[i] );

		counts[i] = sift( libraryDescrs, gray[i], testIms[i]);				//sift and store the descriptors, and return the # of descriptors
		normalize(libraryDescrs, counts[i]);				//Normalize the values of the descriptors

		string outFile = testNames[i];
		outFile.resize(outFile.size()-4);
		outputArrNames[i] = "descr" + outFile;
		outputSingleDescriptor(myfile, libraryDescrs, outputArrNames[i], counts[i] );
	}
	myfile << endl << endl;

	//output the 2D descriptors data array
	myfile << "const double* DescriptorsArray[] = {";
	for( int i=0; i < NUMTESTS - 1; i++ )
	{
		myfile << outputArrNames[i] <<", ";
	}
	myfile << outputArrNames[NUMTESTS - 1] << " };" << endl << endl;

	//output the number of each of descriptors for each image
	myfile << "const int DESCRIPTOR_COUNTS[] = { ";
	for( int i=0; i < NUMTESTS - 1; i++ )
	{
		myfile << counts[i] <<", ";
	}
	myfile << counts[NUMTESTS - 1] << " };" << endl << endl;

	//output the endif
	myfile << "#endif" << endl;

	//close the file
	myfile.close();
}

void siftmatcher::outputSingleDescriptor( ofstream &myfile, float* Descript, string arrName, int size )
{
	myfile << "const double " << arrName << "[] = { ";

	//output the descriptor data array
	for(int i = 0; i < size*DESCRIPTOR_DIMENSION - 1 ; i++)
		myfile << Descript[i]<<", ";
	myfile << Descript[size*DESCRIPTOR_DIMENSION - 1] << " };" << endl;

}
