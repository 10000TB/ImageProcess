#include <Image.h>
#define DEBUG 0

using namespace std;


/*Base Image Methods*/
int Image::parseImage(const string& inputFile, string type){
    char char1,char2;
    int returnValue;
    unsigned int headerValueCount=0;
    unsigned int minValue,maxValue;
    //bool debug;
    ifstream istr(inputFile);
    if(istr.fail()) return formatError("Problem reading the file");

    if(!(istr>>char1)) return formatError("Problem reading the first Character in file");
    else{
         if(char1==type[0]){
         }else{
            return formatError("first Character is not P");
         }
    }
    if(!(istr>>char2)) return formatError("Problem reading the second character in file");
    else{
         if(char2==type[1]){
            headerValueCount=1;
        }else{
            return formatError("second Character is not valid");
        }
    }
    //the rest of the first line in the first line
    if(type.compare("P2")==0 || type.compare("P5")==0){
        returnValue=readHeader(istr, headerValueCount,1,type);
        if(returnValue==-1){
            return -1;
        }else if(returnValue==0){
            minValue=pixels[0];maxValue=pixels[0];
            for(unsigned int i=0;i<pixels.size();i++){
                if(pixels[i]<minValue) minValue=pixels[i];
                if(pixels[i]>maxValue) maxValue=pixels[i];
            }
            minPixel = minValue;
            maxPixel = maxValue;
            return 0;
        }else{
            //unknown error
            return -1;
        }
    }else if(type.compare("P3")==0 || type.compare("P6")==0){
        returnValue=readHeader(istr, headerValueCount,1,type);
        if(returnValue==-1){

            return -1;
        }else if(returnValue==0){
            //read PPM successfully
            return 0;
        }else{
            return -1;
        }
    }
    return 0;
}

