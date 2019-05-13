import shutil, os
import time
import argparse
import sys

curr_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

mplabx_path = "C:/Program Files (x86)/Microchip/MPLABX/v5.15/"

if __name__ == '__main__':

	##########################################################################
	# Argument manipulation
	##########################################################################
	parser = argparse.ArgumentParser(description='''This script updates bin ATPL360 bin file to atpl/bin/folder''')
	parser.add_argument('--h3path', required=False, help='Harmony 3 absolute path: [c:\\MH3]')
	parser.add_argument('--profile', required=True, help='profiles: [G3_PHY], [G3_MAC_RT], [G3_ALL], [PRIME], [ALL]')
	args = parser.parse_args()
	
	time_start = time.time()
	
	profile = 'ALL'
	
	if args.profile is not None:
		profile = args.profile
		
	if args.h3path is not None:
		h3path = args.h3path
		
	print("Searching projects...")
	
	plcpath = h3path + "\\plc\\apps"
	mhcpath = h3path + "\\mhc"	
			
	plcfolders = [x[0] for x in os.walk(plcpath)]

	mplabx = []
	projects = []
	xml_files = []
	counter_prj = 0;
	
	for curr_folder in plcfolders:
		prjFolder = os.path.abspath(curr_folder).endswith(".X")	
		
		if prjFolder:			
			src_path = os.path.dirname(os.path.abspath(curr_folder))
			os.chdir (src_path + "\src\config")	
			for (dirpath, subdirs, files) in os.walk("."):
				for x in files:
					if x.endswith(".xml"):
						addPrj = False
						if (curr_folder.find('prime') != -1):
							mplabx.append(os.path.abspath(curr_folder))
							xml_files.append(x)
							addPrj = True
						else:
							band = x[-8:-3]
							if (curr_folder.find(band) != -1):
								mplabx.append(os.path.abspath(curr_folder))
								xml_files.append(x)
								addPrj = True
						
						if addPrj:
							prjFile = os.path.dirname(os.path.abspath(curr_folder)) + "\\src\\config\\" + x[:-4] + "\\harmony.prj"
							projects.append(prjFile)
							counter_prj = counter_prj + 1
	
	counter_error = 0
	counter_success = 0
	os.chdir (mhcpath)
	
	print("Found " + str(counter_prj) + " projects. Building projects...")
	for project,configuration in zip(mplabx,xml_files):
		pathSplit = os.path.split(project)
		
		# Generate Make file
		cmd_make = '\"' + mplabx_path + 'mplab_platform/bin/prjMakefilesGenerator.bat" '
		cmd_make = cmd_make + project
		print("Creating makefile: " + project)
		failMake = os.system (cmd_make)
		if (failMake):
			print("MAKEFILE ERROR:" + str(failMake) + " : " + cmd_make)
			sys.exit(-1)
		else:
			print("MAKEFILE SUCCESS")
			
		# Build project
		os.chdir (project)
		logBuildFile = os.path.dirname(os.path.abspath(mhcpath)) + "\\build_" + pathSplit[1][:-2] + "_log.txt"
		cmd_build = '\"' + mplabx_path + 'gnuBins/GnuWin32/bin/make" '
		cmd_build = cmd_build + "-f nbproject/Makefile-"+ configuration[:-4] + ".mk SUBPROJECTS= .build-conf"		
		print("Building project: " + project)
		failBuild = os.system (cmd_build + ">>" + logBuildFile)
		if (failBuild):
			print("BUILD ERROR:" + str(failBuild) + " : " + cmd_build)
			counter_error = counter_error + 1
			sys.exit(-1)
		else:
			print("BUILD SUCCESS")
			counter_success = counter_success + 1
		
	duration = time.time() - time_start
	
	print("Compilation is finished.")
	print("Successfully built " + str(counter_success) + " of " + str(counter_success + counter_error) + " projects")
	print("INFO - Compilation finished in " + str(duration) + " seconds")
	
