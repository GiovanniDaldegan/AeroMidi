<CsoundSynthesizer>
<CsOptions>
-+rtmidi=portmidi -M0 -odac
</CsOptions>

<CsInstruments>
sr      = 44100
kr      = 441
ksmps   = 100
nchnls  = 2

massign 0,0
massign 1,1

instr 1
    ifreq cpsmidi
    iamp ampmidi 18000

    kamp midictrl 2

    ; som 1 - síntese subtrativa de ruído nos 3 primeiros harmônicos da frequência base

    i1f1 = (ifreq * 1)
    i1f2 = (ifreq * 2)
    i1f3 = (ifreq * 3)
    i1f4 = (ifreq * 5)
    i1f5 = (ifreq * 7)

    a1noise rand 1

    i1band = 0.001                                  ; fator multiplicativo das bandas dos pass-filters
    a1fil1 	reson a1noise, i1f1, (i1f1 * i1band)
    a1fil2	reson a1noise, i1f2, (i1f2 * i1band)
    a1fil3	reson a1noise, i1f3, (i1f3 * i1band)
    a1fil4	reson a1noise, i1f4, (i1f3 * i1band)
    a1fil5	reson a1noise, i1f5, (i1f3 * i1band)

    a1filmix = (a1fil1 + a1fil2 + a1fil3 + a1fil4 + a1fil5) / 5

    a1sig 	balance a1filmix, a1noise
    a1amp	linenr iamp, 0.06, 0.08, 0.005

    a1final = (a1sig * a1amp)

    ; som 2 - ondas senoidais imitando o espectro de uma ocarina

    ; modulação da frequência base para criar dinamismo
    a2mod1 oscili 3, 2, 1
    a2mod2 oscili 3, 3.2, 1
    a2modfinal = (a2mod1 + a2mod2) / 2

    a2port1 oscili 10000 / 2, a2modfinal + ifreq * 1, 1
    a2port2 oscili 10000 / 3, a2modfinal + ifreq * 2, 1
    a2port3 oscili 10000 / 2, a2modfinal + ifreq * 3, 1
    a2port4 oscili 10000 / 3, a2modfinal + ifreq * 5, 1
    a2port5 oscili 10000 / 4, a2modfinal + ifreq * 7, 1

    a2final = (a2port1 + a2port2 + a2port3 + a2port4 + a2port5) / 5
    a1amp	linenr iamp, 0.06, 0.02, 0.001

    a2final = a2final * a1amp

    ; síntese dos dois sons
    afinal = (a1final + a2final) / 2

    outs afinal, afinal

endin

</CsInstruments>

<CsScore>
f0 30000060
f1 0 4096 10 1
i1 0 360
e
</CsScore>

</CsoundSynthesizer>