int Image::readHeader(ifstream& istr, int headerValueCount, int isFirstLine, string type){
    string newLine;
    stringstream ss;
    int numCount;

    newLine = "";
    if(isFirstLine==0){
        newLine = newLineAfterSkipEmptyAndCommentedLines(istr);
        if(newLine.compare("IllegalCharacter")==0){
              return formatError("illegal character(there exist non-digit character outside comment line(s))");
        }
        else if(newLine.compare("EOF")==0){
            return warning("Reaching the end of file before getting enough image data:(pixel value upper limit");
        }else if(newLine.compare("FailReadingLine")==0){
            return warning("Fail to read a new line to retrieve pixel value upper limit");
        }else{
          /*
            get a new valid line successfully
          */
        }
    }
    else{
        if(getline(istr,newLine)==0){
            return formatError("fail to read a newLine for header value");
        }
    }
    ss<<newLine;
    if(checkIllegalChar(newLine))
        return formatError("illegal Character in header");
    numCount=theNumberOfDigitInLine(newLine);

    if(headerValueCount==1){
        if(numCount==3){
                //all three numbers are at first line
                if(!(ss>>width)) return formatError("Problem reading width");
                if(!(ss>>height)) return formatError("Problem reading height");
                if(!(ss>>upperLimit)) return formatError("Problem reading upperLimit");
                if(width==0||height==0) return formatError("the Width of height is zero");
                if(upperLimit>255) return formatError("The declared max is over 255");
                headerValueCount+=3;

                if(type.compare("P5")==0){
                    if(readP5Pixels(istr)==0)
                        return 0;
                    else
                        return -1;
                }else if(type.compare("P2")==0){
                    if(readP2Pixels(istr)==0)
                        return 0;
                    else
                        return -1;
                }else if(type.compare("P3")==0){
                    if(readP3Pixels(istr)==0)
                        return 0;
                    else
                        return -1;
                }else if(type.compare("P6")==0){
                    if(readP6Pixels(istr)==0)
                        return 0;
                    else
                        return -1;
                }else{
                    return formatError("Unknown magic type");
                }
        }else if(numCount==2){
                //width and height are at first line
                if(!(ss>>width)) return formatError("Problem reading width");
                if(!(ss>>height)) return formatError("Problem reading height");
                if(width==0||height==0) return formatError("the Width or height is zero");
                headerValueCount+=2;
                return readHeader(istr,headerValueCount,0,type);
        }else if(numCount==1){
                //only width at first line
                if(!(ss>>width)) return formatError("Problem reading width");
                if(width==0) return formatError("the Width is zero");
                headerValueCount+=1;
                return readHeader(istr,headerValueCount,0,type);
        }else{
            if(isFirstLine==1){
                if(numCount==0){
                   return readHeader(istr,headerValueCount,0,type);
                }else
                    return formatError("there is extra number in header");
            }
            else
                return formatError("Missing Numbers or there is extra number in header");
        }
    }else if(headerValueCount==2){
        if(numCount==2){
            if(!(ss>>height)) return formatError("problem reading height");
            if(!(ss>>upperLimit)) return formatError("problem reading upperLimit");
            if(height==0) return formatError("the height is zero");
            if(upperLimit>255) return formatError("the declared max is over 255");
            headerValueCount+=2;

            if(type.compare("P5")==0){
                if(readP5Pixels(istr)==0)
                    return 0;
                else
                    return -1;
            }else if(type.compare("P2")==0){
                if(readP2Pixels(istr)==0)
                    return 0;
                else
                    return -1;
            }else if(type.compare("P3")==0){
                if(readP3Pixels(istr)==0)
                    return 0;
                else
                    return -1;
            }else if(type.compare("P6")==0){
                if(readP6Pixels(istr)==0)
                    return 0;
                else
                    return -1;
            }else{
                return formatError("Unknown magic type");
            }
        }else if(numCount==1){
            if(!(ss>>height)) return formatError("Problem reading height");
            if(height==0) return formatError("the height is zero");
            headerValueCount+=1;
            return readHeader(istr,headerValueCount,0,type);
        }else{
            return formatError("Missing Numbers or there is extra number in header");
        }
    }else if(headerValueCount==3){
        if(numCount==1){
            if(!(ss>>upperLimit)) return formatError("Problem reading upperLimit");
            if(upperLimit>255) return formatError("declared max is over 255");
            headerValueCount+=1;
            if(type.compare("P5")==0){
                if(readP5Pixels(istr)==0)
                    return 0;
                else
                    return -1;
            }else if(type.compare("P2")==0){
                if(readP2Pixels(istr)==0)
                    return 0;
                else
                    return -1;
            }else if(type.compare("P3")==0){
                if(readP3Pixels(istr)==0)
                    return 0;
                else
                    return -1;
            }else if(type.compare("P6")==0){
                if(readP6Pixels(istr)==0)
                    return 0;
                else
                    return -1;
            }else{
                return formatError("Unknown magic type");
            }
        }else{
            return formatError("Missing Numbers or there is extra number in header");
        }
    }else{
        return 0;
    //will no be executed here
    //since when headerValueCount==4 , this function will  not be called
    }
    return 0;
}

int Image::readP2Pixels(ifstream& istr){

        int parsedResult;
        string newLine;
        if(getline(istr,newLine)==0)
            return formatError("fail to read first line of pixels");
        else{
            parsedResult = parseALineOfPiexlsToVector(width,upperLimit,newLine,pixels);
            switch(parsedResult){
                case 8: return formatError("Illegal character(in pixel matrix)");
                case 9: return formatError("Found negative pixel value");
                case 10: return formatError("Found pixel value that is greater than upper limit");
                //    case 11: return formatError("The number of pixel value in a line is SMALLER than width");
                //    case 12: return formatError("The number of pixel value in a line is GREATER than width");
                case 0: //successfully parsed a line of pixel values
                default: break;
            }
        }

        if(pixels.size()==width*height)
        {
            istr.close();
            return 0;
        } //height value is 1, only have one line of pixels, parse finished

        while(pixels.size()<width*height){
            newLine.clear();
            newLine = newLineAfterSkipEmptyLines(istr);
            if(newLine.compare("EOF")==0){
                return formatError("the number of pixels not equal to width*height!");
            }else if(newLine.compare("FailReadingLine")==0){
                return formatError("the number of pixels not equal to width*height!");
            }else if(newLine.compare("IllegalCharacter")==0){
                return formatError("Illegal Character in a line in pixels matrix");
            }else{
                parsedResult = parseALineOfPiexlsToVector(width,upperLimit,newLine,pixels);
                switch(parsedResult){
                    case 8: return formatError("Illegal character(in pixel matrix)");
                    case 9: return formatError("Found negative pixel value");
                    case 10: return formatError("Found pixel value that is greater than upper limit");
                    case 0: //successfully parsed a line of pixel values
                    default: break;
                }
            }
        }
        //TODO:
        //here should check if reach the end of the file
        //
        while(!istr.eof()){
            if(findExtralIllegalCharFromTheRestOfFile(istr)){
                return formatError("There is extra character after all the lines in pixel matrix");
            }
        }
        if(height*width == pixels.size())
        {
            istr.close();
            return 0;
        }else{
            return formatError("Total number of pixel values do not equal to Width*Height");
        }
}

