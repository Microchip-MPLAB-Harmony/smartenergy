import shutil, os
import time
import argparse
import sys

curr_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

if __name__ == '__main__':

	##########################################################################
	# Argument manipulation
	##########################################################################
	parser = argparse.ArgumentParser(description='''This script updates bin ATPL360B bin file to atpl/bin/folder''')
	parser.add_argument('--h3path', required=False, help='Harmony 3 absolute path: [c:\\MH3]')
	parser.add_argument('--project', required=False, help='Name of the project, including path info')
	args = parser.parse_args()
	
	time_start = time.time()
	
	projName = ''
		
	if args.h3path is not None:
		h3path = args.h3path
				
	if args.project is not None:
		projName = args.project
		
	print("Searching projects...")
	
	plcpath = h3path + "\\smartenergy_plc\\apps"
	mhcpath = h3path + "\\mhc"	
			
	plcfolders = [x[0] for x in os.walk(plcpath)]

	mplabx = []
	projects = []
	xml_files = []
	counter_prj = 0
	dummy = 0
	
	for curr_folder in plcfolders:
		prjFolder = os.path.abspath(curr_folder).endswith(".X")	
		
		if prjFolder:
			if projName != '':
				# Apply Profile Name filter
				if curr_folder.find(projName) == -1:
					continue
			
			mplabx.append(os.path.abspath(curr_folder))
			config_name = curr_folder.split("\\")[-1][:-2]
			prjFile = os.path.dirname(os.path.abspath(curr_folder)) + "\\src\\config\\" + config_name + "\\harmony.prj"
			projects.append(prjFile)
			xml_files.append(config_name + ".xml")
			counter_prj = counter_prj + 1
			
			for (dirpath, subdirs, files) in os.walk("."):
				for x in files:
					# Remove .bin files
					if x.endswith(".bin"):
						binfile = dirpath + "\\" + x
						os.remove(binfile)
	
	logGlobalFile = os.path.dirname(os.path.abspath(mhcpath)) + "\\MH3_global_log.txt"
	
	counter_error = 0
	counter_success = 0
	os.chdir (mhcpath)
	
	print("Found " + str(counter_prj) + " projects. Regenerating projects...")
	for project,mplab,config in zip(projects, mplabx, xml_files):
		cmd = "java -Xverify:none -jar mhc.jar -mode=gen -log=\"DEBUG\" -fw=../ -ide=mplabx "
		cmd = cmd + "-mplabX=\"" + mplab  + "\" "
		cmd = cmd + "-c=\"" + project + "\" "
		cmd = cmd + "-loadstate=\"" + config + "\" "
		cmd = cmd + "-diff=\"overwrite\" -ignorePackPaths"
		pathSplit = os.path.split(mplab)
		logFile = os.path.dirname(os.path.abspath(mhcpath)) + "\\gen_" + pathSplit[1][:-2] + "_log.txt"
		print("MPLABX: " + mplab)
		failure = os.system (cmd + ">>" + logFile)
		if (failure):
			print("ERROR:" + str(failure) + " : " + cmd)
			counter_error = counter_error + 1
			cmd_open = "java -Xverify:none -jar mhc.jar -mode=gui -fw=../"
			cmd_open = cmd_open + " -c=\" " + project + "\""
			os.system (cmd_open)
			#sys.exit(-1)
		else:
			print("SUCCESS")
			counter_success = counter_success + 1
	
	duration = time.time() - time_start
	
	print("Project generation is finished.")
	print("Successfully ran " + str(counter_success) + " of " + str(counter_success + counter_error) + " projects")
	print("INFO - Execution finished in " + str(duration) + " seconds")
	
	sys.exit(0)