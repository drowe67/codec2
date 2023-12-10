# Makefile for codec2.pdf
#
# usage:
#   Build codec2 with -DDUMP (see README)
#   cd ~/codec2/doc
#   make

# Set these externally with an env variable (e.g. for GitHub action) to override
# defaults below. Need to run cmake with -DDUMP

CODEC2_SRC ?= $(HOME)/codec2
CODEC2_BINARY ?= $(HOME)/codec2/build_linux/src

PATH := $(PATH):$(CODEC2_BINARY)

DOCNAME := codec2
PLOT_FILES := hts2a_37_sn.tex hts2a_37_sw.tex hts2a_37_lpc_lsp.tex hts2a_37_lpc_pf.tex

$(DOCNAME).pdf: $(PLOT_FILES) $(DOCNAME).tex $(DOCNAME)_refs.bib
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
	rm *.blg *.bbl *.aux *.log $(DOCNAME).pdf