int Image::scaleP2Image(ofstream& ofs){
     unsigned int i=0,j=0,k=0;
     if(ofs.is_open()){
            ofs << "P2\n";
            ofs <<"\n";
            ofs << width;
            ofs << " ";
            ofs << height;
            ofs << "\n";
            ofs << 255;
            ofs << "\n";
            k=0;
            if(maxPixel==minPixel){
                return warning("The Image Cannot be scaled(MaxPixel equal to minPixel)");
            }
            for(i=0;i<height;i++){
                for(j=0;j<width;j++){
                    if(!ofs.is_open()){
                        return warning("OfStream got interrupted, ofstream is not open");
                    }
                    ofs << (int)round((float)255*(float)(pixels[k]-minPixel)/(float)(maxPixel-minPixel));
                    ofs << " ";
                    if(DEBUG){
                        cerr<<"Pixel :"<<((int)round(255*(float)(pixels[k]-minPixel)/(float)(maxPixel-minPixel)))<<" inserted"<<endl;
                    }
                    k++;
                }
                ofs<<"\n";
            }
     }else{
         cerr<<endl<<"Output file cannot be opened"<<endl;
         return -1;
     }
     return 0;
}

int Image::readP3Pixels(ifstream& istr){
        int parsedResult;
        string newLine;
//        PPMpixels.reserve(width*height*3);
        if(getline(istr,newLine)==0)
            return formatError("fail to read first line of pixels");
        else{
            parsedResult = parseALineOfPiexlsToVector(width,upperLimit,newLine,PPMpixels);
            switch(parsedResult){
                case 8: return formatError("Illegal character(in pixel matrix)");
                case 9: return formatError("Found negative pixel value");
                case 10: return formatError("Found pixel value that is greater than upper limit");
                //    case 11: return formatError("The number of pixel value in a line is SMALLER than width");
                //    case 12: return formatError("The number of pixel value in a line is GREATER than width");
                case 0: //successfully parsed a line of pixel values
                default: break;
            }
        }

        if(PPMpixels.size()==3*width*height)
        {
            istr.close();
            return 0;
        } //height value is 1, only have one line of pixels, parse finished

        while(PPMpixels.size()<3*width*height){
            newLine.clear();
            newLine = newLineAfterSkipEmptyLines(istr);
            if(newLine.compare("EOF")==0){
                return formatError("the number of pixels not equal to width*height!");
            }else if(newLine.compare("FailReadingLine")==0){
                return formatError("the number of pixels not equal to width*height!");
            }else if(newLine.compare("IllegalCharacter")==0){
                return formatError("Illegal Character in a line in pixels matrix");
            }else{
                parsedResult = parseALineOfPiexlsToVector(width,upperLimit,newLine,PPMpixels);
                switch(parsedResult){
                    case 8: return formatError("Illegal character(in pixel matrix)");
                    case 9: return formatError("Found negative pixel value");
                    case 10: return formatError("Found pixel value that is greater than upper limit");
                    case 0: //successfully parsed a line of pixel values
                    default: break;
                }
            }
        }
        //TODO:
        //here should check if reach the end of the file
        //
        while(!istr.eof()){
            if(findExtralIllegalCharFromTheRestOfFile(istr)){
                return formatError("There is extra character after all the lines in pixel matrix");
            }
        }
        if(3*height*width == PPMpixels.size())
        {
            istr.close();
            cerr<<"finished reading P3";
            return 0;
        }else{
            return formatError("Total number of pixel values do not equal to Width*Height");
        }
}

