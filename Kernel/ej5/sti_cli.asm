global _cli
global _sti

section .text
_cli:
    cli         ; Deshabilita interrupciones
    ret

_sti:
    sti         ; Habilita interrupciones
    ret
