1. Go inside randdataset-1.0.1 and type in sequence:
	aclocal;autoconf;autoheader;automake;./configure;make

2. You should now have the executable randdataset inside randdataset-1.0.1/src. Create a directory on the root named "datasets" and copy the executable in it.

3. You must have the boost libraries (from boost.org) installed in the system. Edit the file Makefile in the root and uncomment either line 22 or 23.

3. Go to the root and type:
	make all

4. If the compilation fails, most probably there is either a problem with the boost library or the Makefile does not point to it correctly.

5. If you want to run a single instance try:
	./main help
to see the parameters you will have to pass.