int Image::readP6Pixels(ifstream& istr){
    //cerr<<"width:"<<width<<"  height:"<<height;
    const unsigned int tot = width * height*3;
    unsigned int i=0,k=0;


    char temp[1];

    PPMpixels.clear();
    PPMpixels.reserve(tot);
    for(k=0;k<3;k++){
        for(i=0;i<height;i++){
            char byteArray[width+1];
            istr.read(byteArray,width);
            if(istr.fail()) return formatError("Could not read binary pixel");
            byteArray[width]='\0';

            for(unsigned int j=0; j < width; j++){
                unsigned int curPix = static_cast<int>(static_cast<unsigned char>(byteArray[j]));
                if (curPix > upperLimit) return formatError("Pixel value invalid");
                PPMpixels.push_back(curPix); // save to vector
            //cerr<<"PPMpixels.size()="<<PPMpixels.size()<<endl;
            //cout<<i<<' '<<curPix<<endl;
            }

        }
    }
 //   istr.read(byteArray, tot); // read all bytes to this array, one byte for each pixel
 //   if(istr.fail())
 //   byteArray[tot] = '\0';
 //   PPMpixels.clear();
 //   PPMpixels.reserve(tot);

//   for(unsigned int i=0; i < tot; i++){
//        unsigned int curPix = static_cast<int>(static_cast<unsigned char>(byteArray[i]));
//        if (curPix > upperLimit) return formatError("Pixel value invalid");
//        PPMpixels.push_back(curPix); // save to vector
        //cerr<<"PPMpixels.size()="<<PPMpixels.size()<<endl;
        //cout<<i<<' '<<curPix<<endl;
//    }
    //cerr<<"total pixels:"<<PPMpixels.size()<<" tot:"<<tot<<endl;

   // cout<<"PPMpixels.size()="<<PPMpixels.size()<<" tot="<<tot<<endl;

    if(PPMpixels.size()==tot)
        return 0;
    else
        return formatError("The Number of pixels is not equal to 3*Width*height");

    istr.read(temp, 1);
    temp[1]='\0';
    if(!istr.eof()){
        cerr<<"extra binary pixels";
        return -1;
    }else
        return 0;
}

int Image::readP5Pixels(ifstream& istr){
    const unsigned int tot = width * height;
    unsigned int i=0;

    char temp[1];

    PPMpixels.clear();
    PPMpixels.reserve(tot);
    for(i=0;i<height;i++){
        char byteArray[width+1];
        istr.read(byteArray,width);
        if(istr.fail()) return formatError("Could not read binary pixel");
        byteArray[width]='\0';

        for(unsigned int j=0; j < width; j++){
            unsigned int curPix = static_cast<int>(static_cast<unsigned char>(byteArray[j]));
            if (curPix > upperLimit) return formatError("Pixel value invalid");
            PPMpixels.push_back(curPix); // save to vector
            //cerr<<"PPMpixels.size()="<<PPMpixels.size()<<endl;
            //cout<<i<<' '<<curPix<<endl;
        }

    }

    if(pixels.size()==tot)
        return 0;
    else
        return formatError("The Number of pixels is not equal to Width*height");

    istr.read(temp, 1);
    temp[1]='\0';
    if(!istr.eof()){
        cerr<<"extra binary pixels";
        return -1;
    }else{
        return 0;
    }
}

int Image::scaleP5Image(ofstream& ofs){
    //ofstream ofs(file);
    //char byteArray[pixels.size()];
    unsigned int temValue;
    //long pos;
    if(ofs.fail()) return formatError("Unable to write an output file");
    ofs<<"P5\n";
    ofs<<width;
    ofs<<" ";
    ofs<<height;
    ofs<<"\n";
    ofs<<255;
    ofs<<"\n";
 //   pos=ofs.tellp();
 //   ofs.close();
 //   ofstream ofs(file);
 //   ofs.seekp(pos);
    unsigned int j=0,k=0;
    for(unsigned int i=0;i<height;i++){
        k=0;char byteArray[width];
        for(k=0;k<width;k++){
            temValue = (unsigned int)round((float)255*(float)(pixels[j]-minPixel)/(float)(maxPixel-minPixel));
            byteArray[k]=static_cast<char>(temValue);
            j++;
        }
        ofs.write(byteArray,width);
        if(ofs.fail()) return formatError("Unable to write an output file");
    }


    //for(unsigned int i=0;i<pixels.size();i++){
    //    temValue=(unsigned int)round((float)255*(float)(pixels[i]-minPixel)/(float)(maxPixel-minPixel));
    //    byteArray[i]=static_cast<char>(temValue);
    //}
    //ofs.write(byteArray,pixels.size());
    //if(ofs.fail()) return formatError("Unable to write an output file");

    return 0;
}

