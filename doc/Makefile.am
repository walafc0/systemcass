
if BUILD_DOCS
ps_pdf_doc=SystemCASS.pdf SystemCASS.ps
eps_figures=SystemeType.eps Automate.eps
endif

doc_DATA = $(ps_pdf_doc)
#latex_opts=-interaction batchmode #
latex_opts= -halt-on-error

%.pdf: %.ps
	ps2pdf $<

%.ps: %.dvi
	dvips -o $@ $<

%.bbl : %.aux
	bibtex $*

%.aux : %.tex %.bib $(eps_figures)
	latex $(latex_opts) $<

%.dvi %.bbl: %.tex %.bib $(eps_figures)
	latex $(latex_opts) $<
	latex $(latex_opts) $<

%.eps: %.fig
	fig2dev -L eps $< $@
