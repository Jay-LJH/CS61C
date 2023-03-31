.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
	# Prologue
	bgt a1,zero,loop_start
	li a0,36
	j exit
loop_start:
	lw a2,0(a0)
	
	xor a2,a2,a2
	sw a2,0(a0)

loop_continue:
	addi a1, a1, -1
	addi a0,a0, 4
	bne a1,x0,loop_start

loop_end:
	

	# Epilogue


	ret
