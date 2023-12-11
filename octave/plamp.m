% plamp.m
% Plot ampltiude modelling information from c2sim dump files.

function plamp(samname, f, epslatex=0)

  plot_sw = 1;

  sn_name = strcat(samname,"_sn.txt");
  Sn = load(sn_name);

  sw_name = strcat(samname,"_sw.txt");
  Sw = load(sw_name);

  sw__name = strcat(samname,"_sw_.txt");
  if (file_in_path(".",sw__name))
    Sw_ = load(sw__name);
  endif

  ew_name = strcat(samname,"_ew.txt");
  if (file_in_path(".",ew_name))
    Ew = load(ew_name);
  endif

  E_name = strcat(samname,"_E.txt");
  if (file_in_path(".",E_name))
    E = load(E_name);
  endif

  rk_name = strcat(samname,"_rk.txt");
  if (file_in_path(".",rk_name))
    Rk = load(rk_name);
  endif

  model_name = strcat(samname,"_model.txt");
  model = load(model_name);

  modelq_name = strcat(samname,"_qmodel.txt");
  if (file_in_path(".",modelq_name))
    modelq = load(modelq_name);
  endif

  pw_name = strcat(samname,"_pw.txt");
  if (file_in_path(".",pw_name))
    Pw = load(pw_name);
  endif
  pwb_name = strcat(samname,"_pwb.txt");
  if (file_in_path(".",pwb_name))
    Pwb = load(pwb_name);
  endif

  lsp_name = strcat(samname,"_lsp.txt");
  if (file_in_path(".",lsp_name))
    lsp = load(lsp_name);
  endif

  phase_name = strcat(samname,"_phase.txt");
  if (file_in_path(".",phase_name))
    phase = load(phase_name);
  endif

  phase_name_ = strcat(samname,"_phase_.txt");
  if (file_in_path(".",phase_name_))
    phase_ = load(phase_name_);
  endif

  snr_name = strcat(samname,"_snr.txt");
  if (file_in_path(".",snr_name))
    snr = load(snr_name);
  endif

  if epslatex, [textfontsize linewidth] = set_fonts(); end

  k = ' ';
  do
    figure(1); clf;
    clf;
    s = [ Sn(2*f-1,:) Sn(2*f,:) ];
    plot(s,'b');
    axis([1 length(s) -30000 30000]);
    xlabel('Time (samples)'); ylabel('Amplitude');
    
    figure(2); clf;
    Wo = model(f,1);
    L = model(f,2);
    Am = model(f,3:(L+2));
    plot((1:L)*Wo*4000/pi, 20*log10(Am),"+-r");
    axis([1 4000 -10 80]);
    hold on;
    if plot_sw
      plot((0:255)*4000/256, Sw(f,:),"b");
    end
    hold off; grid minor;
    ylabel ('Amplitude (dB)'); xlabel('Frequency (Hz)');
    
    figure(3); clf;
    hold on;
    plot((0:255)*4000/256, Sw(f,:),"b");
    plot((1:L)*Wo*4000/pi, 20*log10(Am),"+-r");
    plot((0:255)*4000/256, E(f)+10*log10(Pwb(f,:)),"g");
    plot(lsp(f,:)*4000/pi, 75,"g+");
    hold off; grid minor;
    axis([1 4000 -10 80]);
    ylabel ('Amplitude (dB)'); xlabel('Frequency (Hz)');

    figure(4); clf;
    hold on;
    plot((0:255)*4000/256, E(f)+10*log10(Pwb(f,:)),"g");
    plot((0:255)*4000/256, 10*log10(Pw(f,:)),"r");
    hold off; grid minor;
    axis([1 4000 -10 80]);
    ylabel ('Amplitude (dB)'); xlabel('Frequency (Hz)');

    % print EPS file

    if epslatex
      sz = "-S300,200";
      figure(1);
      fn = sprintf("%s_%d_sn.tex",samname,f);
      print(fn,"-depslatex",sz); printf("\nprinting... %s\n", fn);

      % file of points to plot in sinusoidal model
      fn = sprintf("%s_%d_sn.txt",samname,f);
      t_length = 4; s_max = 2; s=s*s_max/max(abs(s));
      N = length(s); t = (0:N-1)*t_length/N;
      s_save = [t' s']; size(s_save)
      save("-ascii",fn,"s_save"); printf("printing... %s\n", fn);
      
      figure(2);
      fn = sprintf("%s_%d_sw.tex",samname,f);
      print(fn,"-depslatex",sz); printf("printing... %s\n", fn);
   
      figure(3);
      fn = sprintf("%s_%d_lpc_lsp.tex",samname,f);
      print(fn,"-depslatex",sz); printf("printing... %s\n", fn);
   
      figure(4);
      fn = sprintf("%s_%d_lpc_pf.tex",samname,f);
      print(fn,"-depslatex",sz); printf("printing... %s\n", fn);
   
     restore_fonts(textfontsize,linewidth);
    endif

    % interactive menu

    printf("\rframe: %d  menu: n-next  b-back  s-plot_sw  q-quit", f);
    fflush(stdout);
    k = kbhit();
    if k == 'n'; f = f + 1; endif
    if k == 'b'; f = f - 1; endif
    if k == 's'
        if plot_sw; plot_sw = 0; else; plot_sw = 1; end
    endif

  until (k == 'q')
  printf("\n");

endfunction

function [textfontsize linewidth] = set_fonts(font_size=12)
  textfontsize = get(0,"defaulttextfontsize");
  linewidth = get(0,"defaultlinelinewidth");
  set(0, "defaulttextfontsize", font_size);
  set(0, "defaultaxesfontsize", font_size);
  set(0, "defaultlinelinewidth", 0.5);
end

function restore_fonts(textfontsize,linewidth)
  set(0, "defaulttextfontsize", textfontsize);
  set(0, "defaultaxesfontsize", textfontsize);
  set(0, "defaultlinelinewidth", linewidth);
end

function print_eps_restore(fn,sz,textfontsize,linewidth)
  print(fn,"-depslatex",sz);
  printf("printing... %s\n", fn);
  restore_fonts(textfontsize,linewidth);
end

