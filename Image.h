#ifndef IMAGE_H_INCLUDE
#define IMAGE_H_INCLUDE

/*
 *read a pgm file and parse the pgm format and return the min, avg, max pixel value
 *
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <cctype>
#include <string>
#include <math.h>

using namespace std;

class Image{
public:

    /* public Base Image Methods*/
     int parseImage(const string& inputFile, string type);
     int readHeader(ifstream& istr, int headerValueCount, int isFirstLine, string type);

    /*ASCII PGM file*/
     int readP2Pixels(ifstream& istr);
     int scaleP2Image(ofstream& ofs);

    /*Binary PGM file*/
     int readP5Pixels(ifstream& istr);
     int scaleP5Image(ofstream& ofs);

    /*ASCII PPM file*/
     int readP3Pixels(ifstream& istr);

     /*Binary PPm file*/
     int readP6Pixels(ifstream& istr);

     int writeFileWithTypeAndOfs(const string& type,ofstream& ofs);

     /*Edges of Image*/
     int EdgesCalculator();

     /*PPM to PGM
        precondition: a PPM image read in and no format error
     */
     int PPM2PGM();

     unsigned int width=0;
     unsigned int height=0;
     unsigned int upperLimit=0;
     unsigned int maxPixel=0;
     unsigned int minPixel=0;
     vector<unsigned int> resultVector={};
     vector<unsigned int> pixels={};
     vector<unsigned int> PPMpixels={};
     vector<vector<unsigned int>> pixelValues={};


private:
    /*Edge calculation*/
     vector<vector<int>> IHorizontal={};
     vector<vector<int>> IVertical={};
     vector<vector<unsigned int>> E={};



    /*Helper Methods*/
     int pixelsOneToTwoDimension();
     int pixelsTwoToOneDimension();
};
/*non-class helper methods*/
int edgeValueCalculator(const vector<vector<unsigned int>>& pixelValues, int x,int y,int direction);
int theNumberOfDigitInLine(string str);
bool findExtralIllegalCharFromTheRestOfFile(ifstream& sReader);
string newLineAfterSkipEmptyLines(ifstream& sReader);
int parseALineOfPiexlsToVector(const unsigned int& width,const unsigned int& pixelValueLimit,const string& str,vector<unsigned int>& pixels);
bool checkIllegalChar(const string& str);
int parsePixelValueUpperLimit(const string& str,unsigned int &upperLimit);
int parseHeightAndWidth(const string& str,unsigned int &height,unsigned int &width);
string newLineAfterSkipEmptyAndCommentedLines(ifstream& sReader);
bool p2Validator(const string& str);
bool firstLineWhitespaceValidator(const string& str);
int warning(const string& warningMsg);
int formatError(const string& msg);

#endif // IMAGE_H_INCLUDE