/* write image with type and ofs*/
int Image::writeFileWithTypeAndOfs(const string& type,ofstream& ofs){
    unsigned int i=0, j=0, k=0;

    if(type.compare("P2")==0){
        if(ofs.is_open()){
            ofs << "P2\n";
            ofs <<"\n";
            ofs << width;
            ofs << " ";
            ofs << height;
            ofs << "\n";
            ofs << upperLimit;
            ofs << "\n";
            k=0;
            for(i=0;i<height;i++){
                for(j=0;j<width;j++){
                    if(!ofs.is_open()){
                        return warning("OfStream got interrupted, ofstream is not open");
                    }
                    ofs<<pixels[k];
                    ofs<<" ";
                    k++;
                }
                ofs<<"\n";
            }
        }else{
            cerr<<endl<<"Output file cannot be opened"<<endl;
            return -1;
        }

    }else if(type.compare("P5")==0){
        if(ofs.fail()) return formatError("Unable to write an output file");
        ofs<<"P5\n";
        ofs<<width;
        ofs<<" ";
        ofs<<height;
        ofs<<"\n";
        ofs<<upperLimit;
        ofs<<"\n";

        unsigned int j=0,k=0;
        for(unsigned int i=0;i<height;i++){
            k=0;char byteArray[width];
            for(k=0;k<width;k++){
                byteArray[k]=static_cast<char>(pixels[j]);
                j++;
            }
            ofs.write(byteArray,width);
            if(ofs.fail()) return formatError("Unable to write an output file");
        }

 //       for(unsigned int i=0;i<pixels.size();i++){
               // cerr<<"pixel: "<<pixels[i]<<endl;
//            byteArray[i]=static_cast<char>(pixels[i]);
//        }
//        ofs.write(byteArray,pixels.size());

    }
    return 0;
}

/*Image Edges*/
int Image::EdgesCalculator(){
/*pre-condition: already got image data in pixles*/
    unsigned int i=0,j=0;
    vector<int> tempHVector;
    int tempHValue;
    vector<int> tempVVector;
    int tempVValue;
    vector<unsigned int> tempEVector;
    unsigned int tempEValue;
    /*get Image Data into [][] vector*/
    pixelsOneToTwoDimension();

    /*calculate vertical and horizontal edges*/
    for(i=0;i<pixelValues.size();i++)
    {
        tempEVector.clear();
        tempHVector.clear();
        tempVVector.clear();
        for(j=0;j<pixelValues[i].size();j++){
            tempHValue = edgeValueCalculator(pixelValues, i, j, 0);
            tempHVector.push_back(tempHValue);
            tempVValue = edgeValueCalculator(pixelValues, i, j, 1);
            tempVVector.push_back(tempVValue);
            tempEValue = round(sqrt(tempHValue*tempHValue+tempVValue*tempVValue));
            if(tempEValue<minPixel) minPixel=tempEValue;
            if(tempEValue>maxPixel) maxPixel=tempEValue;
            tempEVector.push_back(tempEValue);
           // cerr<<"pixel value:"<<pixelValues[i][j]<<"  Ihorizontal:"<<tempHValue<<" IVerticalValue:"<<tempVValue<<"  E:"<<round(sqrt(tempHValue*tempHValue+tempVValue*tempVValue))<<endl;
        }
        IHorizontal.push_back(tempHVector);
        IVertical.push_back(tempVVector);
        E.push_back(tempEVector);
          //  cerr<<"Here";
    }
    //updated max min value

    if(minPixel==maxPixel){
        cerr<<"minPixel and maxPixel in calculated image are equal!, cannot be rescaled!"<<endl;
        return -1;
    }

    pixelsTwoToOneDimension();

    return 0;
}

