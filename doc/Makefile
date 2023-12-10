# Makefile for codec2.pdf
#
# usage:
#   Build codec2 with -DUNITEST=1 (see README)
#   cd ~/codec2/doc
#   make

# Set these externally with an env variable (e.g. for GitHub action) to override
# defaults below. Need to run cmake with -DDUMP

CODEC2_SRC ?= $(HOME)/codec2
CODEC2_BINARY ?= $(HOME)/codec2/build_linux

PATH := $(PATH):$(CODEC2_BINARY)/src

DOCNAME := codec2
PLOT_FILES := hts2a_37_sn.tex hts2a_37_sw.tex hts2a_37_lpc_lsp.tex hts2a_37_lpc_pf.tex

# For automated tests we always want to build the PDF, despite codec2.pdf existing in the repo
.PHONY: pdf
pdf: $(PLOT_FILES) $(DOCNAME).tex $(DOCNAME)_refs.bib
	pdflatex $(DOCNAME).tex
	bibtex $(DOCNAME).aux
	pdflatex $(DOCNAME).tex
	pdflatex $(DOCNAME).tex

$(PLOT_FILES):
	echo $(PATH)
	c2sim $(CODEC2_SRC)/raw/hts2a.raw --dump hts2a --lpc 10 --lsp --lpcpf
	DISPLAY=""; printf "plamp('hts2a',f=37,epslatex=1)\nq\n" | octave-cli -qf -p $(CODEC2_SRC)/octave

.PHONY: clean
clean:
	rm -f *.blg *.bbl *.aux *.log *.out $(DOCNAME).pdf hts2a*