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

def region_plot(fname, fname2, time, output_dir, julday):

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

	lats = ncfile.variables["yu_ocean"][:]
	lons = ncfile.variables["xu_ocean"][:]
	u = ncfile.variables["u"][time,0,:,:]

	ncfile2 = nc.Dataset(fname2,'r')
	v = ncfile2.variables["v"][time,0,:,:]

	lons2 = lons

	u = ma.array(u)
	u = ma.masked_where(u <= -10.0, u)
	u = ma.masked_where(u >= 10.0, u)
	u = ma.masked_where(u == 0.0, u)

	u, lons = addcyclic(u, lons)

	v = ma.array(v)
	v = ma.masked_where(v <= -10.0, v)
	v = ma.masked_where(v >= 10.0, v)
	v = ma.masked_where(v == 0.0, v)

	v, lons2 = addcyclic(v, lons2)

	field = ((u**2 + v**2)**0.5)
	field = np.log(field)

	x,y = m(*np.meshgrid(lons[:],lats[:]))

	# cm.Blues looks pretty good
	colorMap = mpl.cm.Blues
	m.drawmapboundary(fill_color='white')
	nlevs = 256 
	clevs = np.linspace(-4.0,0.5,num=nlevs)
	
	cs = m.contourf(x,y,field, levels=clevs, cmap=colorMap, extend='both', norm=mpl.colors.normalize(),antialiased=False )
	m.fillcontinents(color='black', lake_color='black', zorder=1)
	save_file = "%s/%f.png" % (output_dir,julday)
	plt.savefig(save_file)
	
	ncfile.close()


# main entry point
file_name = sys.argv[1]
file_name2 = sys.argv[2]
time = int(sys.argv[3])
output = sys.argv[4]
julday = float(sys.argv[5])
fig = plt.figure(linewidth=0,frameon=False)
plt.clf()
region_plot(file_name,file_name2, time, output,julday)

