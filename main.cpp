#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <cctype>
#include <string>
#include <cmath>

#include <Image.h>
#define DEBUG 0
using namespace std;

int readPPMImageWithFile(Image& image,const string& fileName, string& imageType);
int readPGMImageWithFile(Image& image,const string& fileName, string& imageType);
int writeOutputWithPixelsAndType(const Image& image,const vector<unsigned int>& pixels,const unsigned int maxDiff,const string& type, ofstream& ofs);

int main(int argc, char* argv[]){
    Image image1;
    int imageResult;
    string imageType,firstImageType,outputImageType;
    string inputFile1, outputFile;

    if(argc!=3)
    {
        cerr<<"Missing or extra parameter!"<<endl;//checking the number of input files
        return -1;
    }

    inputFile1=argv[1];outputFile=argv[2];

    if(inputFile1.compare(outputFile)==0)
    {
        cerr<<"Output file cannot be same as one of the input files!"<<endl;//file name equal to each other
        return -1;
    }

    //read input file
    imageResult = readPPMImageWithFile(image1,inputFile1,imageType);

    if(imageResult==-1){
        return -1;
    }else if(imageResult==0){
        //image read successfully
        if((imageType[0]=='P')&&(imageType[1]=='3')){
            firstImageType="P3";
            outputImageType="P2";
        }else if((imageType[0]=='P')&&(imageType[1]=='6')){
            firstImageType="P6";
            outputImageType="P5";
        }
        //PPM to PGM
        imageResult = image1.PPM2PGM();

        if(imageResult==-1) return -1;

        ofstream ofs;
        ofs.open(outputFile);
        if(ofs.fail()){cerr<<"fail to open the output file"<<endl; return -1;}

        if(outputImageType.compare("P2")==0){
            return image1.writeFileWithTypeAndOfs("P2",ofs);
        }else if(outputImageType.compare("P5")==0){
            return image1.writeFileWithTypeAndOfs("P5",ofs);
        }else{
            cerr<<"Unknown output Image type"<<endl;
            return -1;
        }

    }else{
        cerr<<"unknown return value from readPPMImageWithFile()"<<endl;
        return -1;
    }
 /*
 legacy code from P5: after read a PGM file, calculate edges and output the edges.
    imageResult = readPGMImageWithFile(image1,inputFile1,imageType);
    if(imageResult==-1){
        return -1;
    }else if(imageResult==0){
        //first image read successfully
        //store the output type
        if((imageType[0]=='P')&&(imageType[1]=='2')){
            firstImageType="P2";
            outputImageType="P2";
        }else if((imageType[0]=='P')&&(imageType[1]=='5')){
            firstImageType="P5";
            outputImageType="P5";
        }

        //Calculate edge and convert to one dimensional pixel vector
        imageResult=image1.EdgesCalculator();
        if(imageResult==-1) return -1;

        ofstream ofs;
        ofs.open(outputFile);
        if(ofs.fail()){cerr<<"fail to open the output file"<<endl; return -1;}

        if(outputImageType.compare("P2")==0){
           return image1.scaleP2Image(ofs);
        }else if(outputImageType.compare("P5")==0){
            return image1.scaleP5Image(ofs);
        }else{
            cerr<<"unknown output Image type"<<endl;
            return -1;
        }
    }else{
        //TODO:
        cerr<<"unknown error"<<endl;
        return -1;
    }
*/
}

int readPPMImageWithFile(Image& image,const string& fileName, string& imageType){
    int imageResult;

    ifstream istr(fileName);
    if(istr.fail())
    {
        cerr<<"Unable to open one input file!"<<endl;
        return -1;
    }else{
        /* first file opened successfully*/
        getline(istr, imageType);
    }

    if((imageType[0]=='P')&&(imageType[1]=='6')){
        /*binary PPM*/
        istr.close();
        imageResult = image.parseImage(fileName,"P6");
        if(imageResult==-1){
            cerr<<"Fail to read the image provided successfully"<<endl;
            return -1;
        }else if(imageResult==0){
            /*successfully read an Binary PPM image*/
            return 0;
        }else{
            cerr<<"Unknown parseImage return value"<<endl;
            return -1;
        }

    }else if((imageType[0]=='P')&&(imageType[1]=='3')){
        /*ASCII PPM*/
        istr.close();
        imageResult = image.parseImage(fileName,"P3");

        if(imageResult==-1){
            cerr<<"Fail to read the image provided successfully"<<endl;
            return -1;
        }else if(imageResult==0){
            /*successfully read an ASCII PPM image*/
            return 0;
        }else{
            cerr<<"Unknown parseImage return value"<<endl;
            return -1;
        }
    }else{
        cerr<<"The first two characters are not P6, nor P3";
        return -1;
    }
    return 0;
}

