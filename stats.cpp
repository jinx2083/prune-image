#include "stats.h"
#include <cmath>

using namespace std;
using namespace cs221util;

// the stats matrix is [y][x]


long stats::getSum(char channel, pair<int, int> ul, pair<int, int> lr) {


    //case 1: when ul is (0,0)
    if (ul.first == 0 && ul.second ==0){
        switch(channel) {
            case 'r' :
                return sumRed[lr.first][lr.second];
            case 'g' :
                return sumGreen[lr.first][lr.second];
            case 'b' :
                return sumBlue[lr.first][lr.second];
            default:
                break;
        }
    }

    //case 2: when ul is (a,0)
    if (ul.second ==0 && ul.first != 0){
        switch(channel) {
            case 'r' :
                return sumRed[lr.first][lr.second]- sumRed[ul.first-1][lr.second];
            case 'g' :
                return sumGreen[lr.first][lr.second] - sumGreen[ul.first-1][lr.second];
            case 'b' :
                return sumBlue[lr.first][lr.second] - sumBlue[ul.first-1][lr.second];
            default:
                break;
        }
    }

    //case 3: when ul is (0,b)
    if (ul.second !=0 && ul.first == 0){
        switch(channel) {
            case 'r' :
                return sumRed[lr.first][lr.second]- sumRed[lr.first][ul.second-1];
            case 'g' :
                return sumGreen[lr.first][lr.second] - sumGreen[lr.first][ul.second-1];
            case 'b' :
                return sumBlue[lr.first][lr.second] - sumBlue[lr.first][ul.second-1];
            default:
                break;
        }
    }

        //case 4: when ul is (a,b)  (a,b >0)
    else if (ul.first !=0 && ul.second != 0){
        switch(channel) {
            case 'r' :
                return sumRed[lr.first][lr.second] - sumRed[ul.first-1][lr.second] - sumRed[lr.first][ul.second-1] + sumRed[ul.first-1][ul.second-1];
            case 'g' :
                return sumGreen[lr.first][lr.second] - sumGreen[ul.first-1][lr.second] - sumGreen[lr.first][ul.second-1] + sumGreen[ul.first-1][ul.second-1];
            case 'b' :
                return sumBlue[lr.first][lr.second] - sumBlue[ul.first-1][lr.second] - sumBlue[lr.first][ul.second-1] + sumBlue[ul.first-1][ul.second-1];
            default:
                break;
        }
    }
    return 0;
}

long stats::getSumSq(char channel, pair<int, int> ul, pair<int, int> lr) {

//case 1: when ul is (0,0)
    if (ul.first == 0 && ul.second ==0){
        switch(channel) {
            case 'r' :
                return sumsqRed[lr.first][lr.second];
            case 'g' :
                return sumsqGreen[lr.first][lr.second];
            case 'b' :
                return sumsqBlue[lr.first][lr.second];
            default:
                break;
        }
    }

//case 2: when ul is (a,0)
    if (ul.second ==0 && ul.first != 0){
        switch(channel) {
            case 'r' :
                return sumsqRed[lr.first][lr.second]- sumsqRed[ul.first-1][lr.second];
            case 'g' :
                return sumsqGreen[lr.first][lr.second] - sumsqGreen[ul.first-1][lr.second];
            case 'b' :
                return sumsqBlue[lr.first][lr.second] - sumsqBlue[ul.first-1][lr.second];
            default:
                break;
        }
    }

//case 3: when ul is (0,b)
    if (ul.second !=0 && ul.first == 0){
        switch(channel) {
            case 'r' :
                return sumsqRed[lr.first][lr.second]- sumsqRed[lr.first][ul.second-1];
            case 'g' :
                return sumsqGreen[lr.first][lr.second] - sumsqGreen[lr.first][ul.second-1];
            case 'b' :
                return sumsqBlue[lr.first][lr.second] - sumsqBlue[lr.first][ul.second-1];
            default:
                break;
        }
    }

        //case 4: when ul is (a,b)  (a,b >0)
    else if (ul.first !=0 && ul.second != 0){
        switch(channel) {
            case 'r' :
                return sumsqRed[lr.first][lr.second] - sumsqRed[ul.first-1][lr.second] - sumsqRed[lr.first][ul.second-1] + sumsqRed[ul.first-1][ul.second-1];
            case 'g' :
                return sumsqGreen[lr.first][lr.second] - sumsqGreen[ul.first-1][lr.second] - sumsqGreen[lr.first][ul.second-1] + sumsqGreen[ul.first-1][ul.second-1];
            case 'b' :
                return sumsqBlue[lr.first][lr.second] - sumsqBlue[ul.first-1][lr.second] - sumsqBlue[lr.first][ul.second-1] + sumsqBlue[ul.first-1][ul.second-1];
            default:
                break;
        }
    }
    return 0;

}

