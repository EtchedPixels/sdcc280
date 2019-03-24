# Deleting all files created by building the program
# --------------------------------------------------
#include $(top_builddir)/Makefile.common
ASDIR	= ..
ASMAK	= $(ASDIR)/asxmak/linux/build
ASEXE	= $(ASDIR)/asxmak/linuc/exe

tidy:
	rm -f *core *.lst *.hlr *.rel *.rst *.map *.sym foo*
	
clean:	tidy
	rm -f $(ASOBJECTS) *.o
	rm -f $(ASMAK)/asz280
	rm -f $(ASEXE)/asz280
#	rm -f asz280

spotless:	clean
	rm -f *[%~] *.[oa]
	rm -f $(ASX)


