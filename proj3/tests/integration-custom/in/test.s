addi s0, x0, 0x100
addi s1,x0,16
sw   s1, 0(s0)
sw   s1, 4(s0)
lw   t0, 4(s0)
lh   t0, 4(s0)