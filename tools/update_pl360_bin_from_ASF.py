import shutil, os
import argparse

curr_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

if __name__ == '__main__':

	##########################################################################
	# Argument manipulation
	##########################################################################
	parser = argparse.ArgumentParser(description='''This script updates bin ATPL360 bin file to atpl/bin/folder''')
	parser.add_argument('--asfpath', required=False, help='ASF absolute path: [c:\\Asf]')
	parser.add_argument('--profile', required=True, help='profiles: [G3_PHY], [G3_MAC_RT], [G3_ALL], [PRIME], [ALL]')
	args = parser.parse_args()
	
	profile = 'ALL'
	
	if args.profile is not None:
		profile = args.profile
		
	if args.asfpath is not None:
		asfpath = args.asfpath	
		
	os.chdir (curr_path)
	if (profile == 'G3_PHY') | (profile == 'G3_ALL') | (profile == 'ALL'):
		# CEN A
		filename_src = asfpath + "\\atpl\\bin\\ATPL360B_G3_CENA.bin"
		filename_dst = curr_path + "\\driver\\pl360\\src\\bin\\PL360_G3_CENA.bin"
		shutil.copyfile(filename_src, filename_dst)
		filename_dst = curr_path + "\\apps\\driver\\phy_pl360\\g3_getting_started\\firmware\\src\\config\\sam_e70_xpld\\driver\\pl360\\bin\\PL360.bin"
		shutil.copyfile(filename_src, filename_dst)
		filename_dst = curr_path + "\\apps\\tools\\g3_phy_tester_tool\\firmware\\src\\config\\sam_e70_xpld\\driver\\pl360\\bin\\PL360.bin"
		shutil.copyfile(filename_src, filename_dst)
		# CEN B
		filename_src = asfpath + "\\atpl\\bin\\ATPL360B_G3_CENB.bin"
		filename_dst = curr_path + "\\driver\\pl360\\src\\bin\\PL360_G3_CENB.bin"
		shutil.copyfile(filename_src, filename_dst)
		# FCC
		filename_src = asfpath + "\\atpl\\bin\\ATPL360B_G3_FCC.bin"
		filename_dst = curr_path + "\\driver\\pl360\\src\\bin\\PL360_G3_FCC.bin"
		shutil.copyfile(filename_src, filename_dst)
		
	if (profile == 'PRIME') | (profile == 'ALL'):	
		filename_src = asfpath + "\\atpl\\bin\\ATPL360B_PRIME.bin"
		filename_dst = curr_path + "\\driver\\pl360\\src\\bin\\PL360_PRIME.bin"
		shutil.copyfile(filename_src, filename_dst)
		filename_dst = curr_path + "\\apps\\driver\\phy_pl360\\prime_getting_started\\firmware\\src\\config\\sam_e70_xpld\\driver\\pl360\\bin\\PL360.bin"
		shutil.copyfile(filename_src, filename_dst)
		filename_dst = curr_path + "\\apps\\tools\\prime_phy_tester_tool\\firmware\\src\\config\\sam_e70_xpld\\driver\\pl360\\bin\\PL360.bin"
		shutil.copyfile(filename_src, filename_dst)
		
	print("PL360 bin files moved OK")
