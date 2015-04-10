from mpl_toolkits.basemap import Basemap
from mpl_toolkits.basemap import interp, shiftgrid,addcyclic
import numpy as np
import numpy.ma as ma
import math
import matplotlib.pyplot as plt
import matplotlib as mpl
import os, sys, commands
from pylab import gca 
import netCDF4 as nc
import colormaps

def region_plot(fname, time, output_dir, julday):

	ncfile = nc.Dataset(fname,'r')

	fig = plt.figure(figsize=(20.48,10.24))
	ax = plt.axes([0,0,1,1],frameon=False)
	ax.set_axis_off()

	# set up a Basemap
	latrng = (-90.0,90.0)
	lonrng = (0,360)

	m = Basemap(projection='cyl',llcrnrlat=latrng[0],urcrnrlat=latrng[1],
        llcrnrlon=lonrng[0],urcrnrlon=lonrng[1],resolution='h',
        area_thresh=800,fix_aspect=False)

	# for salinity
	lats = ncfile.variables["yt_ocean"][:]
	lons = ncfile.variables["xt_ocean"][:]
	field = ncfile.variables["salt"][time,0,:,:]

	field = ma.array(field)
	field = ma.masked_where(field == -1.0e10, field)
	field = ma.masked_where(field<= -10,field)
	field = ma.masked_where(field>= 100, field)

	field, lons = addcyclic(field, lons)

	#field_min = np.min(field) 
	#field_max = np.max(field)
	
	x,y = m(*np.meshgrid(lons[:],lats[:]))

	# do one with jet colormap
	colorMap = mpl.cm.jet
	
	m.drawmapboundary(fill_color='white')
	nlevs = 256
	clevs = np.linspace(28,38,num=nlevs)
	
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
plt.clf()
region_plot(file_name,time, output,julday)
