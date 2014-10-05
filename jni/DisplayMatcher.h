#ifndef DISPLAY_SIFT_MATCHER_H
#define DISPLAY_SIFT_MATCHER_H

#include "globals.h"

#include "Image.h"
#include<string>

extern "C" {
#include "generic.h"
#include "sift.h"
#include "kdtree.h"
}



class DisplayMatcher
{
public:
	DisplayMatcher();
	~DisplayMatcher();

	void processTestImage( string imageName );
	void processLibraryImage(string libraryName);

	//Build the kdforest and add the data into the forest
	void buildForest();

	//Finds all the descriptor matches between the test image and each of the library images
	//matches is the output
	int findMatches();

	//Add the matches to the images themselves, and then display them
	void outputMatches(string testImageName, string libImageName);

	/////INTERNAL USE ONLY/////
	void normalize( float* descr, int numDescr );
	void convertToGrayscale(Image img, vl_sift_pix *gray );
	int sift( float* d, float* pixels, Image I, bool first );
	vl_sift_pix* initializeImage( string name, Image& image );

	void insertMarkers( Image& I, int* xyPairs, int numPairs);
	void outputImage(Image& testImage, string testImageName);

private:
	float* testDescriptors;
	float* libraryDescriptors;
	Image testImage;
	Image libImage;
	int numLibDescriptors;
	int numTestDescriptors;
	_VlKDForest* forest;
	
	int xyPairsTest[2*MAXDESCRIPTORS];
	int xyPairsLibrary[2*MAXDESCRIPTORS];

	int indexPairs[2*MAXDESCRIPTORS];
	int numMatches;
};








#endif