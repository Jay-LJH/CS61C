.globl classify

.text
# =====================================
# COMMAND LINE ARGUMENTS
# =====================================
# Args:
#   a0 (int)        argc
#   a1 (char**)     argv
#   a1[1] (char*)   pointer to the filepath string of m0
#   a1[2] (char*)   pointer to the filepath string of m1
#   a1[3] (char*)   pointer to the filepath string of input matrix
#   a1[4] (char*)   pointer to the filepath string of output file
#   a2 (int)        silent mode, if this is 1, you should not print
#                   anything. Otherwise, you should print the
#                   classification and a newline.
# Returns:
#   a0 (int)        Classification
# Exceptions:
#   - If there are an incorrect number of command line args,
#     this function terminates the program with exit code 31
#   - If malloc fails, this function terminates the program with exit code 26
#
# Usage:
#   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
classify:
	li t0,5
	bne a0,t0,exit1
	addi sp,sp,-76
	sw ra,0(sp)
	sw s0,4(sp)
	sw s1,8(sp)
	sw s2,12(sp)
	sw s3,16(sp)
	sw s4,20(sp)
	sw s5,24(sp)
	sw s6,28(sp)
	sw s7,32(sp)
	mv s0,a1
	mv s1,a2
	
	# Read pretrained m0, s3 -> m0
	lw a0,4(s0)
	addi a1,sp,36
	addi a2,sp,40
	jal ra,read_matrix
	mv s3,a0

	# Read pretrained m1 s4 -> m1
	lw a0,8(s0)
	addi a1,sp,44
	addi a2,sp,48
	jal ra,read_matrix
	mv s4,a0

	# Read input matrix s5 -> input
	lw a0,12(s0)
	addi a1,sp,52
	addi a2,sp,56
	jal ra,read_matrix
	mv s5,a0
    

	# Compute h = matmul(m0, input) s6 -> h
	#first malloc enough place
	lw t0,36(sp)
	lw t1,56(sp)
	sw t0,60(sp)
	sw t1,64(sp)
	mul a1,t0,t1
	slli a0,a1,2
	jal ra,malloc
	beq a0,x0,exit0
	mv s6,a0
	
	mv a0,s3
	lw a1,36(sp)
	lw a2,40(sp)
	mv a3,s5
	lw a4,52(sp)
	lw a5,56(sp)
	mv a6,s6
	jal ra,matmul
	
	# Compute h = relu(h)
	mv a0,s6
	lw t0,60(sp)
	lw t1,64(sp)
	mul a1,t0,t1
	jal ra,relu

	# Compute o = matmul(m1, h) s7->o
	lw t0,44(sp)
	lw t1,64(sp)
	sw t0,68(sp)
	sw t1,72(sp)
	mul a0,t0,t1
	slli a0,a0,2
	jal ra,malloc
	beq a0,x0,exit0
	mv s7,a0

	mv a0,s4
	lw a1,44(sp)
	lw a2,48(sp)
	mv a3,s6
	lw a4,60(sp)
	lw a5,64(sp)
	mv a6,s7
	jal ra,matmul
	ebreak

	# Write output matrix o
	lw a0,16(s0)
	mv a1,s7
	lw a2,68(sp)
	lw a3,72(sp)
	jal ra,write_matrix

	# Compute and return argmax(o)
	mv a0,s7
	lw t0,68(sp)
	lw t1,72(sp)
	mul a1,t0,t1
	jal ra,argmax
	mv s0,a0
    
	# If enabled, print argmax(o) and newline
	bne s1,x0,done
	jal ra,print_int
	li a0 '\n'
	jal ra,print_char

done:
	mv a0,s6
	jal ra,free
	mv a0,s7
	jal ra,free
	mv a0,s0
	
	lw ra,0(sp)
	lw s0,4(sp)
	lw s1,8(sp)
	lw s2,12(sp)
	lw s3,16(sp)
	lw s4,20(sp)
	lw s5,24(sp)
	lw s6,28(sp)
	lw s7,32(sp)
	addi sp,sp,76
	ret
exit0:
	li a0,26
	j exit	
exit1:
	li a0,31
	j exit