/*PPM 2 PGM*/
int Image::PPM2PGM(){
    /*PPM image read in pixels in PPMpixels and calculated into pixels*/
    unsigned int i=2;
    unsigned int PPMpixelSize=PPMpixels.size();
    pixels.clear();
    pixels.reserve(PPMpixelSize/3);
    for(i=2;i<PPMpixelSize;i=i+3){
        pixels.push_back(round((PPMpixels[i-2]+PPMpixels[i-1]+PPMpixels[i])/3));
    }
    return 0;
}



/*
 Helper methods
*/

/*
    direction==0 horizontal
    direction==1 vertical
*/
int edgeValueCalculator(const vector<vector<unsigned int>>& I, int x,int y,int direction){
    int result=0;
    int width=I[0].size();
    int height=I.size();
    if(x==0 && y==0){
        if(direction==0){
               result = 0 + 2*I[x+1][y] + I[x+1][y+1] - 0 - 2*0 - 0;
        }else if(direction==1){
               result = 0 + 2*I[x][y+1] + I[x+1][y+1] - 0 - 2*0 - 0;
        }else{
            cerr<<"Direction is neither horizontal, nor vertical"<<endl;
            return -1;
        }
    }else if(x==0 && y!=0){
        if(direction==0){
            if(y==width-1)
            {
                result = I[x+1][y-1] + 2*I[x+1][y] + 0 - 0 - 2*0 - 0;
            }else
            {
                result = I[x+1][y-1] + 2*I[x+1][y] + I[x+1][y+1] - 0 - 2*0 - 0;
            }
        }else if(direction==1){
            if(y==width-1)
            {
                result = 0 + 0 + 0 - 0 - 2*I[x][y-1] - I[x+1][y-1];
            }else
            {
                result = 0 + 2*I[x][y+1] + I[x+1][y+1] - 0 - 2*I[x][y-1] - I[x+1][y-1];
            }
        }else{
            cerr<<"Direction is neither horizontal, nor vertical"<<endl;
            return -1;
        }

    }else if(x!=0 && y==0){
        if(direction==0){
            if(x==height-1)
            {
            result = 0 + 0 + 0 - 0 - 2*I[x-1][y] - I[x-1][y+1];
            }else{
            result = 0 + 2*I[x+1][y] + I[x+1][y+1] - 0 - 2*I[x-1][y] - I[x-1][y+1];
            }
        }else if(direction==1){
            if(x==height-1)
            {
            result = I[x-1][y+1] + 2*I[x][y+1] - 0 - 2*0 - 0;
            }else
            {
            result = I[x-1][y+1] + 2*I[x][y+1] + I[x+1][y+1] - 0 - 2*0 - 0;
            }
        }else{
            cerr<<"Direction is neither horizontal, nor vertical"<<endl;
            return -1;
        }
    }else if(x!=0 && y!=0){
        /*x!=0 and y!=0*/
        if(direction==0){
            if((x==height-1) && (y==width-1)){
                result = 0+0+0-I[x-1][y-1] - 2*I[x-1][y]-0;
            }else if((x==height-1) && (y!=width-1))
            {
                result = 0 + 0 + 0 - I[x-1][y-1] - 2*I[x-1][y] - I[x-1][y+1];
            }else if((x!=height-1)&&(y==width-1))
            {
                result = I[x+1][y-1] + 2*I[x+1][y] + 0 - I[x-1][y-1] - 2*I[x-1][y] - 0;
            }else{
                result = I[x+1][y-1] + 2*I[x+1][y] + I[x+1][y+1] - I[x-1][y-1] - 2*I[x-1][y] - I[x-1][y+1];
            }

        }else if(direction==1){
            if((x==height-1) && (y==width-1)){
                result = 0 + 0 + 0 - I[x-1][y-1] - 2*I[x][y-1] - 0;
            }else if((x==height-1) && (y!=width-1))
            {
                result = I[x-1][y+1] + 2*I[x][y+1] + 0 - I[x-1][y-1] - 2*I[x][y-1] - 0;
            }else if((x!=height-1)&&(y==width-1))
            {
                result = 0 + 0 + 0 - I[x-1][y-1] - 2*I[x][y-1] - I[x+1][y-1];
            }else{
                result = I[x-1][y+1] + 2*I[x][y+1] + I[x+1][y+1] - I[x-1][y-1] - 2*I[x][y-1] - I[x+1][y-1];
            }

        }else{
            cerr<<"Direction is neither horizontal, nor vertical"<<endl;
            return -1;
        }
    }else{
        cerr<<"unknown error with index x and y"<<endl;
        return -1;
    }

    return result;
}

