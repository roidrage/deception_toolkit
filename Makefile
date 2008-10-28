all:
	@echo " currently only make docs is supported."

docs:
	doxygen doc/deceptiond.dxg

dvi: cleandocs
	cd doc \
		&& latex diploma.tex \
		&& latex diploma.tex

ps: dvi
	cd doc \
		&& dvips -o diploma.ps diploma.dvi

pdf: cleandocs
	cd doc \
		&& pdflatex diploma.tex \
		&& pdflatex diploma.tex \
		&& thumbpdf diploma.pdf \
		&& pdflatex diploma.tex \

slides:
	cd doc \
		&& latex slides.tex \
		&& dvips -o slides.ps slides.dvi \
		&& ps2pdf slides.ps

cleandocs:
	cd doc \
		&& rm -f diploma.ps diploma.pdf diploma.dvi diploma.aux diploma.out \
		diploma.lof diploma.lot diploma.lol diploma.log diploma.toc \
		diploma.ilg diploma.glx diploma.glo diploma.gxg diploma.gxs \
		diploma.tpt
