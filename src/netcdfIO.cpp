#include "netcdfIO.h"

/*
void open_netcdf(slicer *E){
    
    if(nc_open(E->input_file, NC_WRITE | NC_SHARE, &E->ncid))
        fail("netcdf file %s not found\n", E->input_file);
    
}

void close_netcdf(slicer *E){
    
    if(nc_close(E->ncid))
        fail("netcdf file %s could not be closed!\n", E->input_file);
    
}
*/

void ncio::get_data(){
    // printf("bathy filename = %s\n", E->b.fname);
    // printf("lat_name = %s\n", E->b.lat_name);

    // got the file info, lets read it into memory
    int ncid;
    int varid;
    int retval;


    // open the file
    if((retval = nc_open(fname.c_str(), NC_NOWRITE, &ncid)))
        ERR(retval);

    // get the lat dimension sizes
    if((retval = nc_inq_dimid(ncid, lat_name.c_str(), &varid)))
        ERR(retval);

    if((retval = nc_inq_dimlen(ncid,varid,&nlat)))
        ERR(retval);

    //printf("nlat = %d\n", E->b.nlat);

    // get the lon dimension sizes
    if((retval = nc_inq_dimid(ncid, lon_name.c_str(), &varid)))
        ERR(retval);

    if((retval = nc_inq_dimlen(ncid,varid,&nlon)))
        ERR(retval);
    
    //cout << "nlats = " << nlat << endl;
    //cout << "nlons = " << nlon << endl;

    // printf("nlon = %d\n", E->b.nlon);

    // malloc room for the arrays
    //E->bathy_lat = malloc(E->b.nlat*sizeof(double));
    //E->bathy_lon = malloc(E->b.nlon*sizeof(double));
    //E->bathy_field = malloc2d_double(E->b.nlat, E->b.nlon);
    
    lat.resize((int)nlat);
    lon.resize((int)nlon);
    //field.resize((int)nlat, vector<float>((int)nlon));
    field.resize((int)nlat*(int)nlon);
    //cout << "done malloc" << endl;
    
    // read the data
    nc_inq_varid(ncid, lat_name.c_str(), &varid);
    nc_get_var_float(ncid, varid, &lat[0]);

    nc_inq_varid(ncid, lon_name.c_str(), &varid);
    nc_get_var_float(ncid, varid, &lon[0]);

    nc_inq_varid(ncid, field_name.c_str(), &varid);
    //cout << " getting field: " << field_name.c_str() << endl;
    //nc_get_var_float(ncid, varid, &field[0][0]);
    nc_get_var_float(ncid, varid, &field[0]);
}