int Image::pixelsOneToTwoDimension(){
    unsigned int i=0,j=0,counter=0;
    vector<unsigned int> line={};
    /*clear vector before adding new elements*/
    pixelValues.clear();

    for(i=0;i<height;i++)
    {
        line.clear();
        for(j=0;j<width;j++){
            line.push_back(pixels[counter]);
            counter++;
        }
        pixelValues.push_back(line);
    }
    return 0;
}
int Image::pixelsTwoToOneDimension(){
    unsigned int i=0,j=0;
    /*clear vector before adding new things*/
    pixels.clear();

    for(i=0;i<E.size();i++){
        for(j=0;j<E[i].size();j++){
            pixels.push_back(E[i][j]);
        }
    }
    return 0;
}

int theNumberOfDigitInLine(string str){
    unsigned int i=0,digitCount=0;
    for(;i<str.length();i++){
        if(isdigit(str[i])){
            digitCount++;
            while(isdigit(str[i])&&(i<str.length())){
                i++;
            }
            if(i>=str.length()) return digitCount;
            else    i--;
        }
    }
    return digitCount;
}

bool findExtralIllegalCharFromTheRestOfFile(ifstream& sReader){
     string tempStr;
     unsigned int i=0;
     if(getline(sReader,tempStr)!=0)
     {
         for(i=0;i<tempStr.size();i++){
            if(!isspace(tempStr[i])){
                return true;
            }
         }
     }
     return false;
}

string newLineAfterSkipEmptyLines(ifstream& sReader){
      unsigned int i=0;
      string tempStr;
      if(sReader.eof()) return "EOF";
      if(getline(sReader,tempStr)==0)
        return "FailReadingLine";
      while((i<tempStr.size())&&(isspace(tempStr[i]))){
           i++;
      }
      if(i>=tempStr.size()){
           return newLineAfterSkipEmptyLines(sReader);
      }else{
          if(!isdigit(tempStr[i])){
              return "IllegalCharacter";
          }else{
              return tempStr;
          }
      }
}

int parseALineOfPiexlsToVector(const unsigned int& width,const unsigned int& pixelValueLimit,const string& str,vector<unsigned int>& pixels){
      unsigned int counter=0,pixelValue;
      istringstream iss(str);
      //
      //check if there is any illegal character in pixels matrix lines
      if(checkIllegalChar(str))
         return  8; //Illegal character outside the comment line

      while(iss>>pixelValue){
            if(pixelValue<0) return 9;//found negative pixel value;
            if(pixelValueLimit<=255)
            {
                if(pixelValue>pixelValueLimit) return 10;//found pixel value greater than upper limit
            }else{
                if(pixelValue>255) return 10; //found pixel value greater than upper limit
            }
            counter++;
            pixels.push_back(pixelValue);
      }
      // Here
      // TODO: ? check what is the last character when exit the while loop
      //
      //if(counter<width)
      //    return 11;// the number of pixel value in this line is smaller than width;
      //if(counter>width)
      //    return 12;// the number of pixel value in this line is bigger than width;
      return 0; //successfully parsed all pixel values
}

bool checkIllegalChar(const string& str){
      unsigned int i=0;
      for(i=0;i<str.size();i++){
           if((!isspace(str[i]))&&(!isdigit(str[i]))){
                 return true; //Illegal character outside the comment line
           }
      }
      return false;
}

