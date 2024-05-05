# Makefile for codec2.pdf
#
# usage:
#   Build codec2 with -DUNITEST=1 (see README)
#   cd ~/codec2/doc
#   make

DOCNAME ?= codec2

# Set these externally to override defaults. JOBNAME sets the output file basename, 
# and avoids over writing codec2.pdf (e.g. when we are running a doc build test, but don't actually
# want to change codec2.pdf in the repo) 

CODEC2_SRC ?= $(HOME)/codec2
CODEC2_BINARY ?= $(HOME)/codec2/build_linux
JOBNAME ?= $(DOCNAME)

PATH := $(PATH):$(CODEC2_BINARY)/src

PLOT_FILES := hts2a_37_sn.tex hts2a_37_sw.tex hts2a_37_lpc_lsp.tex hts2a_37_lpc_pf.tex

$(DOCNAME).pdf: $(PLOT_FILES) $(DOCNAME).tex $(DOCNAME)_refs.bib
	pdflatex -shell-escape -file-line-error -jobname=$(JOBNAME) $(DOCNAME).tex 
	bibtex   $(JOBNAME).aux
	pdflatex -shell-escape -file-line-error -jobname=$(JOBNAME) $(DOCNAME).tex
	pdflatex -shell-escape -file-line-error -jobname=$(JOBNAME) $(DOCNAME).tex

$(PLOT_FILES):
	echo $(PATH)
	c2sim $(CODEC2_SRC)/raw/hts2a.raw --dump hts2a --lpc 10 --lsp --lpcpf
	DISPLAY=""; printf "plamp('hts2a',f=37,epslatex=1)\nq\n" | octave-cli -qf -p $(CODEC2_SRC)/octave

.PHONY: clean
clean:
	rm -f *.blg *.bbl *.aux *.log *.out hts2a*