int readPGMImageWithFile(Image& image,const string& fileName, string& imageType){
    int imageResult;

    ifstream istr(fileName);
    if(istr.fail())
    {
        cerr<<"Unable to open one input file!"<<endl;
        return -1;
    }else{
        /* first file opened successfully*/
        getline(istr, imageType);
    }

    if((imageType[0]=='P')&&(imageType[1]=='2')){
        /*ASCII format image*/
        istr.close();
        imageResult = image.parseImage(fileName,"P2");

        if(imageResult==-1){
            cerr<<endl<<"Fail to read the image provided successfully"<<endl;
            return -1;
        }else if(imageResult==0){
             /*successfully read a Ascii image*/
            return 0;
        }else{
                cerr<<endl<<"Unknown Error"<<endl;
                return -1;
                //impossible
        }
    }else if((imageType[0]=='P')&&(imageType[1]=='5')){
        istr.close();
        imageResult = image.parseImage(fileName,"P5");

        if(imageResult==-1){
            cerr<<endl<<"Fail to read the image provided successfully"<<endl;
            return -1;
        }else if(imageResult==0){
            /*successfully p[arsed a binary image*/
            return 0;
        }else{
            cerr<<endl<<"Unknown Error"<<endl;
            return -1;
            //impossible
        }

    }else{
        cerr<<"The first two characters are not P5, nor P2";
        return -1;
    }
    return 0;
}

int writeOutputWithPixelsAndType(const Image& image,const vector<unsigned int>& pixels,const unsigned int maxDiff,const string& type, ofstream& ofs){
    cout<<maxDiff;
    if(type.compare("P5")==0){

        char byteArray[pixels.size()];
        ofs<<"P5\n";
        ofs<<image.width;
        ofs<<" ";
        ofs<<image.height;
        ofs<<"\n";
        ofs<<255;
        ofs<<"\n";
        for(unsigned int i=0;i<pixels.size();i++){
            byteArray[i]=static_cast<char>(pixels[i]);
        }
        ofs.write(byteArray,pixels.size());
        if(ofs.fail()){
            cerr<<"Unable to write an output file"<<endl;
            return -1;
        }
        return 0;

    }else if(type.compare("P2")==0){
        unsigned int i=0,j=0,k=0;
        if(ofs.is_open()){
            ofs << "P2\n";
            ofs <<"\n";
            ofs << image.width;
            ofs << " ";
            ofs << image.height;
            ofs << "\n";
            ofs << 255;
            ofs << "\n";
            k=0;

            for(i=0;i<image.height;i++){
                for(j=0;j<image.width;j++){
                    if(!ofs.is_open()){
                         cerr<<"OfStream got interrupted, ofstream is not open"<<endl;
                         return -1;
                    }
                    ofs << pixels[k];
                    ofs << " ";
                    if(DEBUG){
                        cerr<<"Pixel :"<<pixels[k]<<" inserted"<<endl;
                    }
                    k++;
                }
                ofs<<"\n";
            }
            return 0;
        }else{
            cerr<<endl<<"Output file cannot be opened"<<endl;
            return -1;
        }

    }else
    {
    cerr<<"Unknown output type in write output func"<<endl;
    return -1;
    }

    return 0;
}

/*
*******************************************************************
    scaleImage test List:
*******************************************************************
    1.missing arguments( there should be two: first input file, second output file)
       1.1 fail to open first file

    2. fail to reading image
        2.1 no access
        2.2 format issue
            2.2.3 more than one digit height


    3  fail to write image
        3.0 create a new file if second argument is not existing
        3.1 no access to the existing image
*/

/*
*******************************************************************
    Read Image test List
*******************************************************************
    A)Image not properly formatted:
        1.first line: first two character of the file is not 'P2'(there is sth before it).
                      there is non-whites character following P2.
        2.skip one or more empty lines or lines with pure whitespaces or whitespaces concatenated with # , which is followed by any Char until the end of line
        3.expect width and height
        4. check 2 again.
        5.expect height
        5. check 2 but #is illegal now
        6. (only whitespace and digit legal)read pixels line by line
        6. end of file, with whitespaces ot empty lines with whitespaces
    3. begin with #,comment line after first line, before pixel values 4. multiple comment line


*/
