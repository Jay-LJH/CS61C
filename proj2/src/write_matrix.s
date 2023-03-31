.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fwrite error or eof,
#     this function terminates the program with error code 30
# ==============================================================================
write_matrix:
	addi sp,sp,-24
	sw ra,0(sp)
	sw s0,4(sp)
	sw s1,8(sp)
	sw s2,12(sp)
	sw a2,16(sp)
	sw a3,20(sp)
	mv s0,a0
	mv s1,a1
	
	mul s2,a2,a3

	#open fd
	li a1,1
	jal ra,fopen
	li a1,-1
	beq	a0,a1,exit0
	mv s0,a0

	#write row and columns
	addi a1,sp,16
	li a2,1
	li a3,4
	jal ra,fwrite
	li a1,1
	bne a0,a1,exit1
	mv a0,s0
	addi a1,sp,20
	li a2,1
	li a3,4
	jal ra,fwrite
	li a1,1
	bne a0,a1,exit1

	#write content
	mv a0,s0
	mv a1,s1
	mv a2,s2
	li a3,4
	jal ra,fwrite
	bne a0,s2,exit1

	#call fclose
	mv a0,s0
	jal ra,fclose
	bne a0,x0,exit2

	#restore s0-s2
	lw ra,0(sp)
	lw s0,4(sp)
	lw s1,8(sp)
	lw s2,12(sp)
	addi sp,sp,24
	ret

exit0:
	li a0,27
	j exit

exit1:
	li a0,30
	j exit

exit2:
	li a0,28
	j exit