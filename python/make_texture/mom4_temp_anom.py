from mpl_toolkits.basemap import Basemap
from mpl_toolkits.basemap import shiftgrid,addcyclic
import numpy as np
import numpy.ma as ma
import math
import matplotlib.pyplot as plt
import matplotlib as mpl
import os, sys, commands
import netCDF4 as nc
import colormaps

def region_plot(fname, time, output_dir, julday):

	ncfile = nc.Dataset(fname,'r')

	fig = plt.figure(figsize=(20,10))
	ax = plt.axes([0,0,1,1],frameon=False)
	ax.set_axis_off()

	# set up a Basemap
	latrng = (-90.0,90.0)
	lonrng = (0,360)

	m = Basemap(projection='cyl',llcrnrlat=latrng[0],urcrnrlat=latrng[1],
        llcrnrlon=lonrng[0],urcrnrlon=lonrng[1],resolution='h',
        area_thresh=800,fix_aspect=False)

	lats = ncfile.variables["yt_ocean"][:]
	lons = ncfile.variables["xt_ocean"][:]
	field = ncfile.variables["temp"][time,0,:,:]

	aveNcfile = nc.Dataset('ocean_temp_annual.nc','r')
	average = aveNcfile.variables["temp"][0,0,:,:]
	
	field = field - average

	field = ma.array(field)
	field = ma.masked_where(field == -1.0e10, field)
	field = ma.masked_where(field<= -10,field)
	field = ma.masked_where(field>= 100, field)

	field, lons = addcyclic(field, lons)

	x,y = m(*np.meshgrid(lons[:],lats[:]))

	# this is the blue to orange colormap used for the eta anomaly textures
	colorMap = colormaps.make_colormap({0.:'#191970',  0.2:'#448BB9',  0.35:'#1e90ff', 0.5:'w', 0.6:'#F8BC4E', 0.7:'#ffa500', 0.8:'#ff8c00',0.9:'#F56D2E',  1.0:'#CC4000'}) 
	#colorMap =  colormaps.make_colormap({0.:'#010233', 0.3:'#01034B',0.4:'#02035F',0.42:'#28297B', 0.45:'#3B3D8E', 0.5:'w', 0.55:'#B32222',0.58:'#731515',0.6:'#600202',0.7:'#4B0202', 1.0:'#330101'})  # red to blue

	m.drawmapboundary(fill_color='white')
	nlevs = 256 
	clevs = np.linspace(-8,8,num=nlevs)
	
	cs = m.contourf(x,y,field, levels=clevs, cmap=colorMap, extend='both', norm=mpl.colors.normalize(),antialiased=False )
	m.fillcontinents(color='black', lake_color='black', zorder=1)

    save_file = "%s/%f.png" % (output_dir,julday)
	plt.savefig(save_file)
	
	ncfile.close()


# main entry point
file_name = sys.argv[1]
time = int(sys.argv[2])
output = sys.argv[3]
julday = float(sys.argv[4])
fig = plt.figure(linewidth=0,frameon=False)

region_plot(file_name,time, output,julday)

