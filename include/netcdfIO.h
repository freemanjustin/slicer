#ifndef NETCDFIO_H
#define NETCDFIO_H

#include <netcdf.h>
#include <iostream>
#include <vector>

using namespace std;

#define ERR(e) {cerr << "Error in netcdf load" << endl;}

//void open_netcdf(slicer *E);
//void close_netcdf(slicer *E);


class ncio{
    public:
        string  fname;
        string  lat_name;
        string  lon_name;
        string  field_name;

        size_t nlat;
        size_t nlon;
        std::vector<double>  lat;
        std::vector<double>  lon;
        //std::vector< std::vector<float> >  field;
        std::vector<float> field;

        void get_data();
};
#endif
