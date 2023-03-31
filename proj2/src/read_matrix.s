.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
#   - If malloc returns an error,
#     this function terminates the program with error code 26
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fread error or eof,
#     this function terminates the program with error code 29
# ==============================================================================
read_matrix:
	addi sp,sp,-20
	sw ra,0(sp)
	sw s0,4(sp)
	sw s1,8(sp)
	sw s2,12(sp)
	sw s3,16(sp)
	mv s0,a0
	mv s1,a1
	mv s2,a2

	#open file
	mv a1,x0
	jal ra,fopen
	li a1,-1
	beq	a0,a1,exit1

	#read rows and columns
	mv s3,a0
	mv a1,s1
	li a2,4
	jal ra,fread
	li a1,4
	bne a0,a1,exit3
	
    mv a0,s3
	mv a1,s2
	li a2,4
	jal ra,fread
	li a1,4
	bne a0,a1,exit3
    
	# malloc space for matrix
	lw a0,0(s1)
	lw a1,0(s2)
	mul a0,a0,a1
	slli a0,a0,2
	mv s1,a0
	jal ra,malloc
	beq a0,x0,exit0

	#read the matrix, s3 fd,s0 for the pointer,s1 size
	mv s0,a0
	mv a0,s3
	mv a1,s0
	mv a2,s1
	jal ra,fread
	bne a0,s1,exit3

	#close file
	mv a0,s3
	jal ra,fclose
	bne a0,x0,exit2
	mv a0,s0

	#restore s0-s3
	lw ra,0(sp)
	lw s0,4(sp)
	lw s1,8(sp)
	lw s2,12(sp)
	lw s3,16(sp)
	addi sp,sp,20
	ret

exit0:
	li a0,26
	j exit

exit1:
	li a0,27
	j exit

exit2:
	li a0,28
	j exit

exit3:
	li a0,29
	j exit