int parsePixelValueUpperLimit(const string& str,unsigned int &upperLimit){
     unsigned int i=0;
     string tempStr;
     while((i<str.size())&&(isspace(str[i]))){
           i++;
     }
     if(i>=str.size()){ return 5;}//"EXPECT_PIXEL_UPPER_LIMIT"
     if(!isdigit(str[i])){ return 6;}//"ILLEGAL_CHARACTER_OUTSIDE_COMMENT"
     tempStr+=str[i];
     i++;
     while((i<str.size())&&(isdigit(str[i]))){
           tempStr+=str[i];
           i++;
     }
     if(i>=str.size()){
            upperLimit = stoi(tempStr);
            return 0;} // "parsed successfully"
     if(!isspace(str[i])) return 6;//"ILLEGAL_CHARACTER_OUTSIDE_COMMENT"
     while(i<str.size()){
          if(!isspace(str[i])){
            return 7;//"EXTRA_CHARACTER_IN_MAX_PIXEL_LINE"
          }
          i++;
     }
     upperLimit = stoi(tempStr);
     return 0;// "parsed successfully"
}

int parseHeightAndWidth(const string& str,unsigned int &height,unsigned int &width){
    unsigned int i=0;
    string tempStr;
    while((i<str.size())&&(!isdigit(str[i]))){
         i++;
    }
    if(i>=str.size()) return 1;//"EMPTY_LINE_EXPECT_HEIGHT_WIDTH"
    tempStr+=str[i];
    i++;
    while((i<str.size())&&(isdigit(str[i]))){
        tempStr+=str[i];
        i++;
    }
    if(i>=str.size()) return 2;//"EXPECT_SECOND_NUMBER"
    if(!isspace(str[i])){return 3;}//"ILLEGAL_CHARACTER"
    width=stoi(tempStr);
    while((i<str.size())&&isspace(str[i])){
        i++;
    }
    if(i>=str.size()) return 2;//"EXPECT_SECOND_NUMBER"
    if(!isdigit(str[i])) return 3;//"ILLEGAL_CHARACTER"
    tempStr="";
    tempStr+=str[i];
    i++;
    while((i<str.size())&&(isdigit(str[i]))){
        tempStr+=str[i];
        i++;
    }
    if(i>=str.size()){
        height=stoi(tempStr);
        return 0;//"PARSED"
    }else{
        if(!isspace(str[i])){
            return 3;//"ILLEGAL_CHARACTER"
        }else{
           i++;
           while(i<str.size()){
               if(!isspace(str[i])){
                    return 4;//"EXTRA_CHARACTER_FOR_HEIGHT_WIDTH_LINE"
               }
               i++;
           }
        height=stoi(tempStr);
        return 0;//"PARSED"
        }
    }
}

string newLineAfterSkipEmptyAndCommentedLines(ifstream& sReader){
    unsigned int i=0;
    string tempStr;
    if(sReader.eof()) return "EOF";

    if(!getline(sReader,tempStr)){

        return "FailReadingLine";
        }

    while((i<tempStr.size())&&(isspace(tempStr[i]))){
        i++;
    }
    if(i>=tempStr.size()){
        return newLineAfterSkipEmptyAndCommentedLines(sReader);
    }else{
        if(tempStr[i]=='#'){
             return newLineAfterSkipEmptyAndCommentedLines(sReader);
        }else{
             if(isdigit(tempStr[i])){
                   return tempStr;
             }else{
                 return "IllegalCharacter";
             }
        }
    }

}

bool p2Validator(const string& str){
   if(str.size()<2) return false;
   if((str[0]=='P')&&(str[1]=='2')){
         if(str.size()==2) return true;
         if(isspace(str[2])){
              if(firstLineWhitespaceValidator(str))
              {
                 return true;
              }else
              {
                  formatError("after the first white-space after P2, there are at least one non-white-space character before the end of the line");
                  return false;
              }
         }else{
               formatError("P2 is not immediately followed by a white-space");
               return false;
         }
   }else{
         formatError("first two character are not the combination of 'P' and '2'");
         return false;
   }
}

bool firstLineWhitespaceValidator(const string& str){
    unsigned int i=3;
    if(str.size()==3){
        return true;
    }else{
        while(i<str.size()){
            if(!isspace(str[i])) return false;
            i++;
        }
        return true;
    }
}

int warning(const string& warningMsg){
   cerr<<"Warning: "<<warningMsg<<endl;
   return -1;
}

int formatError(const string& msg){
    cerr<<"Error: "<< msg<<endl;
    return -1;
}