stats::stats(PNG &im) {

    twoDVectorGenerator(im);

    //fillin the data of (0,0)
    sumRed[0][0] = im.getPixel(0,0)->r;
    sumBlue[0][0] = im.getPixel(0,0)->b;
    sumGreen[0][0] = im.getPixel(0,0)->g;
    sumsqRed[0][0] = im.getPixel(0,0)->r * im.getPixel(0,0)->r;
    sumsqBlue[0][0] = im.getPixel(0,0)->b * im.getPixel(0,0)->b;
    sumsqGreen[0][0] = im.getPixel(0,0)->g * im.getPixel(0,0)->g;



    //fill in the y=0 column
    //fill in the x column
    for (unsigned int x=0;x<im.width();x++){
        for (unsigned int y=0;y<im.height();y++){

            if (x==0 && y==0){
                continue;
            }
            if (y==0){

                sumRed[x][y] = sumRed[x-1][y] + im.getPixel(x,y)->r;
                 sumBlue[x][y] =  sumBlue[x-1][y] + im.getPixel(x,y)->b;
                 sumGreen[x][y] =  sumGreen[x-1][y] + im.getPixel(x,y)->g;
                 sumsqRed[x][y] = (im.getPixel(x,y)->r*im.getPixel(x,y)->r) + sumsqRed[x-1][y] ;
                sumsqBlue[x][y] = (im.getPixel(x,y)->b*im.getPixel(x,y)->b) + sumsqBlue[x-1][y] ;
                sumsqGreen[x][y] = (im.getPixel(x,y)->g*im.getPixel(x,y)->g) + sumsqGreen[x-1][y] ;

            }
            else if (x==0){

                 sumRed[x][y] =  sumRed[x][y-1] + im.getPixel(x,y)->r;
                 sumBlue[x][y] =  sumBlue[x][y-1] + im.getPixel(x,y)->b;
                 sumGreen[x][y] =  sumGreen[x][y-1] + im.getPixel(x,y)->g;
                 sumsqRed[x][y] = (im.getPixel(x,y)->r)*(im.getPixel(x,y)->r) + sumsqRed[x][y-1] ;
                 sumsqBlue[x][y] = (im.getPixel(x,y)->b)*(im.getPixel(x,y)->b) + sumsqRed[x][y-1] ;
                 sumsqGreen[x][y] = (im.getPixel(x,y)->g)*(im.getPixel(x,y)->g) + sumsqRed[x][y-1] ;


            }
            else {
                sumRed[x][y] = im.getPixel(x,y)->r + sumRed[x][y-1] + sumRed[x-1][y] - sumRed[x-1][y-1];
                sumBlue[x][y] = im.getPixel(x, y)->b + sumBlue[x - 1][y] + sumBlue[x][y - 1] - sumBlue[x - 1][y - 1];
                sumGreen[x][y] = im.getPixel(x, y)->g + sumGreen[x - 1][y] + sumGreen[x][y - 1] - sumGreen[x - 1][y - 1];
                sumsqRed[x][y] = (im.getPixel(x, y)->r * im.getPixel(x,y)->r) + sumsqRed[x-1][y] + sumsqRed[x][y -1] - sumsqRed[x-1][y-1];
                sumsqBlue[x][y] = (im.getPixel(x, y)->b * im.getPixel(x,y)->b) + sumsqBlue[x-1][y] + sumsqBlue[x][y -1] - sumsqBlue[x-1][y-1];
                sumsqGreen[x][y] = (im.getPixel(x, y)->g * im.getPixel(x,y)->g) + sumsqGreen[x-1][y] + sumsqGreen[x][y -1] - sumsqGreen[x-1][y-1];

            }

        }
    }
}

long stats::getScore(pair<int, int> ul, pair<int, int> lr) {
    //todo
    long red = getSumSq('r',ul,lr) - (getSum('r',ul,lr)* getSum('r',ul,lr) / rectArea(ul,lr));
    long blue = getSumSq('b',ul,lr) - (getSum('b',ul,lr)* getSum('b',ul,lr) / rectArea(ul,lr));
    long green = getSumSq('g',ul,lr) - (getSum('g',ul,lr)* getSum('g',ul,lr) / rectArea(ul,lr));
    return red + blue + green;
}

RGBAPixel stats::getAvg(pair<int, int> ul, pair<int, int> lr) {
    //todo
    if(ul==lr){
        RGBAPixel p1((int) getSum('r',ul,lr),getSum('g',ul,lr),getSum('b',ul,lr));
        return p1;
    }
    else {
        RGBAPixel p2((int) getSum('r', ul, lr) / rectArea(ul, lr), (int) getSum('g', ul, lr) / rectArea(ul, lr),
                     (int) getSum('b', ul, lr) / rectArea(ul, lr));
        return p2;
    }
}


long stats::rectArea(pair<int, int> ul, pair<int, int> lr) {
        return (lr.first - ul.first +1)*(lr.second - ul.second +1);
}

void stats::twoDVectorGenerator(PNG &im) {
    for (unsigned int i=0;i<im.width();i++) {

        vector<long> myRed(im.width());
        vector<long> myBlue(im.width());
        vector<long> myGreen(im.width());
        vector<long> mysqRed(im.width());
        vector<long> mysqBlue(im.width());
        vector<long> mysqGreen(im.width());
        for (unsigned int j = 0; j < im.height(); j++) {
            myRed.push_back(-1);
            myBlue.push_back(-1);
            myGreen.push_back(-1);
            mysqRed.push_back(-1);
            mysqBlue.push_back(-1);
            mysqGreen.push_back(-1);
        }
        sumRed.push_back(myRed);
        sumGreen.push_back(myGreen);
        sumBlue.push_back(myBlue);
        sumsqRed.push_back(mysqRed);
        sumsqGreen.push_back(mysqGreen);
        sumsqBlue.push_back(mysqBlue);
    }
}