import shutil, os
import argparse

curr_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

if __name__ == '__main__':

	##########################################################################
	# Argument manipulation
	##########################################################################
	parser = argparse.ArgumentParser(description='''This script updates bin ATPL360 bin file to atpl/bin/folder''')
	parser.add_argument('--h3path', required=False, help='Harmony 3 absolute path: [c:\\MH3]')
	parser.add_argument('--profile', required=True, help='profiles: [G3_PHY], [G3_MAC_RT], [G3_ALL], [PRIME], [ALL]')
	args = parser.parse_args()
	
	profile = 'ALL'
	
	if args.profile is not None:
		profile = args.profile
		
	if args.h3path is not None:
		h3path = args.h3path
		
	plcpath = h3path + "\\plc\\apps"
	mhcpath = h3path + "\\mhc"	
			
	plcfolders = [x[0] for x in os.walk(plcpath)]

	folder_counter = 0
	mplabx = []
	projects = []
	xml_files = []
	
	for curr_folder in plcfolders:
		prjFolder = os.path.abspath(curr_folder).endswith(".X")	
		
		if prjFolder:			
			src_path = os.path.dirname(os.path.abspath(curr_folder))
			os.chdir (src_path + "\src\config")	
			for (dirpath, subdirs, files) in os.walk("."):
				for x in files:
					if x.endswith(".xml"):
						addPrj = False
						if ((curr_folder.find('getting_started') != -1) or (curr_folder.find('prime') != -1)):
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
	
	os.chdir (mhcpath)			
	for project,mplab,config in zip(projects, mplabx, xml_files):
		cmd = "java -Xverify:none -jar mhc.jar -mode=gen -log=\"DEBUG\" -fw=../ -ide=mplabx "
		cmd = cmd + "-mplabX=\"" + mplab  + "\" "
		cmd = cmd + "-c=\"" + project + "\" "
		cmd = cmd + "-loadstate=\"" + config + "\" "
		cmd = cmd + "-diff=\"overwrite\" -ignorePackPaths"
		os.system (cmd)
		
	print("Project generation is finished")
